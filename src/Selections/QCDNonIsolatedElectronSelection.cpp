/*
 * QCDNonIsolatedElectronSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDNonIsolatedElectronSelection.h"

namespace BAT {

QCDNonIsolatedElectronSelection::QCDNonIsolatedElectronSelection(unsigned int numberOfSelectionSteps) :
		QCDPFRelIsoEPlusJetsSelection(numberOfSelectionSteps) {

}

QCDNonIsolatedElectronSelection::~QCDNonIsolatedElectronSelection() {
}

bool QCDNonIsolatedElectronSelection::passesTriggerSelection(const EventPtr event) const {
	if (!useNonIsoTrigger_)
		return TopPairEPlusJetsReferenceSelection::passesTriggerSelection(event);
	else {
		unsigned int runNumber(event->runnumber());
		if (event->isRealData()) {
			if (runNumber >= 160404 && runNumber <= 163869)
				return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
			else if (runNumber > 163869 && runNumber <= 178380)
				return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);
			else if (runNumber > 178380)
				return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30);
			else
				return false;
		} else {
			//Fall11 MC
			return event->HLT(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);
		}
	}
}

bool QCDNonIsolatedElectronSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
	const ElectronCollection allElectrons(event->Electrons());
//
	unsigned int nGoodElectrons(0), nGoodNonIsolatedElectrons(0), nGoodIsolatedElectrons(0);

	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
		const ElectronPointer electron(allElectrons.at(index));
		if (isGoodElectron(electron)) {
			++nGoodElectrons;
			if (electron->pfRelativeIsolation(Globals::electronIsolationCone) < 0.2)
				++nGoodIsolatedElectrons;

			if (electron->pfRelativeIsolation(Globals::electronIsolationCone) > 0.2)
				++nGoodNonIsolatedElectrons;
		}
	}
	//no electrons below 0.2 in PFRelIso and at least one electron with PFIso > 0.2
	return nGoodNonIsolatedElectrons > 0 && nGoodIsolatedElectrons == 0;
}

unsigned int QCDNonIsolatedElectronSelection::prescale(const EventPtr event) const {
	unsigned int runNumber(event->runnumber());
	unsigned int prescale(1);

	if (useNonIsoTrigger_) {
		if (event->isRealData()) {
			if (runNumber >= 160404 && runNumber <= 163869)
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_CentralTriJet30);
			else if (runNumber > 163869 && runNumber <= 178380)
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);
			else if (runNumber > 178380)
				prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFJet30);
		} else
			prescale = event->HLTPrescale(HLTriggers::HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30);

	}
	return prescale;
}

} /* namespace BAT */
