/*
 * Selections.cpp
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Commissioning/Selections.h"

namespace BAT {

void Selections::analyse(const EventPtr event){

}

void Selections::createHistograms(){

}

Selections::Selections(HistogramManagerPtr histMan) :
		BasicAnalyser(histMan),//
		topEplusJetsReferenceSelection_(new TopPairEPlusJetsReferenceSelection()){

}

Selections::~Selections() {
}

} /* namespace BAT */
