/*
 * QCDPFRelIsoSelection.cpp
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDPFRelIsoEPlusJetsSelection.h"
#include <iostream>

using namespace std;

namespace BAT {

QCDPFRelIsoEPlusJetsSelection::QCDPFRelIsoEPlusJetsSelection(unsigned int numberOfSelectionSteps) :
		TopPairEPlusJetsReferenceSelection(numberOfSelectionSteps) {

}

QCDPFRelIsoEPlusJetsSelection::~QCDPFRelIsoEPlusJetsSelection() {
}

bool QCDPFRelIsoEPlusJetsSelection::passesTriggerSelection(const EventPtr event) const {
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

bool QCDPFRelIsoEPlusJetsSelection::hasExactlyOneIsolatedLepton(const EventPtr event) const {
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

bool QCDPFRelIsoEPlusJetsSelection::passesConversionVeto(const EventPtr event) const {
	const LeptonPointer signalLepton = this->signalLepton(event);
	const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
	return signalElectron->passConversionVeto();
}

const ElectronPointer QCDPFRelIsoEPlusJetsSelection::MostIsolatedElectron(const ElectronCollection& electrons) const {
	double bestIsolation = 999999999;
	unsigned int bestIsolatedLepton = 990;

	for (unsigned int index = 0; index < electrons.size(); ++index) {
		double currentIsolation(electrons.at(index)->pfRelativeIsolation(0.3));

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

unsigned int QCDPFRelIsoEPlusJetsSelection::prescale(const EventPtr event) const {
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

const LeptonPointer QCDPFRelIsoEPlusJetsSelection::signalLepton(const EventPtr event) const {
//	if (!hasExactlyOneIsolatedLepton(event)) {
//		cerr << "An error occurred in QCD*Selection in event = " << event->eventnumber();
//		cerr << ", run = " << event->runnumber() << ", lumi = " << event->lumiblock() << "!" << endl;
//		cerr << "File = " << event->file() << endl;
//		cerr
//				<< "Access exception: No signal lepton could be found. Event doesn't pass 'hasExactlyOneIsolatedLepton' selection"
//				<< endl;
//		throw "Access exception: No signal lepton could be found. Event doesn't pass 'hasExactlyOneIsolatedLepton' selection";
//	}

	const ElectronCollection allElectrons(event->Electrons());
	ElectronCollection goodElectrons;
	for (unsigned int index = 0; index < allElectrons.size(); ++index) {
		const ElectronPointer electron(allElectrons.at(index));
		if (isGoodElectron(electron)) {
			goodElectrons.push_back(electron);
		}
	}

	return MostIsolatedElectron(goodElectrons);

}

} /* namespace BAT */
