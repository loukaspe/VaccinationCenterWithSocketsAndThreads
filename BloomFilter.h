/* IMPORTANT DISCLAIMER:

My classes BloomFilter and BitArray are copied from another university project, and
specifically from Operating Systems with A.Delis.
Only change made was about the size that you give in the Description of the Exercise (100KBytes).
No other change made since their use was exactly what I needed for here too. */

#ifndef UNTITLED_BLOOMFILTER_H
#define UNTITLED_BLOOMFILTER_H

#include "BitArray.h"
#include <cassert>

class BloomFilter {
public:
    explicit BloomFilter(int = 819200);
    /* Ctor to be used in the Sockets. Because countryName, virusName and BitArray
     * are pointers, we have to also transfer them through the sockets and them pass
     * them to the BloomFilter */
    BloomFilter(char*, char*, BitArray*, int = 819200);
    void add(char*);
    bool check(char*);
    char *getVirusName() const;
    void setVirusName(char *virusName);
    char *getCountryName() const;
    void setCountryName(char *countryName);
    BitArray *getBitArray() const;
    void setBitArray(BitArray *bitArray);
private:
    const static int NUMBER_OF_HASH_FUNCTIONS;
    int size;
    BitArray* bitArray;

    unsigned long firstHashFunction(char*);
    unsigned long secondHashFunction(char*);
    unsigned long* getHashDigests(char*);
    char* virusName;
    char* countryName;
};

#endif //UNTITLED_BLOOMFILTER_H