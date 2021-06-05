/* Class to operate the application of Travel Monitor */

#ifndef VACCINATIONCENTERUSINGSOCKETSANDTHREADS_TRAVELMONITOR_H
#define VACCINATIONCENTERUSINGSOCKETSANDTHREADS_TRAVELMONITOR_H

static const int NUMBER_OF_MONITOR_ARGS_WITHOUT_COUNTRIES_PATHS = 12;
static const int INITIAL_PORT = 10000;

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <sys/wait.h>
#include <poll.h>
#include <netinet/in.h>
#include "PipeReader.h"
#include "PipeWriter.h"
#include "BloomFilterLinkedList.h"

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
    /* Arrays of data for the socket handling */
    int *createdSocketFds;
    int *acceptedSocketFds;
    struct sockaddr_in *servers;
    struct sockaddr **serversPointers;
    struct sockaddr_in *clients;
    struct sockaddr **clientsPointers;
    int *ports;

    void createMonitorArguments();
    void createSocketForMonitor(int);
    void bindToSocketForMonitor(int);
    void listenToSocketForMonitor(int);
    void acceptSocketForMonitor(int);
    int readNumberFromSocket(int);
    void closeSocket(int);

    static const char* FORK_ERROR;
    static const char* MALLOC_FAIL_ERROR_MESSAGE;
public:
    TravelMonitorClient(int, int, int, int, char*, int);
    void createMonitorsAndPassThemData();
};


#endif //VACCINATIONCENTERUSINGSOCKETSANDTHREADS_TRAVELMONITOR_H
