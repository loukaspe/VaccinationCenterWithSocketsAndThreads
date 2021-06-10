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
    char string[totalBytes];

    if(totalBytes < this->bufferSize) {
        if (::read(acceptedSocketFd, string, totalBytes) < 0) {
            Helper::handleError(READ_ERROR, errno);
        }

        return Helper::copyString(string);
    }

    char rawBytes[this->bufferSize + 1];
    int readBytes = 0;
    int chunk;

    while(readBytes < totalBytes) {
        chunk = ::read(acceptedSocketFd, rawBytes, this->bufferSize);
        rawBytes[this->bufferSize] = '\0';
        if (chunk < 0) {
            Helper::handleError(READ_ERROR, errno);
        }

        if(readBytes == 0) {
            strncpy(string, rawBytes, chunk);
            string[this->bufferSize] = '\0';
        } else {
            strncat(string, rawBytes, chunk);
        }

        readBytes += chunk;
    }

    return Helper::copyString(string);
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


int *Socket::readArrayOfInts(int numberOfElements) {
    int *array = (int*) malloc(numberOfElements * sizeof(int));

    for(int i = 0; i < numberOfElements; i++) {
        array[i] = readNumber();
    }

    return array;
}

void Socket::writeArrayOfInts(int* array, int numberOfElements) {
    for(int i = 0; i < numberOfElements; i++) {
        writeNumber(array[i]);
    }
}


void Socket::writeBitArrayInChunks(BitArray* bitArray) {
    /* BitArray is an array of ints. So we firstly pass the array in order to set him
     * to the read bitArray */
    this->writeNumber(bitArray->getNumberOfBitArrayInts());
    this->writeArrayOfInts(bitArray->getBitArray(), bitArray->getNumberOfBitArrayInts());

    int totalBytes = sizeof(BitArray);

    int writtenBytes = 0;
    int chunk;

    if(totalBytes < this->bufferSize) {
        if (::write(createdSocketFd, bitArray, totalBytes) < 0) {
            Helper::handleError(WRITE_ERROR, errno + 4);
        }

        return;
    }

    while(writtenBytes < totalBytes) {
        chunk = ::write(createdSocketFd, bitArray, this->bufferSize);
        if (chunk < 0) {
            Helper::handleError(WRITE_ERROR, errno + 5);
        }

        // We move the string pointer chunk chars ahead to continue the writing
        // from the point is was stopped
        bitArray += chunk;
        writtenBytes += chunk;
    }
}

BitArray* Socket::readBitArrayInChunks() {
    /* BitArray is an array of ints. So we firstly read the array in order to set him
     * to the read bitArray */
    int numberOfBitArrayInts = this->readNumber();
    int *intsOfBitArray = this->readArrayOfInts(numberOfBitArrayInts);

    int bitArraySize = sizeof(BitArray);

    BitArray* bitArray = (BitArray*) malloc(bitArraySize);
    if (bitArray == NULL) {
        Helper::handleError("Error: Could not allocate memory", errno);
    }

    if(bitArraySize <= this->bufferSize) {
        if (::read(acceptedSocketFd, bitArray, bitArraySize) < 0) {
            Helper::handleError(READ_ERROR, errno);
        }

        bitArray->setBitArray(intsOfBitArray);

        return bitArray;
    }

    char rawBytes[this->bufferSize];
    int readBytes = 0;
    int chunk;

    while(readBytes < bitArraySize) {
        chunk = ::read(acceptedSocketFd, rawBytes, this->bufferSize);
        if (chunk < 0) {
            Helper::handleError(READ_ERROR, errno);
        }

        memcpy(bitArray + readBytes, rawBytes, chunk);
        readBytes += chunk;
    }

    bitArray->setBitArray(intsOfBitArray);

    return bitArray;
}

void Socket::closeSocket() {
    close(createdSocketFd);
    close(acceptedSocketFd);
}