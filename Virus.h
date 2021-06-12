/* Class that represents a Virus. It has a virusName, a SkipList of Vaccinated
 * people, a SkipList of NOT Vaccinated people and a BloomFilter for NOT Vaccinated
 * people*/

#ifndef ERGASIA1_VIRUS_H
#define ERGASIA1_VIRUS_H

#include "NotVaccinatedSkipList.h"
#include "VaccinatedSkipList.h"
#include "BloomFilter.h"


class Virus {
public:
    char *getVirusName() const;

    VaccinatedSkipList *getVaccinatedPeopleList() const;

    NotVaccinatedSkipList *getNotVaccinatedPeopleList() const;

    BloomFilter *getVaccinatedPeopleBloomFilter() const;

    Virus(char *virusName, VaccinatedSkipList *vaccinatedPeopleList, NotVaccinatedSkipList *notVaccinatedPeopleList,
          BloomFilter *notVaccinatedPeopleBloomFilter);

    void printIfPersonIsVaccinated(char*, bool);

private:
    char *virusName;
    VaccinatedSkipList *vaccinatedPeopleList;
    NotVaccinatedSkipList *notVaccinatedPeopleList;
    BloomFilter *vaccinatedPeopleBloomFilter;
};

#endif //ERGASIA1_VIRUS_H
