#ifndef UNTITLED_HELPER_H
#define UNTITLED_HELPER_H

/* Helper Class contains various function that should not technically be
 * part of other classes and have more general usage */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <dirent.h>
#include <sys/stat.h>

using namespace std;

class Helper {
public:
    static char* copyString(char*);
    static void handleError(const char*, int = 0);
    static bool inArray(char*, char**, int);
    static char* removeNewLineCharacterFromString(char*);
    static char** getAllSubdirectoriesNames(char* path);
    static int getAllSubdirectoriesNumber(char* path);
    static char** getAllFilesNames(char* path);
    static int getAllFilesNumber(char* path);
    static int getCeilingOfDividedInts(int, int);
private:
    static const char* STRING_COPY_ERROR;
    static bool hasStringNewLineCharacterInTheEnd(char*);
};

#endif //UNTITLED_HELPER_H