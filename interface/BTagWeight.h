#ifndef BTagWeight_H
#define BTagWeight_H

#include "RecoObjects/Jet.h"
#include <boost/shared_ptr.hpp>
#include <iostream>
/**
 * Original by Andrea Rizzi
 * Modified by Lukasz Kreczko
 */
namespace BAT {

class BTagWeight {
public:
	BTagWeight();

	double weight(const JetCollection jets, const int systematic, const std::vector<std::string> bJetWPs) const;

	void setNumberOfBtags(unsigned int min, unsigned int max);

	bool filter(unsigned int t) const;

	double getEfficiency( const unsigned int, const JetPointer, bool centralMCEfficiency=false ) const;
	std::vector<double> getScaleFactor( const double, const JetPointer ) const;

	std::vector<double> getBScaleFactor(const JetPointer, double uncertaintyFactor = 1.) const;
	std::vector<double> getCScaleFactor(const JetPointer) const;
	std::vector<double> getUDSGScaleFactor(const JetPointer) const;

	float getBEfficiency(const JetPointer, bool centralMCEfficiency=false) const;
	double getCEfficiency(const JetPointer, bool centralMCEfficiency=false) const;
	double getUDSGEfficiency(const JetPointer, bool centralMCEfficiency=false) const;

private:
	unsigned int minNumberOfTags_;
	unsigned int maxNumberOfTags_;

};

double BjetWeights(const JetCollection jets, const int systematic, const std::vector<std::string> bJetWPs);
}
#endif

