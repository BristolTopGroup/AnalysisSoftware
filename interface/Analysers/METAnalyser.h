/*
 * METAnalyser.h
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#ifndef METANALYSER_H_
#define METANALYSER_H_

#include "BasicAnalyser.h"
#include "../RecoObjects/MET.h"
#include "../RecoObjects/Particle.h"
#include "../RecoObjects/Jet.h"
namespace BAT {

class METAnalyser: public BAT::BasicAnalyser {
public:
	METAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "METAnalysis");
	virtual ~METAnalyser();
	void analyse(const EventPtr);
	void analyse(const EventPtr, const ParticlePointer);
	void analyse(const EventPtr, const ParticlePointer, const JetCollection);

	void createHistograms();
protected:
	void analyseTransverseMass(const EventPtr, const ParticlePointer);
	void analyse_ST(const EventPtr, const ParticlePointer, const JetCollection);
	void analyse_HT(const EventPtr, const JetCollection);
};

typedef boost::scoped_ptr<METAnalyser> METAnalyserLocalPtr;

} /* namespace BAT */
#endif /* METANALYSER_H_ */
