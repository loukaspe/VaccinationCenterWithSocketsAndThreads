#include <getopt.h>

const char *PROGRAM_OPTIONS = "m:b:c:s:i:t:";
const char *WRONG_PROGRAM_USAGE_ERROR = "Usage %s -m [numMonitors] -b [socketBufferSize] "
                                        "-c [socketBufferSize]  -s [sizeOfBloom] -i [input_dir] "
                                        "-t [numberOfThreads]\n";

int main(int argc, char *argv[]) {
    int opt;

    int numberOfMonitors;
    int socketBufferSize;
    int cyclicBufferSize;
    int bloomSizeInKiloBytes;
    char *inputDirectory;
    int numberOfThreads;

    while ((opt = getopt(argc, argv, PROGRAM_OPTIONS)) != -1) {
        switch (opt) {
            case 'm':
                numberOfMonitors = atoi(optarg);
                break;
            case 'b':
                socketBufferSize = atoi(optarg);
                break;
            case 's':
                bloomSizeInKiloBytes = atoi(optarg);
                break;
            case 'c':
                cyclicBufferSize = atoi(optarg);
                break;
            case 'i':
                inputDirectory = Helper::copyString(optarg);
                break;
            case 't':
                numberOfThreads = atoi(optarg);
                break;
            default:
                Helper::handleError(WRONG_PROGRAM_USAGE_ERROR);
        }
    }

    return 0;
}