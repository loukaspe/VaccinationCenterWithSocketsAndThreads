#ifndef ERGASIA1_NOTVACCINATEDPERSONSKIPLISTNODE_H
#define ERGASIA1_NOTVACCINATEDPERSONSKIPLISTNODE_H

#include "Person.h"

#ifndef MAX_LEVEL
#define MAX_LEVEL 16
#endif

using namespace std;

static int DEFAULT_NOT_VACCINATED_LIST_KEY = -1;

class NotVaccinatedPersonSkipListNode {
private:
    Person* person;
    int key;
    NotVaccinatedPersonSkipListNode **next;
public:
    NotVaccinatedPersonSkipListNode();

    NotVaccinatedPersonSkipListNode(int key);

    ~NotVaccinatedPersonSkipListNode();

    Person* getPerson() const;

    void setPerson(Person* value);

    int getKey() const;

    NotVaccinatedPersonSkipListNode *getNextNodeWithKey(int key) const;

    void setNextNodeWithKeyTo(int key, NotVaccinatedPersonSkipListNode *givenNode);

    void printPersonInfo();
};


#endif //ERGASIA1_NOTVACCINATEDPERSONSKIPLISTNODE_H
