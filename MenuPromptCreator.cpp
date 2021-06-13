#include "MenuPromptCreator.h"

const size_t MenuPromptCreator::SIZE_OF_INPUT_BUFFER = 1024;
char *MenuPromptCreator::AVAILABLE_COMMANDS[MenuPromptCreator::COMMANDS_NUMBER] = {
        "/travelRequest",
        "/travelStats",
        "/addVaccinationRecords",
        "/searchVaccinationStatus",
        "/exit",
        "/help",
};

const char *MenuPromptCreator::INSERT_YOUR_COMMAND_MESSAGE = "Please enter your "
                                                             "command: ";
const char *MenuPromptCreator::NOT_VALID_COMMAND_MESSAGE = "This is not a valid"
                                                           " command. "
                                                           "To see the available command please type '/help'";
const char *MenuPromptCreator::READING_COMMAND_ERROR_MESSAGE = "ERROR: There was"
                                                               " an error reading your command.";

const char *MenuPromptCreator::AVAILABLE_COMMANDS_MESSAGE = "Commands' list: \n"
                                                            "1) /travelRequest citizenID date countryFrom countryTo virusName\n"
                                                            "2) /travelStats virusName date1 date2 [country]\n"
                                                            "3) /addVaccinationRecords country\n"
                                                            "4) /searchVaccinationStatus citizenID\n"
                                                            "5) /exit\n\n";

void MenuPromptCreator::showAllOptions() {
    cout << MenuPromptCreator::AVAILABLE_COMMANDS_MESSAGE << endl;
}

int MenuPromptCreator::executeInputCommand() {
    const int CONTINUE_EXECUTION = 1;
    const int EXIT = 0;
    const char *SPACE_DELIMITER = " ";

    char *command = NULL;
    char *citizenId = NULL;
    char *countryTo = NULL;
    char *countryFrom = NULL;
    char *virusName = NULL;
    Date *date1 = NULL;
    Date *date2 = NULL;
    char *possibleExtraArgument = NULL;
    char *argumentOne = NULL;
    char *argumentTwo = NULL;
    char *argumentThree = NULL;
    char *argumentFour = NULL;

    char *input = (char *) malloc(
            MenuPromptCreator::SIZE_OF_INPUT_BUFFER * sizeof(char)
    );

    if (input == NULL) {
        Helper::handleError(MenuPromptCreator::READING_COMMAND_ERROR_MESSAGE);
    }

    cout << MenuPromptCreator::INSERT_YOUR_COMMAND_MESSAGE;

    /* READ WHOLE INPUT */

    if (
            fgets(
                    input,
                    MenuPromptCreator::SIZE_OF_INPUT_BUFFER,
                    stdin
            ) == NULL
            ) {
        Helper::handleError(MenuPromptCreator::READING_COMMAND_ERROR_MESSAGE);
    };

    /* Fgets returns a '\n' character in the end of the input, so if one exists
     * we remove it. */
    input = Helper::removeNewLineCharacterFromString(input);

    /* READ COMMAND */

    command = strtok(input, SPACE_DELIMITER);
    // We check if there is a command and if it is in the available commands' array
    if (
            command == NULL
            || !Helper::inArray(
                    command,
                    MenuPromptCreator::AVAILABLE_COMMANDS,
                    MenuPromptCreator::COMMANDS_NUMBER
            )
            ) {
        cout << MenuPromptCreator::NOT_VALID_COMMAND_MESSAGE << endl;
        free(input);
        return CONTINUE_EXECUTION;
    }

    // /travelRequest citizenID date countryFrom countryTo virusName
    if (strcmp(command, MenuPromptCreator::AVAILABLE_COMMANDS[0]) == 0) {

        citizenId = strtok(NULL, SPACE_DELIMITER);
        argumentOne = strtok(NULL, SPACE_DELIMITER);
        countryFrom = strtok(NULL, SPACE_DELIMITER);
        countryTo = strtok(NULL, SPACE_DELIMITER);
        virusName = strtok(NULL, SPACE_DELIMITER);
        possibleExtraArgument = strtok(NULL, SPACE_DELIMITER);

        if (
                citizenId == NULL
                || argumentOne == NULL
                || countryFrom == NULL
                || countryTo == NULL
                || virusName == NULL
                || possibleExtraArgument != NULL
                ) {
            cout << MenuPromptCreator::NOT_VALID_COMMAND_MESSAGE << endl;
            free(input);
            return CONTINUE_EXECUTION;
        }

        date1 = new Date(argumentOne);

        this->travelMonitorClient->travelRequest(
           citizenId,
           date1,
           countryFrom,
           countryTo,
           virusName
        );

        free(input);
        return CONTINUE_EXECUTION;
    }

    // /travelStats virusName date1 date2 [country]
    if (strcmp(command, MenuPromptCreator::AVAILABLE_COMMANDS[1]) == 0) {

        argumentOne = strtok(NULL, SPACE_DELIMITER);
        argumentTwo = strtok(NULL, SPACE_DELIMITER);
        argumentThree = strtok(NULL, SPACE_DELIMITER);

        if (
                argumentOne == NULL
                || argumentTwo == NULL
                || argumentThree == NULL
                ) {
            cout << MenuPromptCreator::NOT_VALID_COMMAND_MESSAGE << endl;
            free(input);
            return CONTINUE_EXECUTION;
        }

        argumentFour = strtok(NULL, SPACE_DELIMITER);

        if (argumentFour == NULL) {
            virusName = argumentOne;
            date1 = new Date(argumentTwo);
            date2 = new Date(argumentThree);

            this->travelMonitorClient->travelStatsForAllCountries(
                virusName,
                date1,
                date2
            );
        }
        else {
            virusName = argumentOne;
            date1 = new Date(argumentTwo);
            date2 = new Date(argumentThree);
            countryTo = argumentFour;

            this->travelMonitorClient->travelStatsForCountry(
                virusName,
                date1,
                date2,
                countryTo
            );
        }

        free(input);
        return CONTINUE_EXECUTION;
    }


    // /addVaccinationRecords country
    if (strcmp(command, MenuPromptCreator::AVAILABLE_COMMANDS[2]) == 0) {

        countryTo = strtok(NULL, SPACE_DELIMITER);
        possibleExtraArgument = strtok(NULL, SPACE_DELIMITER);

        if (
                countryTo == NULL
                || possibleExtraArgument != NULL
                ) {
            cout << MenuPromptCreator::NOT_VALID_COMMAND_MESSAGE << endl;
            free(input);
            return CONTINUE_EXECUTION;
        }

        this->travelMonitorClient->addVaccinationRecords(countryTo);

        free(input);
        return CONTINUE_EXECUTION;
    }

    // /searchVaccinationStatus citizenID
    if (strcmp(command, MenuPromptCreator::AVAILABLE_COMMANDS[3]) == 0) {
        citizenId = strtok(NULL, SPACE_DELIMITER);
        possibleExtraArgument = strtok(NULL, SPACE_DELIMITER);

        if (citizenId == NULL || possibleExtraArgument != NULL) {
            cout << MenuPromptCreator::NOT_VALID_COMMAND_MESSAGE << endl;
            free(input);
            return CONTINUE_EXECUTION;
        }

        this->travelMonitorClient->searchVaccinationStatus(citizenId);

        free(input);
        return CONTINUE_EXECUTION;
    }

    // exit command
    if (strcmp(command, MenuPromptCreator::AVAILABLE_COMMANDS[4]) == 0) {
        this->travelMonitorClient->exit();
        free(input);
        return EXIT;
    }

    // help command
    if (strcmp(command, MenuPromptCreator::AVAILABLE_COMMANDS[5]) == 0) {
        showAllOptions();
        free(input);
        return CONTINUE_EXECUTION;
    }
}

void MenuPromptCreator::createAndExecute() {
    // Continue execution until executeInputCommand() returns 0 (EXIT signal)
    while (executeInputCommand()) {}
}

MenuPromptCreator::MenuPromptCreator(
    TravelMonitorClient *travelMonitorClient
) :travelMonitorClient(travelMonitorClient) {}
