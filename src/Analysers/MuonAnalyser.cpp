/*
 * MuonAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/MuonAnalyser.h"

namespace BAT {

void MuonAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {

}

void MuonAnalyser::createHistograms() {

}

MuonAnalyser::MuonAnalyser(boost::shared_ptr<HistogramManager> histMan) :
	BasicAnalyser(histMan) {

}

MuonAnalyser::~MuonAnalyser() {
}

} /* namespace BAT */
