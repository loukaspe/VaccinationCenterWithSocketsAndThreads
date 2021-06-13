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

    static const char* FORK_ERROR;
public:
    TravelMonitorClient(int, int, int, int, char*, int);
    void createMonitorsAndPassThemData();
    void readDataFromMonitors();
};


#endif //VACCINATIONCENTERUSINGSOCKETSANDTHREADS_TRAVELMONITOR_H
