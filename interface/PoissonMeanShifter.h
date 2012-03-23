/*
 * PoissonMeanShifter.h
 *
 *  Created on: 15 Mar 2012
 *      Author: kreczko
 */

#ifndef POISSONMEANSHIFTER_H_
#define POISSONMEANSHIFTER_H_

namespace BAT {

class PoissonMeanShifter {
public:
	PoissonMeanShifter(double shift);
	virtual ~PoissonMeanShifter();

	double ShiftWeight(unsigned int ibin) const;
	double ShiftWeight(double pvnum) const;

private:
	static double p0_minus[20], p1_minus[20], p2_minus[20], p1_expoM[5], p2_expoM[5];
	static double p0_plus[20], p1_plus[20], p2_plus[20], p1_expoP[5], p2_expoP[5];
	double Pweight_[25];

};

} /* namespace BAT */
#endif /* POISSONMEANSHIFTER_H_ */
