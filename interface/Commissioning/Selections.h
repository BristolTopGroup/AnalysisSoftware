/*
 * Selections.h
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#ifndef SELECTIONS_H_
#define SELECTIONS_H_

#include "../Analysers/BasicAnalyser.h"
#include "../Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../Selections/TopPairEPlusJetsZprimeSelection.h"
#include "../Selections/TopPairEplusJetsPlusMETSelection.h"
#include "../Selections/QCDPFRelIsoSelection.h"
#include "../TopPairEventCandidate.h"

#include <string>

namespace BAT {

class Selections: public BAT::BasicAnalyser {
public:
	Selections(HistogramManagerPtr);
	virtual ~Selections();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();

private:
	SelectionPointer topEplusJetsReferenceSelection_, topEplusJetsZprimeSelection_, topEplusJetsPlusMETSelection_,
			qcdPFRelIsoSelection_;
	EventPtr currentEvent_;
	TopPairEventCandidatePtr currentTopEvent_;

	void commissionTopEplusJetsReferenceSelection(const EventPtr event);
	void commissionTopEplusJetsZprimeSelection(const EventPtr event);
	void commissionTopEplusJetsPlusMETSelection(const EventPtr event);
	void commissionQCDPFRelIsoSelection(const EventPtr event);
	void testResult(bool news, bool old, std::string step) const;
};

} /* namespace BAT */
#endif /* SELECTIONS_H_ */
