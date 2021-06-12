/* IMPORTANT DISCLAIMER:

My classes BloomFilter and BitArray are copied from another university project, and
specifically from Operating Systems with A.Delis.
Only change made was about the size that you give in the Description of the Exercise (100KBytes).
No other change made since their use was exactly what I needed for here too. */


/* Implementation of Bloom Filter was based on:
 * 1. Your PDF http://cgi.di.uoa.gr/~ad/k22/Bloom_Filters.pdf
 * 2. https://www.geeksforgeeks.org/bloom-filters-introduction-and-python-implementation/
 * 3. http://www.mathcs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
 *    which is help for Bit Array Implementation */

#include "BloomFilter.h"

/* Const number of Hash Functions that will be used in the Bloom Filter
 * based on your propositions in section 6 of the given
 * PDF in http://cgi.di.uoa.gr/~ad/k22/Bloom_Filters.pdf */

const int BloomFilter::NUMBER_OF_HASH_FUNCTIONS = 3;

// Default size = 100 * 1024 Bytes = 100 * 1024 * 8 bits = 819200 bits
BloomFilter::BloomFilter(int size): size(size)
{
    this->bitArray = new BitArray(size);
}

BloomFilter::BloomFilter(
    char *countryName,
    char *virusName,
    BitArray *bitArray,
    int size
)
    :countryName(countryName),
    virusName(virusName),
    bitArray(bitArray),
    size(size)
{}

/* As Hash Functions I use djb2 and sdbm without implementing it myself. Source of
 * algorithms is: http://www.cse.yorku.ca/~oz/hash.html
 * Permission for this in: https://piazza.com/class/k1631q2t5o11rj?cid=6 */

unsigned long BloomFilter::firstHashFunction(char* key) {
    unsigned long hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

unsigned long BloomFilter::secondHashFunction(char* key) {
    unsigned long hash = 0;
    int c;

    while (c = *key++)
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

/* Based on the Kirsch-Mitzenmacher Optimization we only need 2 Hash Functions
 * and the we use the following transformation to compute the digests of the k
 * hash functions: g(x) = h1(x) + ih2(x) 0 ≤ i ≤ k − 1
 * The Function returns an array of the digests  */

unsigned long* BloomFilter::getHashDigests(char* key) {

    unsigned long* digests = (unsigned long*) malloc( BloomFilter::NUMBER_OF_HASH_FUNCTIONS * sizeof(unsigned long) );

    int i;
    unsigned long hashFunctionsSum;
    for(i = 0; i < BloomFilter::NUMBER_OF_HASH_FUNCTIONS; i++) {
        hashFunctionsSum  = firstHashFunction(key) +
                            i * secondHashFunction(key);

        digests[i] = hashFunctionsSum % this->size;
    }

    return digests;
}

void BloomFilter::add(char* key) {
    unsigned long* digests = getHashDigests(key);

    for(int i = 0; i < NUMBER_OF_HASH_FUNCTIONS; i++) {
        bitArray->setBit(digests[i]);
        assert( bitArray->isSet( digests[i] ) );
    }
}

bool BloomFilter::check(char* key) {
    unsigned long* digests = getHashDigests(key);

    for(int i = 0; i < NUMBER_OF_HASH_FUNCTIONS; i++) {
        if( !bitArray->isSet( digests[i] ) ) {
            return false;
        }
    }

    return true;
}

char *BloomFilter::getVirusName() const {
    return virusName;
}

void BloomFilter::setVirusName(char *virusName) {
    BloomFilter::virusName = virusName;
}

char *BloomFilter::getCountryName() const {
    return countryName;
}

void BloomFilter::setCountryName(char *countryName) {
    BloomFilter::countryName = countryName;
}

BitArray *BloomFilter::getBitArray() const {
    return bitArray;
}

void BloomFilter::setBitArray(BitArray *bitArray) {
    BloomFilter::bitArray = bitArray;
}
