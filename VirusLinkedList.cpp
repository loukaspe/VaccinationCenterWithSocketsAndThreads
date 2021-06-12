#include "VirusLinkedList.h"

VirusLinkedList::VirusLinkedList() {
    this->head = NULL;
    this->size = 0;
}


void VirusLinkedList::addAtStart(Virus *virus) {
    VirusLinkedListNode *newNode = new VirusLinkedListNode(virus);

    newNode->next = this->head;
    this->head = newNode;
    this->size++;
}

VirusLinkedListNode *VirusLinkedList::findByName(char *virusName) {
    VirusLinkedListNode *current = this->head;
    while (current != NULL) {
        if (strcmp(current->getVirus()->getVirusName(), virusName) == 0) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

void VirusLinkedList::traverseAndCheckIfVaccinated(
        VirusLinkedListNode *initialRecursiveNode,
        char *citizenId
) {
    if (initialRecursiveNode == NULL) {
        return;
    }

    Virus *tempVirus = initialRecursiveNode->getVirus();
    bool shouldPrintVirusName = true;
    tempVirus->printIfPersonIsVaccinated(citizenId, shouldPrintVirusName);

    this->traverseAndCheckIfVaccinated(initialRecursiveNode->next, citizenId);
}

void VirusLinkedList::checkIfVaccinatedForAllViruses(char *citizenId) {
    this->traverseAndCheckIfVaccinated(this->head, citizenId);
}

int VirusLinkedList::getSize() const {
    return size;
}

VirusLinkedListNode *VirusLinkedList::getHead() const {
    return head;
}
