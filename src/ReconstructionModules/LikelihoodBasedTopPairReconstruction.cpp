/*
 * LikelihoodBasedTopPairReconstruction.cpp
 *
 *  Created on: 25 Aug 2011
 *      Author: kreczko
 */

#include "../../interface/ReconstructionModules/LikelihoodBasedTopPairReconstruction.h"
#include <boost/lexical_cast.hpp>

using namespace std;

namespace BAT {

LikelihoodBasedTopPairReconstruction::LikelihoodBasedTopPairReconstruction(const LeptonPointer lepton, const METPointer met, const JetCollection jets, const JetCollection bjets):
		solutions(),
		alreadyReconstructed(false),
		met(met),
		jets(jets),
		bjets(bjets),
		leptonFromW(lepton) {
}

bool LikelihoodBasedTopPairReconstruction::meetsInitialCriteria() const {
	return met != 0 && leptonFromW != 0 && jets.size() >= 4;
}

std::string LikelihoodBasedTopPairReconstruction::getDetailsOnFailure() const {
	std::string msg = "Initial Criteria not met: \n";
	if (leptonFromW == 0)
		msg += "Electron from W: not filled \n";
	else
		msg += "Electron from W: filled \n";

	if (met == 0)
		msg += "Missing transverse energy: not filled \n";
	else
		msg += "Missing transverse energy: filled \n";
	std::string nJets(boost::lexical_cast<std::string>(jets.size()));
	if (jets.size() < 4)
		msg += "Number of jets is too small:" + nJets + ", should be >= 4 \n";
	else
		msg += "Number of jets is OK:" + nJets + "\n";

	return msg;
}



LikelihoodBasedTopPairReconstruction::~LikelihoodBasedTopPairReconstruction() {
}

TtbarHypothesisCollection LikelihoodBasedTopPairReconstruction::getAllSolutions(){
	if(!alreadyReconstructed)
		reconstruct();

	SmallerDiscriminatorIsBetter solutionComparator = SmallerDiscriminatorIsBetter();
	std::sort(solutions.begin(), solutions.end(), solutionComparator);
	return solutions;
}

const TtbarHypothesisPointer LikelihoodBasedTopPairReconstruction::getBestSolution() {
	const TtbarHypothesisPointer bestSolution = getAllSolutions().front();//sorted by quality, front == best
	return bestSolution;
}

void LikelihoodBasedTopPairReconstruction::reconstruct() {
	typedef unsigned short ushort;

	// Loop b jets to get hadronic b candidate
	for (ushort hadBindex = 0; hadBindex < bjets.size(); ++hadBindex) {
		JetPointer hadBJet = bjets[hadBindex];
		LeptonPointer signallepton = leptonFromW;
		METPointer MET = met;

		if (!meetsHadronicBJetRequirement(hadBJet))
			continue;
		// Loop b jets to get leptonic b candidate
		for (ushort lepBindex = 0; lepBindex < bjets.size(); ++lepBindex) {
			JetPointer lepBJet = bjets[lepBindex];
			if (lepBindex == hadBindex || !meetsLeptonicBJetRequirement(lepBJet))
				continue;

			// Loop light jets to get jets from W


			for ( ushort jet1Index=0; jet1Index < (jets.size()-1); ++jet1Index ) {
				for ( ushort jet2Index=jet1Index+1; jet2Index < jets.size(); ++jet2Index ) {

			// for (ushort jet1Index = 0; jet1Index < jets.size(); ++jet1Index) {
			// 	for (ushort jet2Index = 0; jet2Index < jets.size(); ++jet2Index) {
					JetPointer jet1FromW = jets[jet1Index];
					JetPointer jet2FromW = jets[jet2Index];

					if (jet2Index == jet1Index || !meetsJetFromWRequirement(jet1FromW, jet2FromW))
						continue;

					// Put all particles together
					TtbarHypothesisPointer solution(new TtbarHypothesis());
					//leptons
					solution->leptonFromW = leptonFromW;
					double nuChi2 = -1;
					solution->neutrinoFromW = getNeutrinoSolution( lepBJet, signallepton, MET, nuChi2 );
					solution->neutrinoChi2 = nuChi2;

					//jets
					solution->hadronicBJet = hadBJet;
					solution->leptonicBjet = lepBJet;
					solution->jet1FromW = jet1FromW;
					solution->jet2FromW = jet2FromW;

					//combine reconstructed objects
					solution->combineReconstructedObjects();

					// Get discrimnant for this solution
					solution->discriminator = getDiscriminator(solution);


					// Store if event is physical (checks masses of tops and Ws > 0 )
					if (meetsGlobalRequirement(solution)){
						solutions.push_back(solution);
					}

					else
						continue;
				}
			}
		}
	}
	alreadyReconstructed = true;
}


ParticlePointer LikelihoodBasedTopPairReconstruction::getNeutrinoSolution(JetPointer BJet, LeptonPointer Lepton, METPointer met, double& neutrinoChi2) {
	// Do the neutrino reconstruction
	NeutrinoSolver neutrinoSolver( &(Lepton->getFourVector()), &(BJet->getFourVector()), 80, 173 );
	double test = -1;
	FourVector neutrino = neutrinoSolver.GetBest(met->getFourVector().Px(), met->getFourVector().Py(), 25., 25., 0., test );
	neutrinoChi2 = test;

	return ParticlePointer (new Particle(neutrino.Energy(), neutrino.Px(), neutrino.Py(), neutrino.Pz()));
}

bool LikelihoodBasedTopPairReconstruction::meetsHadronicBJetRequirement(JetPointer hadBJet){
	return true;
}

bool LikelihoodBasedTopPairReconstruction::meetsLeptonicBJetRequirement(JetPointer lepBJet){
	return true;
}

bool LikelihoodBasedTopPairReconstruction::meetsJetFromWRequirement(JetPointer jet1, JetPointer jet2){
	return true;
}

bool LikelihoodBasedTopPairReconstruction::meetsGlobalRequirement(const TtbarHypothesisPointer solution){
	return solution->isPhysical();
}

double LikelihoodBasedTopPairReconstruction::getDiscriminator(const TtbarHypothesisPointer solution) const{
	// Given all the info for this solution/permutation, calculate the discriminant
	// cout << "------------------------------------------------------" << endl;

	// Print csv of hadronic b jet and probablitity from correct b histogram
	JetPointer hadBJet = solution->hadronicBJet;
	JetPointer lepBJet = solution->leptonicBjet;
	ParticlePointer hadTop = solution->hadronicTop;
	ParticlePointer lepTop = solution->leptonicTop;
	ParticlePointer hadW = solution->hadronicW;
	ParticlePointer lepW = solution->leptonicW;

	double NeutrinoChi2 = solution->neutrinoChi2;

	double hadBJetCSV = hadBJet->getBTagDiscriminator(BAT::BtagAlgorithm::value::CombinedSecondaryVertexV2);
	double lepBJetCSV = lepBJet->getBTagDiscriminator(BAT::BtagAlgorithm::value::CombinedSecondaryVertexV2);

	double Wmass = hadW->mass();
	double Topmass = hadTop->mass();


	double probCorrectHadB = Globals::csvCorrectPermHistogram->Interpolate( hadBJetCSV );
	double probIncorrectHadB = Globals::csvIncorrectPermHistogram->Interpolate( hadBJetCSV );

	double probCorrectLepB = Globals::csvCorrectPermHistogram->Interpolate( lepBJetCSV );
	double probIncorrectLepB = Globals::csvIncorrectPermHistogram->Interpolate( lepBJetCSV );

	double probCorrectNuChi2 = Globals::NuChiCorrectPermHistogram->Interpolate( NeutrinoChi2 );
	double probIncorrectNuChi2 = Globals::NuChiIncorrectPermHistogram->Interpolate( NeutrinoChi2 );

	double probCorrectWBMass = Globals::HadronicRecoCorrectPermHistogram->Interpolate( Wmass , Topmass );
	double probIncorrectWBMass = Globals::HadronicRecoIncorrectPermHistogram->Interpolate( Wmass , Topmass );

	if (probCorrectWBMass == 0){
		probCorrectWBMass = 0.000000001;
	}
	if (probIncorrectWBMass == 0){
		probIncorrectWBMass = 0.000000001;
	}

	double NuChi2Disc =  - log(probCorrectNuChi2/probIncorrectNuChi2);
	double CSVDisc = - log((probCorrectHadB/probIncorrectHadB)*(probCorrectLepB/probIncorrectLepB));
	double MassDisc = - log(probCorrectWBMass/probIncorrectWBMass);
	double likelihoodratio = 20;
	// cout << "Mass Hadronic T : " << hadTop->mass() << ", Mass Hadronic W : " << hadW->getFourVector().M() << endl;
	// cout << "Mass Leptonic T : " << lepTop->getFourVector().M() << ", Mass Leptonic W : " << lepW->getFourVector().M() << endl;
	// cout << "Neutrino Chi Sq : " << NeutrinoChi2 << endl;

	if (NeutrinoChi2 >= 0.0 && Wmass <= 490 && Topmass <= 490){


		solution->CSVDiscriminator = CSVDisc;
		solution->MassDiscriminator = MassDisc;
		solution->NuChi2Discriminator = NuChi2Disc;

		likelihoodratio = - log(probCorrectWBMass/probIncorrectWBMass) - log(probCorrectNuChi2/probIncorrectNuChi2) - log((probCorrectHadB/probIncorrectHadB)*(probCorrectLepB/probIncorrectLepB));

		// if (solution->isCorrect()){

		// 	cout << "Mass Hadronic T : " << hadTop->mass() << ", Mass Hadronic W : " << hadW->mass() << endl;
		// 	cout << "Mass Leptonic T : " << lepTop->mass() << ", Mass Leptonic W : " << lepW->mass() << endl;

		// 	cout << "probCorrectHadB : " << hadBJetCSV << " prob : " << probCorrectHadB << endl;
		// 	cout << "probIncorrectHadB : " << hadBJetCSV << " prob : " << probIncorrectHadB << endl;	
		// 	// cout << "probCorrectLepB : " << lepBJetCSV << " prob : " << probCorrectLepB << endl;
		// 	// cout << "probIncorrectLepB : " << lepBJetCSV << " prob : " << probIncorrectLepB << endl;

		// 	// cout << "CorrectNu : " << NeutrinoChi2 << " prob : " << probCorrectNuChi2 << endl;
		// 	// cout << "IncorrectNu : " << NeutrinoChi2 << " prob : " << probIncorrectNuChi2 << endl;

		// 	// cout << "CorrectMassReco : " << Wmass << " " << Topmass << " prob : " << probCorrectWBMass << endl;
		// 	// cout << "IncorrectMassReco : " << Wmass << " " << Topmass <<  " prob : " << probIncorrectWBMass << endl;

		// 	cout << "MassDisc : " << MassDisc << endl;
		// 	cout << "CSVDisc : " << CSVDisc << endl;
		// 	cout << "NuChi2Disc : " << NuChi2Disc << endl;

		// 	cout << "Likelihood Test : " << likelihoodratio << endl;
		// 	}
	}

	else{
		solution->CSVDiscriminator = 10;
		solution->MassDiscriminator = 10;
		solution->NuChi2Discriminator = 10;
	}
return likelihoodratio; 
}

} /* namespace BAT */
