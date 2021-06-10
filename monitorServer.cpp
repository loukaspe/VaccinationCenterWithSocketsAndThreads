#include "MonitorServer.h"
#include <getopt.h>
#include "Socket.h"

static const int NUMBER_OF_OPTIONS_ARGUMENTS = 10;
using namespace std;

const char *PROGRAM_OPTIONS = "p:t:b:c:s:";
const char *WRONG_PROGRAM_USAGE_ERROR = "Usage %s -p [port] -b [socketBufferSize] "
                                        "-c [socketBufferSize]  -s [sizeOfBloom] -t [numberOfThreads]\n";

char **getPathsNameFromCommandLineArguments(int, char **, int);

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
    int numberOfPaths = argc - NUMBER_OF_OPTIONS_ARGUMENTS - 1;

    char **paths = getPathsNameFromCommandLineArguments(numberOfPaths, argv, argc);

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

    /************************************
     * TESTS FOR SOCKETS
     ************************************/

    char* test = "this is a huge string, like my .. hmmm lets get serious this is a very big string end.";
    int testLength = strlen(test) + 1;

    // Number, String, Number
    socket->writeNumber(testLength);
    socket->writeStringInChunks(test);
    socket->writeNumber(5);

    // Array Of Ints
    int numberOfElements = 819200;
    int array[numberOfElements];
    for(int i = 0; i < numberOfElements; i++) {
        array[i] = i;
    }
    socket->writeNumber(numberOfElements);
    socket->writeArrayOfInts(array, numberOfElements);

    // BitArray
    BitArray *bitArray = new BitArray(numberOfElements);
    bitArray->setBitArray(array);
    socket->writeBitArrayInChunks(bitArray);

    socket->closeSocket();

    return 0;
}

char **getPathsNameFromCommandLineArguments(int numberOfPaths, char **argv, int argc) {
    char **paths = (char **) malloc(numberOfPaths * sizeof(char *));
    for (int i = numberOfPaths - 1; i >= 0; i--) {
        paths[i] = argv[argc - 1 - i];
    }

    return paths;
}