#ifndef ERGASIA1_VACCINATION_H
#define ERGASIA1_VACCINATION_H

#include "Person.h"
#include "Date.h"

class Vaccination {
private:
    Person* person;
    Date* date;
public:
    Vaccination(Person *person, Date *date);

    Person *getPerson() const;

    Date* getDate() const;

    void printInfo();
};


#endif //ERGASIA1_VACCINATION_H
