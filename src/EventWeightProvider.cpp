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
		DATAdistribution(), //
		pileUpWeights(), //
		numberOfEventsWithTooHighPileUp(0) {

	generate_weights();
}

EventWeightProvider::~EventWeightProvider() {

}

double EventWeightProvider::getWeight(DataType::value type) {
	if (type <= DataType::SingleMuon_ReReco_RunD)
		return 1.;
	else
		return xsection[type] * Globals::luminosity / numberOfProcessedEvents[type];
}

double EventWeightProvider::reweightPileUp(unsigned int numberOfVertices) {
	if (numberOfVertices >= pileUpWeights.size()) {
		++numberOfEventsWithTooHighPileUp;
		return 1.;
	}

	return pileUpWeights.at(numberOfVertices);
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

	if (Globals::energyInTeV == 13) {
		pileUpWeights = generateWeights(Spring2015_50ns);
	}

//	cout << "Pile up weights" << endl;
//
//	for (unsigned int index = 0; index < pileUpWeights.size(); ++index){
//		cout << index << ": " << pileUpWeights.at(index) << endl;
//	}
}

boost::array<double, NWEIGHTSSIZE> EventWeightProvider::generateWeights(
		const boost::array<double, NWEIGHTSSIZE> inputMC) {
	boost::array<double, NWEIGHTSSIZE> weights;


	// Check that integral of estimatedPileUp is 1
	if ( estimatedPileUp->Integral() != 1 && estimatedPileUp->Integral() != 0 ) {
		estimatedPileUp->Scale( 1 / estimatedPileUp->Integral() );
	}

	double s = 0.0;

	for (unsigned int npu = 0; npu < inputMC.size(); ++npu) {
		if (npu >= (unsigned int) estimatedPileUp->GetNbinsX())
			break;
		if ( npu == 0 ) {
			DATAdistribution[npu] = 0;
			weights[npu] = 0;
			continue;
		}
		else {
			DATAdistribution[npu] = estimatedPileUp->GetBinContent(estimatedPileUp->GetXaxis()->FindBin(npu));			
		}

		if (inputMC[npu-1] > 0) {
			weights[npu] = DATAdistribution[npu] / inputMC[npu-1];
		}
		else
			weights[npu] = 0;
		s += DATAdistribution[npu];
	}
	/**
	 * normalize weights such that the total sum of weights over thw whole sample is 1.0,
	 * i.e., sum_i  result[i] * npu_probs[i] should be 1.0 (!)
	 */
	for (unsigned int npu = 0; npu < pileUpWeights.size(); ++npu) {
		weights[npu] /= s;
	}

	return weights;
}

unsigned long EventWeightProvider::getNumberOfEventsWithTooHighPileUp() const {
	return numberOfEventsWithTooHighPileUp;
}

} // namespace BAT
