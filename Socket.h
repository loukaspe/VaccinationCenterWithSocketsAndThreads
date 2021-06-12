#ifndef VACCINATIONCENTERUSINGSOCKETSANDTHREADS_SOCKET_H
#define VACCINATIONCENTERUSINGSOCKETSANDTHREADS_SOCKET_H

#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include "Helper.h"
#include "BloomFilter.h"

class Socket {
private:
    int createdSocketFd;
    int acceptedSocketFd;
    int bufferSize;
    int port;
    struct sockaddr_in server;
    struct sockaddr *serverPointer;
    struct sockaddr_in client;
    struct sockaddr *clientPointer;

    static const char* CREATE_ERROR;
    static const char* LISTEN_ERROR;
    static const char* BIND_ERROR;
    static const char* ACCEPT_ERROR;
    static const char* CONNECT_ERROR;
    static const char* READ_ERROR;
    static const char* WRITE_ERROR;
public:
    Socket(int, int);
    ~Socket();
    void createSocket(struct hostent * = NULL);
    void bindToSocket();
    int listenToSocket();
    int acceptSocket();
    void connectToSocket();
    int readNumber(bool = false);
    void writeNumber(int, bool = false);
    char* readStringInChunks(int, bool = false);
    void writeStringInChunks(char*, bool = false);
    int* readArrayOfInts(int, bool = false);
    void writeArrayOfInts(int*, int, bool = false);
    BloomFilter* readBloomFilterInChunks(bool = false);
    void writeBloomFilterInChunks(BloomFilter*, bool = false);
    BitArray* readBitArrayInChunks(bool = false);
    void writeBitArrayInChunks(BitArray*, bool = false);
    void closeSocket();
};

#endif //VACCINATIONCENTERUSINGSOCKETSANDTHREADS_SOCKET_H
