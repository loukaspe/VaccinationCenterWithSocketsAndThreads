#ifndef VACCINATIONCENTERUSINGSOCKETSANDTHREADS_BLOOMFILTERLINKEDLISTNODE_H
#define VACCINATIONCENTERUSINGSOCKETSANDTHREADS_BLOOMFILTERLINKEDLISTNODE_H

#include "BloomFilter.h"

class BloomFilterLinkedListNode {
public:
    BloomFilterLinkedListNode *next;
    BloomFilterLinkedListNode(BloomFilter*);
    BloomFilter *getBloomFilter() const;
private:
    BloomFilter* bloomFilter;
};


#endif //VACCINATIONCENTERUSINGSOCKETSANDTHREADS_BLOOMFILTERLINKEDLISTNODE_H
