/*
 * MTtbarAnalyser.h
 *
 *  Created on: 5 Sep 2011
 *      Author: kreczko
 */

#ifndef MTTBARANALYSER_H_
#define MTTBARANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class MTtbarAnalyser: public BAT::BasicAnalyser {
public:
	MTtbarAnalyser(boost::shared_ptr<HistogramManager> histMan);
	virtual ~MTtbarAnalyser();
	void analyse(const TopPairEventCandidate& ttbarEvent);

	void createHistograms();

protected:
	void analyseFourJetChi2(const TopPairEventCandidate& ttbarEvent);
	void analyseThreeJetChi2(const TopPairEventCandidate& ttbarEvent);
	void analyseFourJetTopMassDifference(const TopPairEventCandidate& ttbarEvent);
	void analyseFourJetChi2QCDConversionBackground(const TopPairEventCandidate& ttbarEvent);
	void fillHistograms(std::string subcollection, std::string suffix = "");

	void createHistogramsFor(std::string collection);

protected:
	double weight;
	TtbarHypothesisCollection allSolutions;


};

} /* namespace BAT */
#endif /* MTTBARANALYSER_H_ */
