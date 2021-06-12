/*
 * Class for handling Dates
 * */

#ifndef ERGASIA1_DATE_H
#define ERGASIA1_DATE_H

#include <time.h>

#include "Helper.h"

using namespace std;

class Date {
public:
    Date(char*);
    char* toString() const;
    bool isBiggerOrEqualThan(Date*);
    bool isSmallerOrEqualThan(Date*);
private:
    time_t dateTimeInSeconds;
    tm dateTimeObject;
    char* dateAsString;
    int year;
    int month;
    int day;
};


#endif //ERGASIA1_DATE_H
