#ifndef BTAGWEIGHT2011_H
#define BTAGWEIGHT2011_H

#include "RecoObjects/Jet.h"
/**
 * Original by Andrea Rizzi
 * Modified by Lukasz Kreczko
 */
namespace BAT {

class BTagWeight2011 {
public:
	BTagWeight2011();

	double weight(unsigned int numberOf_b_Jets, unsigned int numberOf_c_Jets, unsigned int numberOf_udsg_Jets,
			double mean_bJetEfficiency, double mean_cFJetEfficiency, double mean_udsgJetEfficiency,
			double scaleFactor_b, double scaleFactor_c, double scaleFactor_udsg, unsigned int numberOfTags) const;
	std::vector<double> weights(const JetCollection jets, unsigned int numberOfBtags) const;

	std::vector<double> weights(unsigned int numberOf_b_Jets, unsigned int numberOf_c_Jets,
			unsigned int numberOf_udsg_Jets, double mean_bJetEfficiency, double mean_cFJetEfficiency,
			double mean_udsgJetEfficiency, double scaleFactor_b, double scaleFactor_c, double scaleFactor_udsg,
			unsigned int numberOfTags) const;

	std::vector<double> weights(double averageScaleFactor, unsigned int numberOfTags) const;

	void setNumberOfBtags(unsigned int min, unsigned int max);

	JetCollection getBJets(const JetCollection jets) const;
	JetCollection getCJets(const JetCollection jets) const;
	JetCollection getUDSGJets(const JetCollection jets) const;

	bool filter(unsigned int t) const;

	double getAverageBScaleFactor(const JetCollection, double uncertaintyFactor = 1.) const;
	double getBScaleFactor(const JetPointer jet, double uncertaintyFactor = 1.) const;
	double getAverageBEfficiency() const;
	double getAverageCScaleFactor(const JetCollection) const;
	double getCScaleFactor(const JetPointer) const;
	double getAverageCEfficiency() const;
	double getAverageUDSGScaleFactor(const JetCollection) const;
	double getUDSGScaleFactor(const JetPointer) const;
	double getAverageUDSGEfficiency(const JetCollection) const;

	double getMeanUDSGScaleFactor(double jetPT) const;
	double getMinUDSGScaleFactor(double jetPT) const;
	double getMaxUDSGScaleFactor(double jetPT) const;

	double getMeanUDSGEfficiency(double jetPT) const;
private:
	unsigned int minNumberOfTags_;
	unsigned int maxNumberOfTags_;

};

std::vector<double> BjetWeights2011(const JetCollection jets, unsigned int numberOfBtags);
}
#endif

