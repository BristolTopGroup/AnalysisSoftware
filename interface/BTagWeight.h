#ifndef BTAGWEIGHT_H
#define BTAGWEIGHT_H

#include "RecoObjects/Jet.h"

namespace BAT {

class BTagWeight {
public:
	BTagWeight(unsigned int minNumberOfTags, unsigned int maxNumberOfTags);

	double weight(unsigned int numberOf_b_Jets, unsigned int numberOf_c_Jets, unsigned int numberOf_udsg_Jets,
			double mean_bJetEfficiency, double mean_cFJetEfficiency, double mean_udsgJetEfficiency,
			double scaleFactor_b, double scaleFactor_c, double scaleFactor_udsg, unsigned int numberOfTags) const;

	std::vector<double> BjetWeights(const JetCollection jets, unsigned int numberOfBtags) const;

private:
	unsigned int minNumberOfTags_;
	unsigned int maxNumberOfTags_;

	JetCollection getBJets(const JetCollection jets) const;
	JetCollection getCJets(const JetCollection jets) const;
	JetCollection getUDSGJets(const JetCollection jets) const;

	bool filter(unsigned int t) const;

	double getAverageBScaleFactor(const JetCollection, double uncertaintyFactor = 1.) const;
	double getAverageBEfficiency() const;
	double getAverageCScaleFactor(const JetCollection) const;
	double getAverageCEfficiency() const;
	double getAverageUDSGScaleFactor(const JetCollection) const;
	double getAverageUDSGEfficiency(const JetCollection) const;

	double getMeanUDSGScaleFactor(double jetPT) const;
	double getMinUDSGScaleFactor(double jetPT) const;
	double getMaxUDSGScaleFactor(double jetPT) const;

	double getMeanUDSGEfficiency(double jetPT) const;
};
}
#endif

