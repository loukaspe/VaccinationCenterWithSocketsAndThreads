#ifndef ERGASIA1_PERSONLINKEDLISTNODE_H
#define ERGASIA1_PERSONLINKEDLISTNODE_H

#include "Person.h"

class PersonLinkedListNode {
public:
    PersonLinkedListNode *next;
    Person *person;

    PersonLinkedListNode(Person *);
};


#endif //ERGASIA1_PERSONLINKEDLISTNODE_H
