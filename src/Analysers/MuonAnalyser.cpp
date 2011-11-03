/*
 * MuonAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/MuonAnalyser.h"

namespace BAT {

MuonAnalyser::MuonAnalyser(boost::shared_ptr<HistogramManager> histMan) :
	BasicAnalyser(histMan) {

}

MuonAnalyser::~MuonAnalyser() {
}

} /* namespace BAT */
