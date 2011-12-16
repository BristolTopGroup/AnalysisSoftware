/*
 * Efficiency.cpp
 *
 *  Created on: 15 Dec 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/Efficiency.h"

namespace BAT {

void Efficiency::countPositive(DataType::value type, double weight){
	positives_[type] += weight;
}

void Efficiency::countNegative(DataType::value type, double weight){
	negatives_[type] += weight;
}

double Efficiency::getEfficiency(DataType::value type){
	double denominator = positives_[type] + negatives_[type];
	return positives_[type]/denominator;
}

double Efficiency::Positives(DataType::value type){
	return positives_[type];
}

double Efficiency::Negatives(DataType::value type){
	return negatives_[type];
}

Efficiency::Efficiency() :
		positives_(), //
		negatives_() //
{

}

Efficiency::~Efficiency() {
}


} /* namespace BAT */
