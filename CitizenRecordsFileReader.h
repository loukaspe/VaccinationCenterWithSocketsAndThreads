#ifndef ERGASIA1_CITIZENRECORDSFILEREADER_H
#define ERGASIA1_CITIZENRECORDSFILEREADER_H

#include "VaccinationCenter.h"

class CitizenRecordsFileReader {
public:
    CitizenRecordsFileReader(VaccinationCenter*);
    void readAndUpdateStructures(char* filename);

private:
    char *fileName;
    VaccinationCenter* vaccinationCenter;
    static const char *NO_INPUT_FILE_ERROR;
    static const char *OPEN_FILE_ERROR;
    static const char *OPEN_FILE_READ_MODE;
    static const char *SPACE_DELIMITER;
    static const char *ERROR_IN_READING_FILE;

    static bool setBooleanIsVaccinatedFromStringValue(char *);

    bool isPersonAlreadyInsertedAsVaccinated(Virus *, char *);

    bool isPersonAlreadyInsertedAsNotVaccinated(Virus *, char *);
};

#endif //ERGASIA1_CITIZENRECORDSFILEREADER_H
