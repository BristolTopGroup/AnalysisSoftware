/*
 * ChiSquaredBasedTopPairReconstruction.cpp
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#include "../../interface/ReconstructionModules/ChiSquaredBasedTopPairReconstruction.h"
#include <vector>
#include <numeric>

namespace BAT {

double const ChiSquaredBasedTopPairReconstruction::leptonic_top_mass = 169.0;
double const ChiSquaredBasedTopPairReconstruction::leptonic_top_mass_sigma = 16.3;
double const ChiSquaredBasedTopPairReconstruction::hadronic_W_mass = 83.;
double const ChiSquaredBasedTopPairReconstruction::hadronic_W_mass_sigma = 10.9;
double const ChiSquaredBasedTopPairReconstruction::hadronic_top_mass = 174.7;
double const ChiSquaredBasedTopPairReconstruction::hadronic_top_mass_sigma = 14.6;
double const ChiSquaredBasedTopPairReconstruction::pt_ttbarSystem = 0.;
double const ChiSquaredBasedTopPairReconstruction::pt_ttbarSystem_sigma = 50.;
double const ChiSquaredBasedTopPairReconstruction::HTSystem = 1;
double const ChiSquaredBasedTopPairReconstruction::HTSystem_sigma = 0.15;

double ChiSquaredBasedTopPairReconstruction::getDiscriminator(const TtbarHypothesisPointer solution) const{
	std::vector<double> chiSquaredTerms;

	//trasnverse momentum of the ttbar system
	double pttbar = solution->PtTtbarSystem();
	double pttbarDifference = TMath::Power(pttbar - pt_ttbarSystem, 2);
	double pttbarError = 2 * TMath::Power(pt_ttbarSystem_sigma, 2);
	double pttbarTerm = pttbarDifference / pttbarError;
	chiSquaredTerms.push_back(pttbarTerm);

	//fraction of sum(pt) of selected jets divided by sum(pt) of all jets
	double htSystemDifference = TMath::Power(solution->sumPt() / HT(jets.size()) - HTSystem, 2);
	double htSystemError = 2 * TMath::Power(HTSystem_sigma, 2);
	double htSystemTerm = htSystemDifference / htSystemError;
	chiSquaredTerms.push_back(htSystemTerm);

	//mass of the hadronic W
	double WmassDifference = TMath::Power(solution->hadronicW->mass() - hadronic_W_mass, 2);
	double WmassError = 2 * TMath::Power(hadronic_W_mass_sigma, 2);
	double WmassTerm = WmassDifference / WmassError;
	chiSquaredTerms.push_back(WmassTerm);

	//mass of the hadronic top quark
	double topMassDifference = TMath::Power(solution->hadronicTop->mass() - hadronic_top_mass, 2);
	double topMassError = 2 * TMath::Power(hadronic_top_mass_sigma, 2);
	double topMassTerm = topMassDifference / topMassError;
	chiSquaredTerms.push_back(topMassTerm);

	//mass of the leptonic top quark
	double topMass = solution->leptonicTop->mass();
	double massDifference = TMath::Power(topMass - leptonic_top_mass, 2);
	double massError = 2 * TMath::Power(leptonic_top_mass_sigma, 2);
	double massTerm = massDifference / massError;
	chiSquaredTerms.push_back(massTerm);

	double totalChi2(0);
	for(unsigned short index = 0; index < chiSquaredTerms.size(); ++index){
		totalChi2 += chiSquaredTerms.at(index);
	}

	return totalChi2;
}


double ChiSquaredBasedTopPairReconstruction::HT(unsigned short jetLimit) const {
    double HT(0);
    unsigned short limit = jets.size();
    if (limit > jetLimit + 1)
        limit = jetLimit + 1;

    for (unsigned short index = 0; index < limit; ++index)
        HT += jets.at(index)->pt();

    return HT;
}

ChiSquaredBasedTopPairReconstruction::ChiSquaredBasedTopPairReconstruction(
		const LeptonPointer electron, const METPointer met,
		const JetCollection jets) :
		BasicTopPairReconstruction(electron, met, jets) {

}

ChiSquaredBasedTopPairReconstruction::~ChiSquaredBasedTopPairReconstruction() {
}

} /* namespace BAT */
