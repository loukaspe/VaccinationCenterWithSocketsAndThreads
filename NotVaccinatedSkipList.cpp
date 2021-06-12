#include "NotVaccinatedSkipList.h"

NotVaccinatedSkipList::NotVaccinatedSkipList() :
        currentHighestLevel(STARTING_NOT_VACCINATED_LIST_LEVEL),
        numberOfNodesExceptFirstAndLast(STARTING_NOT_VACCINATED_LIST_SIZE) {
    firstNode = new NotVaccinatedPersonSkipListNode();
    lastNode = new NotVaccinatedPersonSkipListNode(MAX_SKIP_LIST_SIZE);

    for (int tempKey = 0; tempKey < MAX_LEVEL; tempKey++) {
        firstNode->setNextNodeWithKeyTo(tempKey, lastNode);
    }
}

NotVaccinatedSkipList::~NotVaccinatedSkipList() {
    NotVaccinatedPersonSkipListNode *tempNode;
    NotVaccinatedPersonSkipListNode *tempNode2;

    tempNode = firstNode->getNextNodeWithKey(0);
    while (tempNode != NULL) {
        tempNode2 = tempNode;
        tempNode = tempNode->getNextNodeWithKey(0);
        delete tempNode2;
    }

    delete firstNode;
}

Person* NotVaccinatedSkipList::search(int searchKey) {
    NotVaccinatedPersonSkipListNode *tempNode = firstNode;

    for (int i = MAX_LEVEL - 1; i >= 0; --i) {
        while ((tempNode->getNextNodeWithKey(i))->getKey() < searchKey) {
            tempNode = tempNode->getNextNodeWithKey(i);
        }
    }

    tempNode = tempNode->getNextNodeWithKey(0);

    if (tempNode->getKey() == searchKey) {
        return tempNode->getPerson();
    }

    return NULL;
}

void NotVaccinatedSkipList::insert(int searchKey, Person* newPerson) {
    NotVaccinatedPersonSkipListNode **nodesThatMayNeedToBeUpdated = new NotVaccinatedPersonSkipListNode *[MAX_LEVEL];
    int newLevel;
    NotVaccinatedPersonSkipListNode *tempNode = firstNode;

    for (int i = MAX_LEVEL - 1; i >= 0; --i) {
        while ((tempNode->getNextNodeWithKey(i))->getKey() < searchKey) {
            tempNode = tempNode->getNextNodeWithKey(i);
        }

        nodesThatMayNeedToBeUpdated[i] = tempNode;
    }

    tempNode = tempNode->getNextNodeWithKey(0);
    if (tempNode->getKey() == searchKey) {
        delete nodesThatMayNeedToBeUpdated;
        return;
    }

    newLevel = setLevelOfNodeByCalculatingPossibilities();
    if (newLevel > currentHighestLevel) {
        currentHighestLevel = newLevel;
    }
    tempNode = new NotVaccinatedPersonSkipListNode(atoi(newPerson->getCitizenId()));
    tempNode->setPerson(newPerson);
    for (int i = 0; i <= newLevel; ++i) {
        tempNode->setNextNodeWithKeyTo(
                i,
                nodesThatMayNeedToBeUpdated[i]->getNextNodeWithKey(i)
        );
        nodesThatMayNeedToBeUpdated[i]->setNextNodeWithKeyTo(i, tempNode);
    }

    numberOfNodesExceptFirstAndLast++;

    delete nodesThatMayNeedToBeUpdated;
}

void NotVaccinatedSkipList::print() {
    NotVaccinatedPersonSkipListNode *tempNode = firstNode->getNextNodeWithKey(0);
    if (tempNode == lastNode) {
        return;
    }

    while (tempNode != lastNode) {
        tempNode->printPersonInfo();
        tempNode = tempNode->getNextNodeWithKey(0);
    }
}

int NotVaccinatedSkipList::setLevelOfNodeByCalculatingPossibilities() {
    srand(time(NULL));
    int level = STARTING_NOT_VACCINATED_LIST_LEVEL;
    while (
            (rand() % 100) < NOT_VACCINATED_LIST_KEY_POSSIBILITY_LIMIT_FOR_UPDATING_LEVEL
            && level < MAX_LEVEL
            ) {
        level++;
    }

    return level;
}

