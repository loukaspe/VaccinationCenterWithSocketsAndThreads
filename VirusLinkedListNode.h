#ifndef ERGASIA1_VIRUSLINKEDLISTNODE_H
#define ERGASIA1_VIRUSLINKEDLISTNODE_H

#include "Virus.h"

class VirusLinkedListNode {
public:
    VirusLinkedListNode *next;
    VirusLinkedListNode(Virus*);
    Virus *getVirus() const;
private:
    Virus* virus;
};


#endif //ERGASIA1_VirusLINKEDLISTNODE_H
