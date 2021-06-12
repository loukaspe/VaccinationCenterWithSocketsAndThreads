#include "Vaccination.h"

Vaccination::Vaccination(Person *person, Date *date) : person(person), date(date) {}

Person *Vaccination::getPerson() const {
    return person;
}

Date *Vaccination::getDate() const {
    return date;
}

void Vaccination::printInfo() {
    this->person->printInfo();
    cout << "Vaccinated on " << this->getDate() << endl;
}
