/*
 * VertexAnalyser.h
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#ifndef VERTEXANALYSER_H_
#define VERTEXANALYSER_H_

#include "BasicAnalyser.h"
#include "../EventWeightProvider.h"

namespace BAT {

class VertexAnalyser: public BAT::BasicAnalyser {
public:
	VertexAnalyser(boost::shared_ptr<HistogramManager> histMan, boost::shared_ptr<EventWeightProvider> weights);
	virtual ~VertexAnalyser();

	void analyse(const TopPairEventCandidate& ttbarEvent);
	void createHistograms();

private:
	boost::shared_ptr<EventWeightProvider> weights;
};

}

#endif /* VERTEXANALYSER_H_ */
