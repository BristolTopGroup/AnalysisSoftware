/*
 * QCDPFRelIsoSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDPFRelIsoSelection.h"

namespace BAT {

QCDPFRelIsoSelection::QCDPFRelIsoSelection(unsigned int numberOfSelectionSteps) :
		TopPairEPlusJetsReferenceSelection(numberOfSelectionSteps) {

}

QCDPFRelIsoSelection::~QCDPFRelIsoSelection() {
}

bool QCDPFRelIsoSelection::passesTriggerSelection(const EventPtr event) const {
	return TopPairEPlusJetsReferenceSelection::passesTriggerSelection(event);
//	unsigned int runNumber(event->runnumber());
//	if (event->isRealData()) {
//		if (runNumber >= 160404 && runNumber <= 163869)
//			return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
//		else if (runNumber > 163869 && runNumber <= 165633)
//			return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);
//		else if (runNumber > 165633 && runNumber <= 178380)
//			return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
//		else if (runNumber > 178380)
//			return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30);
//		else
//			return false;
//	} else {
//		return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30)
//				|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30)
//				|| event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30);
//	}
}

bool QCDPFRelIsoSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	const ElectronCollection allElectrons(event->Electrons());

	unsigned int nGoodElectrons(0), nGoodIsolatedElectrons(0);

	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
		const ElectronPointer electron(allElectrons.at(index));
		if (isGoodElectron(electron)) {
			++nGoodElectrons;
			if (isIsolated(electron))
				++nGoodIsolatedElectrons;
		}
	}

	return nGoodElectrons > 0 && nGoodIsolatedElectrons < 2;
}

bool QCDPFRelIsoSelection::passesConversionRejectionMissingLayers(const EventPtr event) const {
	const ElectronPointer electron = MostIsolatedElectron(event->GoodElectrons());
	return electron->innerLayerMissingHits() == 0;
}

const ElectronPointer QCDPFRelIsoSelection::MostIsolatedElectron(const ElectronCollection& electrons) const {
	double bestIsolation = 999999999;
	unsigned int bestIsolatedLepton = 990;

	for (unsigned int index = 0; index < electrons.size(); ++index) {
		double currentIsolation(electrons.at(index)->pfRelativeIsolation(Globals::electronIsolationCone));

		if (currentIsolation < bestIsolation) {
			bestIsolation = currentIsolation;
			bestIsolatedLepton = index;
		}
	}
	ElectronPointer electron(new Electron());

	if (bestIsolatedLepton < electrons.size())
		electron = electrons.at(bestIsolatedLepton);

	return electron;
}

bool QCDPFRelIsoSelection::passesConversionRejectionPartnerTrack(const EventPtr event) const {
	const ElectronPointer electron = MostIsolatedElectron(event->GoodElectrons());
	bool isConversion = fabs(electron->dCotThetaToClosestTrack()) < 0.02 && fabs(electron->distToClosestTrack()) < 0.02;
	return !isConversion;
}

} /* namespace BAT */
