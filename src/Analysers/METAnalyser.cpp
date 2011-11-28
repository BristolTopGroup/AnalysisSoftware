/*
 * METAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/METAnalyser.h"

namespace BAT {

void METAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {

}

void METAnalyser::createHistograms() {

}

METAnalyser::METAnalyser(boost::shared_ptr<HistogramManager> histMan) :
	BasicAnalyser(histMan) {

}

METAnalyser::~METAnalyser() {
}

} /* namespace BAT */
