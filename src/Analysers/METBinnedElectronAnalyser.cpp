/*
 * METBinnedElectronAnalyser.cpp
 *
 *  Created on: 3 Jul 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/METBinnedElectronAnalyser.h"

namespace BAT {

METBinnedElectronAnalyser::METBinnedElectronAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		metbins_() {

}

METBinnedElectronAnalyser::~METBinnedElectronAnalyser() {
}

void METBinnedElectronAnalyser::setMETbins(std::vector<double> metbins){
	metbins_ = metbins;
}

} /* namespace BAT */

