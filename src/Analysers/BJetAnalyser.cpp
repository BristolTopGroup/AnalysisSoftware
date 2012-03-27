/*
 * BJetAnalyser.cpp
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */
#include "../../interface/Analysers/BJetAnalyser.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {

void BJetAnalyser::analyse(const EventPtr event) {
	double weight =  event->weight();
    histMan_->setCurrentHistogramFolder("BJetAnalysis");
    const JetCollection allJets =  event->Jets();

    for(unsigned int jetIndex = 0; jetIndex < allJets.size(); ++jetIndex){
    	const JetPointer jet(allJets.at(jetIndex));
    	double ssvHE_disc = jet->getBTagDiscriminator(BtagAlgorithm::SimpleSecondaryVertexHighEfficiency);
    	histMan_->H1D("ssv_HE_disc")->Fill(ssvHE_disc, weight);
    	if(abs(jet->partonFlavour()) == 5)
    		histMan_->H1D("ssv_HE_disc_bjets")->Fill(ssvHE_disc, weight);
    }
}


void BJetAnalyser::createHistograms() {
    histMan_->setCurrentHistogramFolder("BJetAnalysis");

    histMan_->addH1D("ssv_HE_disc", "ssv_HE_disc", 1000, -1., 10.);
    histMan_->addH1D("ssv_HE_disc_bjets", "ssv_HE_disc_bjets", 1000, -1., 10.);

}

BJetAnalyser::BJetAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
    BasicAnalyser(histMan, histogramFolder) {

}

BJetAnalyser::~BJetAnalyser() {
}
}
