#ifndef ERGASIA1_VACCINATEDPERSONSKIPLISTNODE_H
#define ERGASIA1_VACCINATEDPERSONSKIPLISTNODE_H

#include "Vaccination.h"

#ifndef MAX_LEVEL
#define MAX_LEVEL 16
#endif

static int VACCINATED_LIST_KEY = -1;

class VaccinatedPersonSkipListNode {
private:
    Vaccination* vaccination;
    int key;
    VaccinatedPersonSkipListNode **next;
public:
    VaccinatedPersonSkipListNode();

    VaccinatedPersonSkipListNode(int key);

    ~VaccinatedPersonSkipListNode();

    Vaccination* getVaccination() const;

    void setVaccination(Vaccination* vaccination);

    int getKey() const;

    VaccinatedPersonSkipListNode *getNextNodeWithKey(int key) const;

    void setNextNodeWithKeyTo(int key, VaccinatedPersonSkipListNode *givenNode);

    void printVaccinationInfo();
};

#endif //ERGASIA1_VACCINATEDPERSONSKIPLISTNODE_H
