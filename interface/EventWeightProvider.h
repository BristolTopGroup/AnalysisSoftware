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
#include <boost/shared_ptr.hpp>
#include "TH1D.h"

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
    boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> numberOfProcessedEvents;
//    boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> numberOfSkimmedEvents;
    boost::shared_ptr<TH1D> estimatedPileUp;
    boost::array<double, 25> pileUpWeights;
    unsigned long numberOfEventsWithTooHighPileUp;
//    void defineNumberOfSkimmedEvents();
    void defineNumberOfProducedEvents();
public:


    EventWeightProvider(float lumiInInversePb, unsigned short tev = 7, std::string pileUpEstimationFile = "pileUp.root");
    ~EventWeightProvider();

//    void useSkimEfficiency(bool use);

//    float getExpectedNumberOfEvents(DataType::value type);
    float getWeight(DataType::value type);
    float reweightPileUp(unsigned int numberOfVertices);
    boost::shared_ptr<TH1D> getPileUpHistogram(std::string pileUpEstimationFile);
    void generate_flat10_weights();
    unsigned long getNumberOfEventsWithTooHighPileUp() const;

};
}

#endif /* EVENTWEIGHTPROVIDER_H_ */
