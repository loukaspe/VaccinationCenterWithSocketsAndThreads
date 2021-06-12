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

int main(int argc, char **argv) {
    int opt;
    int port;
    int socketBufferSize;
    int cyclicBufferSize;
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

    int totalNumberOfFiles = getTotalFilesNumber(
        numberOfCountryNames,
        countryNames,
        inputDirectoryStringLength,
        inputDirectory
    );

//    cout << "T " << totalNumberOfFiles << endl;

    char** filesNames = getFilesNames(
        numberOfCountryNames,
        countryNames,
        inputDirectoryStringLength,
        inputDirectory,
        totalNumberOfFiles
    );

//    for(int k = 0; k < totalNumberOfFiles; k++) {
//        cout << filesNames[k] << endl;
//    }

    /*
     *
    PersonLinkedList *people = new PersonLinkedList();
    VirusLinkedList *viruses = new VirusLinkedList();
    CountryLinkedList *countries = new CountryLinkedList();

    VaccinationCenter *vaccinationCenter = new VaccinationCenter(
            people,
            viruses,
            countries
    );

    CitizenRecordsFileReader *fileReader = new CitizenRecordsFileReader(
            vaccinationCenter
    );

    Monitor* monitor = new Monitor(
        people,
        viruses,
        countries,
        vaccinationCenter,
        fileReader
    );
    */

    /*
     * SEND FILES
     *
     *
    // Every Virus in the Monitor has one BloomFilter. So the Monitor will send
    // to the TravelMonitor numberOfViruses BloomFilters.
    int numberOfViruses = viruses->getSize();
    int numberOfBloomFiltersSent = numberOfViruses;

    pipeWriter->openPipe();

    // Send through pipes the number of expected bloom filters
    pipeWriter->writeNumber(numberOfBloomFiltersSent);

    VirusLinkedListNode *current = viruses->getHead();
    while(current != NULL) {
        // Send through pipes the bloom filters of the Monitor
        BloomFilter* temp = current->getVirus()->getVaccinatedPeopleBloomFilter();
        pipeWriter->writeBloomFilterInChunks(temp);
        current = current->next;
    }
     *
     */
    socket->closeSocket();

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
            path = (char*) malloc(
                    inputDirectoryStringLength
                    + countryNameSize
                    + 2 * directoryCharacterSize
                    + inputFileSize
                    + 1
            );
            strcpy(path, inputDirectory);
            strcat(path, directoryCharacter);
            strcat(path, countryNames[i]);
            strcat(path, directoryCharacter);
            strcat(path, countriesFile[j]);

            filesNames[k] = Helper::copyString(path);
            k++;
            delete path;
        }
    }

    return filesNames;
}
