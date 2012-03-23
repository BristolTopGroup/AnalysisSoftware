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
	VertexAnalyser(HistogramManagerPtr histMan, EventWeightProviderPtr weights);
	virtual ~VertexAnalyser();

	void analyse(const EventPtr);
	void createHistograms();

private:
	EventWeightProviderPtr weights;
};

}

#endif /* VERTEXANALYSER_H_ */
