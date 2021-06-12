#include "CountryLinkedListNode.h"

CountryLinkedListNode::CountryLinkedListNode(Country *country) {
    this->country = country;
    this->next = NULL;
}

Country *CountryLinkedListNode::getCountry() const {
    return country;
}
