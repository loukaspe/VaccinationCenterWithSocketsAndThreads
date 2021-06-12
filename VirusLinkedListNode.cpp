#include "VirusLinkedListNode.h"

VirusLinkedListNode::VirusLinkedListNode(Virus* virus) {
    this->virus = virus;
    this->next = NULL;
}

Virus *VirusLinkedListNode::getVirus() const {
    return virus;
}
