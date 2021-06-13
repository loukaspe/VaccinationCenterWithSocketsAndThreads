#include "Helper.h"

const char *Helper::STRING_COPY_ERROR = "ERROR: Something went wrong with Helper Copy\n";

void Helper::handleError(const char *errorMessage, int errorNumber) {
    if (errorNumber == 0) {
        fprintf(
                stderr,
                "%s",
                errorMessage
        );
    }
    else {
        fprintf(
                stderr,
                "%s with errorNo %d",
                errorMessage,
                errorNumber
        );
    }

    exit(EXIT_FAILURE);
}


char *Helper::copyString(char *source) {
    char *destination;
    int stringLength = strlen(source);

    destination = (char *) malloc(
            stringLength * sizeof(char)
    );

    if (destination == NULL) {
        fprintf(stderr, "%s", STRING_COPY_ERROR);
        exit(EXIT_FAILURE);
    }

    strcpy(destination, source);
    return destination;
}

bool Helper::inArray(char *needle, char **haystack, int haystackSize) {
    int i;
    for (i = 0; i < haystackSize; i++) {
        if (strcmp(needle, haystack[i]) == 0) {
            return true;
        }
    }

    return false;
}

bool Helper::hasStringNewLineCharacterInTheEnd(char *string) {
    int length = strlen(string);
    return string[length - 1] == '\n';
}

char *Helper::removeNewLineCharacterFromString(char *string) {
    if (hasStringNewLineCharacterInTheEnd(string)) {
        int length = strlen(string);
        char *stringWithoutNewLine = (char *) malloc(
                length * sizeof(char)
        );

        int i;
        for (i = 0; i < length; i++) {
            if (string[i] == '\n') {
                stringWithoutNewLine[i] = '\0';
                continue;
            }

            stringWithoutNewLine[i] = string[i];
        }

        return stringWithoutNewLine;
    }

    return string;
}

char **Helper::getAllSubdirectoriesNames(char *path) {
    int numberOfSubdirectories = 0;
    char **subdirectories = NULL;
    DIR *directory = opendir(path);
    struct dirent *directoryEntry;
    struct stat sb;

    if (directory == NULL) {
        cout << "ERROR: " << errno << endl;
        Helper::handleError("Could not open current directory");
    }

    while ((directoryEntry = readdir(directory)) != NULL) {
        // We don't count as subdirectory the current directory, the parent
        // directory and all other file types
        if (
                strcmp(directoryEntry->d_name, ".") == 0
                || strcmp(directoryEntry->d_name, "..") == 0
                || fstatat(
                        dirfd(directory),
                        directoryEntry->d_name,
                        &sb,
                        0
                ) < 0
                || !S_ISDIR(sb.st_mode)
                ) {
            continue;
        }
        subdirectories = (char **) realloc(
                subdirectories,
                (numberOfSubdirectories + 1) * sizeof(char *)
        );
        subdirectories[numberOfSubdirectories] = Helper::copyString(directoryEntry->d_name);
        numberOfSubdirectories++;
    }

    closedir(directory);
    return subdirectories;
}

int Helper::getAllSubdirectoriesNumber(char *path) {
    int numberOfSubdirectories = 0;
    DIR *directory = opendir(path);
    struct dirent *directoryEntry;
    struct stat sb;

    if (directory == NULL) {
        cout << "ERROR: " << errno << endl;
        Helper::handleError("Could not open current directory");
    }

    while ((directoryEntry = readdir(directory)) != NULL) {
        // We don't count as subdirectory the current directory, the parent
        // directory and all other file types
        if (
                strcmp(directoryEntry->d_name, ".") == 0
                || strcmp(directoryEntry->d_name, "..") == 0
                || fstatat(
                        dirfd(directory),
                        directoryEntry->d_name,
                        &sb,
                        0
                ) < 0
                || !S_ISDIR(sb.st_mode)
                ) {
            continue;
        }
        numberOfSubdirectories++;
    }

    closedir(directory);
    return numberOfSubdirectories;
}

/* Function that given two ints, it divides them and the the ceiling of their quotient */
int Helper::getCeilingOfDividedInts(int a, int b) {
    return (int) ceil(
            (float) a / (float) b
    );
}

char **Helper::getAllFilesNames(char *path) {
    int numberOfFiles = 0;
    char **files = NULL;
    DIR *directory = opendir(path);
    struct dirent *directoryEntry;
    struct stat sb;

    if (directory == NULL) {
        cout << "ERROR: " << errno << endl;
        Helper::handleError("Could not open current directory");
    }

    while ((directoryEntry = readdir(directory)) != NULL) {
        if (
                fstatat(
                        dirfd(directory),
                        directoryEntry->d_name,
                        &sb,
                        0
                ) < 0
                || S_ISDIR(sb.st_mode)
                ) {
            continue;
        }

        files = (char **) realloc(
                files,
                (numberOfFiles + 1) * sizeof(char *)
        );

        files[numberOfFiles] = Helper::copyString(directoryEntry->d_name);
        numberOfFiles++;
    }

    closedir(directory);
    return files;
}

int Helper::getAllFilesNumber(char *path) {
    int numberOfFiles = 0;
    DIR *directory = opendir(path);
    struct dirent *directoryEntry;
    struct stat sb;

    if (directory == NULL) {
        cout << "ERROR: " << errno << endl;
        Helper::handleError("Could not open current directory");
    }

    while ((directoryEntry = readdir(directory)) != NULL) {
        if (
                fstatat(
                        dirfd(directory),
                        directoryEntry->d_name,
                        &sb,
                        0
                ) < 0
                || S_ISDIR(sb.st_mode)
                ) {
            continue;
        }

        numberOfFiles++;
    }

    closedir(directory);
    return numberOfFiles;
}


char** Helper::getCountiesNamesFromLastCommandLineArguments(int numberOfPaths, char **argv, int argc) {
    char **countryNames = (char **) malloc(numberOfPaths * sizeof(char *));
    for (int i = numberOfPaths - 1; i >= 0; i--) {
        countryNames[i] = argv[argc - 1 - i];
    }

    return countryNames;
}

int Helper::getTotalFilePathsNumber(
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

char** Helper::getFilePathsNames(
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

