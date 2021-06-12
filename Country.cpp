#include "Country.h"

Country::Country(char *name, VirusLinkedList *viruses) :
        name(name),
        viruses(viruses),
        numberOfCitizens(0),
        numberOfVaccinatedCitizensUnder20yearsOld(0),
        numberOfVaccinatedCitizensFrom20To40yearsOld(0),
        numberOfVaccinatedCitizensFrom40To60yearsOld(0),
        numberOfVaccinatedCitizensOver60yearsOld(0) {
}

void Country::addCitizen(int age) {
    this->numberOfCitizens++;

    if (age <= 20) {
        this->numberOfVaccinatedCitizensUnder20yearsOld++;
    }
    else if (age > 20 && age <= 40) {
        this->numberOfVaccinatedCitizensFrom20To40yearsOld++;
    }
    else if (age > 40 && age <= 60) {
        this->numberOfVaccinatedCitizensFrom40To60yearsOld++;
    }
    else if (age > 60) {
        this->numberOfVaccinatedCitizensOver60yearsOld++;
    }
}

void Country::printCountryStatisticsForDates(char *virusName, Date *date1, Date *date2) {
    VirusLinkedListNode *node = this->viruses->findByName(virusName);
    if (node == NULL) {
        cout << "There is no Virus with name " << virusName << endl;
        return;
    }

    Virus *virus = node->getVirus();
    if (virus == NULL) {
        cout << "There is no Virus with name " << virusName << endl;
        return;
    }

    CountryVaccinationStatistics statistics = virus->getVaccinatedPeopleList()
            ->getNumberOfCitizensOfCountryVaccinatedBetweenDates(
                    this->name,
                    date1,
                    date2
            );

    float percentageOfVaccinatedPeople =
            (float) statistics.numberOfVaccinatedCitizens /
            (float) numberOfCitizens;

    if(numberOfCitizens == 0) {
        percentageOfVaccinatedPeople = 0;
    }

    cout << this->name << " "
         << statistics.numberOfVaccinatedCitizens << " "
         << percentageOfVaccinatedPeople << "%" << endl;
}

void Country::printCountryStatisticsPerAgeForDates(char *virusName, Date *date1, Date *date2) {
    VirusLinkedListNode *node = this->viruses->findByName(virusName);
    if (node == NULL) {
        cout << "There is no Virus with name " << virusName << endl;
        return;
    }

    Virus *virus = node->getVirus();
    if (virus == NULL) {
        cout << "There is no Virus with name " << virusName << endl;
        return;
    }

    CountryVaccinationStatistics statistics = virus->getVaccinatedPeopleList()
            ->getNumberOfCitizensOfCountryVaccinatedBetweenDates(
                    this->name,
                    date1,
                    date2
            );

    float percentageOfVaccinatedPeopleUnder20 =
            (float) statistics.numberOfVaccinatedCitizensUnder20yearsOld /
            (float) this->numberOfVaccinatedCitizensUnder20yearsOld;

    if(numberOfVaccinatedCitizensUnder20yearsOld == 0) {
        percentageOfVaccinatedPeopleUnder20 = 0;
    }

    float percentageOfVaccinatedPeopleBetween20And40 =
            (float) statistics.numberOfVaccinatedCitizensFrom20To40yearsOld /
            (float) this->numberOfVaccinatedCitizensFrom20To40yearsOld;

    if(numberOfVaccinatedCitizensFrom20To40yearsOld == 0) {
        percentageOfVaccinatedPeopleBetween20And40 = 0;
    }

    float percentageOfVaccinatedPeopleBetween40And60 =
            (float) statistics.numberOfVaccinatedCitizensFrom40To60yearsOld /
            (float) this->numberOfVaccinatedCitizensFrom40To60yearsOld;

    if(numberOfVaccinatedCitizensFrom40To60yearsOld == 0) {
        percentageOfVaccinatedPeopleBetween40And60 = 0;
    }

    float percentageOfVaccinatedPeopleOver60 =
            (float) statistics.numberOfVaccinatedCitizensOver60yearsOld /
            (float) this->numberOfVaccinatedCitizensOver60yearsOld;

    if(numberOfVaccinatedCitizensOver60yearsOld == 0) {
        percentageOfVaccinatedPeopleOver60 = 0;
    }

    cout << endl << this->name << " " << endl

         << "0-20 " << statistics.numberOfVaccinatedCitizensUnder20yearsOld << " "
         << percentageOfVaccinatedPeopleUnder20 << "%" << endl

         << "20-40 " << statistics.numberOfVaccinatedCitizensFrom20To40yearsOld << " "
         << percentageOfVaccinatedPeopleBetween20And40 << "%" << endl

         << "40-60 " << statistics.numberOfVaccinatedCitizensFrom40To60yearsOld << " "
         << percentageOfVaccinatedPeopleBetween40And60 << "%" << endl

         << "60+ " << statistics.numberOfVaccinatedCitizensOver60yearsOld << " "
         << percentageOfVaccinatedPeopleOver60 << "%" << endl;
}

char *Country::getName() const {
    return name;
}
