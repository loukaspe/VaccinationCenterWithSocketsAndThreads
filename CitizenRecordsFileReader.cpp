#include "CitizenRecordsFileReader.h"

const char *CitizenRecordsFileReader::NO_INPUT_FILE_ERROR = "ERROR: No Input File was found.\n";
const char *CitizenRecordsFileReader::OPEN_FILE_ERROR = "ERROR: Could not open file ";
const char *CitizenRecordsFileReader::OPEN_FILE_READ_MODE = "r";
const char *CitizenRecordsFileReader::SPACE_DELIMITER = " ";
const char *CitizenRecordsFileReader::ERROR_IN_READING_FILE = "ERROR: Cannot read file\n";

CitizenRecordsFileReader::CitizenRecordsFileReader(
        VaccinationCenter *vaccinationCenter
) : vaccinationCenter(vaccinationCenter) {}

// 889 John Papadopoulos Greece 52 COVID-19 YES 27-12-2020
// 889 John Papadopoulos Greece 52 Η1Ν1 ΝΟ

void CitizenRecordsFileReader::readAndUpdateStructures(char* filename) {
    if (filename == NULL) {
        Helper::handleError(NO_INPUT_FILE_ERROR);
    }

    FILE *inputFile;
    char *line = NULL;
    size_t length = 0;
    char *temp;
    char *citizenId;
    char *firstName;
    char *lastName;
    char *country;
    int age;
    char *virusName;
    bool isVaccinated;
    Date *date;
    Virus *tempVirus;

    inputFile = fopen(filename, OPEN_FILE_READ_MODE);
    if (inputFile == NULL) {
        Helper::handleError(OPEN_FILE_ERROR);
    }

    // I consider that the inputFile is correctly formatted as per the way the
    // data is given
    while (getline(&line, &length, inputFile) != -1) {
        temp = strtok(line, SPACE_DELIMITER);
        citizenId = Helper::copyString(temp);

        temp = strtok(NULL, SPACE_DELIMITER);
        firstName = Helper::copyString(temp);

        temp = strtok(NULL, SPACE_DELIMITER);
        lastName = Helper::copyString(temp);

        temp = strtok(NULL, SPACE_DELIMITER);
        country = Helper::copyString(temp);

        temp = strtok(NULL, SPACE_DELIMITER);
        age = atoi(temp);

        temp = strtok(NULL, SPACE_DELIMITER);
        virusName = Helper::copyString(temp);

        temp = strtok(NULL, SPACE_DELIMITER);
        temp = Helper::removeNewLineCharacterFromString(temp);
        isVaccinated = setBooleanIsVaccinatedFromStringValue(temp);

        this->vaccinationCenter->checkAndAddCountry(country);

        this->vaccinationCenter->checkAndAddPerson(
                citizenId,
                firstName,
                lastName,
                country,
                age
        );

        this->vaccinationCenter->checkAndAddVirus(virusName, country);

        tempVirus = this->vaccinationCenter->getViruses()->findByName(virusName)->getVirus();

        if (isVaccinated) {
            temp = strtok(NULL, SPACE_DELIMITER);
            temp = Helper::removeNewLineCharacterFromString(temp);
            date = new Date(temp);

            if (this->isPersonAlreadyInsertedAsNotVaccinated(tempVirus, citizenId)) {
                cout << "ERROR IN RECORD " << line << endl;
                continue;
            }

            Vaccination *newVaccination = new Vaccination(
                    this->vaccinationCenter->getPeople()->findByCitizenId(citizenId)->person,
                    date
            );

            tempVirus->getVaccinatedPeopleList()->insert(
                    atoi(citizenId),
                    newVaccination
            );
            tempVirus->getVaccinatedPeopleBloomFilter()->add(citizenId);
        }
        else {
            if (this->isPersonAlreadyInsertedAsVaccinated(tempVirus, citizenId)) {
                cout << "ERROR IN RECORD " << line << endl;
                continue;
            }

            tempVirus->getNotVaccinatedPeopleList()->insert(
                    atoi(citizenId),
                    this->vaccinationCenter->getPeople()->findByCitizenId(citizenId)->person
            );
        }
    }

    if (ferror(inputFile)) {
        Helper::handleError(ERROR_IN_READING_FILE);
    }

    free(line);
    fflush(inputFile);
    fclose(inputFile);
}

bool CitizenRecordsFileReader::setBooleanIsVaccinatedFromStringValue(char *isVaccinatedAsString) {
    if (strcmp(isVaccinatedAsString, "YES") == 0) {
        return true;
    }

    if (strcmp(isVaccinatedAsString, "NO") == 0) {
        return false;
    }

    return false;
}

bool CitizenRecordsFileReader::isPersonAlreadyInsertedAsVaccinated(
        Virus *virus,
        char *citizenId
) {
    return (virus->getVaccinatedPeopleList()->search(atoi(citizenId)) != NULL);
}

bool CitizenRecordsFileReader::isPersonAlreadyInsertedAsNotVaccinated(
        Virus *virus,
        char *citizenId
) {
    return (virus->getNotVaccinatedPeopleList()->search(atoi(citizenId)) != NULL);
}


