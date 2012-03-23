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

#define NWEIGHTSSIZE 50

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
	boost::shared_ptr<TH1D> estimatedPileUp;
	boost::array<double, NWEIGHTSSIZE> DATAdistribution;
	boost::array<double, NWEIGHTSSIZE> pileUpWeights;
	unsigned long numberOfEventsWithTooHighPileUp;
	void defineNumberOfProducedEvents();
public:

	EventWeightProvider(std::string datasetInformationFile);
//	EventWeightProvider(boost::shared_ptr<TH1D>,boost::shared_ptr<TH1D>);
//	EventWeightProvider(std::vector<double>, std::vector<double>);
	~EventWeightProvider();
	double getWeight(const EventPtr) const;
	double getPUWeight(const EventPtr) const;

	//@deprecated
	double getWeight(DataType::value type);
	//@deprecated
	double reweightPileUp(unsigned int);
//	boost::shared_ptr<TH1D> getPileUpHistogram(std::string pileUpEstimationFile);
	void generate_weights();
	boost::array<double, NWEIGHTSSIZE> generateWeights(const boost::array<double, NWEIGHTSSIZE>);
	unsigned long getNumberOfEventsWithTooHighPileUp() const;

};

typedef boost::shared_ptr<EventWeightProvider> EventWeightProviderPtr;
/**
 * Input sample distributions from
 * https://twiki.cern.ch/twiki/bin/view/CMS/PileupMCReweightingUtilities#Sample_Input_Distributions
 */

/* Flat10+Tail distribution taken directly from MixingModule input:
 * (Can be used for Spring11 and Summer11 if you don't worry about small shifts in the mean)
 * SHOULD be used for 3-D Reweighting, as this is the "true" input for all Summer11 samples.
 */
//const boost::array<double, NWEIGHTSSIZE> probdistFlat10 = { { 0.0698146584, 0.0698146584, 0.0698146584, 0.0698146584,
//		0.0698146584, 0.0698146584, 0.0698146584, 0.0698146584, 0.0698146584, 0.0698146584, 0.0698146584, 0.0630151648,
//		0.0526654164, 0.0402754482, 0.0292988928, 0.0194384503, 0.0122016783, 0.007207042, 0.004003637, 0.0020278322,
//		0.0010739954, 0.0004595759, 0.0002229748, 0.0001028162, 4.58337152809607E-05, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
//		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } };

/*
 * Summer11 PU_S3 and PU_S4, distribution obtained by only looking at the in-time crossing.
 * This is the "spike+smear" distribution, RECOMMENDED FOR REWEIGHTING.
 */

const boost::array<double, NWEIGHTSSIZE> PoissonOneXDist = { { 1.45346E-01, 6.42802E-02, 6.95255E-02, 6.96747E-02,
		6.92955E-02, 6.84997E-02, 6.69528E-02, 6.45515E-02, 6.09865E-02, 5.63323E-02, 5.07322E-02, 4.44681E-02,
		3.79205E-02, 3.15131E-02, 2.54220E-02, 2.00184E-02, 1.53776E-02, 1.15387E-02, 8.47608E-03, 6.08715E-03,
		4.28255E-03, 2.97185E-03, 2.01918E-03, 1.34490E-03, 8.81587E-04, 5.69954E-04, 3.61493E-04, 2.28692E-04,
		1.40791E-04, 8.44606E-05, 5.10204E-05, 3.07802E-05, 1.81401E-05, 1.00201E-05, 5.80004E-06, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 } };

// Summer11 PU_S4, distribution obtained by averaging the number of interactions
// in each beam crossing to estimate the true mean.

const boost::array<double, NWEIGHTSSIZE> PoissonIntDist = { { 0.104109, 0.0703573, 0.0698445, 0.0698254, 0.0697054,
		0.0697907, 0.0696751, 0.0694486, 0.0680332, 0.0651044, 0.0598036, 0.0527395, 0.0439513, 0.0352202, 0.0266714,
		0.019411, 0.0133974, 0.00898536, 0.0057516, 0.00351493, 0.00212087, 0.00122891, 0.00070592, 0.000384744,
		0.000219377, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
		0.0, 0.0, 0.0, 0.0, 0.0 } };

// Distribution used for Fall2011 MC.

const boost::array<double, NWEIGHTSSIZE> Fall2011 = { { 0.003388501, 0.010357558, 0.024724258, 0.042348605, 0.058279812,
		0.068851751, 0.072914824, 0.071579609, 0.066811668, 0.060672356, 0.054528356, 0.04919354, 0.044886042,
		0.041341896, 0.0384679, 0.035871463, 0.03341952, 0.030915649, 0.028395374, 0.025798107, 0.023237445,
		0.020602754, 0.0180688, 0.015559693, 0.013211063, 0.010964293, 0.008920993, 0.007080504, 0.005499239,
		0.004187022, 0.003096474, 0.002237361, 0.001566428, 0.001074149, 0.000721755, 0.000470838, 0.00030268,
		0.000184665, 0.000112883, 6.74043E-05, 3.82178E-05, 2.22847E-05, 1.20933E-05, 6.96173E-06, 3.4689E-06,
		1.96172E-06, 8.49283E-07, 5.02393E-07, 2.15311E-07, 9.56938E-08 } };
}

#endif /* EVENTWEIGHTPROVIDER_H_ */
