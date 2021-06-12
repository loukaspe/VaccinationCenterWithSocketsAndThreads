/* Based on http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html */
#include "BitArray.h"

const char* BitArray::WRONG_SIZE_ERROR = "ERROR: Size of BitArray must be greated than zero.\n";
const int BitArray::IS_SET = 1;
const int BitArray::IS_NOT_SET = 0;

/* n is the desired size of BitArray */
BitArray::BitArray(int n) {
    if(n <= 0) {
        Helper::handleError(BitArray::WRONG_SIZE_ERROR);
    }

    int numberOfIntRequiredForNBits = ceil(
            ( (double) n ) / ( (double) 32 )
    );

    this->size = n;

    this->numberOfBitArrayInts = numberOfIntRequiredForNBits;
    this->bitArray = (int*) malloc(
            numberOfIntRequiredForNBits * sizeof(int)
    );
}

BitArray::~BitArray() {
    free(this->bitArray);
}

void BitArray::setBit(int position) {
    this->bitArray[position/32] |= 1 << (position%32);
}

int BitArray::isSet(int position) {
    if ( (this->bitArray[position/32] & (1 << (position%32) )) != 0 ) {
        return BitArray::IS_SET;
    }

    return BitArray::IS_NOT_SET;
}

void BitArray::setBitArray(int *bitArray) {
    this->bitArray = bitArray;
}

int BitArray::getSize() const {
    return size;
}

int *BitArray::getBitArray() {
    return this->bitArray;
}

int BitArray::getNumberOfBitArrayInts() const {
    return numberOfBitArrayInts;
}
