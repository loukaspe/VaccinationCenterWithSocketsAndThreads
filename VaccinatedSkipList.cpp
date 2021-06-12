#include "VaccinatedSkipList.h"

VaccinatedSkipList::VaccinatedSkipList() :
        currentHighestLevel(STARTING_VACCINATED_LIST_LEVEL),
        numberOfNodesExceptFirstAndLast(STARTING_VACCINATED_LIST_SIZE) {
    firstNode = new VaccinatedPersonSkipListNode();
    lastNode = new VaccinatedPersonSkipListNode(MAX_SKIP_LIST_SIZE);

    for (int tempKey = 0; tempKey < MAX_LEVEL; tempKey++) {
        firstNode->setNextNodeWithKeyTo(tempKey, lastNode);
    }
}

VaccinatedSkipList::~VaccinatedSkipList() {
    VaccinatedPersonSkipListNode *tempNode;
    VaccinatedPersonSkipListNode *tempNode2;

    tempNode = firstNode->getNextNodeWithKey(0);
    while (tempNode != NULL) {
        tempNode2 = tempNode;
        tempNode = tempNode->getNextNodeWithKey(0);
        delete tempNode2;
    }

    delete firstNode;
}

Vaccination *VaccinatedSkipList::search(int searchKey) {
    VaccinatedPersonSkipListNode *tempNode = firstNode;

    for (int i = MAX_LEVEL - 1; i >= 0; --i) {
        while ((tempNode->getNextNodeWithKey(i))->getKey() < searchKey) {
            tempNode = tempNode->getNextNodeWithKey(i);
        }
    }

    tempNode = tempNode->getNextNodeWithKey(0);

    if (tempNode->getKey() == searchKey) {
        return tempNode->getVaccination();
    }

    return NULL;
}

void VaccinatedSkipList::insert(int searchKey, Vaccination *newVaccination) {
    VaccinatedPersonSkipListNode **nodesThatMayNeedToBeUpdated = new VaccinatedPersonSkipListNode *[MAX_LEVEL];
    int newLevel;
    VaccinatedPersonSkipListNode *tempNode = firstNode;

    for (int i = MAX_LEVEL - 1; i >= 0; --i) {
        while ((tempNode->getNextNodeWithKey(i))->getKey() < searchKey) {
            tempNode = tempNode->getNextNodeWithKey(i);
        }

        nodesThatMayNeedToBeUpdated[i] = tempNode;
    }

    tempNode = tempNode->getNextNodeWithKey(0);
    if (tempNode->getKey() == searchKey) {
        delete nodesThatMayNeedToBeUpdated;
        return;
    }

    newLevel = setLevelOfNodeByCalculatingPossibilities();
    if (newLevel > currentHighestLevel) {
        currentHighestLevel = newLevel;
    }
    tempNode = new VaccinatedPersonSkipListNode(atoi(newVaccination->getPerson()->getCitizenId()));
    tempNode->setVaccination(newVaccination);
    for (int i = 0; i <= newLevel; ++i) {
        tempNode->setNextNodeWithKeyTo(
                i,
                nodesThatMayNeedToBeUpdated[i]->getNextNodeWithKey(i)
        );
        nodesThatMayNeedToBeUpdated[i]->setNextNodeWithKeyTo(i, tempNode);
    }

    numberOfNodesExceptFirstAndLast++;

    delete nodesThatMayNeedToBeUpdated;
}

int VaccinatedSkipList::setLevelOfNodeByCalculatingPossibilities() {
    srand(time(NULL));
    int level = STARTING_VACCINATED_LIST_LEVEL;
    while (
            (rand() % 100) < VACCINATED_LIST_KEY_POSSIBILITY_LIMIT_FOR_UPDATING_LEVEL
            && level < MAX_LEVEL
            ) {
        level++;
    }

    return level;
}

CountryVaccinationStatistics VaccinatedSkipList::getNumberOfCitizensOfCountryVaccinatedBetweenDates(
        char *countryName,
        Date *date1,
        Date *date2
) {
    VaccinatedPersonSkipListNode *tempNode = firstNode->getNextNodeWithKey(0);
    if (tempNode == lastNode) {
        return {0};
    }

    CountryVaccinationStatistics statistics = {0};
    Vaccination *tempVaccination;
    Person *tempPerson;
    Date *tempDate;
    int tempAge;

    while (tempNode != lastNode) {
        tempVaccination = tempNode->getVaccination();
        tempPerson = tempVaccination->getPerson();
        tempAge = tempPerson->getAge();
        tempDate = tempVaccination->getDate();
        if (
                tempVaccination != NULL
                && tempPerson != NULL
                && tempDate != NULL
                && tempPerson->getCountry() == countryName
                && tempDate->isBiggerOrEqualThan(date1)
                && tempDate->isSmallerOrEqualThan(date2)
                ) {
            statistics.numberOfVaccinatedCitizens++;

            if (tempAge <= 20) {
                statistics.numberOfVaccinatedCitizensUnder20yearsOld++;
            }
            else if (tempAge > 20 && tempAge <= 40) {
                statistics.numberOfVaccinatedCitizensFrom20To40yearsOld++;
            }
            else if (tempAge > 40 && tempAge <= 60) {
                statistics.numberOfVaccinatedCitizensFrom40To60yearsOld++;
            }
            else if (tempAge > 60) {
                statistics.numberOfVaccinatedCitizensOver60yearsOld;
            }
        }

        tempNode = tempNode->getNextNodeWithKey(0);
    }

    return statistics;
}