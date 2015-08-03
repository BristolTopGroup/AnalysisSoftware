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

	double weight(const JetCollection jets) const;

	void setNumberOfBtags(unsigned int min, unsigned int max);

	bool filter(unsigned int t) const;

	double getEfficiency( const unsigned int, const JetPointer ) const;
	std::vector<double> getScaleFactor( const double, const JetPointer ) const;

	std::vector<double> getBScaleFactor(const JetPointer, double uncertaintyFactor = 1.) const;
	std::vector<double> getCScaleFactor(const JetPointer) const;
	std::vector<double> getUDSGScaleFactor(const JetPointer) const;

	float getBEfficiency(const JetPointer) const;
	double getCEfficiency(const JetPointer) const;
	double getUDSEfficiency(const JetPointer) const;
	double getGEfficiency(const JetPointer) const;

private:
	unsigned int minNumberOfTags_;
	unsigned int maxNumberOfTags_;

};

double BjetWeights(const JetCollection jets);
}
#endif

