/*
 * METAnalyser.h
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#ifndef METANALYSER_H_
#define METANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class METAnalyser: public BAT::BasicAnalyser {
public:
	METAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "METAnalysis");
	virtual ~METAnalyser();

	void analyse(const EventPtr);
	void analyseTransverseMass(const METPointer, const ParticlePointer, double weight);
	double transverseMass(const METPointer, const ParticlePointer) const;
	void createHistograms();
};

typedef boost::scoped_ptr<METAnalyser> METAnalyserLocalPtr;

} /* namespace BAT */
#endif /* METANALYSER_H_ */
