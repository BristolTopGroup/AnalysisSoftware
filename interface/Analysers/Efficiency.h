/*
 * Efficiency.h
 *
 *  Created on: 15 Dec 2011
 *      Author: kreczko
 */

#ifndef EFFICIENCY_H_
#define EFFICIENCY_H_
#include "boost/array.hpp"
#include "../DataTypes.h"

namespace BAT {

class Efficiency {
public:
	Efficiency();
	virtual ~Efficiency();
	void countPositive(DataType::value type, double weight);
	void countNegative(DataType::value type, double weight);
	double getEfficiency(DataType::value type);
	double Positives(DataType::value type);
	double Negatives(DataType::value type);
private:
	boost::array<double, DataType::NUMBER_OF_DATA_TYPES> positives_;
	boost::array<double, DataType::NUMBER_OF_DATA_TYPES> negatives_;
};

} /* namespace BAT */
#endif /* EFFICIENCY_H_ */
