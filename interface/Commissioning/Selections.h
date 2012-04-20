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
#include "../Selections/TopPairEPlusJetsRefAsymJetsMETSelection.h"
#include "../Selections/TopPairEplusJetsRefAsymJetsSelection.h"
#include "../Selections/QCDPFRelIsoEPlusJetsSelection.h"
#include "../Selections/QCDConversionsSelection.h"
#include "../Selections/QCDNonIsolatedElectronSelection.h"
#include "../TopPairEventCandidate.h"

#include <string>

namespace BAT {

class Selections: public BAT::BasicAnalyser {
public:
	Selections(HistogramManagerPtr histMan, std::string histogramFolder = "Selections");
	virtual ~Selections();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();

private:
	SelectionPointer topEplusJetsReferenceSelection_, topEplusJetsZprimeSelection_, topEplusJetsPlusMETSelection_,
			qcdPFRelIsoSelection_, qcdConversionSelection_, qcdNonIsoSelection_;
	EventPtr currentEvent_;
	TopPairEventCandidatePtr currentTopEvent_;

	void commissionTopEplusJetsReferenceSelection(const EventPtr event);
	void commissionTopEplusJetsZprimeSelection(const EventPtr event);
	void commissionTopEplusJetsPlusMETSelection(const EventPtr event);
	void commissionQCDPFRelIsoSelection(const EventPtr event);
	void commissionQCDConversionSelection(const EventPtr event);
	void commissionQCDNonIsoSelection(const EventPtr event);
	void testResult(bool news, bool old, std::string step) const;
};

} /* namespace BAT */
#endif /* SELECTIONS_H_ */
