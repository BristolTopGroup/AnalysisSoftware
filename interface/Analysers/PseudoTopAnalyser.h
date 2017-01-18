/*
 * PseudoTopAnalyser.h
 *
 *  Created on: 24 Feb 2015
 *      Author: ejclemen
 */

#ifndef PseudoTopAnalyser_H_
#define PseudoTopAnalyser_H_

#include "BasicAnalyser.h"

namespace BAT {

class PseudoTopAnalyser: public BAT::BasicAnalyser {
public:
	PseudoTopAnalyser(TreeManagerPtr treeMan, std::string histogramFolder = "TTbar_plus_X_analysis/Unfolding");
	virtual ~PseudoTopAnalyser();
	void analyse(const EventPtr);

	void createTrees();

	static double const minLeptonPt_;
	static double const maxLeptonAbsEta_;
	static double const minVetoLeptonPt_;
	static double const maxVetoLeptonAbsEta_;
	static double const minNeutrinoSumPt_;
	static double const minWMt_;
	static unsigned int const minNJets_;
	static unsigned int const minNBJets_;
	static double const minJetPt_;
	static double const maxJetAbsEta_;

protected:
	bool passesEventSelection(const MCParticlePointer pseudoLepton, const JetCollection pseudoJets, const MCParticleCollection pseudoBs, const ParticleCollection allPseudoLeptons );
};
typedef boost::scoped_ptr<PseudoTopAnalyser> PseudoTopAnalyserLocalPtr;
typedef boost::shared_ptr<PseudoTopAnalyser> PseudoTopAnalyserPtr;
} /* namespace BAT */
#endif /* PseudoTopAnalyser_H_ */
