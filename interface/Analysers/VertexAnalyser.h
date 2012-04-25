/*
 * VertexAnalyser.h
 *
 *  Created on: Nov 24, 2011
 *      Author: lkreczko
 */

#ifndef VERTEXANALYSER_H_
#define VERTEXANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class VertexAnalyser: public BAT::BasicAnalyser {
public:
	VertexAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "VertexAnalyser");
	virtual ~VertexAnalyser();

	virtual void analyse(const EventPtr);
	virtual void createHistograms();
};

}

#endif /* VERTEXANALYSER_H_ */
