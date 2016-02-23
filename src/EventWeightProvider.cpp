/*
 * CrossSections.cpp
 *
 *  Created on: 13 Aug 2010
 *      Author: kreczko
 */

#include "../interface/EventWeightProvider.h"
#include "../interface/GlobalVariables.h"

#include "TFile.h"

#include <boost/scoped_ptr.hpp>
#include <boost/filesystem.hpp>

#include <iostream>

using namespace std;
namespace BAT {

EventWeightProvider::EventWeightProvider(string datasetInformationFile) :
		datasetInfo_(datasetInformationFile), //
		xsection(datasetInfo_.getCrossSections()), //
		numberOfProcessedEvents(datasetInfo_.getArrayOfProcessedEvents()), //
		estimatedPileUp(Globals::estimatedPileup), //
		estimatedPileUp_up(Globals::estimatedPileup_up), //
		estimatedPileUp_down(Globals::estimatedPileup_down), //
		DATAdistribution(), //
		pileUpWeights(), //
		numberOfEventsWithTooHighPileUp(0) {

	generate_weights();
}

EventWeightProvider::~EventWeightProvider() {

}

double EventWeightProvider::getWeight(DataType::value type) {
	if (type <= DataType::SingleMuon)
		return 1.;
	else
		return xsection[type] * Globals::luminosity / numberOfProcessedEvents[type];
}

double EventWeightProvider::reweightPileUp(unsigned int numberOfVertices, int systematic ) {

	boost::array<double, NWEIGHTSSIZE>* weights = &pileUpWeights;
	if ( systematic == -1 ) {
		weights = &pileUpWeights_down;
	}
	else if ( systematic == 1 ) {
		weights = &pileUpWeights_up;
	}

	if (numberOfVertices >= weights->size()) {
		++numberOfEventsWithTooHighPileUp;
		return 1.;
	}

	return weights->at(numberOfVertices);
}

double EventWeightProvider::reweightTopPt(const EventPtr event) {
	const MCParticleCollection genPart(event->GenParticles());

	double topPt = 0.;
	double tbarPt = 0.;
	for (unsigned int i = 0; i < genPart.size(); i++) {

		if (genPart.at(i)->pdgId() == (6))
		  	  topPt = genPart.at(i)->pt();

		if (genPart.at(i)->pdgId() == (-6))
		  	  tbarPt = genPart.at(i)->pt();
	}

	double SFtop = exp(0.159-0.00141*topPt);
	double SFtbar = exp(0.159-0.00141*tbarPt);

	double weight=sqrt(SFtop*SFtbar);
  return weight;
}

//boost::shared_ptr<TH1D> EventWeightProvider::getPileUpHistogram(std::string pileUpEstimationFile) {
//	using namespace std;
//
//	if (!boost::filesystem::exists(pileUpEstimationFile))
//		throw "Exception in EventWeightProvider::getPileUpHistogram: could not find pile-up file '"
//				+ pileUpEstimationFile + "'";
//
//	cout << "Using pile-up estimation file " << pileUpEstimationFile << endl;
//	boost::scoped_ptr<TFile> file(TFile::Open(pileUpEstimationFile.c_str()));
//	boost::shared_ptr<TH1D> pileUp((TH1D*) file->Get("pileup")->Clone());
//	file->Close();
//
//	return pileUp;
//}

void EventWeightProvider::generate_weights() {


	/*
	 * TODO:
	 * Summer11 PU_S3 and PU_S4 in-time PU only: PoissonOneXDist
	 * if Summer11 OOT average PU, use PoissonIntDist
	 * If Spring11 or Summer11 3D reweighting, use probdistFlat10 (compare to true)
	 * Fall11, use Fall2011
	 */


	pileUpWeights = generateWeights(Startup2015_25ns, estimatedPileUp);
	pileUpWeights_up = generateWeights(Startup2015_25ns, estimatedPileUp_up);
	pileUpWeights_down = generateWeights(Startup2015_25ns, estimatedPileUp_down);


	cout << "Pile up weights" << endl;

	for (unsigned int index = 0; index < pileUpWeights.size(); ++index){
		cout << index << ": " << pileUpWeights.at(index) << endl;
	}
}

boost::array<double, NWEIGHTSSIZE> EventWeightProvider::generateWeights(
		const boost::array<double, NWEIGHTSSIZE> inputMC,
		const boost::shared_ptr<TH1D> dataEstimatedPileUp ) {
	boost::array<double, NWEIGHTSSIZE> weights;


	// Check that integral of dataEstimatedPileUp is 1

	std::cout << "Data Estimated PU Integral : " << dataEstimatedPileUp->Integral() << std::endl;

	if ( dataEstimatedPileUp->Integral() != 1 && dataEstimatedPileUp->Integral() != 0 ) {
		dataEstimatedPileUp->Scale( 1 / dataEstimatedPileUp->Integral() );
	}

	double s = 0.0;
	double s2 = 0.0;
	double s3 = 0.0;
	double s4 = 0.0;
	double s5 = 0.0;

	for (unsigned int npu = 0; npu < inputMC.size(); ++npu) {

		// +1-1 problem
		DATAdistribution[npu] = dataEstimatedPileUp->GetBinContent(npu+1);

		if (inputMC[npu] > 0) {
			weights[npu] = DATAdistribution[npu] / inputMC[npu];
		}
		else
			weights[npu] = 0;
		// s += DATAdistribution[npu];
		s += weights[npu]*inputMC[npu];
		s2 += DATAdistribution[npu];
		s4 += weights[npu];
		s5 += inputMC[npu];
	}


	/**
	 * normalize weights such that the total sum of weights over the whole sample is 1.0,
	 * i.e., sum_i  result[i] * npu_probs[i] should be 1.0 (!)
	 */
	for (unsigned int npu = 0; npu < weights.size(); ++npu) {
		weights[npu] /= s;
		// std::cout << "Bin Weight : " << npu << " is : " << weights[npu] << std::endl;
		s3 += weights[npu]*inputMC[npu];
	}
	std::cout << "PU MC :" << s5 << std::endl;
	std::cout << "PU Data :" << s2 << std::endl;
	std::cout << "PU Weight :" << s4 << std::endl;
	std::cout << "PU MC*Weight SF :" << s << std::endl;
	std::cout << "PU MC*ReWeight SF, s3 :" << s3 << std::endl;

	return weights;
}

unsigned long EventWeightProvider::getNumberOfEventsWithTooHighPileUp() const {
	return numberOfEventsWithTooHighPileUp;
}

} // namespace BAT
