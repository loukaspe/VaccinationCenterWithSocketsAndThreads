#include "BloomFilterLinkedListNode.h"

BloomFilterLinkedListNode::BloomFilterLinkedListNode(BloomFilter* bloomFilter) {
    this->bloomFilter = bloomFilter;
    this->next = NULL;
}

BloomFilter *BloomFilterLinkedListNode::getBloomFilter() const {
    return bloomFilter;
}
