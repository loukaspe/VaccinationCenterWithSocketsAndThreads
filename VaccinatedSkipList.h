#ifndef ERGASIA1_VACCINATEDSKIPLIST_H
#define ERGASIA1_VACCINATEDSKIPLIST_H

#ifndef MAX_SKIP_LIST_SIZE
#define MAX_SKIP_LIST_SIZE 100000000
#endif

#include "VaccinatedPersonSkipListNode.h"


struct CountryVaccinationStatistics {
    int numberOfVaccinatedCitizens;
    int numberOfVaccinatedCitizensUnder20yearsOld;
    int numberOfVaccinatedCitizensFrom20To40yearsOld;
    int numberOfVaccinatedCitizensFrom40To60yearsOld;
    int numberOfVaccinatedCitizensOver60yearsOld;
};

static const int STARTING_VACCINATED_LIST_LEVEL = 1;
static const int STARTING_VACCINATED_LIST_SIZE = 0;
static const int VACCINATED_LIST_KEY_POSSIBILITY_LIMIT_FOR_UPDATING_LEVEL = 25;

class VaccinatedSkipList {
private:
    VaccinatedPersonSkipListNode *firstNode;
    VaccinatedPersonSkipListNode *lastNode;
    int currentHighestLevel;
    int numberOfNodesExceptFirstAndLast;

    int setLevelOfNodeByCalculatingPossibilities();

public:
    VaccinatedSkipList();

    virtual ~VaccinatedSkipList();

    Vaccination* search(int searchKey);

    void insert(int searchKey, Vaccination* newVaccination);

    CountryVaccinationStatistics getNumberOfCitizensOfCountryVaccinatedBetweenDates(char *, Date *, Date *);
};


#endif //ERGASIA1_VACCINATEDSKIPLIST_H
