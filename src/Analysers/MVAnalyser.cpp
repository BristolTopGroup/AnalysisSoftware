/*
 * MVAnalyser.cpp
 *
 *  Created on: Nov 29, 2011
 *      Author: lkreczko
 */

#include "../../interface/Analysers/MVAnalyser.h"

namespace BAT {

MVAnalyser::MVAnalyser(HistogramManagerPtr histMan): BasicAnalyser(histMan) {
}

MVAnalyser::~MVAnalyser() {
}

void MVAnalyser::analyse(const TopPairEventCandidate& event)
{

}

void MVAnalyser::createHistograms() {

}
}

