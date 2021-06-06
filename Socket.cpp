#include "Socket.h"

const char* Socket::CREATE_ERROR = "Error in creating socket";
const char* Socket::BIND_ERROR = "Error in binding socket";
const char* Socket::LISTEN_ERROR = "Error in listening to socket";
const char* Socket::ACCEPT_ERROR = "Error in accepting socket connection";
const char* Socket::CONNECT_ERROR = "Error connecting to socket";
const char* Socket::READ_ERROR = "Error reading from socket";
const char* Socket::WRITE_ERROR = "Error writing to socket";

Socket::Socket(int bufferSize, int port) : bufferSize(bufferSize), port(port) {
    serverPointer = (struct sockaddr *) &server ;
    clientPointer = (struct sockaddr *) &client ;
}

Socket::~Socket() {}

void Socket::createSocket(struct hostent *host) {
    if (
        (createdSocketFd = socket(AF_INET, SOCK_STREAM, 0)) < 0
    ) {
        Helper::handleError(CREATE_ERROR, errno);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    if(host == NULL) {
        server.sin_addr.s_addr = htonl(INADDR_ANY);
    } else {
        memcpy(&server.sin_addr, host->h_addr, host->h_length);
    }
}

void Socket::bindToSocket() {
    if (
        bind(createdSocketFd, serverPointer, sizeof(server)) < 0
    ) {
        Helper::handleError(BIND_ERROR, errno);
    }
}

void Socket::listenToSocket() {
    // One Socket Per Connection
    if (
        listen(createdSocketFd, 1) < 0
    ) {
        Helper::handleError(LISTEN_ERROR, errno);
    }
}

void Socket::acceptSocket() {
    socklen_t clientLength = sizeof(client);

    if (
        (acceptedSocketFd = accept(createdSocketFd, clientPointer, &clientLength))< 0
    ) {
        Helper::handleError(ACCEPT_ERROR, errno);
    }

//    close(createdSocketFds[i]);
}

void Socket::connectToSocket() {
    if (connect(createdSocketFd, serverPointer, sizeof(server)) < 0) {
        Helper::handleError(CONNECT_ERROR, errno);
    }
    //    do
    //        connectStatus = connect(clientSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    //    while (connectStatus < 0);
}

int Socket::readNumber() {
    int number;

    if(read(acceptedSocketFd, &number, sizeof(int)) < 0) {
        Helper::handleError(READ_ERROR, errno);
    }

    return number;
}

void Socket::writeNumber(int number) {
    if (write(createdSocketFd, &number, sizeof(int)) < 0) {
        Helper::handleError(WRITE_ERROR, errno);
    }
}

void Socket::closeSocket() {
    close(createdSocketFd);
    close(acceptedSocketFd);
}
