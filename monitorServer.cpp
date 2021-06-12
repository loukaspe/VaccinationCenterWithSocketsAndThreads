#include "MonitorServer.h"
#include <getopt.h>
#include "Socket.h"

static const int NUMBER_OF_OPTIONS_ARGUMENTS = 10;
using namespace std;

const char *PROGRAM_OPTIONS = "p:t:b:c:s:";
const char *WRONG_PROGRAM_USAGE_ERROR = "Usage %s -p [port] -b [socketBufferSize] "
                                        "-c [socketBufferSize]  -s [sizeOfBloom] -t [numberOfThreads]\n";

char **getCountiesNamesFromCommandLineArguments(int, char **, int);
int getTotalFilesNumber(int, char**, int, char*);
char** getFilesNames(int, char**, int, char*, int);
void *threadReadFilesAndUpdateStructures(void*);

int cyclicBufferSize;
int numberOfFilesMovedToBuffer;
int numberOfReadFiles;
int totalNumberOfFiles;
int nextPositionOfBufferToBeRead = 0;
char** cyclicBuffer;
char** filesNames;
PersonLinkedList *people;
VirusLinkedList *viruses;
CountryLinkedList *countries;
VaccinationCenter *vaccinationCenter;
CitizenRecordsFileReader *fileReader;

pthread_mutex_t readFromCyclicBufferLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t updateCyclicBufferLock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char **argv) {
    int opt;
    int port;
    int socketBufferSize;
    int bloomSizeInKiloBytes;
    char *inputDirectory;
    int numberOfThreads;

    // Host declaration (static declaration of hostname with large size)
    struct hostent *host;
    char hostname[256];

    // Number of paths given to the monitor server is equal to the argc - numberOfOptionArguments
    // (for example -b bufferSize) - 1 (the program name)
    int numberOfCountryNames = argc - NUMBER_OF_OPTIONS_ARGUMENTS - 1;

    char **countryNames = getCountiesNamesFromCommandLineArguments(numberOfCountryNames, argv, argc);

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

    totalNumberOfFiles = getTotalFilesNumber(
        numberOfCountryNames,
        countryNames,
        inputDirectoryStringLength,
        inputDirectory
    );

//    cout << "T " << totalNumberOfFiles << endl;

    filesNames = getFilesNames(
        numberOfCountryNames,
        countryNames,
        inputDirectoryStringLength,
        inputDirectory,
        totalNumberOfFiles
    );

    pthread_t thread[numberOfThreads];
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
//    /*
//     * SEND FILES
//     *
//     */
//    // Every Virus in the Monitor has one BloomFilter. So the Monitor will send
//    // to the TravelMonitor numberOfViruses BloomFilters.
//    int numberOfViruses = viruses->getSize();
//    int numberOfBloomFiltersSent = numberOfViruses;
//
//    pipeWriter->openPipe();
//
//    // Send through pipes the number of expected bloom filters
//    pipeWriter->writeNumber(numberOfBloomFiltersSent);
//
//    VirusLinkedListNode *current = viruses->getHead();
//    while(current != NULL) {
//        // Send through pipes the bloom filters of the Monitor
//        BloomFilter* temp = current->getVirus()->getVaccinatedPeopleBloomFilter();
//        pipeWriter->writeBloomFilterInChunks(temp);
//        current = current->next;
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
    ) {
        Helper::handleError("Error: mutex_destroy()", error);
    }

//    pthread_exit ( NULL ) ;
    return 0;
}

char **getCountiesNamesFromCommandLineArguments(int numberOfPaths, char **argv, int argc) {
    char **countryNames = (char **) malloc(numberOfPaths * sizeof(char *));
    for (int i = numberOfPaths - 1; i >= 0; i--) {
        countryNames[i] = argv[argc - 1 - i];
    }

    return countryNames;
}

int getTotalFilesNumber(
    int numberOfCountries,
    char** countryNames,
    int inputDirectoryStringLength,
    char* inputDirectory
) {
    int totalNumberOfFiles = 0;

    char* directoryCharacter = "/";
    int directoryCharacterSize = strlen(directoryCharacter);

    for(int i = 0; i < numberOfCountries; i++) {
        // We build the path for reading the files with the records
        int countryNameSize = strlen(countryNames[i]);
        char *path = (char*) malloc(
                inputDirectoryStringLength + countryNameSize + directoryCharacterSize + 1
        );
        strcpy(path, inputDirectory);
        strcat(path, directoryCharacter);
        strcat(path, countryNames[i]);

        int numberOfFiles = Helper::getAllFilesNumber(path);

        totalNumberOfFiles += numberOfFiles;

        delete path;
    }

    return totalNumberOfFiles;
}

char **getFilesNames(
    int numberOfCountries,
    char** countryNames,
    int inputDirectoryStringLength,
    char* inputDirectory,
    int totalNumberOfFiles
) {
    char** filesNames = (char**) malloc(totalNumberOfFiles * sizeof(char*));
    // Temp variable for keeping index in the double for loop for the filesNames
    // array
    int k = 0;

    char* directoryCharacter = "/";
    int directoryCharacterSize = strlen(directoryCharacter);

    for(int i = 0; i < numberOfCountries; i++) {
        // We build the path for reading the files with the records
        int countryNameSize = strlen(countryNames[i]);
        char *path = (char*) malloc(
                inputDirectoryStringLength + countryNameSize + directoryCharacterSize + 1
        );
        strcpy(path, inputDirectory);
        strcat(path, directoryCharacter);
        strcat(path, countryNames[i]);

        int numberOfFiles = Helper::getAllFilesNumber(path);
        char** countriesFile = Helper::getAllFilesNames(path);

        delete path;
        for(int j = 0; j < numberOfFiles; j++) {
            // We build the path for each records' file
            int inputFileSize = strlen(countriesFile[j]);
            filesNames[k] = (char*) malloc(
                    inputDirectoryStringLength
                    + countryNameSize
                    + 2 * directoryCharacterSize
                    + inputFileSize
                    + 1
            );
            strcpy(filesNames[k], inputDirectory);
            strcat(filesNames[k], directoryCharacter);
            strcat(filesNames[k], countryNames[i]);
            strcat(filesNames[k], directoryCharacter);
            strcat(filesNames[k], countriesFile[j]);

            k++;
        }
    }

    return filesNames;
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

        //Fill Cyclic Buffer's empty positions
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

    pthread_exit (NULL) ;
}