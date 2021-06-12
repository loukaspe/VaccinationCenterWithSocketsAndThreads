#include "VaccinationCenter.h"

VaccinationCenter::VaccinationCenter(
        PersonLinkedList *people,
        VirusLinkedList *viruses,
        CountryLinkedList *countries
) : people(people), viruses(viruses), countries(countries) {}

void VaccinationCenter::vaccineStatusBloom(char *citizenId, char *virusName) {
    VirusLinkedListNode *node = this->viruses->findByName(virusName);

    if (node == NULL) {
        cout << "No Virus with name " << virusName << endl;
        return;
    }

    Virus *virus = node->getVirus();

    if (virus == NULL) {
        cout << "No Virus with name " << virusName << endl;
        return;
    }

    bool isVaccinated = virus->getVaccinatedPeopleBloomFilter()->check(citizenId);

    if (isVaccinated) {
        cout << "MAYBE" << endl;
        return;
    }

    cout << "NOT VACCINATED" << endl;
}

void VaccinationCenter::vaccineStatusForAllViruses(char *citizenId) {
    this->viruses->checkIfVaccinatedForAllViruses(citizenId);
}

void VaccinationCenter::vaccineStatusForSpecificVirus(char *citizenId, char *virusName) {
    VirusLinkedListNode *tempNode = this->viruses->findByName(virusName);
    if (tempNode == NULL) {
        cout << "There is no Virus with name " << virusName << endl;
        return;
    }

    Virus *tempVirus = tempNode->getVirus();
    if (tempVirus == NULL) {
        cout << "There is no Virus with name " << virusName << endl;
        return;
    }

    bool shouldPrintVirusName = false;
    tempVirus->printIfPersonIsVaccinated(citizenId, shouldPrintVirusName);
}

void VaccinationCenter::populationStatusForAllCountries(
        char *virusName,
        Date *date1,
        Date *date2
) {
    this->countries->printStatisticsForDates(virusName, date1, date2);
}

void VaccinationCenter::populationStatusForCountry(
        char *countryName,
        char *virusName,
        Date *date1,
        Date *date2
) {
    CountryLinkedListNode *node = this->countries->findByName(countryName);
    if (node == NULL) {
        cout << "There is no country with name " << countryName << endl;
        return;
    }

    Country *country = this->countries->findByName(countryName)->getCountry();
    if (country == NULL) {
        cout << "There is no country with name " << countryName << endl;
        return;
    }

    country->printCountryStatisticsForDates(virusName, date1, date2);
}

void VaccinationCenter::VaccinationCenter::popStatusByAgeForAllCountries(
        char *virusName,
        Date *date1,
        Date *date2
) {
    this->countries->printStatisticsPerAgeForDates(virusName, date1, date2);
}

void VaccinationCenter::popStatusByAgeForCountry(
        char *countryName,
        char *virusName,
        Date *date1,
        Date *date2
) {
    CountryLinkedListNode *node = this->countries->findByName(countryName);
    if (node == NULL) {
        cout << "There is no country with name " << countryName << endl;
        return;
    }

    Country *country = this->countries->findByName(countryName)->getCountry();
    if (country == NULL) {
        cout << "There is no country with name " << countryName << endl;
        return;
    }

    country->printCountryStatisticsPerAgeForDates(virusName, date1, date2);
}

void VaccinationCenter::insertVaccinated(
        char *citizenId,
        char *firstName,
        char *lastName,
        char *country,
        int age,
        char *virusName,
        Date *date
) {
    this->checkAndAddCountry(country);
    this->checkAndAddPerson(
            citizenId,
            firstName,
            lastName,
            country,
            age
    );
    this->checkAndAddVirus(virusName, country);

    VirusLinkedListNode *virusNode = this->viruses->findByName(virusName);
    Virus *virus = virusNode->getVirus();

    Vaccination *possibleVaccination = virus->getVaccinatedPeopleList()->search(atoi(citizenId));
    if (possibleVaccination != NULL) {
        cout << "ERROR: CITIZEN " << citizenId << " ALREADY VACCINATED ON "
             << possibleVaccination->getDate()->toString() << endl;
        return;
    }

    Vaccination *newVaccination = new Vaccination(
            this->people->findByCitizenId(citizenId)->person,
            date
    );

    virus->getVaccinatedPeopleList()->insert(
            atoi(citizenId),
            newVaccination
    );
    virus->getVaccinatedPeopleBloomFilter()->add(citizenId);
}

void VaccinationCenter::insertNotVaccinated(
        char *citizenId,
        char *firstName,
        char *lastName,
        char *country,
        int age,
        char *virusName
) {
    this->checkAndAddCountry(country);
    this->checkAndAddPerson(
            citizenId,
            firstName,
            lastName,
            country,
            age
    );
    this->checkAndAddVirus(virusName, country);

    VirusLinkedListNode *virusNode = this->viruses->findByName(virusName);
    Virus *virus = virusNode->getVirus();

    Vaccination *possibleVaccination = virus->getVaccinatedPeopleList()->search(atoi(citizenId));
    if (possibleVaccination != NULL) {
        cout << "ERROR: CITIZEN " << citizenId << " ALREADY VACCINATED ON "
             << possibleVaccination->getDate()->toString() << endl;
        return;
    }

    virus->getNotVaccinatedPeopleList()->insert(
            atoi(citizenId),
            this->people->findByCitizenId(citizenId)->person
    );
}

void VaccinationCenter::vaccinateNow(
        char *citizenId,
        char *firstName,
        char *lastName,
        char *country,
        int age,
        char *virusName
) {
    time_t todayInSeconds = time(NULL);
    struct tm todayTimeObject = *localtime(&todayInSeconds);
    char todayAsString[10];
    sprintf(
            todayAsString,
            "%02d-%02d-%04d",
            todayTimeObject.tm_mday,
            todayTimeObject.tm_mon + 1,
            todayTimeObject.tm_year + 1900);
    Date *today = new Date(todayAsString);

    this->insertVaccinated(
            citizenId,
            firstName,
            lastName,
            country,
            age,
            virusName,
            today
    );
}

void VaccinationCenter::listNotVaccinatedPersonsForVirus(char *virusName)
{
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

    virus->getNotVaccinatedPeopleList()->print();
}

void VaccinationCenter::checkAndAddCountry(char *country) {
    if (this->countries->findByName(country) == NULL) {
        Country *newCountry = new Country(
                country,
                this->viruses
        );

        this->countries->addAtStart(newCountry);
    }
}

void VaccinationCenter::checkAndAddPerson(
        char *citizenId,
        char *firstName,
        char *lastName,
        char *country,
        int age
) {
    if (this->people->findByCitizenId(citizenId) == NULL) {
        Person *newPerson = new Person(
                citizenId,
                firstName,
                lastName,
                country,
                age
        );

        this->people->addAtStart(newPerson);
        this->countries->findByName(country)->getCountry()->addCitizen(age);
    }
}

void VaccinationCenter::checkAndAddVirus(char *virusName, char *countryName) {
    if (this->viruses->findByName(virusName) == NULL) {
        BloomFilter *newBloomFilter = new BloomFilter();
        newBloomFilter->setCountryName(countryName);
        newBloomFilter->setVirusName(virusName);
        VaccinatedSkipList *newVaccinatedSkipList = new VaccinatedSkipList();
        NotVaccinatedSkipList *newNotVaccinatedSkipList = new NotVaccinatedSkipList();

        Virus *newVirus = new Virus(
                virusName,
                newVaccinatedSkipList,
                newNotVaccinatedSkipList,
                newBloomFilter
        );

        this->viruses->addAtStart(newVirus);
    }

}

PersonLinkedList *VaccinationCenter::getPeople() const {
    return people;
}

VirusLinkedList *VaccinationCenter::getViruses() const {
    return viruses;
}
