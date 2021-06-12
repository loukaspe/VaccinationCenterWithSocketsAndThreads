#include "Virus.h"

Virus::Virus(
    char *virusName,
    VaccinatedSkipList *vaccinatedPeopleList,
    NotVaccinatedSkipList *notVaccinatedPeopleList,
    BloomFilter *notVaccinatedPeopleBloomFilter
) : virusName(virusName),
    vaccinatedPeopleList(vaccinatedPeopleList),
    notVaccinatedPeopleList(notVaccinatedPeopleList),
    vaccinatedPeopleBloomFilter(notVaccinatedPeopleBloomFilter) {}

char *Virus::getVirusName() const {
    return virusName;
}

VaccinatedSkipList *Virus::getVaccinatedPeopleList() const {
    return vaccinatedPeopleList;
}

NotVaccinatedSkipList *Virus::getNotVaccinatedPeopleList() const {
    return notVaccinatedPeopleList;
}

BloomFilter *Virus::getVaccinatedPeopleBloomFilter() const {
    return vaccinatedPeopleBloomFilter;
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
