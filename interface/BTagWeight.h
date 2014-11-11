#ifndef BTagWeight_H
#define BTagWeight_H

#include "RecoObjects/Jet.h"
/**
 * Original by Andrea Rizzi
 * Modified by Lukasz Kreczko
 */
namespace BAT {

class BTagWeight {
public:
	BTagWeight();

	double weight(const JetCollection jets) const;

	void setNumberOfBtags(unsigned int min, unsigned int max);

	bool filter(unsigned int t) const;

	double getEfficiency( const unsigned int, const JetPointer ) const;
	std::vector<double> getScaleFactor( const double, const JetPointer ) const;

	std::vector<double> getBScaleFactor(const JetPointer, double uncertaintyFactor = 1.) const;
	double getBEfficiency(const JetPointer) const;
	std::vector<double> getCScaleFactor(const JetPointer) const;
	double getCEfficiency(const JetPointer) const;
	std::vector<double> getUDSGScaleFactor(const JetPointer) const;
	double getUDSGEfficiency(const JetPointer) const;

	double getMeanUDSGScaleFactor(double jetPT, double jetEta) const;
	double getMinUDSGScaleFactor(double jetPT, double jetEta) const;
	double getMaxUDSGScaleFactor(double jetPT, double jetEta) const;

	double getMeanUDSGEfficiency(double jetPT) const;
private:
	unsigned int minNumberOfTags_;
	unsigned int maxNumberOfTags_;

};

std::vector<double> BjetWeights(const JetCollection jets, unsigned int numberOfBtags);
}
#endif

