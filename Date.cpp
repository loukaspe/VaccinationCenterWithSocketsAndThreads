#include "Date.h"

Date::Date(char *dateAsString) {
    this->dateAsString = Helper::copyString(dateAsString);

    this->dateTimeObject = {0};

    sscanf(
            this->dateAsString,
            "%2d-%2d-%4d",
            &day,
            &month,
            &year
    );

    this->dateTimeObject.tm_year = year - 1900;
    this->dateTimeObject.tm_mon = month - 1;
    this->dateTimeObject.tm_mday = day;

    this->dateTimeInSeconds = mktime(&dateTimeObject);
}

bool Date::isBiggerOrEqualThan(Date *givenDate) {
    return (this->dateTimeInSeconds >= givenDate->dateTimeInSeconds);
}

bool Date::isSmallerOrEqualThan(Date *givenDate) {
    return (this->dateTimeInSeconds <= givenDate->dateTimeInSeconds);
}

char *Date::toString() const {
    return this->dateAsString;
}
