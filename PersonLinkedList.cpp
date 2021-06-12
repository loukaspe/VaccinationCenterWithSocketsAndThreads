#include "PersonLinkedList.h"

PersonLinkedList::PersonLinkedList() {
    this->head = NULL;
    this->size = 0;
}


void PersonLinkedList::addAtStart(Person* person) {
    PersonLinkedListNode* newNode = new PersonLinkedListNode(person);

    newNode->next = this->head;
    this->head = newNode;
    this->size++;
}

PersonLinkedListNode *PersonLinkedList::findByCitizenId(char * citizenId) {
    PersonLinkedListNode* current = this->head;
    while (current != NULL)
    {
        if (strcmp(current->person->getCitizenId(), citizenId) == 0) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

