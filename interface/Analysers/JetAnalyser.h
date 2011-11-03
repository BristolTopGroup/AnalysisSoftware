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
	JetAnalyser(boost::shared_ptr<HistogramManager> histMan);
	virtual ~JetAnalyser();
};

} /* namespace BAT */
#endif /* JETANALYSER_H_ */
