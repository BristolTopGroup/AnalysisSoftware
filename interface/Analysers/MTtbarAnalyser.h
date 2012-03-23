/*
 * MTtbarAnalyser.h
 *
 *  Created on: 5 Sep 2011
 *      Author: kreczko
 */

#ifndef MTTBARANALYSER_H_
#define MTTBARANALYSER_H_

#include "BasicAnalyser.h"
#include <string>
namespace BAT {

namespace MttbarAnalysis {
enum type {
	ElectronPlusJets, MuonPlusJets
};
}

class MTtbarAnalyser: public BAT::BasicAnalyser {
public:
	MTtbarAnalyser(HistogramManagerPtr histMan);
	virtual ~MTtbarAnalyser();
	void analyse(const EventPtr);

	void createHistograms();

protected:
	void analyseFourJetChi2(const TopPairEventCandidatePtr);
	void analyseThreeJetChi2(const TopPairEventCandidatePtr);
	void analyseFourJetTopMassDifference(const TopPairEventCandidatePtr);
	void analyseFourJetChi2QCDConversionBackground(const TopPairEventCandidatePtr);
	void fillHistograms(std::string subcollection, std::string suffix = "");

	void createHistogramsFor(std::string collection);

protected:
	double weight;
	std::string currentType;
	TtbarHypothesisCollection allSolutions;

};

} /* namespace BAT */
#endif /* MTTBARANALYSER_H_ */
