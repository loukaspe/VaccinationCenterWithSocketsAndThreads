#include "TravelMonitorClient.h"

const char *TravelMonitorClient::FORK_ERROR = "ERROR: fork() failed to create a new process";

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
    // Ports are set serially from an initial value (ie 10001, 10002, 10003 etc)
    this->sockets = (Socket**) malloc(numberOfMonitors * sizeof(Socket*));
    this->ports = (int*) malloc(numberOfMonitors * sizeof(int));
    this->acceptedSocketFds = (int*) malloc(numberOfMonitors * sizeof(int));

    for (int i = 0; i < numberOfMonitors; i++) {
        ports[i] = INITIAL_PORT + i;
        this->sockets[i] = new Socket(
            socketBufferSize,
            ports[i]
        );
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

        this->sockets[i]->createSocket();
        this->sockets[i]->bindToSocket();
        this->sockets[i]->listenToSocket();
        acceptedSocketFds[i] = this->sockets[i]->acceptSocket();

        /* Because in Prj2 I did not pass paths to monitors, but country names
         * I did not change that due to lack of time. So I pass countryNames as
         * args in the execvp() and then I also have to pass the input_dir so that
         * the monitors can build the paths on their own */
        int inputDirectoryStringLength = strlen(inputDirectory);
        this->sockets[i]->writeNumber(inputDirectoryStringLength, true);
        this->sockets[i]->writeStringInChunks(inputDirectory, true);
    }
}

void TravelMonitorClient::readDataFromMonitors() {
    struct pollfd pollFds[numberOfMonitors];

    memset(pollFds, 0 , sizeof(pollFds));

    for(int i = 0; i < numberOfMonitors; i++) {
        pollFds[i].fd = acceptedSocketFds[i];
        pollFds[i].events = POLLIN;
    }
    
    int numberOfFds = numberOfMonitors;
    
    bool isAllDataReceived = false;
    for(;;) {
        for(int i = 0; i < numberOfMonitors; i++) {
            if(pollFds[i].fd != -1) {
                isAllDataReceived = false;
                break;
            }
        }

        if(isAllDataReceived == true) {
            break;
        }

        int pollResult = poll(pollFds, numberOfFds, NO_POLL_TIMEOUT);

        if(pollResult == -1) {
            Helper::handleError("Error with poll()", errno);
        }

        if(pollResult == 0) {
            Helper::handleError("Error: poll timeout");
        }

        for(int i = 0; i < numberOfMonitors; i++) {
            if (pollFds[i].revents & POLLIN) {
                if (pollFds[i].fd == this->acceptedSocketFds[i]) {

                    /* Receive BloomFilters from Monitors */
                    int numberOfBloomFiltersExpected = sockets[i]->readNumber(true);
                    cout << "Expect " << numberOfBloomFiltersExpected << endl;

                    int k = 0;
                    for(int j = 0; j < numberOfBloomFiltersExpected; j++) {
                        BloomFilter* bloomFilter = sockets[i]->readBloomFilterInChunks(true);
                        cout << "ooooooooooooooCountry: " << bloomFilter->getCountryName() << " and " << "Virus: " << bloomFilter->getVirusName() << endl;                        cout << "Received BF " << k << endl;
                        k++;
                    }

                    sockets[i]->closeSocket();
                    pollFds[i].events = 0;
                    pollFds[i].fd = -1;
                }
            }
        }

        // I assume that all data is received. In the start of the loop I check if all
        // pollFds.fd are -1, else all data is not received
        isAllDataReceived = true;
    }

    wait(NULL);
}
