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
	float getBEfficiency(const JetPointer) const;
	double getCEfficiency(const JetPointer) const;
	// double getMeanUDSGEfficiency(const JetPointer);
	double getUDSEfficiency(const JetPointer) const;
	double getGEfficiency(const JetPointer) const;

	// double getMeanUDSGEfficiency(double jetPT) const;
private:
	unsigned int minNumberOfTags_;
	unsigned int maxNumberOfTags_;

};

std::vector<double> BjetWeights(const JetCollection jets, unsigned int numberOfBtags);
}
#endif

