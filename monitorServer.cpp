#include "MonitorServer.h"
#include <getopt.h>
#include "Socket.h"
#include "TravelMonitorClient.h"

static const int NUMBER_OF_OPTIONS_ARGUMENTS = 10;
using namespace std;

const char *PROGRAM_OPTIONS = "p:t:b:c:s:";
const char *WRONG_PROGRAM_USAGE_ERROR = "Usage %s -p [port] -b [socketBufferSize] "
                                        "-c [socketBufferSize]  -s [sizeOfBloom] -t [numberOfThreads]\n";

void *threadReadFilesAndUpdateStructures(void*);

int cyclicBufferSize;
int numberOfFilesMovedToBuffer;
int numberOfReadFiles;
int totalNumberOfFiles;
int numberOfThreads;
int nextPositionOfBufferToBeRead = 0;
int numberOfThreadFinishedReadingFiles = 0;
char** cyclicBuffer;
char** filesNames;
PersonLinkedList *people;
VirusLinkedList *viruses;
CountryLinkedList *countries;
VaccinationCenter *vaccinationCenter;
CitizenRecordsFileReader *fileReader;

pthread_mutex_t readFromCyclicBufferLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t updateCyclicBufferLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mainThreadWaitToSendBloomFiltersLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t mainThreadWaitToSendBloomFiltersConditionVariable;

int main(int argc, char **argv) {
    int opt;
    int port;
    int socketBufferSize;
    int bloomSizeInKiloBytes;
    char *inputDirectory;
    numberOfThreads;

    /* Variables expected from TravelMonitor for responding to user requests */
    int citizenIdLength;
    char* citizenId;
    int dateLength;
    char* dateAsString;
    Date* date;
    int countryFromLength;
    char* countryFrom;
    int virusNameLength;
    char* virusName;

    // Host declaration (static declaration of hostname with large size)
    struct hostent *host;
    char hostname[256];

    // Number of paths given to the monitor server is equal to the argc - numberOfOptionArguments
    // (for example -b bufferSize) - 1 (the program name)
    int numberOfCountryNames = argc - NUMBER_OF_OPTIONS_ARGUMENTS - 1;

    char **countryNames = Helper::getCountiesNamesFromLastCommandLineArguments(
        numberOfCountryNames,
        argv,
        argc
    );

//    for (int i = 0; i < argc; i++) {
//        cout << argv[i] << endl;
//    }

    while ((opt = getopt(argc, argv, PROGRAM_OPTIONS)) != -1) {
        switch (opt) {
            case 'p':
                port = atoi(optarg);
                break;
            case 't':
                numberOfThreads = atoi(optarg);
                break;
            case 'b':
                socketBufferSize = atoi(optarg);
                break;
            case 'c':
                cyclicBufferSize = atoi(optarg);
                break;
            case 's':
                bloomSizeInKiloBytes = atoi(optarg);
                break;
            default:
                Helper::handleError(WRONG_PROGRAM_USAGE_ERROR);
        }
    }

    people = new PersonLinkedList();
    viruses = new VirusLinkedList();
    countries = new CountryLinkedList();

    vaccinationCenter = new VaccinationCenter(
            people,
            viruses,
            countries
    );

    fileReader = new CitizenRecordsFileReader(
            vaccinationCenter
    );

    Socket *socket = new Socket(socketBufferSize, port);

    int hostNameResult = gethostname(hostname, sizeof(hostname));
    if (hostNameResult == -1) {
        Helper::handleError("Error getting hostname");
    }

    host = gethostbyname(hostname);
    socket->createSocket(host);
    socket->connectToSocket();

    int inputDirectoryStringLength = socket->readNumber();
    inputDirectory = socket->readStringInChunks(inputDirectoryStringLength);
//    cout << inputDirectory << endl;

    totalNumberOfFiles = Helper::getTotalFilePathsNumber(
        numberOfCountryNames,
        countryNames,
        inputDirectoryStringLength,
        inputDirectory
    );

//    cout << "T " << totalNumberOfFiles << endl;

    filesNames = Helper::getFilePathsNames(
        numberOfCountryNames,
        countryNames,
        inputDirectoryStringLength,
        inputDirectory,
        totalNumberOfFiles
    );

    pthread_t thread[numberOfThreads];
    pthread_cond_init(&mainThreadWaitToSendBloomFiltersConditionVariable, NULL) ;
    cyclicBuffer = (char**) malloc(cyclicBufferSize * sizeof(char*));;
    numberOfFilesMovedToBuffer = 0;
    numberOfReadFiles = 0;
    int error;

    for(int i = 0; i < cyclicBufferSize; i++) {
        cyclicBuffer[i] = filesNames[i];
        numberOfFilesMovedToBuffer++;
    }

    for(int i = 0; i < numberOfThreads; i++) {
        if((
            error = pthread_create (
                &thread[i],
                NULL ,
                threadReadFilesAndUpdateStructures,
                (void*)cyclicBufferSize
            )
        )) {
             Helper::handleError("Error: pthread_create()", error);
        }
    }

//    MonitorServer* monitor = new MonitorServer(
//        people,
//        viruses,
//        countries,
//        vaccinationCenter,
//        fileReader
//    );
//
    if((
        error = pthread_mutex_lock(&mainThreadWaitToSendBloomFiltersLock)
    )) {
        Helper::handleError("Error: main thread waiting to send mutex lock", error);
    }

    /* Waiting on threads to finish reading from input files in order to send data */
    while(numberOfThreadFinishedReadingFiles < numberOfThreads) {
        pthread_cond_wait (
            &mainThreadWaitToSendBloomFiltersConditionVariable,
            &mainThreadWaitToSendBloomFiltersLock
        );
        cout << "Waited on " << numberOfThreadFinishedReadingFiles << endl;
    }

    if((
        error = pthread_mutex_unlock(&mainThreadWaitToSendBloomFiltersLock)
    )) {
        Helper::handleError("Error: main thread waiting to send mutex lock", error);
    }

    /* Send Bloom Filters back to TravelMonitor*/

    // Firstly we count the Bloom Filter that we are gonna send to TravelMonitor
    int numberOfBloomFiltersSent = 0;

    VirusLinkedListNode *currentVirus = viruses->getHead();
    BloomFilterLinkedListNode *currentBloomFilter;
    while(currentVirus != NULL) {
        // Send through socket the bloom filters of the Monitor
        BloomFilterLinkedList* bloomFiltersList = currentVirus->getVirus()->getVaccinatedPeopleBloomFilterLinkedList();
        currentBloomFilter = bloomFiltersList->getHead();
        while(currentBloomFilter != NULL) {
            numberOfBloomFiltersSent++;
            currentBloomFilter = currentBloomFilter->next;
        }

        currentVirus = currentVirus->next;
    }

    // Send through sockets the number of expected bloom filters
    socket->writeNumber(numberOfBloomFiltersSent);
    cout << "Will send " << numberOfBloomFiltersSent << endl;

    int i = 0;
    currentVirus = viruses->getHead();
    while(currentVirus != NULL) {
        // Send through socket the bloom filters of the Monitor
        BloomFilterLinkedList* bloomFiltersList = currentVirus->getVirus()->getVaccinatedPeopleBloomFilterLinkedList();
        currentBloomFilter = bloomFiltersList->getHead();
        while(currentBloomFilter != NULL) {
            BloomFilter* temp = currentBloomFilter->getBloomFilter();
            cout << "\t---" << temp->getVirusName() << " " << temp->getCountryName() << endl;
            if(temp == NULL) {
                Helper::handleError("Error: Attempted sending null BloomFilter");
            }
            socket->writeBloomFilterInChunks(temp);
            cout << "Send BF " << i << endl;
            i++;
            currentBloomFilter = currentBloomFilter->next;
        }

        currentVirus = currentVirus->next;
    }

    // Monitor stay alive to serve the user requests
//    while(true) {
//        int typeOfRequest = socket->readNumber();
//        cout << "OUAOY " << typeOfRequest << endl;
//        if(typeOfRequest == TravelMonitorClient::TRAVEL_REQUEST) {
//            citizenIdLength = socket->readNumber();
//            citizenId = socket->readStringInChunks(citizenIdLength);
//            dateLength = socket->readNumber();
//            dateAsString = socket->readStringInChunks(dateLength);
//            date = new Date(dateAsString);
//            countryFromLength = socket->readNumber();
//            countryFrom = socket->readStringInChunks(countryFromLength);
//            virusNameLength = socket->readNumber();
//            virusName = socket->readStringInChunks(virusNameLength);
//
//            cout << citizenId << endl;
//            cout << dateAsString << endl;
//            cout << countryFrom << endl;
//            cout << virusName << endl;
//        } else if (typeOfRequest == TravelMonitorClient::TRAVEL_STATS) {
//
//        } else if (typeOfRequest == TravelMonitorClient::ADD_VACCINATION_RECORDS) {
//
//        } else if (typeOfRequest == TravelMonitorClient::SEARCH_VACCINATION_STATUS) {
//
//        } else if (typeOfRequest == TravelMonitorClient::EXIT) {
//            break;
//        } else {
//            Helper::handleError("Error: No such command exists in MonitorServer");
//        }
//    }

    socket->closeSocket();

    for(int i = 0; i < numberOfThreads; i++) {
        if(
            (error = pthread_join(thread[i], NULL))
        ) {
            Helper::handleError("Error: pthread_join()", error);
        }
    }

    if (
        (error = pthread_mutex_destroy(&readFromCyclicBufferLock))
        || (error = pthread_mutex_destroy(&updateCyclicBufferLock))
        || (error = pthread_mutex_destroy(&mainThreadWaitToSendBloomFiltersLock))
    ) {
        Helper::handleError("Error: mutex_destroy()", error);
    }

    if (
        (error = pthread_cond_destroy(&mainThreadWaitToSendBloomFiltersConditionVariable))
    ) {
        Helper::handleError("Error: condition_variable_destroy()", error);
    }

//    pthread_exit ( NULL ) ;
    return 0;
}

void *threadReadFilesAndUpdateStructures(void* arg) {
    int error;
    while(true) {
        if(numberOfReadFiles == totalNumberOfFiles) {
            break;
        }

        /* Read filename from Cyclic Buffer */
        if((
            error = pthread_mutex_lock(&readFromCyclicBufferLock)
        )) {
            Helper::handleError("Error: read mutex lock", error);
        }

        if(cyclicBuffer[nextPositionOfBufferToBeRead] != NULL) {
            cout << "Read " << cyclicBuffer[nextPositionOfBufferToBeRead] << " position " << nextPositionOfBufferToBeRead << " from thread " << pthread_self() << endl;
            fileReader->readAndUpdateStructures(cyclicBuffer[nextPositionOfBufferToBeRead]);
            cyclicBuffer[nextPositionOfBufferToBeRead] = NULL;
            nextPositionOfBufferToBeRead = (++nextPositionOfBufferToBeRead)%cyclicBufferSize;
            numberOfReadFiles++;
        }

        if((
            error = pthread_mutex_unlock(&readFromCyclicBufferLock)
        )) {
            Helper::handleError("Error: read mutex unlock", error);
        }

        if(numberOfFilesMovedToBuffer == totalNumberOfFiles) {
            continue;
        }

        if((
            error = pthread_mutex_lock(&updateCyclicBufferLock)
        )) {
            Helper::handleError("Error: update mutex lock", error);
        }

        /* Fill Cyclic Buffer's empty positions */
        for(int i = 0; i < cyclicBufferSize; i++) {
            if(cyclicBuffer[i] == NULL && numberOfFilesMovedToBuffer < totalNumberOfFiles) {
                cyclicBuffer[i] = filesNames[numberOfFilesMovedToBuffer];
                numberOfFilesMovedToBuffer++;
            }
        }

        if((
            error = pthread_mutex_unlock(&updateCyclicBufferLock)
        )) {
            Helper::handleError("Error: update mutex unlock", error);
        }
    }

    /* Notify main thread that it can send data */

    if((
        error = pthread_mutex_lock(&mainThreadWaitToSendBloomFiltersLock)
    )) {
        Helper::handleError("Error: main thread waiting to send mutex lock", error);
    }

    cout << "Finished reading data" << endl;
    numberOfThreadFinishedReadingFiles++;

    if(numberOfThreadFinishedReadingFiles == numberOfThreads) {
        pthread_cond_signal(
            &mainThreadWaitToSendBloomFiltersConditionVariable
        );
    }

    if((
        error = pthread_mutex_unlock(&mainThreadWaitToSendBloomFiltersLock)
    )) {
        Helper::handleError("Error: main thread waiting to send mutex lock", error);
    }

//    pthread_exit (NULL) ;
}