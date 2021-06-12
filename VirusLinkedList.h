/* A simple Linked List to hold the Viruses. Don't care about it's complexity since
 * the viruses' number is limited */

#ifndef ERGASIA1_VIRUSLINKEDLIST_H
#define ERGASIA1_VIRUSLINKEDLIST_H

#include "./VirusLinkedListNode.h"

class VirusLinkedList {
public:
    VirusLinkedList();
    void addAtStart(Virus *);
    VirusLinkedListNode *findByName(char *);
    void checkIfVaccinatedForAllViruses(char*);
    int getSize() const;
    VirusLinkedListNode *getHead() const;
private:
    void traverseAndCheckIfVaccinated(VirusLinkedListNode *, char *);
    VirusLinkedListNode *head;
    int size;
};


#endif //ERGASIA1_VIRUSLINKEDLIST_H
