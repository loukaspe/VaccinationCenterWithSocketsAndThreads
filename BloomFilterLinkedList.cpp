#include "BloomFilterLinkedList.h"

BloomFilterLinkedList::BloomFilterLinkedList() {
    this->head = NULL;
    this->size = 0;
}


void BloomFilterLinkedList::addAtStart(BloomFilter *bloomFilter) {
    BloomFilterLinkedListNode *newNode = new BloomFilterLinkedListNode(bloomFilter);

    newNode->next = this->head;
    this->head = newNode;
    this->size++;
}

BloomFilterLinkedListNode *BloomFilterLinkedList::findByVirusAndCountry(
        char *virusName,
        char *countryName
) {
    BloomFilterLinkedListNode *current = this->head;
    while (current != NULL) {
        if (
            strcmp(current->getBloomFilter()->getVirusName(), virusName) == 0
            && strcmp(current->getBloomFilter()->getCountryName(), countryName) == 0
        ) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

int BloomFilterLinkedList::getSize() const {
    return size;
}
