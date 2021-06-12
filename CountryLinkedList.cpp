#include "CountryLinkedList.h"

CountryLinkedList::CountryLinkedList() {
    this->head = NULL;
    this->size = 0;
}


void CountryLinkedList::addAtStart(Country *country) {
    CountryLinkedListNode *newNode = new CountryLinkedListNode(country);

    newNode->next = this->head;
    this->head = newNode;
    this->size++;
}

bool CountryLinkedList::isEmpty() {
    return this->size == 0;
}

CountryLinkedListNode *CountryLinkedList::findByName(char *countryName) {
    CountryLinkedListNode *current = this->head;
    while (current != NULL) {
        if (strcmp(current->getCountry()->getName(), countryName) == 0) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

void CountryLinkedList::traverseAndPrintStatisticsForDates(
        CountryLinkedListNode *initialRecursiveNode,
        char *virusName,
        Date *date1,
        Date *date2
) {
    if (initialRecursiveNode == NULL) {
        return;
    }

    Country *tempCountry = initialRecursiveNode->getCountry();

    tempCountry->printCountryStatisticsForDates(virusName, date1, date2);

    this->traverseAndPrintStatisticsForDates(
            initialRecursiveNode->next,
            virusName,
            date1,
            date2
    );
}

void CountryLinkedList::printStatisticsForDates(
        char *virusName,
        Date *date1,
        Date *date2
) {
    this->traverseAndPrintStatisticsForDates(this->head, virusName, date1, date2);
}

void CountryLinkedList::traverseAndPrintStatisticsPerAgeForDates(
        CountryLinkedListNode *initialRecursiveNode,
        char *virusName,
        Date *date1,
        Date *date2
) {
    if (initialRecursiveNode == NULL) {
        return;
    }

    Country *tempCountry = initialRecursiveNode->getCountry();
    tempCountry->printCountryStatisticsPerAgeForDates(virusName, date1, date2);

    this->traverseAndPrintStatisticsPerAgeForDates(
            initialRecursiveNode->next,
            virusName,
            date1,
            date2
    );
}

void CountryLinkedList::printStatisticsPerAgeForDates(
        char *virusName,
        Date *date1,
        Date *date2
) {
    this->traverseAndPrintStatisticsPerAgeForDates(this->head, virusName, date1, date2);
}