/*
 * Class for operating on people and viruses and performing the functions of the
 * Exercise. It will be injected on the MenuPromptCreator
 * */

#ifndef ERGASIA1_VACCINATIONCENTER_H
#define ERGASIA1_VACCINATIONCENTER_H


#include "Date.h"
#include "PersonLinkedList.h"
#include "VirusLinkedList.h"
#include "CountryLinkedList.h"

class VaccinationCenter {
public:
    VaccinationCenter(PersonLinkedList *, VirusLinkedList *, CountryLinkedList *);

    void vaccineStatusBloom(char *citizenId, char *virusName);

    void vaccineStatusForAllViruses(char *citizenId);

    void vaccineStatusForSpecificVirus(char *citizenId, char *virusName);

    void populationStatusForAllCountries(char *virusName, Date *date1, Date *date2);

    void populationStatusForCountry(
            char *country,
            char *virusName,
            Date *date1,
            Date *date2
    );

    void popStatusByAgeForAllCountries(char *virusName, Date *date1, Date *date2);

    void popStatusByAgeForCountry(char *country, char *virusName, Date *date1, Date *date2);

    void insertVaccinated(
            char *citizenId,
            char *firstName,
            char *lastName,
            char *country,
            int age,
            char *virusName,
            Date *date
    );

    void insertNotVaccinated(
            char *citizenId,
            char *firstName,
            char *lastName,
            char *country,
            int age,
            char *virusName
    );

    void vaccinateNow(
            char *citizenId,
            char *firstName,
            char *lastName,
            char *country,
            int age,
            char *virusName
    );

    void listNotVaccinatedPersonsForVirus(char *virusName);

    void checkAndAddPerson(char *, char *, char *, char *, int);

    void checkAndAddCountry(char *);

    void checkAndAddVirus(char *, char*);

    PersonLinkedList *getPeople() const;

    VirusLinkedList *getViruses() const;
private:
    VirusLinkedList *viruses;
    CountryLinkedList *countries;
    PersonLinkedList *people;
};


#endif //ERGASIA1_VACCINATIONCENTER_H
