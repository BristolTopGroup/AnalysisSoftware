/*
 * EventCountAnalyser.h
 *
 *  Created on: 24 Mar 2012
 *      Author: kreczko
 */

#ifndef EVENTCOUNTANALYSER_H_
#define EVENTCOUNTANALYSER_H_

#include "BasicAnalyser.h"
#include "../Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../Selections/TopPairEplusJetsPlusMETSelection.h"
#include "../Selections/QCDNonIsolatedElectronSelection.h"
#include "../Selections/QCDConversionSelection.h"
#include "../Selections/QCDPFRelIsoSelection.h"

namespace BAT {

class EventCountAnalyser: public BAT::BasicAnalyser {
public:
	EventCountAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "EventCount");
	virtual ~EventCountAnalyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();
	void setHistogramLabels();

	void topEPlusJetsReferenceSelection(const EventPtr event);
	void qcdSelections(const EventPtr event);

private:
	SelectionPointer topEplusJetsRefSelection_, topEplusJetsPlusMETSelection_;
	SelectionPointer qcdNonIsoElectronSelection_, qcdConversionSelection_, qcdPFRelIsoSelection_;
};

} /* namespace BAT */
#endif /* EVENTCOUNTANALYSER_H_ */

