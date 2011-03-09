/*
 * EventCounter.h
 *
 *  Created on: 4 Jul 2010
 *      Author: kreczko
 */

#ifndef EVENTCOUNTER_H_
#define EVENTCOUNTER_H_
#include "boost/multi_array.hpp"

namespace BAT {
typedef boost::multi_array<unsigned long, 3> integerCounter;
typedef integerCounter::size_type size_type_int;

typedef boost::multi_array<double, 3> floatCounter;
typedef floatCounter::size_type size_type_float;

struct Counter {
private:
    unsigned int dim1, dim2, dim3;
    integerCounter unweightedEntries;
    floatCounter weightedEntries;
    unsigned int eventType;

public:
    Counter() {
    }

    Counter(unsigned int dimension1, unsigned int dimension2, unsigned int dimension3) :
        dim1(dimension1),
        dim2(dimension2),
        dim3(dimension3),
        unweightedEntries(boost::extents[dimension1][dimension2][dimension3]),
        weightedEntries(boost::extents[dimension1][dimension2][dimension3]) {
    }

    ~Counter() {
    }

    void increase(const unsigned int dimension1, const unsigned int dimension2, const unsigned int dimension3,
            const double weight = 1.0) {
        unweightedEntries[dimension1][dimension2][dimension3]++;
        weightedEntries[dimension1][dimension2][dimension3] += weight;
    }

    unsigned int getEntries(unsigned int dimension1, unsigned int dimension2, unsigned int dimension3) const{
        return unweightedEntries[dimension1][dimension2][dimension3];
    }

    float getWeightedEntries(unsigned int dimension1, unsigned int dimension2, unsigned int dimension3) const{
        return weightedEntries[dimension1][dimension2][dimension3];
    }

    unsigned int sumThirdDimension(unsigned int dimension1, unsigned int dimension2) const{
        unsigned int sum = 0;
        for(unsigned int third = 0; third < dim3; ++third){
            sum += weightedEntries[dimension1][dimension2][third];
        }
        return sum;
    }

    unsigned int sumThirdDimensionUnweighted(unsigned int dimension1, unsigned int dimension2) const{
            unsigned int sum = 0;
            for(unsigned int third = 0; third < dim3; ++third){
                sum += unweightedEntries[dimension1][dimension2][third];
            }
            return sum;
        }

    unsigned int getSizeOfFirstDimension() const{
        return unweightedEntries.size();
    }

    unsigned int getSizeOfSecondDimension() const{
        return unweightedEntries.shape()[1];
    }

    unsigned int getSizeOfThirdDimension() const{
        return unweightedEntries.shape()[2];
    }

    //TODO: subcount

};
}
#endif /* EVENTCOUNTER_H_ */
