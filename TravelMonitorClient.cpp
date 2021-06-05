#include "TravelMonitorClient.h"

const char *TravelMonitorClient::FORK_ERROR = "ERROR: fork() failed to create a new process";
const char *TravelMonitorClient::MALLOC_FAIL_ERROR_MESSAGE = "ERROR: malloc() failed to allocate memory";

TravelMonitorClient::TravelMonitorClient(
        int numberOfMonitors,
        int socketBufferSize,
        int cyclicBufferSize,
        int bloomSizeInKiloBytes,
        char *inputDirectory,
        int numberOfThreads
) :
        numberOfMonitors(numberOfMonitors),
        socketBufferSize(socketBufferSize),
        cyclicBufferSize(cyclicBufferSize),
        bloomSizeInKiloBytes(bloomSizeInKiloBytes),
        inputDirectory(inputDirectory),
        numberOfThreads(numberOfThreads) {

    this->bloomFilters = new BloomFilterLinkedList();

    // We need numberOfMonitors sockets: one foreach monitorServer
    // We initialise the sockets' variables' arrays
    // Ports are set serially from an initial value (ie 10001, 10002, 10003 etc)
    this->createdSocketFds = (int *) malloc(numberOfMonitors * sizeof(int));
    this->acceptedSocketFds = (int *) malloc(numberOfMonitors * sizeof(int));
    this->ports = (int *) malloc(numberOfMonitors * sizeof(int));
    this->servers = (struct sockaddr_in *) malloc(numberOfMonitors * sizeof(struct sockaddr_in));
    this->serversPointers = (struct sockaddr **) malloc(numberOfMonitors * sizeof(struct sockaddr *));
    this->clients = (struct sockaddr_in *) malloc(numberOfMonitors * sizeof(struct sockaddr_in));
    this->clientsPointers = (struct sockaddr **) malloc(numberOfMonitors * sizeof(struct sockaddr *));

    for (int i = 0; i < numberOfMonitors; i++) {
        serversPointers[i] = (struct sockaddr *) &servers[i];
        clientsPointers[i] = (struct sockaddr *) &clients[i];
        ports[i] = INITIAL_PORT + i;
    }

    createMonitorArguments();
}

/* Function that creates a 2D string array that holds the arguments' array for
 * each one of the numberOfMonitor MonitorsServers. Firstly we read all the countries'
 * subdirectories names and number for each monitor */
void TravelMonitorClient::createMonitorArguments() {
    this->monitorArguments = (char ***) malloc((numberOfMonitors) * sizeof(char **));

    char **countrySubdirectories = Helper::getAllSubdirectoriesNames(this->inputDirectory);
    int numberOfCountrySubdirectories = Helper::getAllSubdirectoriesNumber(this->inputDirectory);
    int remainingNumberOfCountrySubdirectories = numberOfCountrySubdirectories;
    int divisionRemainder;
    int numberOfCountriesPassedToMonitor;
    int numberOfMonitorWithSentCountries = numberOfMonitors;

    // Helpful variable for calculation of arrays' index for the paths
    int temp = 0;

    for (int i = 0; i < numberOfMonitors; i++) {
        // Reset of helpful var
        temp = 0;
        numberOfCountriesPassedToMonitor = Helper::getCeilingOfDividedInts(
                remainingNumberOfCountrySubdirectories,
                numberOfMonitorWithSentCountries
        );

        int numberOfArgsForMonitor = NUMBER_OF_MONITOR_ARGS_WITHOUT_COUNTRIES_PATHS
                                     + numberOfCountriesPassedToMonitor;

        this->monitorArguments[i] = (char **) malloc(numberOfArgsForMonitor * sizeof(char *));
        this->monitorArguments[i][0] = "./monitorServer";
        this->monitorArguments[i][1] = "-p";
        this->monitorArguments[i][2] = (char *) malloc(10 * sizeof(char));
        sprintf(this->monitorArguments[i][2], "%d", ports[i]);
        this->monitorArguments[i][3] = "-b";
        this->monitorArguments[i][4] = (char *) malloc(10 * sizeof(char));
        sprintf(this->monitorArguments[i][4], "%d", this->socketBufferSize);
        this->monitorArguments[i][5] = "-c";
        this->monitorArguments[i][6] = (char *) malloc(10 * sizeof(char));
        sprintf(this->monitorArguments[i][6], "%d", this->cyclicBufferSize);
        this->monitorArguments[i][7] = "-s";
        this->monitorArguments[i][8] = (char *) malloc(10 * sizeof(char));
        sprintf(this->monitorArguments[i][8], "%d", this->bloomSizeInKiloBytes);
        this->monitorArguments[i][9] = "-t";
        this->monitorArguments[i][10] = (char *) malloc(10 * sizeof(char));
        sprintf(this->monitorArguments[i][10], "%d", this->numberOfThreads);

        this->monitorArguments[i][numberOfArgsForMonitor - 1] = NULL;


        for (int j = numberOfCountrySubdirectories - 1; j >= 0; j--) {
            divisionRemainder = j % numberOfMonitors;

            if (divisionRemainder == i) {
                this->monitorArguments[i][NUMBER_OF_MONITOR_ARGS_WITHOUT_COUNTRIES_PATHS - 1 +
                                          temp] = countrySubdirectories[j];
                temp++;
            }
        }

        remainingNumberOfCountrySubdirectories -= numberOfCountriesPassedToMonitor;
        numberOfMonitorWithSentCountries--;
    }
}

void TravelMonitorClient::createMonitorsAndPassThemData() {
    // Helpful variable for the socket calling
    socklen_t clientLength;
    for (int i = 0; i < this->numberOfMonitors; i++) {
        pid_t id = fork();

        if (id == -1) {
            perror(FORK_ERROR);
            exit(1);
        }

        if (id == 0) {
            execvp(
                    "./monitorServer",
                    this->monitorArguments[i]
            );

            exit(0);
        }

        this->createSocketForMonitor(i);
        this->bindToSocketForMonitor(i);
        this->listenToSocketForMonitor(i);
        this->acceptSocketForMonitor(i);
    }

    //Wait for results from all with select()
    cout << readNumberFromSocket(0) << endl;
    for(int i = 0; i < numberOfMonitors; i++) {
        closeSocket(i);
    }

    wait(NULL);
}

void TravelMonitorClient::createSocketForMonitor(int i) {
    if (
        (createdSocketFds[i] = socket(AF_INET, SOCK_STREAM, 0)) < 0
    ) {
        Helper::handleError("Error in creating socket", errno);
    }

    servers[i].sin_family = AF_INET;
    servers[i].sin_addr.s_addr = htonl(INADDR_ANY);
    servers[i].sin_port = htons(ports[i]);
}

void TravelMonitorClient::bindToSocketForMonitor(int i) {
    if (
        bind(createdSocketFds[i], serversPointers[i], sizeof(servers[i])) < 0
    ) {
        Helper::handleError("Error in binding socket", errno);
    }
}

void TravelMonitorClient::listenToSocketForMonitor(int i) {
    if (
        // We listen to one connection per monitor
        listen(createdSocketFds[i], 1) < 0
    ) {
        Helper::handleError("Error in listening to socket", errno);
    }
}

void TravelMonitorClient::acceptSocketForMonitor(int i) {
    socklen_t clientLength = sizeof(clients[i]);

    if (
        (acceptedSocketFds[i] = accept(createdSocketFds[i], clientsPointers[i], &clientLength))< 0
    ) {
        Helper::handleError("Error in accepting socket connection", errno);
    }

//    close(createdSocketFds[i]);
}

int TravelMonitorClient::readNumberFromSocket(int i) {
    int number;

    if(read(acceptedSocketFds[i], &number, sizeof(int)) < 0) {
        Helper::handleError("Error reading from socket", errno);
    }

    return number;
}

void TravelMonitorClient::closeSocket(int i) {
    close(createdSocketFds[i]);
    close(acceptedSocketFds[i]);
}
