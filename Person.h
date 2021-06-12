#ifndef ERGASIA1_PERSON_H
#define ERGASIA1_PERSON_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

using namespace std;

class Person {
private:
    char *citizenId;
    char *firstName;
    char *lastName;
    char *country;
    int age;
public:
    virtual ~Person();

    Person(char *citizenId, char *firstName, char *lastName, char *country, int age);

    char *getCitizenId() const;

    char *getFirstName() const;

    char *getLastName() const;

    char *getCountry() const;

    int getAge() const;

    void printInfo();
};

#endif //ERGASIA1_PERSON_H
