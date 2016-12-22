/**
 * CrossSections.h
 *
 *  Created on: 29 Jul 2010
 *      Author: kreczko
 *      Adaptation of
 *      PhysicsTools/Utilities/interface/LumiReWeighting.h
 *      PhysicsTools/Utilities/interface/Lumi3DReWeighting.h
 *
 */

#ifndef EVENTWEIGHTPROVIDER_H_
#define EVENTWEIGHTPROVIDER_H_
#include "DataTypes.h"
#include "Python/DatasetInformation.h"

#include <vector>

#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>

#include "TH1D.h"

#define NWEIGHTSSIZE 75

namespace BAT {

namespace PileUpReweightingMethod {
enum value {
	averagePileUp, //
	inTimePileUpOnly, //
	threeDReweighting
};
}
}
#include "Event.h"
namespace BAT{


class EventWeightProvider {
private:
	DatasetInformation datasetInfo_;
//	float lumiInInversePb;
//	unsigned short tev;
//	bool useSkimEff;
	boost::array<double, DataType::NUMBER_OF_DATA_TYPES> xsection;
	boost::array<unsigned long, DataType::NUMBER_OF_DATA_TYPES> numberOfProcessedEvents;
	boost::shared_ptr<TH1D> estimatedPileUp, estimatedPileUp_up, estimatedPileUp_down;
	boost::array<double, NWEIGHTSSIZE> DATAdistribution;
	boost::array<double, NWEIGHTSSIZE> pileUpWeights, pileUpWeights_up, pileUpWeights_down;
	unsigned long numberOfEventsWithTooHighPileUp;
	void defineNumberOfProducedEvents();
public:

	EventWeightProvider(std::string datasetInformationFile);
	EventWeightProvider(std::string datasetInformationFile, boost::shared_ptr<TH1D>,boost::shared_ptr<TH1D>);
	EventWeightProvider(std::string datasetInformationFile, std::vector<double>, std::vector<double>);
	~EventWeightProvider();
	double getWeight(const EventPtr) const;
	double getPUWeight(const EventPtr) const;

	double reweightTopPt(const EventPtr);
	//@deprecated
	double getWeight(DataType::value type);
	//@deprecated
	double reweightPileUp(unsigned int nVertices, int systematic = 0);
//	boost::shared_ptr<TH1D> getPileUpHistogram(std::string pileUpEstimationFile);
	void generate_weights();
	boost::array<double, NWEIGHTSSIZE> generateWeights(const boost::array<double, NWEIGHTSSIZE>, const boost::shared_ptr<TH1D>);
	unsigned long getNumberOfEventsWithTooHighPileUp() const;

};

typedef boost::shared_ptr<EventWeightProvider> EventWeightProviderPtr;

const boost::array<double, NWEIGHTSSIZE> Moriond2017 = {{
    1.78653e-05 ,
    2.56602e-05,
    5.27857e-05,
    8.88954e-05,
    0.000109362,
    0.000140973,
    0.000240998,
    0.00071209,
    0.00130121,
    0.00245255,
    0.00502589,
    0.00919534,
    0.0146697,
    0.0204126,
    0.0267586,
    0.0337697,
    0.0401478,
    0.0450159,
    0.0490577,
    0.0524855,
    0.0548159,
    0.0559937,
    0.0554468,
    0.0537687,
    0.0512055,
    0.0476713,
    0.0435312,
    0.0393107,
    0.0349812,
    0.0307413,
    0.0272425,
    0.0237115,
    0.0208329,
    0.0182459,
    0.0160712,
    0.0142498,
    0.012804,
    0.011571,
    0.010547,
    0.00959489,
    0.00891718,
    0.00829292,
    0.0076195,
    0.0069806,
    0.0062025,
    0.00546581,
    0.00484127,
    0.00407168,
    0.00337681,
    0.00269893,
    0.00212473,
    0.00160208,
    0.00117884,
    0.000859662,
    0.000569085,
    0.000365431,
    0.000243565,
    0.00015688,
    9.88128e-05,
    6.53783e-05,
    3.73924e-05,
    2.61382e-05,
    2.0307e-05,
    1.73032e-05,
    1.435e-05,
    1.36486e-05,
    1.35555e-05,
    1.37491e-05,
    1.34255e-05,
    1.33987e-05,
    1.34061e-05,
    1.34211e-05,
    1.34177e-05,
    1.32959e-05,
    1.33287e-05
} };

}

#endif /* EVENTWEIGHTPROVIDER_H_ */
