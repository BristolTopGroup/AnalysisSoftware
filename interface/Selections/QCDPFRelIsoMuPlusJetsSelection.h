/*
 * QCDPFRelIsoSelection.h
 *
 *  Created on: 12 Apr 2012
 *      Author: kreczko
 */

#ifndef QCDPFRELISOMUPLUSJETSSELECTION_H_
#define QCDPFRELISOMUPLUSJETSSELECTION_H_

#include "TopPairMuPlusJetsReferenceSelection.h"

namespace BAT {
/**
 * QCD control region selection for relative isolation based on the top pair mu + jets reference selection.
 */
class QCDPFRelIsoMuPlusJetsSelection: virtual public BAT::TopPairMuPlusJetsReferenceSelection {
public:
	QCDPFRelIsoMuPlusJetsSelection(unsigned int numberOfSelectionSteps =
			TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS);
	virtual ~QCDPFRelIsoMuPlusJetsSelection();

	virtual bool passesTriggerSelection(const EventPtr event) const;
	virtual bool hasExactlyOneIsolatedLepton(const EventPtr event) const;
	virtual const MuonPointer MostIsolatedMuon(const MuonCollection&) const;

	virtual unsigned int prescale(const EventPtr event) const;

	virtual const LeptonPointer signalLepton(const EventPtr event) const;
};

typedef boost::shared_ptr<QCDPFRelIsoMuPlusJetsSelection> QCDMuPlusJetsPFRelIsoSelectionPointer;

} /* namespace BAT */
#endif /* QCDPFRELISOMUPLUSJETSSELECTION_H_ */
