/*
 * JetAnalyser.h
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#ifndef JETANALYSER_H_
#define JETANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class JetAnalyser: public BAT::BasicAnalyser {
public:
	JetAnalyser(HistogramManagerPtr histMan);
	virtual ~JetAnalyser();

	void analyse(const EventPtr);
	void createHistograms();
};

} /* namespace BAT */
#endif /* JETANALYSER_H_ */
