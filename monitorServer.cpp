#include "PipeReader.h"
#include "PipeWriter.h"
#include "MonitorServer.h"
#include <getopt.h>
#include <netinet/in.h>
#include <netdb.h>


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

    // Socket variables
    int socketFd;
    struct sockaddr_in server;
    struct sockaddr *serverPointer = (struct sockaddr *) &server;


    // Number of paths given to the monitor server is equal to the argc - numberOfOptionArguments
    // (for example -b bufferSize) - 1 (the program name)
    int numberOfPaths = argc - NUMBER_OF_OPTIONS_ARGUMENTS - 1;

    char **paths = getPathsNameFromCommandLineArguments(numberOfPaths, argv, argc);

    for (int i = 0; i < argc; i++) {
        cout << argv[i] << endl;
    }

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

    /* Create socket */
    if ((socketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        Helper::handleError("Error creating socket");
    }

    int hostNameResult = gethostname(hostname, sizeof(hostname));
    if (hostNameResult == -1) {
        Helper::handleError("Error getting hostname");
    }

    host = gethostbyname(hostname);

    server.sin_family = AF_INET;
    memcpy(&server.sin_addr, host->h_addr, host->h_length);
    server.sin_port = htons(port);

    /* Initiate connection */
    if (connect(socketFd, serverPointer, sizeof(server)) < 0) {
        Helper::handleError("Error connecting to socket", errno);
    }

//    do
//        connectStatus = connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
//    while (connectStatus < 0);

    int a = 5;

    if (write(socketFd, &a, sizeof(int)) < 0) {
        Helper::handleError("Error writing to socket");
    }

    close(socketFd);

    return 0;
}

char **getPathsNameFromCommandLineArguments(int numberOfPaths, char **argv, int argc) {
    char **paths = (char **) malloc(numberOfPaths * sizeof(char *));
    for (int i = numberOfPaths - 1; i >= 0; i--) {
        paths[i] = argv[argc - 1 - i];
    }

    return paths;
}