/*
 * Selections.cpp
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Commissioning/Selections.h"
#include "../../interface/Printers/EventContentPrinter.h"
#include <iostream>

using namespace std;

namespace BAT {

void Selections::analyse(const EventPtr event) {
	currentEvent_ = event;
	commissionTopEplusJetsReferenceSelection(event);
	commissionTopEplusJetsZprimeSelection(event);
	commissionTopEplusJetsPlusMETSelection(event);
	commissionQCDPFRelIsoSelection(event);
}

void Selections::commissionTopEplusJetsReferenceSelection(const EventPtr event) {
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));

	bool resultSelection(
			topEplusJetsReferenceSelection_->passesSelectionStep(event,
					TTbarEPlusJetsReferenceSelection::EvenCleaningAndTrigger));
	bool resultOldSelection(ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex));
	testResult(resultSelection, resultOldSelection, "EvenCleaningAndTrigger");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::OneIsolatedElectron);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::OneIsolatedElectron);
	testResult(resultSelection, resultOldSelection, "OneIsolatedElectron");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::LooseMuonVeto);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::LooseMuonVeto);
	testResult(resultSelection, resultOldSelection, "LooseMuonVeto");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::DiLeptonVeto);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::DileptonVeto);
	testResult(resultSelection, resultOldSelection, "DiLeptonVeto");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::ConversionRejectionMissingLayers);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::ConversionRejection);
	testResult(resultSelection, resultOldSelection, "ConversionRejectionMissingLayers");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::ConversionRejectionPartnerTrack);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::ConversionFinder);
	testResult(resultSelection, resultOldSelection, "ConversionRejectionPartnerTrack");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastOneGoodJet);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastOneGoodJet);
	testResult(resultSelection, resultOldSelection, "AtLeastOneGoodJets");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastTwoGoodJets);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastTwoGoodJets);
	testResult(resultSelection, resultOldSelection, "AtLeastTwoGoodJets");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastThreeGoodJets);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastThreeGoodJets);
	testResult(resultSelection, resultOldSelection, "AtLeastThreeGoodJets");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastFourGoodJets);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets);
	testResult(resultSelection, resultOldSelection, "AtLeastFourGoodJets");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastOneBtag);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)
			&& ttbarCand->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastOneBtag);
	testResult(resultSelection, resultOldSelection, "AtLeastOneBtag");

	resultSelection = topEplusJetsReferenceSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsReferenceSelection::AtLeastTwoBtags);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)
			&& ttbarCand->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastTwoBtags);
	testResult(resultSelection, resultOldSelection, "AtLeastTwoBtags");
}

/**
 * Since the Z' selection inherits from the reference selection testing only the cuts
 */
void Selections::commissionTopEplusJetsZprimeSelection(const EventPtr event) {
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));

	bool resultSelection(
			topEplusJetsZprimeSelection_->passesSelectionUpToStep(event,
					TTbarEPlusJetsZprimeSelection::MissingTransverseEnergy));
	bool resultOldSelection(
			ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::MissingTransverseEnergy));
	testResult(resultSelection, resultOldSelection, "MissingTransverseEnergy");

	resultSelection = topEplusJetsZprimeSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsZprimeSelection::AsymmetricJetCuts);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AsymmetricJetCuts);
	testResult(resultSelection, resultOldSelection, "AsymmetricJetCuts");

	resultSelection = topEplusJetsZprimeSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsZprimeSelection::AtLeastOneBtag);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastOneBtag);
	testResult(resultSelection, resultOldSelection, "AtLeastOneBtag");

	resultSelection = topEplusJetsZprimeSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsZprimeSelection::AtLeastTwoBtags);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastTwoBtags);
	testResult(resultSelection, resultOldSelection, "AtLeastTwoBtags");
}

/**
 * Since the + MET selection inherits from the Z' selection testing only the cuts
 */
void Selections::commissionTopEplusJetsPlusMETSelection(const EventPtr event) {
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));

	bool resultSelection(
			topEplusJetsPlusMETSelection_->passesSelectionUpToStep(event,
					TTbarEPlusJetsPlusMETSelection::AsymmetricJetCuts));
	bool resultOldSelection(
			ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)
					&& ttbarCand->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AsymmetricJetCuts));
	testResult(resultSelection, resultOldSelection, "AsymmetricJetCuts");

	resultSelection = topEplusJetsPlusMETSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsPlusMETSelection::AtLeastOneBtag);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)
			&& ttbarCand->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AsymmetricJetCuts)
			&& ttbarCand->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastOneBtag);
	testResult(resultSelection, resultOldSelection, "AtLeastOneBtag");

	resultSelection = topEplusJetsPlusMETSelection_->passesSelectionUpToStep(event,
			TTbarEPlusJetsPlusMETSelection::AtLeastTwoBtags);
	resultOldSelection = ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::AtLeastFourGoodJets)
			&& ttbarCand->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AsymmetricJetCuts)
			&& ttbarCand->passesEPlusJetsSelectionStep(TTbarEPlusJetsSelection::AtLeastTwoBtags);
	testResult(resultSelection, resultOldSelection, "AtLeastTwoBtags");
}

void Selections::commissionQCDPFRelIsoSelection(const EventPtr event) {
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));
	if (event->runnumber() == 1 && event->lumiblock() == 161061 && event->eventnumber() == 48308576) {
		cout << "Passes trigger: "
				<< qcdPFRelIsoSelection_->passesSelectionStep(event,
						TTbarEPlusJetsReferenceSelection::EvenCleaningAndTrigger);
		cout << " , " << ttbarCand->passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::GoodPrimaryvertex)
				<< endl;
		bool passGoodElectrons = event->GoodElectrons().size() > 0 && event->GoodPFIsolatedElectrons().size() < 2;
		cout << "Passes good electrons: "
				<< qcdPFRelIsoSelection_->passesSelectionStep(event,
						TTbarEPlusJetsReferenceSelection::OneIsolatedElectron);
		cout << " , " << passGoodElectrons << endl;

		const ElectronPointer electron = event->MostPFIsolatedElectron(event->Electrons());
		bool passesNew = qcdPFRelIsoSelection_->passesSelectionStep(event,
				TTbarEPlusJetsReferenceSelection::ConversionRejectionMissingLayers);
		bool passesOld = electron->isFromConversion() == false;

		cout << "Conversion (missing layers: " << passesNew << " , " << passesOld << endl;

		passesNew = passesNew
				&& qcdPFRelIsoSelection_->passesSelectionStep(event,
						TTbarEPlusJetsReferenceSelection::ConversionRejectionPartnerTrack);
		passesOld = passesOld && electron->isTaggedAsConversion(0.02, 0.02) == false;
		cout << "Conversion (missing layers + partner track): " << passesNew << " , " << passesOld << endl;
		cout << "Loose Muon: " << qcdPFRelIsoSelection_->passesSelectionStep(event, TTbarEPlusJetsReferenceSelection::LooseMuonVeto);
		cout << " , " << ttbarCand->ePlusJetsLooseMuonVeto() << endl;
		cout << "Dilepton veto: "<< qcdPFRelIsoSelection_->passesSelectionStep(event, TTbarEPlusJetsReferenceSelection::DiLeptonVeto);
		cout << " , " << ttbarCand->electronPlusJetsZVeto() << endl;

	}
	bool resultSelection(
			qcdPFRelIsoSelection_->passesSelectionUpToStep(event,
					TTbarEPlusJetsReferenceSelection::ConversionRejectionPartnerTrack));
	bool resultOldSelection(ttbarCand->passesEPlusJetsPFIsoSelection());
	testResult(resultSelection, resultOldSelection, "QCD PF RelIso Selection");
}

void Selections::commissionQCDConversionSelection(const EventPtr event) {
	TopPairEventCandidatePtr ttbarCand(new TopPairEventCandidate(*event.get()));

	bool resultSelection(
			qcdConversionSelection_->passesSelectionUpToStep(event,
					TTbarEPlusJetsReferenceSelection::ConversionRejectionPartnerTrack));
	bool resultOldSelection(ttbarCand->passesConversionSelection());
	testResult(resultSelection, resultOldSelection, "QCD Conversion Selection");
}

void Selections::createHistograms() {

}

Selections::Selections(HistogramManagerPtr histMan) :
		BasicAnalyser(histMan), //
		topEplusJetsReferenceSelection_(new TopPairEPlusJetsReferenceSelection()), //
		topEplusJetsZprimeSelection_(new TopPairEPlusJetsZprimeSelection()), //
		topEplusJetsPlusMETSelection_(new TopPairEplusJetsPlusMETSelection()), //
		qcdPFRelIsoSelection_(new QCDPFRelIsoSelection()), //
		qcdConversionSelection_(new QCDConversionSelection()), //
		currentEvent_(), //
		currentTopEvent_() {

}

Selections::~Selections() {
}

void Selections::testResult(bool news, bool old, string step) const {
	if (news != old) {
		cout << "Selections are not equal at step " << step << endl;
		cout << "Result new selection: " << news << endl;
		cout << "Result old selection: " << old << endl;
		cout << "<run, lumi, event>: <" << currentEvent_->runnumber() << ",";
		cout << currentEvent_->lumiblock() << "," << currentEvent_->eventnumber() << ">" << endl;
		EventContentPrinter::printJets(currentEvent_->Jets());
		EventContentPrinter::printElectrons(currentEvent_->Electrons());
		EventContentPrinter::printMuons(currentEvent_->Muons());
		throw "Selections are not equal";
	}
}

} /* namespace BAT */
