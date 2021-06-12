/* A simple Linked List to hold the People */

#ifndef ERGASIA1_PERSONLINKEDLIST_H
#define ERGASIA1_PERSONLINKEDLIST_H

#include "./PersonLinkedListNode.h"

class PersonLinkedList {
public:
    PersonLinkedList();
    void addAtStart(Person*);
    PersonLinkedListNode* findByCitizenId(char*);
private:
    PersonLinkedListNode* head;
    int size;
};


#endif //ERGASIA1_PERSONLINKEDLIST_H
