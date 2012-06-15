/*
 * MTtbarAnalyser.h
 *
 *  Created on: 5 Sep 2011
 *      Author: kreczko
 */

#ifndef MTTBARANALYSER_H_
#define MTTBARANALYSER_H_

#include "BasicAnalyser.h"
#include "../TtbarHypothesis.h"
#include "../Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../Selections/QCDAntiIDEPlusJetsSelection.h"
#include "../Selections/QCDConversionsSelection.h"
#include "../Selections/QCDNonIsolatedElectronSelection.h"
#include <string>
namespace BAT {

namespace MttbarAnalysis {
enum type {
	ElectronPlusJets, MuonPlusJets
};
}

class MTtbarAnalyser: public BAT::BasicAnalyser {
public:
	MTtbarAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "MttbarAnalysis");
	virtual ~MTtbarAnalyser();
	void analyse(const EventPtr);

	void createHistograms();

protected:
	void analyseFourJetChi2(const EventPtr);
	void analyseThreeJetChi2(const EventPtr);
	void analyseFourJetTopMassDifference(const EventPtr);
	void analyseFourJetChi2QCDConversionBackground(const EventPtr);
	void fillHistograms(std::string subcollection, std::string suffix = "");

	void createHistogramsFor(std::string collection);

protected:
	double weight;
	std::string currentType;
	TtbarHypothesisCollection allSolutions;
	TopPairEPlusJetsReferenceSelectionPointer topEplusJetsRefSelection_;
	QCDAntiIDEPlusJetsSelectionPointer qcdAntiIDSelection_;
	QCDConversionsSelectionPointer qcdConversionSelection_;
	QCDNonIsolatedElectronSelectionPointer qcdNonIsoSelection_;

};

} /* namespace BAT */
#endif /* MTTBARANALYSER_H_ */
