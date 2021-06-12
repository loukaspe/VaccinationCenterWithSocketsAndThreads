#ifndef ERGASIA1_COUNTRYLINKEDLISTNODE_H
#define ERGASIA1_COUNTRYLINKEDLISTNODE_H

#include "Country.h"

class CountryLinkedListNode {
public:
    CountryLinkedListNode *next;
    CountryLinkedListNode(Country*);
    Country *getCountry() const;
private:
    Country* country;
};

#endif //ERGASIA1_COUNTRYLINKEDLISTNODE_H
