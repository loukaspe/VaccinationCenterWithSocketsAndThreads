#include "Virus.h"

Virus::Virus(
    char *virusName,
    VaccinatedSkipList *vaccinatedPeopleList,
    NotVaccinatedSkipList *notVaccinatedPeopleList,
    BloomFilterLinkedList *vaccinatedPeopleBloomFilterLinkedList
) : virusName(virusName),
    vaccinatedPeopleList(vaccinatedPeopleList),
    notVaccinatedPeopleList(notVaccinatedPeopleList),
    vaccinatedPeopleBloomFilterLinkedList(vaccinatedPeopleBloomFilterLinkedList) {}

char *Virus::getVirusName() const {
    return virusName;
}

VaccinatedSkipList *Virus::getVaccinatedPeopleList() const {
    return vaccinatedPeopleList;
}

NotVaccinatedSkipList *Virus::getNotVaccinatedPeopleList() const {
    return notVaccinatedPeopleList;
}

BloomFilterLinkedList *Virus::getVaccinatedPeopleBloomFilterLinkedList() const {
    return vaccinatedPeopleBloomFilterLinkedList;
}

/** There is a need to print if the person is Vaccinated with two different ways,
 * by showing or not the VirusName. So an extra argument is added for this purpose
 */
void Virus::printIfPersonIsVaccinated(char* citizenId, bool shouldPrintVirusName) {
    Date *tempDate;
    Vaccination *tempVaccination;
    tempVaccination = this->getVaccinatedPeopleList()->search(atoi(citizenId));

    if(shouldPrintVirusName) {
        if (tempVaccination != NULL) {
            tempDate = tempVaccination->getDate();
            cout << this->getVirusName() << " YES " << tempDate->toString() << endl;
        }
        else {
            cout << this->getVirusName() << " NO" << endl;
        }

        return;
    }

    if (tempVaccination != NULL) {
        tempDate = tempVaccination->getDate();
        cout << "VACCINATED ON " << tempDate->toString() << endl;
    }
    else {
        cout << "NOT VACCINATED" << endl;
    }
}

BloomFilter *Virus::getVaccinatedPeopleBloomFilterByCountry(char *countryName) const {
    BloomFilterLinkedListNode *node = this->vaccinatedPeopleBloomFilterLinkedList->findByVirusAndCountry(
        this->virusName,
        countryName
    );

    if(node == NULL) {
        return NULL;
    }

    return node->getBloomFilter();
}
