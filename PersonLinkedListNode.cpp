#include "PersonLinkedListNode.h"

PersonLinkedListNode::PersonLinkedListNode(Person* person) {
    this->person = person;
    this->next = NULL;
}