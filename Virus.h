/* Class that represents a Virus. It has a virusName, a SkipList of Vaccinated
 * people, a SkipList of NOT Vaccinated people and a LinkedList of BloomFilters
 * for NOT Vaccinated people, one for each country
 * */

#ifndef ERGASIA1_VIRUS_H
#define ERGASIA1_VIRUS_H

#include "NotVaccinatedSkipList.h"
#include "VaccinatedSkipList.h"
#include "BloomFilterLinkedList.h"


class Virus {
public:
    char *getVirusName() const;

    VaccinatedSkipList *getVaccinatedPeopleList() const;

    NotVaccinatedSkipList *getNotVaccinatedPeopleList() const;

    BloomFilterLinkedList *getVaccinatedPeopleBloomFilterLinkedList() const;

    BloomFilter* getVaccinatedPeopleBloomFilterByCountry(char*) const;

    Virus(char *, VaccinatedSkipList *, NotVaccinatedSkipList *, BloomFilterLinkedList *);

    void printIfPersonIsVaccinated(char*, bool);

private:
    char *virusName;
    VaccinatedSkipList *vaccinatedPeopleList;
    NotVaccinatedSkipList *notVaccinatedPeopleList;
    BloomFilterLinkedList *vaccinatedPeopleBloomFilterLinkedList;
};

#endif //ERGASIA1_VIRUS_H
