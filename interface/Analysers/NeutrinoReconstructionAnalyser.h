/*
 * NeutrinoReconstructionAnalyser.h
 *
 *  Created on: Apr 2, 2012
 *      Author: lkreczko
 */

#ifndef NEUTRINORECONSTRUCTIONANALYSER_H_
#define NEUTRINORECONSTRUCTIONANALYSER_H_

#include "BasicAnalyser.h"
#include "../TtbarHypothesis.h"
#include <boost/scoped_ptr.hpp>

namespace BAT {

class NeutrinoReconstructionAnalyser: public BAT::BasicAnalyser {
public:
	NeutrinoReconstructionAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "NeutrinoReconstruction");
	virtual ~NeutrinoReconstructionAnalyser();
	void analyse(const EventPtr event);
	void createHistograms();

	void setMCTTbarHypothesis(const TtbarHypothesisPointer mcEvent);

private:
	TtbarHypothesisPointer truthMatchEvent;
};

typedef boost::scoped_ptr<NeutrinoReconstructionAnalyser> NeutrinoRecoAnalyserLocalPtr;

}

#endif /* NEUTRINORECONSTRUCTIONANALYSER_H_ */
