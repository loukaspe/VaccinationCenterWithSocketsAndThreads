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

int Socket::readNumber(bool isFromServer) {
    int fd;
    if(isFromServer) {
        fd = acceptedSocketFd;
    } else {
        fd = createdSocketFd;
    }

    int number;

    if(read(fd, &number, sizeof(int)) < 0) {
        Helper::handleError(READ_ERROR, errno);
    }

    return number;
}

void Socket::writeNumber(int number, bool isFromServer) {
    int fd;
    if(isFromServer) {
        fd = acceptedSocketFd;
    } else {
        fd = createdSocketFd;
    }

    if (write(fd, &number, sizeof(int)) < 0) {
        Helper::handleError(WRITE_ERROR, errno);
    }
}

char *Socket::readStringInChunks(int totalBytes, bool isFromServer) {
    int fd;
    if(isFromServer) {
        fd = acceptedSocketFd;
    } else {
        fd = createdSocketFd;
    }

    char string[totalBytes];

    if(totalBytes < this->bufferSize) {
        if (::read(fd, string, totalBytes) < 0) {
            Helper::handleError(READ_ERROR, errno);
        }

        return Helper::copyString(string);
    }

    char rawBytes[this->bufferSize + 1];
    int readBytes = 0;
    int chunk;

    while(readBytes < totalBytes) {
        chunk = ::read(fd, rawBytes, this->bufferSize);
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


void Socket::writeStringInChunks(char* string, bool isFromServer) {
    int fd;
    if(isFromServer) {
        fd = acceptedSocketFd;
    } else {
        fd = createdSocketFd;
    }
    
    // Add one byte for the terminating character
    int totalBytes = strlen(string) + 1;

    int writtenBytes = 0;
    int chunk;

    if(totalBytes < this->bufferSize) {
        if (::write(fd, string, totalBytes) < 0) {
            Helper::handleError(WRITE_ERROR, errno);
        }

        return;
    }

    while(writtenBytes < totalBytes) {
        chunk = ::write(fd, string, this->bufferSize);
        if (chunk < 0) {
            Helper::handleError(WRITE_ERROR, errno);
        }

        // We move the string pointer chunk chars ahead to continue the writing
        // from the point is was stopped
        string += chunk;

        writtenBytes += chunk;
    }
}


int *Socket::readArrayOfInts(int numberOfElements, bool isFromServer) {
    int *array = (int*) malloc(numberOfElements * sizeof(int));

    for(int i = 0; i < numberOfElements; i++) {
        array[i] = readNumber(isFromServer);
    }

    return array;
}

void Socket::writeArrayOfInts(int* array, int numberOfElements, bool isFromServer) {
    for(int i = 0; i < numberOfElements; i++) {
        writeNumber(array[i], isFromServer);
    }
}


void Socket::writeBitArrayInChunks(BitArray* bitArray, bool isFromServer) {
    int fd;
    if(isFromServer) {
        fd = acceptedSocketFd;
    } else {
        fd = createdSocketFd;
    }

    /* BitArray is an array of ints. So we firstly pass the array in order to set him
     * to the read bitArray */
    this->writeNumber(bitArray->getNumberOfBitArrayInts(), isFromServer);
    this->writeArrayOfInts(bitArray->getBitArray(), bitArray->getNumberOfBitArrayInts(), isFromServer);

    int totalBytes = sizeof(BitArray);

    int writtenBytes = 0;
    int chunk;

    if(totalBytes < this->bufferSize) {
        if (::write(fd, bitArray, totalBytes) < 0) {
            Helper::handleError(WRITE_ERROR, errno + 4);
        }

        return;
    }

    while(writtenBytes < totalBytes) {
        chunk = ::write(fd, bitArray, this->bufferSize);

        if (chunk < 0) {
            Helper::handleError(WRITE_ERROR, errno + 5);
        }

        // We move the string pointer chunk chars ahead to continue the writing
        // from the point is was stopped
        bitArray += chunk;
        writtenBytes += chunk;
    }
}

BitArray* Socket::readBitArrayInChunks(bool isFromServer) {
    int fd;
    if(isFromServer) {
        fd = acceptedSocketFd;
    } else {
        fd = createdSocketFd;
    }

    /* BitArray is an array of ints. So we firstly read the array in order to set him
     * to the read bitArray */
    int numberOfBitArrayInts = this->readNumber(isFromServer);
    int *intsOfBitArray = this->readArrayOfInts(numberOfBitArrayInts, isFromServer);

    int bitArraySize = sizeof(BitArray);

    BitArray* bitArray = (BitArray*) malloc(bitArraySize);
    if (bitArray == NULL) {
        Helper::handleError("Error: Could not allocate memory", errno);
    }

    if(bitArraySize <= this->bufferSize) {
        if (::read(fd, bitArray, bitArraySize) < 0) {
            Helper::handleError(READ_ERROR, errno);
        }

        bitArray->setBitArray(intsOfBitArray);

        return bitArray;
    }

    char rawBytes[this->bufferSize];
    int readBytes = 0;
    int chunk;

    while(readBytes < bitArraySize) {
        chunk = ::read(fd, rawBytes, this->bufferSize);
        if (chunk < 0) {
            Helper::handleError(READ_ERROR, errno);
        }


        memcpy(bitArray + readBytes, rawBytes, chunk);
        readBytes += chunk;
    }

    bitArray->setBitArray(intsOfBitArray);

    return bitArray;
}


BloomFilter *Socket::readBloomFilterInChunks(bool isFromServer) {
    int fd;
    if(isFromServer) {
        fd = acceptedSocketFd;
    } else {
        fd = createdSocketFd;
    }

    // While reading a BloomFilter, firstly we have to read it's pointer variables,
    // and create them in order to pass them in the new BF
    BitArray* bitArray = this->readBitArrayInChunks(isFromServer);

    int countryNameLength = this->readNumber(isFromServer);
    char* countryName = this->readStringInChunks(countryNameLength, isFromServer);

    int virusNameLength = this->readNumber(isFromServer);
    char* virusName = this->readStringInChunks(virusNameLength, isFromServer);

    int bloomFilterSize = sizeof(BloomFilter);

    BloomFilter* bloomFilter = (BloomFilter*) malloc(bloomFilterSize);
    if (bloomFilter == NULL) {
        Helper::handleError("Error: Could not allocate memory", errno);
    }

    if(bloomFilterSize <= this->bufferSize) {
        if (::read(fd, bloomFilter, bloomFilterSize) < 0) {
            Helper::handleError(READ_ERROR, errno);
        }

        bloomFilter->setVirusName(virusName);
        bloomFilter->setCountryName(countryName);
        bloomFilter->setBitArray(bitArray);

        return bloomFilter;
    }

    char rawBytes[this->bufferSize];
    int readBytes = 0;
    int chunk;

    while(readBytes < bloomFilterSize) {
        chunk = ::read(fd, rawBytes, this->bufferSize);
        if (chunk < 0) {
            Helper::handleError(READ_ERROR, errno);
        }


        memcpy(bloomFilter + readBytes, rawBytes, chunk);
        readBytes += chunk;
    }

    bloomFilter->setVirusName(virusName);
    bloomFilter->setCountryName(countryName);
    bloomFilter->setBitArray(bitArray);

    return bloomFilter;
}

void Socket::writeBloomFilterInChunks(BloomFilter *bloomFilter, bool isFromServer) {
    int fd;
    if(isFromServer) {
        fd = acceptedSocketFd;
    } else {
        fd = createdSocketFd;
    }

    // While writing a BloomFilter, firstly we have to write it's pointer variables
    this->writeBitArrayInChunks(bloomFilter->getBitArray(), isFromServer);

    int countryNameLength = strlen(bloomFilter->getCountryName()) + 1;
    this->writeNumber(countryNameLength, isFromServer);
    this->writeStringInChunks(bloomFilter->getCountryName(), isFromServer);

    int virusNameLength = strlen(bloomFilter->getVirusName()) + 1;
    this->writeNumber(virusNameLength, isFromServer);
    this->writeStringInChunks(bloomFilter->getVirusName(), isFromServer);

    int totalBytes = sizeof(BloomFilter);

    int writtenBytes = 0;
    int chunk;

    if(totalBytes < this->bufferSize) {
        if (::write(fd, bloomFilter, totalBytes) < 0) {
            Helper::handleError(WRITE_ERROR, errno + 2);
        }

        return;
    }

    while(writtenBytes < totalBytes) {
        chunk = ::write(fd, bloomFilter, this->bufferSize);

        if (chunk < 0) {
            Helper::handleError(WRITE_ERROR, errno + 3);
        }

        // We move the string pointer chunk chars ahead to continue the writing
        // from the point is was stopped
        bloomFilter += chunk;
        writtenBytes += chunk;
    }
}

void Socket::closeSocket() {
    close(createdSocketFd);
    close(acceptedSocketFd);
}