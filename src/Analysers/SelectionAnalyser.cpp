/*
 * SelectionAnalyser.cpp
 *
 *  Created on: 15 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Analysers/SelectionAnalyser.h"

namespace BAT {

SelectionAnalyser::SelectionAnalyser(HistogramManagerPtr histMan) :
		BasicAnalyser(histMan) {

}

SelectionAnalyser::~SelectionAnalyser() {
}

void SelectionAnalyser::analyse(const EventPtr event) {

}

void SelectionAnalyser::createHistograms() {
	histMan->setCurrentHistogramFolder("SelectionAnalysis/SimpleCuts");
	histMan->addH1D("deltaPhi_electron_nu", "#Delta#Phi(electron,#nu);events; #Delta#Phi(e,#nu)", 40, 0., 4.);
	histMan->addH1D("deltaPhi_muon_nu", "#Delta#Phi(muon,#nu);events; #Delta#Phi(#mu,#nu)", 40, 0., 4.);
	histMan->addH1D("deltaPhi_electron_leptonicB", "#Delta#Phi(electron,b_{l});events;#Delta#Phi(e,b_{l})", 40, 0., 4.);
	histMan->addH1D("deltaPhi_muon_leptonicB", "#Delta#Phi(muon,b_{l});events; #Delta#Phi(#mu,b_{l})", 40, 0., 4.);

	histMan->addH1D("deltaPhi_nu_leptonicB", "#Delta#Phi(#nu,b_{l});events; #Delta#Phi(#nu,b_{l})", 40, 0., 4.);
	histMan->addH1D("deltaPhi_nu_leptonicB", "#Delta#Phi(#nu,b_{l});events; #Delta#Phi(#nu,b_{l})", 40, 0., 4.);

	histMan->addH1D("deltaPhi_W_leptonicB", "#Delta#Phi(W,b_{l});events; #Delta#Phi(W,b_{l})", 40, 0., 4.);
	histMan->addH1D("deltaPhi_W_leptonicB", "#Delta#Phi(W,b_{l});events; #Delta#Phi(W,b_{l})", 40, 0., 4.);
}

} /* namespace BAT */
