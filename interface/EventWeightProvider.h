/*
 * CrossSections.h
 *
 *  Created on: 29 Jul 2010
 *      Author: kreczko
 */

#ifndef EVENTWEIGHTPROVIDER_H_
#define EVENTWEIGHTPROVIDER_H_
//https://twiki.cern.ch/twiki/bin/view/CMS/CrossSections_3XSeries#crosssections

#include "DataTypes.h"
#include <vector>
#include <boost/array.hpp>

namespace BAT {
namespace sevenTeV {
extern boost::array<float, DataType::NUMBER_OF_DATA_TYPES> getXSections();
}


class EventWeightProvider{
private:
    float lumiInInversePb;
    unsigned short tev;
    bool useSkimEff;
    boost::array<float, DataType::NUMBER_OF_DATA_TYPES> xsection;
    boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> numberOfProducedEvents;
    boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> numberOfSkimmedEvents;
    void defineNumberOfSkimmedEvents();
    void defineNumberOfProducedEvents();
public:


    EventWeightProvider(float lumiInInversePb, unsigned short tev = 7);
    ~EventWeightProvider();

    void useSkimEfficiency(bool use);

    float getExpectedNumberOfEvents(DataType::value type);
    float getWeight(DataType::value type);
    float reweightPileUp(unsigned int numberOfVertices);

};
}

#endif /* EVENTWEIGHTPROVIDER_H_ */
