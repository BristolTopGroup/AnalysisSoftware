 //BinningAnalyser.h

#ifndef BINNINGANALYSER_H_
#define BINNINGANALYSER_H_

#include "BasicAnalyser.h"
#include "../Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../Selections/TopPairMuPlusJetsReferenceSelection.h"

namespace BAT {

class BinningAnalyser: public BAT::BasicAnalyser {
public:
	BinningAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "Binning");
	virtual ~BinningAnalyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();
	void setHistogramLabels();

	void ePlusJetsSignalAnalysis(const EventPtr event);
	void ePlusJetsHTAnalysis(const EventPtr event);
	void ePlusJetsHTskimAnalysis(const EventPtr event);
	void muPlusJetsSignalAnalysis(const EventPtr event);
	void muPlusJetsMETAnalysis(const EventPtr event);
	void ePlusJetsMETAnalysis(const EventPtr event);

private:
	SelectionPointer topEplusJetsRefSelection_, topMuPlusJetsRefSelection_;
};

} /* namespace BAT */
#endif /* BINNINGANALYSER_H_ */
