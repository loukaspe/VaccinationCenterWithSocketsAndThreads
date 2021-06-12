#include "VaccinatedPersonSkipListNode.h"

VaccinatedPersonSkipListNode::VaccinatedPersonSkipListNode() {
    key = VACCINATED_LIST_KEY;
    vaccination = NULL;
    this->next = new VaccinatedPersonSkipListNode *[MAX_LEVEL];
    for (int i = 0; i < MAX_LEVEL; i++) {
        next[i] = NULL;
    }
}

VaccinatedPersonSkipListNode::VaccinatedPersonSkipListNode(int key) : key(key) {
    this->vaccination = NULL;
    this->next = new VaccinatedPersonSkipListNode *[MAX_LEVEL];
    for (int i = 0; i < MAX_LEVEL; i++) {
        next[i] = NULL;
    }
}

VaccinatedPersonSkipListNode::~VaccinatedPersonSkipListNode() {
    delete next;
}

Vaccination* VaccinatedPersonSkipListNode::getVaccination() const {
    return vaccination;
}

void VaccinatedPersonSkipListNode::setVaccination(Vaccination* vaccination) {
    this->vaccination = vaccination;
}

int VaccinatedPersonSkipListNode::getKey() const {
    return key;
}

VaccinatedPersonSkipListNode *VaccinatedPersonSkipListNode::getNextNodeWithKey(int key) const {
    return next[key];
}

void VaccinatedPersonSkipListNode::setNextNodeWithKeyTo(int key, VaccinatedPersonSkipListNode *givenNode) {
    this->next[key] = givenNode;
}

void VaccinatedPersonSkipListNode::printVaccinationInfo() {
    this->vaccination->printInfo();

}
