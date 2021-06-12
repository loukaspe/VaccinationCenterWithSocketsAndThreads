#include "NotVaccinatedPersonSkipListNode.h"

NotVaccinatedPersonSkipListNode::NotVaccinatedPersonSkipListNode() {
    key = DEFAULT_NOT_VACCINATED_LIST_KEY;
    person = NULL;
    this->next = new NotVaccinatedPersonSkipListNode *[MAX_LEVEL];
    for (int i = 0; i < MAX_LEVEL; i++) {
        next[i] = NULL;
    }
}

NotVaccinatedPersonSkipListNode::NotVaccinatedPersonSkipListNode(int key) : key(key) {
    this->person = NULL;
    this->next = new NotVaccinatedPersonSkipListNode *[MAX_LEVEL];
    for (int i = 0; i < MAX_LEVEL; i++) {
        next[i] = NULL;
    }
}

NotVaccinatedPersonSkipListNode::~NotVaccinatedPersonSkipListNode() {
    delete next;
}

Person* NotVaccinatedPersonSkipListNode::getPerson() const {
    return person;
}

void NotVaccinatedPersonSkipListNode::setPerson(Person* value) {
    NotVaccinatedPersonSkipListNode::person = value;
}

int NotVaccinatedPersonSkipListNode::getKey() const {
    return key;
}

NotVaccinatedPersonSkipListNode *NotVaccinatedPersonSkipListNode::getNextNodeWithKey(int key) const {
    return next[key];
}

void NotVaccinatedPersonSkipListNode::setNextNodeWithKeyTo(int key, NotVaccinatedPersonSkipListNode *givenNode) {
    this->next[key] = givenNode;
}

void NotVaccinatedPersonSkipListNode::printPersonInfo() {
    this->person->printInfo();
}
