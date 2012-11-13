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

#define NWEIGHTSSIZE 60

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
	EventWeightProvider(std::string datasetInformationFile, boost::shared_ptr<TH1D>,boost::shared_ptr<TH1D>);
	EventWeightProvider(std::string datasetInformationFile, std::vector<double>, std::vector<double>);
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

const boost::array<double, 60> Summer2012 = { {
        2.560E-06,
        5.239E-06,
        1.420E-05,
        5.005E-05,
        1.001E-04,
        2.705E-04,
        1.999E-03,
        6.097E-03,
        1.046E-02,
        1.383E-02,
        1.685E-02,
        2.055E-02,
        2.572E-02,
        3.262E-02,
        4.121E-02,
        4.977E-02,
        5.539E-02,
        5.725E-02,
        5.607E-02,
        5.312E-02,
        5.008E-02,
        4.763E-02,
        4.558E-02,
        4.363E-02,
        4.159E-02,
        3.933E-02,
        3.681E-02,
        3.406E-02,
        3.116E-02,
        2.818E-02,
        2.519E-02,
        2.226E-02,
        1.946E-02,
        1.682E-02,
        1.437E-02,
        1.215E-02,
        1.016E-02,
        8.400E-03,
        6.873E-03,
        5.564E-03,
        4.457E-03,
        3.533E-03,
        2.772E-03,
        2.154E-03,
        1.656E-03,
        1.261E-03,
        9.513E-04,
        7.107E-04,
        5.259E-04,
        3.856E-04,
        2.801E-04,
        2.017E-04,
        1.439E-04,
        1.017E-04,
        7.126E-05,
        4.948E-05,
        3.405E-05,
        2.322E-05,
        1.570E-05,
        5.005E-06} };


#endif /* EVENTWEIGHTPROVIDER_H_ */
