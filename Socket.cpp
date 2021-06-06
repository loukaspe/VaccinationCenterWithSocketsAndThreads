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

    int enableReusableAddresses = 1;
    int enableReusableAddressesResult = setsockopt(
        createdSocketFd,
        SOL_SOCKET,
        SO_REUSEADDR,
        (char *)&enableReusableAddresses,
        sizeof(enableReusableAddresses)
    );
    if (enableReusableAddressesResult < 0)
    {
        Helper::handleError("Error: Could not enable reusable addresses", errno);
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

int Socket::listenToSocket() {
    // One Socket Per Connection
    if (
        listen(createdSocketFd, 1) < 0
    ) {
        Helper::handleError(LISTEN_ERROR, errno);
    }

    return createdSocketFd;
}

int Socket::acceptSocket() {
    socklen_t clientLength = sizeof(client);

    if (
        (acceptedSocketFd = accept(createdSocketFd, clientPointer, &clientLength))< 0
    ) {
        Helper::handleError(ACCEPT_ERROR, errno);
    }

    return acceptedSocketFd;
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


char *Socket::readStringInChunks(int totalBytes) {
    char* string = (char*) malloc(totalBytes * sizeof(char));
    if (string == NULL) {
        Helper::handleError("Error: Could not allocate memory", errno);
    }

    if(totalBytes < this->bufferSize) {
        if (::read(acceptedSocketFd, string, totalBytes) < 0) {
            Helper::handleError(READ_ERROR, errno);
        }

        return string;
    }

    char rawBytes[this->bufferSize];
    int readBytes = 0;
    int chunk;

    while(readBytes < totalBytes) {
        chunk = ::read(acceptedSocketFd, rawBytes, this->bufferSize);
        if (chunk < 0) {
            Helper::handleError(READ_ERROR, errno);
        }

        strncat(string, rawBytes, chunk);
        readBytes += chunk;
    }

    return string;
}


void Socket::writeStringInChunks(char* string) {
    // Add one byte for the terminating character
    int totalBytes = strlen(string) + 1;

    int writtenBytes = 0;
    int chunk;

    if(totalBytes < this->bufferSize) {
        if (::write(createdSocketFd, string, totalBytes) < 0) {
            Helper::handleError(WRITE_ERROR, errno);
        }

        return;
    }

    while(writtenBytes < totalBytes) {
        chunk = ::write(createdSocketFd, string, this->bufferSize);
        if (chunk < 0) {
            Helper::handleError(WRITE_ERROR, errno);
        }

        // We move the string pointer chunk chars ahead to continue the writing
        // from the point is was stopped
        string += chunk;

        writtenBytes += chunk;
    }
}

void Socket::closeSocket() {
    close(createdSocketFd);
    close(acceptedSocketFd);
}
