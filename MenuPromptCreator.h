#ifndef ERGASIA1_MENUPROMPTCREATOR_H
#define ERGASIA1_MENUPROMPTCREATOR_H

#include "TravelMonitorClient.h"

class MenuPromptCreator {
    // I take as granted that the command size will not be bigger than 1024 chars
    static const size_t SIZE_OF_INPUT_BUFFER;
    static const int COMMANDS_NUMBER = 6;
    static char *AVAILABLE_COMMANDS[MenuPromptCreator::COMMANDS_NUMBER];

    static const char* INSERT_YOUR_COMMAND_MESSAGE;
    static const char* NOT_VALID_COMMAND_MESSAGE;
    static const char* AVAILABLE_COMMANDS_MESSAGE;
    static const char* READING_COMMAND_ERROR_MESSAGE;

public:
    MenuPromptCreator(TravelMonitorClient *travelMonitorClient);

    static void showAllOptions();
    void createAndExecute();
private:
    int executeInputCommand();
    TravelMonitorClient* travelMonitorClient;
};


#endif //ERGASIA1_MENUPROMPTCREATOR_H
