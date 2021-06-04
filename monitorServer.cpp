#include <getopt.h>


static const int NUMBER_OF_OPTIONS_ARGUMENTS = 10;
using namespace std;

const char *PROGRAM_OPTIONS = "p:t:b:c:s:";
const char *WRONG_PROGRAM_USAGE_ERROR = "Usage %s -p [port] -b [socketBufferSize] "
                                        "-c [socketBufferSize]  -s [sizeOfBloom] -t [numberOfThreads]\n";

char** getPathsNameFromCommandLineArguments(int , char**, int);

int main(int argc, char **argv) {
    int opt;

    int port;
    int socketBufferSize;
    int cyclicBufferSize;
    int bloomSizeInKiloBytes;
    char *inputDirectory;
    int numberOfThreads;
    // Number of paths given to the monitor server is equal to the argc - numberOfOptionArguments
    // (for example -b bufferSize) - 1 (the program name)
    int numberOfPaths = argc - NUMBER_OF_OPTIONS_ARGUMENTS - 1;

    char** paths = getPathsNameFromCommandLineArguments(numberOfPaths, argv, argc);


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


    return 0;
}

char** getPathsNameFromCommandLineArguments(int numberOfPaths, char** argv, int argc)
{
    char** paths = (char**) malloc(numberOfPaths * sizeof(char*));
    for(int i = numberOfPaths - 1; i >= 0; i--) {
        paths[i] = argv[argc - 1 - i];
    }

    return paths;
}