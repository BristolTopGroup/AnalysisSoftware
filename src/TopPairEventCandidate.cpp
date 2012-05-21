/*
 * TopPairEventCandidate.cpp
 *
 *  Created on: 9 Jul 2010
 *      Author: kreczko
 */

#include "../interface/TopPairEventCandidate.h"
#include "../interface/GlobalVariables.h"
#include <iostream>
#include <iomanip>
using namespace std;

namespace BAT {

double const TopPairEventCandidate::matched_leptonic_top_mass = 169.0;
double const TopPairEventCandidate::matched_leptonic_top_mass_sigma = 16.3;
double const TopPairEventCandidate::matched_hadronic_W_mass = 83.;
double const TopPairEventCandidate::matched_hadronic_W_mass_sigma = 10.9;
double const TopPairEventCandidate::matched_hadronic_top_mass = 174.7;
double const TopPairEventCandidate::matched_hadronic_top_mass_sigma = 14.6;
double const TopPairEventCandidate::matched_pt_ttbarSystem = 0.;
double const TopPairEventCandidate::matched_pt_ttbarSystem_sigma = 50.;
double const TopPairEventCandidate::matched_HTSystem = 1;
double const TopPairEventCandidate::matched_HTSystem_sigma = 0.15;
double const TopPairEventCandidate::W_mass = 80.389;

NeutrinoSelectionCriterion::value TopPairEventCandidate::usedNeutrinoSelection; // = NeutrinoSelectionCriterion::chi2;
TTbarReconstructionCriterion::value TopPairEventCandidate::usedTTbarReconstruction; // = TTbarReconstructionCriterion::TopMassDifference;

TopPairEventCandidate::TopPairEventCandidate() :
		Event(), //
		electronFromW(), //
		leptonicBJet(), //
		hadronicBJet(), //
		jet1FromW(), //
		jet2FromW(), //
		neutrino1(), //
		neutrino2(), //
		leptonicW1(), //
		leptonicW2(), //
		hadronicW(), //
		leptonicTop1(), //
		leptonicTop2(), //
		hadronicTop(), //
		selectedNeutrino(0), //
		currentSelectedNeutrino(0), //
		hadronicBIndex(0), //
		leptonicBIndex(0), //
		jet1FromWIndex(0), //
		jet2FromWIndex(0), //
		doneReconstruction_(false), //
		conversionTagger(new ConversionTagger()), //
		doneConversionTagging(false), //
		solutions(), //
		compareSolutions() {
}

TopPairEventCandidate::TopPairEventCandidate(const Event& event) :
		Event(event), //
		electronFromW(), //
		leptonicBJet(), //
		hadronicBJet(), //
		jet1FromW(), //
		jet2FromW(), //
		neutrino1(), //
		neutrino2(), //
		leptonicW1(), //
		leptonicW2(), //
		hadronicW(), //
		leptonicTop1(), //
		leptonicTop2(), //
		hadronicTop(), //
		selectedNeutrino(0), //
		currentSelectedNeutrino(0), //
		hadronicBIndex(0), //
		leptonicBIndex(0), //
		jet1FromWIndex(0), //
		jet2FromWIndex(0), //
		doneReconstruction_(false), //
		conversionTagger(new ConversionTagger()), //
		doneConversionTagging(false), //
		solutions(), //
		compareSolutions() {

}

TopPairEventCandidate::~TopPairEventCandidate() {
}

bool TopPairEventCandidate::passesScrapingFilter() const {
	if (tracks.size() > 10) {
		if (numberOfHighPurityTracks_ / (1.0 * tracks.size()) > 0.25)
			return true;
		else
			return false;
	} else
		return isBeamScraping_ == false;
}

bool TopPairEventCandidate::passesElectronHighLevelTrigger() const {
	if (isRealData()) {
		if (runNumber >= 160404 && runNumber <= 163869)
			return HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
		else if (runNumber > 163869 && runNumber <= 165633)
			return HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);
		else if (runNumber > 165633 && runNumber <= 178380)
			return HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
		else if (runNumber > 178380)
			return HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
		else
			return false;
	} else {
		//Fall 11 MC
		return HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
	}

}

bool TopPairEventCandidate::hasOneGoodPrimaryVertex() const {
	return goodVertices.size() >= 1;
}

bool TopPairEventCandidate::hasOnlyOneGoodIsolatedElectron() const {
	if (Event::usePFIsolation)
		return goodPFIsolatedElectrons.size() == 1;
	else
		return goodIsolatedElectrons.size() == 1;
}

bool TopPairEventCandidate::isolatedElectronDoesNotComeFromConversion() const {
	bool passConversion = false;
	if (Event::usePFIsolation) {
		if (goodPFIsolatedElectrons.size() > 0)
			passConversion = goodPFIsolatedElectrons.front()->isFromConversion() == false;
	} else {
		if (goodIsolatedElectrons.size() > 0)
			passConversion = goodIsolatedElectrons.front()->isFromConversion() == false;
	}

	return passConversion;
}

bool TopPairEventCandidate::isolatedElectronNotTaggedAsFromConversion() const {
	bool passConversion = false;
	ElectronPointer electron;
	if (Event::usePFIsolation) {
		if (goodPFIsolatedElectrons.size() > 0)
			electron = goodPFIsolatedElectrons.front();
	} else {
		if (goodIsolatedElectrons.size() > 0)
			electron = goodIsolatedElectrons.front();
	}
	if (electron != 0) {
		if (useCustomConversionTagger) {
			conversionTagger->calculateConversionVariables(electron, tracks, 3.8, 0.45);
			passConversion = conversionTagger->isFromConversion(0.02, 0.02) == false;
		} else {
			passConversion = electron->isTaggedAsConversion(0.02, 0.02) == false;
		}
	}

	return passConversion;
}

bool TopPairEventCandidate::ePlusJetsLooseMuonVeto() const {
	bool hasNoLooseMuon = true;

	for (unsigned int index = 0; index < looseMuons.size(); ++index) {
		const MuonPointer looseMuon = looseMuons.at(index);
		bool passLooseIso = false;

		if (Event::usePFIsolation)
			passLooseIso = looseMuon->pfRelativeIsolation() < 0.2;
		else
			passLooseIso = looseMuon->relativeIsolation() < 0.2;

		if (passLooseIso) {
			hasNoLooseMuon = false;
			break;
		}
	}

	return hasNoLooseMuon;
}

bool TopPairEventCandidate::muPlusJetsLooseMuonVeto() const {
	bool hasNoLooseMuon = true;
	MuonPointer signalMuon;
	if (Event::usePFIsolation && goodPFIsolatedMuons.size() > 0)
		signalMuon = goodPFIsolatedMuons.front();

	if (!Event::usePFIsolation && goodIsolatedMuons.size() > 0)
		signalMuon = goodIsolatedMuons.front();

	for (unsigned int index = 0; index < looseMuons.size(); ++index) {
		const MuonPointer looseMuon = looseMuons.at(index);
		bool passLooseIso = false;

		if (Event::usePFIsolation)
			passLooseIso = looseMuon->pfRelativeIsolation() < 0.2;
		else
			passLooseIso = looseMuon->relativeIsolation() < 0.2;

		if (passLooseIso && (looseMuon != signalMuon)) {
			hasNoLooseMuon = false;
			break;
		}
	}

	return hasNoLooseMuon;
}

bool TopPairEventCandidate::electronPlusJetsZVeto() const {
	unsigned int nLooseElectrons(0);

	for (unsigned int index = 0; index < looseElectrons.size(); ++index) {
		const ElectronPointer looseElectron = looseElectrons.at(index);
		bool passLooseIso = false;

		if (Event::usePFIsolation)
			passLooseIso = looseElectron->pfIsolation() < 0.2;
		else
			passLooseIso = looseElectron->relativeIsolation() < 0.2;

		if (passLooseIso)
			++nLooseElectrons;
	}
	return nLooseElectrons < 2;
}

bool TopPairEventCandidate::muonPlusJetsLooseElectronVeto() const {
	bool hasNoLooseElectron = true;

	for (unsigned int index = 0; index < looseElectrons.size(); ++index) {
		const ElectronPointer looseElectron = looseElectrons.at(index);
		bool passLooseIso = false;

		if (Event::usePFIsolation)
			passLooseIso = looseElectron->pfIsolation() < 0.2;
		else
			passLooseIso = looseElectron->relativeIsolation() < 0.2;

		if (passLooseIso) {
			hasNoLooseElectron = false;
			break;
		}
	}

	return hasNoLooseElectron;
}

bool TopPairEventCandidate::passesMETCut() const {
	return MET()->et() > Globals::METCut;
}

bool TopPairEventCandidate::passesAsymmetricElectronCleanedJetCuts() const {
	if (goodElectronCleanedJets.size() < 3) // good jets have a cut of 30 GeV!
		return false;
	JetPointer leadingJet = goodElectronCleanedJets.front();
	JetPointer secondLeadingJet = goodElectronCleanedJets.at(1);
	JetPointer thirdLeadingJet = goodElectronCleanedJets.at(2);
	return leadingJet->pt() > 70 && secondLeadingJet->pt() > 50 && thirdLeadingJet->pt() > 50;

}

bool TopPairEventCandidate::passesAsymmetricMuonCleanedJetCuts() const {
	if (goodMuonCleanedJets.size() < 3) // good jets have a cut of 30 GeV!
		return false;
	JetPointer leadingJet = goodMuonCleanedJets.front();
	JetPointer secondLeadingJet = goodMuonCleanedJets.at(1);
	JetPointer thirdLeadingJet = goodMuonCleanedJets.at(2);
	return leadingJet->pt() > 70 && secondLeadingJet->pt() > 50 && thirdLeadingJet->pt() > 50;

}

bool TopPairEventCandidate::passesFullTTbarEPlusJetSelection() const {
//    unsigned int newstep = (int) TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS - 1;
	return passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets);
}

bool TopPairEventCandidate::passesEPlusJetsSelectionStepUpTo(enum TTbarEPlusJetsSelection::Step step) const {
	if (step == TTbarEPlusJetsSelection::FilterOutScraping)
		return passesEPlusJetsSelectionStep(step);
	else {
		unsigned int newstep = (int) step - 1;
		return passesEPlusJetsSelectionStep(step)
				&& passesEPlusJetsSelectionStepUpTo((TTbarEPlusJetsSelection::Step) newstep);
	}
}

bool TopPairEventCandidate::passesNMinus1(enum TTbarEPlusJetsSelection::Step omitted) const {
	bool passes(true);

	for (unsigned int cut = 0; cut < TTbarEPlusJetsSelection::NUMBER_OF_SELECTION_STEPS; ++cut) {
		if (cut == (unsigned int) omitted)
			continue;
		passes = passes && passesEPlusJetsSelectionStep((TTbarEPlusJetsSelection::Step) cut);
	}
	return passes;
}

bool TopPairEventCandidate::passesMuPlusJetsSelectionStep(enum TTbarMuPlusJetsSelection::Step step) const {
	switch (step) {
	case TTbarMuPlusJetsSelection::EventCleaning: //done on skim level!
		return hasOneGoodPrimaryVertex();
	case TTbarMuPlusJetsSelection::HighLevelTrigger:
		return passesMuonHighLevelTrigger();
	case TTbarMuPlusJetsSelection::OneIsolatedMuon:
		return hasOnlyOneGoodIsolatedMuon();
	case TTbarMuPlusJetsSelection::LooseMuonVeto:
		return muPlusJetsLooseMuonVeto();
	case TTbarMuPlusJetsSelection::LooseElectronVeto:
		return muonPlusJetsLooseElectronVeto();
	case TTbarMuPlusJetsSelection::AtLeastOneGoodJets:
		return goodMuonCleanedJets.size() > 0;
	case TTbarMuPlusJetsSelection::AtLeastTwoGoodJets:
		return goodMuonCleanedJets.size() > 1;
	case TTbarMuPlusJetsSelection::AtLeastThreeGoodJets:
		return goodMuonCleanedJets.size() > 2;
	case TTbarMuPlusJetsSelection::AtLeastFourGoodJets:
		return goodMuonCleanedJets.size() > 3;
	case TTbarMuPlusJetsSelection::MissingTransverseEnergy:
		return passesMETCut();
	case TTbarMuPlusJetsSelection::AsymmetricJetCuts:
		return passesAsymmetricMuonCleanedJetCuts();
	case TTbarMuPlusJetsSelection::AtLeastOneBtag:
		return goodMuonCleanedBJets.size() > 0;
	case TTbarMuPlusJetsSelection::AtLeastTwoBtags:
		return goodMuonCleanedBJets.size() > 1;
	default:
		return false;
	}
}

bool TopPairEventCandidate::passesMuonHighLevelTrigger() const {
	if (isRealData()) {
		if (runNumber >= 160404 && runNumber <= 165633)
			return HLT(HLTriggers::HLT_Mu17_TriCentralJet30);
		else if (runNumber > 165633 && runNumber <= 173198)
			return HLT(HLTriggers::HLT_IsoMu17_TriCentralJet30);
		else if (runNumber > 173198 && runNumber <= 178380)
			return HLT(HLTriggers::HLT_IsoMu17_eta2p1_TriCentralJet30);
		else if (runNumber > 173198)
			return HLT(HLTriggers::HLT_IsoMu17_eta2p1_TriCentralPFJet30);
		else
			return false;
	} else {
		return HLT(HLTriggers::HLT_Mu17_TriCentralJet30);
	}
}

bool TopPairEventCandidate::hasOnlyOneGoodIsolatedMuon() const {
	if (Event::usePFIsolation)
		return goodPFIsolatedMuons.size() == 1;
	else
		return goodIsolatedMuons.size() == 1;
}

bool TopPairEventCandidate::passesFullTTbarMuPlusJetSelection() const {
	return passesMuPlusJetsSelectionStepUpTo(TTbarMuPlusJetsSelection::AtLeastFourGoodJets);
}

bool TopPairEventCandidate::passesMuPlusJetsSelectionStepUpTo(enum TTbarMuPlusJetsSelection::Step step) const {
	if (step == TTbarMuPlusJetsSelection::EventCleaning)
		return passesMuPlusJetsSelectionStep(step);
	else {
		unsigned int newstep = (int) step - 1;
		return passesMuPlusJetsSelectionStep(step)
				&& passesMuPlusJetsSelectionStepUpTo((TTbarMuPlusJetsSelection::Step) newstep);
	}
}

bool TopPairEventCandidate::passesEPlusJetsSelectionStep(enum TTbarEPlusJetsSelection::Step step) const {
	switch (step) {
	case TTbarEPlusJetsSelection::FilterOutScraping:
		return passesScrapingFilter();
	case TTbarEPlusJetsSelection::HighLevelTrigger:
		return passesElectronHighLevelTrigger();
	case TTbarEPlusJetsSelection::GoodPrimaryvertex:
		return hasOneGoodPrimaryVertex();
	case TTbarEPlusJetsSelection::OneIsolatedElectron:
		return hasOnlyOneGoodIsolatedElectron();
	case TTbarEPlusJetsSelection::ConversionRejection:
		return isolatedElectronDoesNotComeFromConversion();
	case TTbarEPlusJetsSelection::ConversionFinder:
		return isolatedElectronNotTaggedAsFromConversion();
	case TTbarEPlusJetsSelection::LooseMuonVeto:
		return ePlusJetsLooseMuonVeto();
	case TTbarEPlusJetsSelection::AtLeastOneGoodJet:
		return goodElectronCleanedJets.size() > 0;
	case TTbarEPlusJetsSelection::AtLeastTwoGoodJets:
		return goodElectronCleanedJets.size() > 1;
	case TTbarEPlusJetsSelection::AtLeastThreeGoodJets:
		return goodElectronCleanedJets.size() > 2;
	case TTbarEPlusJetsSelection::AtLeastFourGoodJets:
		return goodElectronCleanedJets.size() > 3;
	case TTbarEPlusJetsSelection::DileptonVeto:
		return electronPlusJetsZVeto();
	case TTbarEPlusJetsSelection::MissingTransverseEnergy:
		return passesMETCut();
	case TTbarEPlusJetsSelection::AsymmetricJetCuts:
		return passesAsymmetricElectronCleanedJetCuts();
	case TTbarEPlusJetsSelection::AtLeastOneBtag:
		return goodElectronCleanedBJets.size() > 0;
	case TTbarEPlusJetsSelection::AtLeastTwoBtags:
		return goodElectronCleanedBJets.size() > 1;
	default:
		return false;
	}
}

bool TopPairEventCandidate::passesEPlusJetsRelIsoSelection() const {
	bool passesFirst3 = passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex);
	bool passGoodElectrons = goodElectrons.size() > 0 && goodIsolatedElectrons.size() < 2;
	bool passesBothconversionvetos = false;
	if (passGoodElectrons) {
		const ElectronPointer electron = MostIsolatedElectron(allElectrons);
		if (electron->isGood(Globals::electronID)) {
			if (useCustomConversionTagger) {
				conversionTagger->calculateConversionVariables(electron, tracks, 3.8, 0.45);
				passesBothconversionvetos = electron->isFromConversion() == false
						&& conversionTagger->isFromConversion(0.02, 0.02) == false;
			} else {
				passesBothconversionvetos = electron->isFromConversion() == false
						&& electron->isTaggedAsConversion(0.02, 0.02) == false;
			}
		}

	}
	bool muonVeto = ePlusJetsLooseMuonVeto();
	bool Zveto = electronPlusJetsZVeto();
	return passesFirst3 && passGoodElectrons && passesBothconversionvetos && muonVeto && Zveto;
}

bool TopPairEventCandidate::passesEPlusJetsRelIsoControlSelection() const {
	bool passesFirst3 = passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex);
	bool passGoodElectrons = allElectrons.size() > 0 && goodIsolatedElectrons.size() < 2;
	bool passesBothIsolationvetos = false;
	if (passGoodElectrons) {
		const ElectronPointer electron = MostIsolatedElectron(allElectrons);
		if (electron->isQCDElectron(Globals::electronID)) {
			if (useCustomConversionTagger) {
				conversionTagger->calculateConversionVariables(electron, tracks, 3.8, 0.45);
				passesBothIsolationvetos = electron->isFromConversion() == false
						&& conversionTagger->isFromConversion(0.02, 0.02) == false;
			} else {
				passesBothIsolationvetos = electron->isFromConversion() == false
						&& electron->isTaggedAsConversion(0.02, 0.02) == false;
			}
		}

	}
	bool muonVeto = ePlusJetsLooseMuonVeto();
	bool Zveto = electronPlusJetsZVeto();
	return passesFirst3 && passGoodElectrons && passesBothIsolationvetos && muonVeto && Zveto;
}

bool TopPairEventCandidate::passesEPlusJetsPFIsoSelection() const {
	bool passesFirst3 = passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex);
	bool passGoodElectrons = goodElectrons.size() > 0 && goodPFIsolatedElectrons.size() < 2;
	bool passesBothIsolationvetos = false;
	if (passGoodElectrons) {
		//TODO: shoudn't this be goodElectrons?
		const ElectronPointer electron = MostPFIsolatedElectron(goodElectrons);
		if (electron->isGood(Globals::electronID)) {
//			if (useCustomConversionTagger) {
//				conversionTagger->calculateConversionVariables(electron, tracks, 3.8, 0.45);
//				passesBothIsolationvetos = electron->isFromConversion() == false
//						&& conversionTagger->isFromConversion(0.02, 0.02) == false;
//			} else {
			passesBothIsolationvetos = electron->isFromConversion() == false
					&& electron->isTaggedAsConversion(0.02, 0.02) == false;
//			}
		}

	}
	bool muonVeto = ePlusJetsLooseMuonVeto();
	bool Zveto = electronPlusJetsZVeto();
	return passesFirst3 && passGoodElectrons && passesBothIsolationvetos && muonVeto && Zveto;
}

bool TopPairEventCandidate::passesEPlusJEtsPFIsoControlSelection() const {
	bool passesFirst3 = passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex);
	bool passGoodElectrons = allElectrons.size() > 0 && goodPFIsolatedElectrons.size() < 2;
	bool passesBothIsolationvetos = false;
	if (passGoodElectrons) {
		const ElectronPointer electron = MostPFIsolatedElectron(allElectrons);
		if (electron->isQCDElectron(Globals::electronID)) {
			if (useCustomConversionTagger) {
				conversionTagger->calculateConversionVariables(electron, tracks, 3.8, 0.45);
				passesBothIsolationvetos = electron->isFromConversion() == false
						&& conversionTagger->isFromConversion(0.02, 0.02) == false;
			} else {
				passesBothIsolationvetos = electron->isFromConversion() == false
						&& electron->isTaggedAsConversion(0.02, 0.02) == false;
			}
		}

	}
	bool muonVeto = ePlusJetsLooseMuonVeto();
	bool Zveto = electronPlusJetsZVeto();
	return passesFirst3 && passGoodElectrons && passesBothIsolationvetos && muonVeto && Zveto;
}

bool TopPairEventCandidate::passesConversionSelection() const {
	bool passesFirst6 = passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::DileptonVeto);
	bool isConversion1 = isolatedElectronDoesNotComeFromConversion() == false;
	bool isConversion2 = isolatedElectronNotTaggedAsFromConversion() == false;
	return passesFirst6 && (isConversion1 || isConversion2);
}

bool TopPairEventCandidate::passedEPlusJetsAntiIDSelection() const {
	//no trigger requirement now, later switch to looseID triggers
//    bool passesVertex = hasOneGoodPrimaryVertex();
	bool passesFirst3 = passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::GoodPrimaryvertex);
	bool passElectrons = allElectrons.size() > 0 && goodPFIsolatedElectrons.size() < 2;
	bool passesAntiID = false;
	bool passesConversionVetos = false;
	if (passElectrons) {
		const ElectronPointer electron = MostPFIsolatedElectron(allElectrons);
		passesAntiID = electron->isQCDElectron(Globals::electronID);
		passesConversionVetos = electron->isFromConversion() == false
				&& electron->isTaggedAsConversion(0.02, 0.02) == false;
	}

	return passesFirst3 && passElectrons && passesAntiID && passesConversionVetos;
}
bool TopPairEventCandidate::passesEPlusJetsAntiIsolationSelection() const {
	bool passesFirst3 = passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex);

	unsigned int nGoodElectrons(goodElectrons.size()), nGoodIsolatedElectrons(0);
	for (unsigned int index = 0; index < goodElectrons.size(); ++index) {
		const ElectronPointer electron(goodElectrons.at(index));
		if (electron->pfRelativeIsolation(Globals::electronIsolationCone) < 0.1)
			++nGoodIsolatedElectrons;
	}

	if (!(nGoodElectrons > 0 && nGoodIsolatedElectrons == 0))
		return false;

//	unsigned int nGoodNonIsolatedElectrons(0), nGoodIsolatedElectrons(0);
//
//	for (unsigned int index = 0; index < goodElectrons.size(); ++index) {
//		const ElectronPointer electron(goodElectrons.at(index));
//		if (electron->pfRelativeIsolation(Globals::electronIsolationCone) < 0.1)
//			++nGoodIsolatedElectrons;
//		if (electron->pfRelativeIsolation(Globals::electronIsolationCone) > 0.2)
//			++nGoodNonIsolatedElectrons;
//	}
//	if (!(nGoodNonIsolatedElectrons == 1 && nGoodIsolatedElectrons == 0))
//		return false;

	bool muonVeto = passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::LooseMuonVeto);
	bool zveto = passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::DileptonVeto);

	const ElectronPointer electron = MostPFIsolatedElectron(goodElectrons);
	bool conversionVeto = electron->isFromConversion() == false && electron->isTaggedAsConversion(0.02, 0.02) == false;

	return passesFirst3 && muonVeto && zveto && conversionVeto;
}

bool TopPairEventCandidate::passesEPlusJetsQCDselection() const {
	//require at least one good electron and no good electrons with isolation less than 0.1 and exactly 1 non-isolated electron with PFRelIso>0.2
	if (!(goodElectrons.size() > 0 && goodPFIsolatedElectrons.size() == 0 && goodPFNonIsolatedElectrons.size() == 1))
		return false;

	bool passesFirst3 = passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::GoodPrimaryvertex);

	bool muonVeto = passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::LooseMuonVeto);
	bool zveto = passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::DileptonVeto);
	bool conversionVeto = (goodElectrons.front()->isFromConversion()
			|| goodElectrons.front()->isTaggedAsConversion(0.02, 0.02)) == false;
	return passesFirst3 && muonVeto && zveto && conversionVeto;
}

void TopPairEventCandidate::reconstructTTbarToEPlusJets(ElectronPointer electron) {
	if (goodElectronCleanedJets.size() < 4)
		throw ReconstructionException("Not enough jets available to reconstruct top event using Mass Equality method.");
	electronFromW = electron;
	selectedNeutrino = 0;
	currentSelectedNeutrino = 0;
	reconstructNeutrinos();
	double chosen_TopMassDifference(9999999.);
	double chosen_Chi2Total(9999999.);

	for (unsigned short hadBindex = 0; hadBindex < goodElectronCleanedJets.size(); ++hadBindex) {
		for (unsigned short lepBindex = 0; lepBindex < goodElectronCleanedJets.size(); ++lepBindex) {
			if (lepBindex == hadBindex)
				continue;
			for (unsigned short jet1Index = 0; jet1Index < goodElectronCleanedJets.size(); ++jet1Index) {
				if (jet1Index == lepBindex || jet1Index == hadBindex)
					continue;
				for (unsigned short jet2Index = 0; jet2Index < goodElectronCleanedJets.size(); ++jet2Index) {
					if (jet2Index == jet1Index || jet2Index == lepBindex || jet2Index == hadBindex)
						continue;
					hadronicBJet = goodElectronCleanedJets.at(hadBindex);
					leptonicBJet = goodElectronCleanedJets.at(lepBindex);
					jet1FromW = goodElectronCleanedJets.at(jet1Index);
					jet2FromW = goodElectronCleanedJets.at(jet2Index);

					leptonicW1 = ParticlePointer(new Particle(*neutrino1 + *electronFromW));
					leptonicW2 = ParticlePointer(new Particle(*neutrino2 + *electronFromW));
					hadronicW = ParticlePointer(new Particle(*jet1FromW + *jet2FromW));
					leptonicTop1 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW1));
					leptonicTop2 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW2));
					hadronicTop = ParticlePointer(new Particle(*hadronicBJet + *hadronicW));
					fillHypotheses();
					selectNeutrinoSolution();
					double TopMassDifference = calculateTopMassDifference(currentSelectedNeutrino);
					double chi2 = getTotalChi2(currentSelectedNeutrino);
					switch (usedTTbarReconstruction) {
					case TTbarReconstructionCriterion::TopMassDifference:
						if (TopMassDifference < chosen_TopMassDifference) {
							hadronicBIndex = hadBindex;
							leptonicBIndex = lepBindex;
							jet1FromWIndex = jet1Index;
							jet2FromWIndex = jet2Index;
							chosen_TopMassDifference = TopMassDifference;
							selectedNeutrino = currentSelectedNeutrino;
						}
						break;

					case TTbarReconstructionCriterion::chi2:
						if (chi2 < chosen_Chi2Total) {
							hadronicBIndex = hadBindex;
							leptonicBIndex = lepBindex;
							jet1FromWIndex = jet1Index;
							jet2FromWIndex = jet2Index;
							chosen_Chi2Total = chi2;
							selectedNeutrino = currentSelectedNeutrino;
						}
						break;
					}
				}
			}
		}
	}
	std::sort(solutions.begin(), solutions.end(), compareSolutions);
	hadronicBJet = goodElectronCleanedJets.at(hadronicBIndex);
	leptonicBJet = goodElectronCleanedJets.at(leptonicBIndex);
	jet1FromW = goodElectronCleanedJets.at(jet1FromWIndex);
	jet2FromW = goodElectronCleanedJets.at(jet2FromWIndex);
	leptonicW1 = ParticlePointer(new Particle(*neutrino1 + *electronFromW));
	leptonicW2 = ParticlePointer(new Particle(*neutrino2 + *electronFromW));
	hadronicW = ParticlePointer(new Particle(*jet1FromW + *jet2FromW));
	leptonicTop1 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW1));
	leptonicTop2 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW2));
	hadronicTop = ParticlePointer(new Particle(*hadronicBJet + *hadronicW));
	if (selectedNeutrino == 1)
		ttbarResonance = ParticlePointer(new Particle(*leptonicTop1 + *hadronicTop));
	else
		ttbarResonance = ParticlePointer(new Particle(*leptonicTop2 + *hadronicTop));
	doneReconstruction_ = true;
}

void TopPairEventCandidate::reconstructTTbarToEPlusJetsFrom3Jets(ElectronPointer electron) {
	if (goodElectronCleanedJets.size() < 3)
		throw ReconstructionException("Not enough jets available to reconstruct top event using Mass Equality method.");
	electronFromW = electron;
	selectedNeutrino = 0;
	currentSelectedNeutrino = 0;
	reconstructNeutrinos();
	double chosen_TopMassDifference(9999999.);
	double chosen_Chi2Total(9999999.);

	for (unsigned short hadBindex = 0; hadBindex < goodElectronCleanedJets.size(); ++hadBindex) {
		for (unsigned short lepBindex = 0; lepBindex < goodElectronCleanedJets.size(); ++lepBindex) {
			if (lepBindex == hadBindex)
				continue;
			for (unsigned short jetFromW = 0; jetFromW < goodElectronCleanedJets.size(); ++jetFromW) {
				if (jetFromW == lepBindex || jetFromW == hadBindex)
					continue;
				hadronicBJet = goodElectronCleanedJets.at(hadBindex);
				leptonicBJet = goodElectronCleanedJets.at(lepBindex);
				jet1FromW = goodElectronCleanedJets.at(jetFromW);
				jet2FromW = goodElectronCleanedJets.at(jetFromW);

				leptonicW1 = ParticlePointer(new Particle(*neutrino1 + *electronFromW));
				leptonicW2 = ParticlePointer(new Particle(*neutrino2 + *electronFromW));
				hadronicW = ParticlePointer(new Particle(*jet1FromW));
				leptonicTop1 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW1));
				leptonicTop2 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW2));
				hadronicTop = ParticlePointer(new Particle(*hadronicBJet + *hadronicW));
				fillHypotheses();
				selectNeutrinoSolution();
				double TopMassDifference = calculateTopMassDifference(currentSelectedNeutrino);
				double chi2 = getTotalChi2(currentSelectedNeutrino);
				switch (usedTTbarReconstruction) {
				case TTbarReconstructionCriterion::TopMassDifference:
					if (TopMassDifference < chosen_TopMassDifference) {
						hadronicBIndex = hadBindex;
						leptonicBIndex = lepBindex;
						jet1FromWIndex = jetFromW;
						jet2FromWIndex = jetFromW;
						chosen_TopMassDifference = TopMassDifference;
						selectedNeutrino = currentSelectedNeutrino;
					}
					break;

				case TTbarReconstructionCriterion::chi2:
					if (chi2 < chosen_Chi2Total) {
						hadronicBIndex = hadBindex;
						leptonicBIndex = lepBindex;
						jet1FromWIndex = jetFromW;
						jet2FromWIndex = jetFromW;
						chosen_Chi2Total = chi2;
						selectedNeutrino = currentSelectedNeutrino;
					}
					break;

				}
			}
		}
	}
	std::sort(solutions.begin(), solutions.end(), compareSolutions);
	hadronicBJet = goodElectronCleanedJets.at(hadronicBIndex);
	leptonicBJet = goodElectronCleanedJets.at(leptonicBIndex);
	jet1FromW = goodElectronCleanedJets.at(jet1FromWIndex);
	jet2FromW = goodElectronCleanedJets.at(jet2FromWIndex);
	leptonicW1 = ParticlePointer(new Particle(*neutrino1 + *electronFromW));
	leptonicW2 = ParticlePointer(new Particle(*neutrino2 + *electronFromW));
	hadronicW = ParticlePointer(new Particle(*jet1FromW));
	leptonicTop1 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW1));
	leptonicTop2 = ParticlePointer(new Particle(*leptonicBJet + *leptonicW2));
	hadronicTop = ParticlePointer(new Particle(*hadronicBJet + *hadronicW));
	if (selectedNeutrino == 1)
		ttbarResonance = ParticlePointer(new Particle(*leptonicTop1 + *hadronicTop));
	else
		ttbarResonance = ParticlePointer(new Particle(*leptonicTop2 + *hadronicTop));
	doneReconstruction_ = true;
}

void TopPairEventCandidate::reconstructNeutrinos() {
	boost::array<double, 2> neutrinoPzs = computeNeutrinoPz();
	double energy1 = sqrt(MET()->et() * MET()->et() + neutrinoPzs.at(0) * neutrinoPzs.at(0));
	double energy2 = sqrt(MET()->et() * MET()->et() + neutrinoPzs.at(1) * neutrinoPzs.at(1));
	neutrino1 = ParticlePointer(new Particle(energy1, MET()->px(), MET()->py(), neutrinoPzs.at(0)));
	neutrino2 = ParticlePointer(new Particle(energy2, MET()->px(), MET()->py(), neutrinoPzs.at(1)));

	if (std::isnan(neutrino1->energy()) && std::isnan(neutrino2->energy()))
		throw ReconstructionException("No physical neutrino solution found");
	else if (std::isnan(neutrino1->energy()))
		neutrino1 = neutrino2;
	else if (std::isnan(neutrino2->energy()))
		neutrino2 = neutrino1;
}

const boost::array<double, 2> TopPairEventCandidate::computeNeutrinoPz() {
	if (electronFromW == 0)
		throw ReconstructionException("Could not reconstruct neutrinos: no isolated electrons found");
	if (MET()->energy() == 0)
		throw ReconstructionException("Could not reconstruct neutrinos: no MET found");
	boost::array<double, 2> neutrinoPzs;
	//    const ElectronPointer electron = goodIsolatedElectrons.front();

	double pz1(0), pz2(0);
	double M_e = 0.0005;
	double ee = electronFromW->energy();
	double pxe = electronFromW->px();
	double pye = electronFromW->py();
	double pze = electronFromW->pz();
	double pxnu = MET()->px();
	double pynu = MET()->py();

	double a = W_mass * W_mass - M_e * M_e + 2.0 * pxe * pxnu + 2.0 * pye * pynu;
	double A = 4.0 * (ee * ee - pze * pze);
	double B = -4.0 * a * pze;
	double C = 4.0 * ee * ee * (pxnu * pxnu + pynu * pynu) - a * a;

	double tmproot = B * B - 4.0 * A * C;
	if (tmproot < 0) {
		pz1 = pz2 = -B / (2 * A);
	} else {
		pz1 = (-B + TMath::Sqrt(tmproot)) / (2.0 * A);
		pz2 = (-B - TMath::Sqrt(tmproot)) / (2.0 * A);

	}
	neutrinoPzs[0] = pz1;
	neutrinoPzs[1] = pz2;
	return neutrinoPzs;
}

void TopPairEventCandidate::fillHypotheses() {
	TtbarHypothesisPointer hypothesis1(fillHypothesis(1));
	TtbarHypothesisPointer hypothesis2(fillHypothesis(2));
	solutions.push_back(hypothesis1);
	solutions.push_back(hypothesis2);

}

const TtbarHypothesisPointer TopPairEventCandidate::fillHypothesis(unsigned short int neutrinoSolution) {
	TtbarHypothesisPointer hypothesis(new TtbarHypothesis());
	hypothesis->leptonFromW = electronFromW;
	hypothesis->leptonicBjet = leptonicBJet;
	hypothesis->hadronicBJet = hadronicBJet;
	hypothesis->jet1FromW = jet1FromW;
	hypothesis->jet2FromW = jet2FromW;
	hypothesis->hadronicW = hadronicW;
	hypothesis->hadronicTop = hadronicTop;
	hypothesis->hadronicChi2 = getHadronicChi2();
	if (neutrinoSolution == 1) {
		hypothesis->neutrinoFromW = neutrino1;
		hypothesis->leptonicW = leptonicW1;
		hypothesis->leptonicTop = leptonicTop1;
	} else {
		hypothesis->neutrinoFromW = neutrino2;
		hypothesis->leptonicW = leptonicW2;
		hypothesis->leptonicTop = leptonicTop2;
	}

	hypothesis->totalChi2 = getTotalChi2(neutrinoSolution);
	hypothesis->globalChi2 = getGlobalChi2(neutrinoSolution);
	hypothesis->leptonicChi2 = getLeptonicChi2(neutrinoSolution);
	ParticlePointer resonance(new Particle(*hypothesis->leptonicTop + *hypothesis->hadronicTop));
	hypothesis->resonance = resonance;
	return hypothesis;
}

void TopPairEventCandidate::selectNeutrinoSolution() {

	if (leptonicTop1->mass() < 0 && leptonicTop2->mass() < 0) {
		inspectReconstructedEvent();
		throw ReconstructionException("No valid neutrino solution found");
	} else if (leptonicTop1->mass() < 0 && leptonicTop2->mass() > 0) {
		currentSelectedNeutrino = 2;
	} else if (leptonicTop1->mass() > 0 && leptonicTop2->mass() < 0) {
		currentSelectedNeutrino = 1;
	} else { // both solutions give positive mass
		switch (usedNeutrinoSelection) {
		case NeutrinoSelectionCriterion::TopMassDifference:
			fabs(leptonicTop1->mass() - hadronicTop->mass()) < fabs(leptonicTop2->mass() - hadronicTop->mass()) ? currentSelectedNeutrino =
					1 :
					currentSelectedNeutrino = 2;
			break;
		case NeutrinoSelectionCriterion::chi2:
			getTotalChi2(1) < getTotalChi2(2) ? currentSelectedNeutrino = 1 : currentSelectedNeutrino = 2;
			break;

		case NeutrinoSelectionCriterion::pzClosestToLepton:
			fabs(neutrino1->pz() - electronFromW->pz()) < fabs(neutrino2->pz() - electronFromW->pz()) ? currentSelectedNeutrino =
					1 :
					currentSelectedNeutrino = 2;
			break;

		case NeutrinoSelectionCriterion::mostCentral:
			fabs(neutrino1->pz()) < fabs(neutrino2->pz()) ? currentSelectedNeutrino = 1 : currentSelectedNeutrino = 2;
			break;

		case NeutrinoSelectionCriterion::pzClosestToLeptonOrMostcentralIfAbove300:
			fabs(neutrino1->pz() - electronFromW->pz()) < fabs(neutrino2->pz() - electronFromW->pz()) ? currentSelectedNeutrino =
					1 :
					currentSelectedNeutrino = 2;
			if (fabs(neutrino1->pz()) > 300 || fabs(neutrino2->pz()) > 300)
				fabs(neutrino1->pz()) < fabs(neutrino2->pz()) ? currentSelectedNeutrino = 1 : currentSelectedNeutrino =
						2;
			break;

		case NeutrinoSelectionCriterion::largestValueOfCosine:
			TVector3 p3W, p3e;
			//TODO clean up
			p3W = leptonicW1->getFourVector().Vect();
			p3e = electronFromW->getFourVector().Vect();

			double sinthcm1 = 2. * (p3e.Perp(p3W)) / W_mass;
			p3W = leptonicW2->getFourVector().Vect();
			double sinthcm2 = 2. * (p3e.Perp(p3W)) / W_mass;

			double costhcm1 = TMath::Sqrt(1. - sinthcm1 * sinthcm1);
			double costhcm2 = TMath::Sqrt(1. - sinthcm2 * sinthcm2);
			costhcm1 > costhcm2 ? currentSelectedNeutrino = 1 : currentSelectedNeutrino = 2;
			break;

		}
	}

}

double TopPairEventCandidate::calculateTopMassDifference(unsigned short int neutrinoSolution) const {

	double LeptonicTop1MassDifference = fabs(leptonicTop1->mass() - hadronicTop->mass());
	double LeptonicTop2MassDifference = fabs(leptonicTop2->mass() - hadronicTop->mass());

	if (neutrinoSolution == 1)
		return LeptonicTop1MassDifference;
	else
		return LeptonicTop2MassDifference;

}

double TopPairEventCandidate::getTotalChi2() {
	double totalChi2(9999999);
	double firstTotalChi2 = getTotalChi2(1);
	double secondTotalChi2 = getTotalChi2(2);
	selectedNeutrino == 1 ? totalChi2 = firstTotalChi2 : totalChi2 = secondTotalChi2;
	//    if (firstTotalChi2 < secondTotalChi2) {
	//        selectedNeutrino = 1;
	//        totalChi2 = firstTotalChi2;
	//    } else {
	//        selectedNeutrino = 2;
	//        totalChi2 = secondTotalChi2;
	//    }
	return totalChi2;
}

double TopPairEventCandidate::getTotalChi2(unsigned short int neutrinoSolution) const {
	return getLeptonicChi2(neutrinoSolution) + getHadronicChi2() + getGlobalChi2(neutrinoSolution);
}

double TopPairEventCandidate::getLeptonicChi2(unsigned short int neutrinoSolution) const {
	double topMass(0);
	double angle = leptonicBJet->angle(electronFromW);
	if (neutrinoSolution == 1)
		topMass = leptonicTop1->mass();
	else
		topMass = leptonicTop2->mass();

	return getLeptonicChi2(topMass, angle);
}

double TopPairEventCandidate::getLeptonicChi2(double topMass, double angle) const {
	double massDifference = TMath::Power(topMass - matched_leptonic_top_mass, 2);
	double massError = 2 * matched_leptonic_top_mass_sigma * matched_leptonic_top_mass_sigma;
	double massTerm = massDifference / massError;

	return massTerm;
}

double TopPairEventCandidate::getHadronicChi2() const {
	double WmassDifference = TMath::Power(hadronicW->mass() - matched_hadronic_W_mass, 2);
	double WmassError = 2 * matched_hadronic_W_mass_sigma * matched_hadronic_W_mass_sigma;
	double WmassTerm = WmassDifference / WmassError;

	double topMassDifference = TMath::Power(hadronicTop->mass() - matched_hadronic_top_mass, 2);
	double topMassError = 2 * matched_hadronic_top_mass_sigma * matched_hadronic_top_mass_sigma;
	double topMassTerm = topMassDifference / topMassError;

	return 1 / sqrt(2) * (topMassTerm + WmassTerm);
}

double TopPairEventCandidate::PtRatio() const {
	return TMath::Log(hadronicTop->pt() / hadronicW->pt());
}

double TopPairEventCandidate::getGlobalChi2(unsigned short neutrinoSolution) const {
	double pttbar = PtTtbarSystem(neutrinoSolution);
	double pttbarDifference = TMath::Power(pttbar - matched_pt_ttbarSystem, 2);
	double pttbarError = (2 * matched_pt_ttbarSystem_sigma * matched_pt_ttbarSystem_sigma);
	double pttbarTerm = pttbarDifference / pttbarError;

	double htSystemDifference = TMath::Power(HTSystem() - matched_HTSystem, 2);
	double htSystemError = matched_HTSystem_sigma * matched_HTSystem_sigma * 2;
	double htSystemTerm = htSystemDifference / htSystemError;
	return 1 / sqrt(2) * (pttbarTerm + htSystemTerm);
}

double TopPairEventCandidate::PtTtbarSystem(unsigned short neutrinoSolution) const {
	ParticlePointer combined;
	if (neutrinoSolution == 1)
		combined = ParticlePointer(new Particle(*leptonicTop1 + *hadronicTop));
	else
		combined = ParticlePointer(new Particle(*leptonicTop2 + *hadronicTop));
	return combined->pt();
}

double TopPairEventCandidate::HT(unsigned short jetLimit) const {
	double HT(0);
	unsigned short limit = goodElectronCleanedJets.size();
	if (limit > jetLimit + 1)
		limit = jetLimit + 1;

	for (unsigned short index = 0; index < limit; ++index)
		HT += goodElectronCleanedJets.at(index)->pt();

	return HT;
}

double TopPairEventCandidate::HTSystem() const {
	return sumPt() / HT(8);
}

double TopPairEventCandidate::sumPt() const {
	return leptonicBJet->pt() + hadronicBJet->pt() + jet1FromW->pt() + jet2FromW->pt();
}

void TopPairEventCandidate::throwExpeptionIfNotReconstructed() const {
	if (doneReconstruction_ == false)
		throw ReconstructionException("Can't access reconstructed particles before reconstruction.");
}

const ElectronPointer TopPairEventCandidate::getElectronFromWDecay() const {
	throwExpeptionIfNotReconstructed();
	return electronFromW;
}

const ParticlePointer TopPairEventCandidate::getNeutrinoFromWDecay() const {
	throwExpeptionIfNotReconstructed();
	if (selectedNeutrino == 1)
		return neutrino1;
	else
		return neutrino2;
}

const JetPointer TopPairEventCandidate::getHadronicBJet() const {
	throwExpeptionIfNotReconstructed();
	return hadronicBJet;
}

const JetPointer TopPairEventCandidate::getLeptonicBJet() const {
	throwExpeptionIfNotReconstructed();
	return leptonicBJet;
}

const JetPointer TopPairEventCandidate::getJet1FromHadronicW() const {
	throwExpeptionIfNotReconstructed();
	return jet1FromW;
}

const JetPointer TopPairEventCandidate::getJet2FromHadronicW() const {
	throwExpeptionIfNotReconstructed();
	return jet2FromW;
}

const ParticlePointer TopPairEventCandidate::getLeptonicTop() const {
	throwExpeptionIfNotReconstructed();
	if (selectedNeutrino == 1)
		return leptonicTop1;
	else
		return leptonicTop2;
}

const ParticlePointer TopPairEventCandidate::getHadronicTop() const {
	throwExpeptionIfNotReconstructed();
	return hadronicTop;
}

const ParticlePointer TopPairEventCandidate::getResonance() const {
	throwExpeptionIfNotReconstructed();
	return ttbarResonance;
}

double TopPairEventCandidate::M3() const {
	double m3(0), max_pt(0);
	if (goodElectronCleanedJets.size() >= 3) {
		for (unsigned int index1 = 0; index1 < goodElectronCleanedJets.size() - 2; ++index1) {
			for (unsigned int index2 = index1 + 1; index2 < goodElectronCleanedJets.size() - 1; ++index2) {
				for (unsigned int index3 = index2 + 1; index3 < goodElectronCleanedJets.size(); ++index3) {
					FourVector m3Vector(
							goodElectronCleanedJets.at(index1)->getFourVector()
									+ goodElectronCleanedJets.at(index2)->getFourVector()
									+ goodElectronCleanedJets.at(index3)->getFourVector());
					double currentPt = m3Vector.Pt();
					if (currentPt > max_pt) {
						max_pt = currentPt;
						m3 = m3Vector.M();
					}
				}
			}
		}
	}

	return m3;
}

double TopPairEventCandidate::mttbar() const {
	return getResonance()->mass();
}

const std::vector<TtbarHypothesisPointer>& TopPairEventCandidate::Solutions() const {
	return solutions;
}

void TopPairEventCandidate::inspectReconstructedEvent() const {
	cout << "run " << runNumber << ", event " << eventNumber << endl;
	cout << "leptonic b jet, goodJet index " << leptonicBIndex << endl;
	EventContentPrinter::printJet(leptonicBJet);

	cout << "electron from W" << endl;
	EventContentPrinter::printElectron(electronFromW);

	cout << "MET" << endl;
	EventContentPrinter::printParticle(MET());
	cout << endl;

	cout << "reconstructed neutrino 1(selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(neutrino1);
	cout << endl;

	cout << "reconstructed neutrino 2(selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(neutrino2);
	cout << endl;

	cout << "leptonic W 1 (selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(leptonicW1);
	cout << endl;

	cout << "leptonic W 2 (selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(leptonicW2);
	cout << endl;

	cout << "leptonic top 1 (selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(leptonicTop1);
	cout << endl;

	cout << "leptonic top 2 (selected: " << selectedNeutrino << ")" << endl;
	EventContentPrinter::printParticle(leptonicTop2);
	cout << endl;

	cout << "hadronic b jet, goodJet index " << hadronicBIndex << endl;
	EventContentPrinter::printJet(hadronicBJet);

	cout << "jet1 from W, goodJet index " << jet1FromWIndex << endl;
	EventContentPrinter::printJet(jet1FromW);

	cout << "jet 2 from W, goodJet index " << jet2FromWIndex << endl;
	EventContentPrinter::printJet(jet2FromW);

	cout << "hadronic W" << endl;
	EventContentPrinter::printParticle(hadronicW);
	cout << endl;

	cout << "hadronic top" << endl;
	EventContentPrinter::printParticle(hadronicTop);
	cout << endl;
}

double TopPairEventCandidate::fullHT() const {
	double ht(MET()->pt());

	for (unsigned int index = 0; index < goodIsolatedElectrons.size(); ++index) {
		ht += goodIsolatedElectrons.at(index)->pt();
	}

	for (unsigned int index = 0; index < goodIsolatedMuons.size(); ++index) {
		ht += goodIsolatedMuons.at(index)->pt();
	}

	for (unsigned int index = 0; index < goodElectronCleanedJets.size(); ++index) {
		ht += goodElectronCleanedJets.at(index)->pt();
	}
	return ht;
}

double TopPairEventCandidate::transverseWmass(const ElectronPointer electron) const {
	double energySquared = pow(electron->et() + MET()->et(), 2);
	double momentumSquared = pow(electron->px() + MET()->px(), 2) + pow(electron->py() + MET()->py(), 2);
	double tMassSquared = energySquared - momentumSquared;

	if (tMassSquared > 0)
		return sqrt(tMassSquared);
	else
		return -1;
}

bool TopPairEventCandidate::hasBeenReconstructed() const {
	return doneReconstruction_;
}

}
