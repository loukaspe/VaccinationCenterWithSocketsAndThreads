/* Class to operate the application of Travel Monitor */

#ifndef VACCINATIONCENTERUSINGSOCKETSANDTHREADS_TRAVELMONITOR_H
#define VACCINATIONCENTERUSINGSOCKETSANDTHREADS_TRAVELMONITOR_H

static const int NUMBER_OF_MONITOR_ARGS_WITHOUT_COUNTRIES_PATHS = 12;
static const int INITIAL_PORT = 10000;

static const int NO_POLL_TIMEOUT = -1;

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <sys/wait.h>
#include <poll.h>
#include <netinet/in.h>
#include "BloomFilterLinkedList.h"
#include "Socket.h"
#include "VaccinationCenter.h"

using namespace std;

class TravelMonitorClient {
private:
    int numberOfMonitors;
    int socketBufferSize;
    int cyclicBufferSize;
    int bloomSizeInKiloBytes;
    char *inputDirectory;
    int numberOfThreads;
    BloomFilterLinkedList* bloomFilters;
    char*** monitorArguments;
    Socket** sockets;
    int *ports;
    int *acceptedSocketFds;

    void createMonitorArguments();
    int findMonitorForCountry(char*);

    static const char* FORK_ERROR;
public:
    TravelMonitorClient(int, int, int, int, char*, int);
    ~TravelMonitorClient();
    void createMonitorsAndPassThemData();
    void readDataFromMonitors();
    void travelRequest(char*, Date*, char*, char*, char*);
    void travelStatsForCountry(char*, Date*, Date*, char*);
    void travelStatsForAllCountries(char*, Date*, Date*);
    void addVaccinationRecords(char*);
    void searchVaccinationStatus(char*);
    void exitTravelMonitor();

    /* NUMBER CONSTANTS FOR COMMUNICATION BETWEEN MONITORS AND TRAVEL MONITOR FOR
    * USER REQUESTS */
    static const int TRAVEL_REQUEST = 1;
    static const int TRAVEL_STATS = 2;
    static const int ADD_VACCINATION_RECORDS = 3;
    static const int SEARCH_VACCINATION_STATUS = 4;
    static const int EXIT = 5;
};


#endif //VACCINATIONCENTERUSINGSOCKETSANDTHREADS_TRAVELMONITOR_H
