/*
 * MuonAnalyser.h
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#ifndef MUONANALYSER_H_
#define MUONANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class MuonAnalyser: public BAT::BasicAnalyser {
public:
	MuonAnalyser(HistogramManagerPtr histMan);
	virtual ~MuonAnalyser();

	void analyse(const EventPtr);
	void createHistograms();
};

} /* namespace BAT */
#endif /* MUONANALYSER_H_ */
