/*
 * CrossSections.h
 *
 *  Created on: 29 Jul 2010
 *      Author: kreczko
 */

#ifndef CROSSSECTIONS_H_
#define CROSSSECTIONS_H_
//https://twiki.cern.ch/twiki/bin/view/CMS/CrossSections_3XSeries#crosssections

#include "DataTypes.h"
#include <vector>
#include <boost/array.hpp>

namespace BAT {
namespace sevenTeV {
extern boost::array<float, DataType::NUMBER_OF_DATA_TYPES> getXSections();
}


struct CrossSectionProvider{
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


    CrossSectionProvider(float lumiInInversePb, unsigned short tev = 7);
    ~CrossSectionProvider();

    void useSkimEfficiency(bool use);

    float getExpectedNumberOfEvents(DataType::value type);
    float getWeight(DataType::value type);

};
}

#endif /* CROSSSECTIONS_H_ */
