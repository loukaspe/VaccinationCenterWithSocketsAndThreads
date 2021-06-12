#ifndef ERGASIA1_NOTVACCINATEDSKIPLIST_H
#define ERGASIA1_NOTVACCINATEDSKIPLIST_H

#ifndef MAX_SKIP_LIST_SIZE
#define MAX_SKIP_LIST_SIZE 100000000
#endif

#include "NotVaccinatedPersonSkipListNode.h"

static const int STARTING_NOT_VACCINATED_LIST_LEVEL = 1;
static const int STARTING_NOT_VACCINATED_LIST_SIZE = 0;
static const int NOT_VACCINATED_LIST_KEY_POSSIBILITY_LIMIT_FOR_UPDATING_LEVEL = 25;

class NotVaccinatedSkipList {
private:
    NotVaccinatedPersonSkipListNode *firstNode;
    NotVaccinatedPersonSkipListNode *lastNode;
    int currentHighestLevel;
    int numberOfNodesExceptFirstAndLast;

    int setLevelOfNodeByCalculatingPossibilities();

public:
    NotVaccinatedSkipList();

    virtual ~NotVaccinatedSkipList();

    Person* search(int searchKey);

    void insert(int searchKey, Person* newPerson);

    void print();
};


#endif //ERGASIA1_NOTVACCINATEDSKIPLIST_H
