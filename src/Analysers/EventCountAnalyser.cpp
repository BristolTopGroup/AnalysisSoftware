/*
 * EventCountAnalyser.cpp
 *
 *  Created on: 24 Mar 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/EventCountAnalyser.h"
#include "../../interface/BTagWeight.h"

namespace BAT {

void EventCountAnalyser::analyse(const EventPtr event) {

	//	individualCuts(event);
	topMuPlusJetsReferenceSelectionUnweighted(event);
	topEPlusJetsReferenceSelectionUnweighted(event);

	topMuPlusJetsReferenceSelection(event);
	topEPlusJetsReferenceSelection(event);

//	topMuPlusJetsReferenceSelection2011(event);
//	topEplusJetsPlusMETSelection(event);
//	topEplusJetsZprimeSelection(event);
//	qcdSelections(event);
//	qcdMuPlusJetsSelections(event);
//	qcdAsymJetsSelections(event);
//	qcdAsymJetsMETSelections(event);
//	qcdNonIsoTriggerSelections(event);
//	qcdNonIsoTriggerAsymJetsSelections(event);
//	qcdNonIsoTriggerAsymJetsMETSelections(event);
}

void EventCountAnalyser::topEPlusJetsReferenceSelection(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	//use bjet weights in histograms for electrons
	const JetCollection jetsEle(topEplusJetsRefSelection_->cleanedJets(event));
	const JetCollection bJetsEle(topEplusJetsRefSelection_->cleanedBJets(event));
	unsigned int numberOfBjetsEle(bJetsEle.size());
	vector<double> bjetWeightsEle;
	if (event->isRealData()) {
		for (unsigned int index = 0; index <= numberOfBjetsEle; ++index) {
			if (index == numberOfBjetsEle)
				bjetWeightsEle.push_back(1.);
			else
				bjetWeightsEle.push_back(0);
		}
	} else
		bjetWeightsEle = BjetWeights(jetsEle, numberOfBjetsEle);

	double weight_ge1b = 0.;
	double weight_ge2b = 0.;
	for (unsigned int weightIndex = 0; weightIndex < bjetWeightsEle.size(); ++weightIndex) {
	double b_weight = bjetWeightsEle.at(weightIndex);

	if(weightIndex>0)
	weight_ge1b += b_weight;
	if(weightIndex>1)
	weight_ge2b += b_weight;

	}

	weight_ = event->weight() * prescale_ * scale_;
	//fill all events bin
	histMan_->H1D("TTbarEplusJetsRefSelection")->Fill(-1, weight_);
	histMan_->H1D("TTbarEplusJetsRefSelection_singleCuts")->Fill(-1, weight_);

	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-2; ++step) {
		bool passesStep = topEplusJetsRefSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = topEplusJetsRefSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarEplusJetsRefSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarEplusJetsRefSelection_singleCuts")->Fill(step, weight_);
	}


	if (topEplusJetsRefSelection_->passesSelectionUpToStep(event, TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-2))
		histMan_->H1D("TTbarEplusJetsRefSelection")->Fill(TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-2, weight_ *weight_ge1b);
	if (topEplusJetsRefSelection_->passesSelectionStep(event, TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-2))
		histMan_->H1D("TTbarEplusJetsRefSelection_singleCuts")->Fill(TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-2, weight_ *weight_ge1b);

	if (topEplusJetsRefSelection_->passesSelectionUpToStep(event, TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-1))
		histMan_->H1D("TTbarEplusJetsRefSelection")->Fill(TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-1, weight_ *weight_ge2b);
	if (topEplusJetsRefSelection_->passesSelectionStep(event, TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-1))
		histMan_->H1D("TTbarEplusJetsRefSelection_singleCuts")->Fill(TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-1, weight_ *weight_ge2b);



}

void EventCountAnalyser::topEPlusJetsReferenceSelectionUnweighted(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = 1.;
	//fill all events bin
	histMan_->H1D("TTbarEplusJetsRefSelectionUnweighted")->Fill(-1, weight_);
	histMan_->H1D("TTbarEplusJetsRefSelectionUnweighted_singleCuts")->Fill(-1, weight_);

	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = topEplusJetsRefSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = topEplusJetsRefSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarEplusJetsRefSelectionUnweighted")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarEplusJetsRefSelectionUnweighted_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::individualCuts(const EventPtr event) {

	if(topMuPlusJetsRefSelection_->passesSelectionUpToStep(event, 1)) {
				cout << "run: " << event->runnumber() << " lumi: " << event->lumiblock() << " evt: " << event->eventnumber() << endl;
/*				if(event->isBeamScraping() || !event->passesHBHENoiseFilter() || !event->passesCSCTightBeamHaloFilter() || !event->passesHCALLaserFilter()
						|| !event->passesECALDeadCellFilter() || !event->passesTrackingFailureFilter() || !event->passesEEBadSCFilter()){

							if(event->isBeamScraping())
								cout << "fail beam scrap" << endl;
							if(!event->passesHBHENoiseFilter())
								cout << "fail HBHE noise filter" << endl;
							if(!event->passesCSCTightBeamHaloFilter())
								cout << "fail CSCTightBeamHaloFilter" << endl;
							if(!event->passesHCALLaserFilter())
								cout << "fail HCALLaserFilter" << endl;
							if(!event->passesECALDeadCellTPFilter())
								cout << "fail ECALDeadCellTPFilter" << endl;
							if(!event->passesTrackingFailureFilter())
								cout << "fail TrackingFailureFilter" << endl;
							if(!event->passesEEBadSCFilter())
								cout << "fail EEBadSCFilter()" << endl;
							if(!event->passesECALLaserCorrFilter())
								cout << "fail ECALLaserCorrFilter()" << endl;
							if(!event->passesTrackingPOGFilters())
								cout << "fail TrackingPOGFilters()" << endl;
				}*/
	}

}

void EventCountAnalyser::topMuPlusJetsReferenceSelection(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	if (topMuPlusJetsRefSelection_->passesSelectionUpToStep(event, 1)) {
	const LeptonPointer signalLepton = topMuPlusJetsRefSelection_->signalLepton(event);
	const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
	double efficiencyCorrection = event->isRealData() ? 1. : signalMuon->getEfficiencyCorrection(false);
	 	scale_ = efficiencyCorrection;
	}else{
		scale_ =1;
	}

	//use bjet weights in histograms for muons
	const JetCollection jets(topMuPlusJetsRefSelection_->cleanedJets(event));
	const JetCollection bJets(topMuPlusJetsRefSelection_->cleanedBJets(event));
			unsigned int numberOfBjets(bJets.size());
	vector<double> bjetWeights;
	if (event->isRealData()) {
		for (unsigned int index = 0; index <= numberOfBjets; ++index) {
			if (index == numberOfBjets)
				bjetWeights.push_back(1.);
			else
				bjetWeights.push_back(0);
		}
	} else
		bjetWeights = BjetWeights(jets, numberOfBjets);

	double weight_ge1b = 0.;
	double weight_ge2b = 0.;
	for (unsigned int weightIndex = 0; weightIndex < bjetWeights.size(); ++weightIndex) {
		double b_weight = bjetWeights.at(weightIndex);

	if(weightIndex>0)
	weight_ge1b += b_weight;
	if(weightIndex>1)
	weight_ge2b += b_weight;

	}

	weight_ = event->weight() * prescale_ * scale_;

	histMan_->H1D("TTbarMuPlusJetsRefSelection")->Fill(-1, weight_);
	histMan_->H1D("TTbarMuPlusJetsRefSelection_singleCuts")->Fill(-1, weight_);

	for (unsigned int step = 0; step < TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-2; ++step) {
		bool passesStep = topMuPlusJetsRefSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = topMuPlusJetsRefSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarMuPlusJetsRefSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarMuPlusJetsRefSelection_singleCuts")->Fill(step, weight_);
	}

	if (topMuPlusJetsRefSelection_->passesSelectionUpToStep(event, TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-2))
		histMan_->H1D("TTbarMuPlusJetsRefSelection")->Fill(TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-2, weight_ *weight_ge1b);
	if (topMuPlusJetsRefSelection_->passesSelectionStep(event, TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-2))
		histMan_->H1D("TTbarMuPlusJetsRefSelection_singleCuts")->Fill(TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-2, weight_ *weight_ge1b);

	if (topMuPlusJetsRefSelection_->passesSelectionUpToStep(event, TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-1))
		histMan_->H1D("TTbarMuPlusJetsRefSelection")->Fill(TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-1, weight_ *weight_ge2b);
	if (topMuPlusJetsRefSelection_->passesSelectionStep(event, TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-1))
		histMan_->H1D("TTbarMuPlusJetsRefSelection_singleCuts")->Fill(TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS-1, weight_ * prescale_ *weight_ge2b);

}

void EventCountAnalyser::topMuPlusJetsReferenceSelectionUnweighted(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = 1.;
	histMan_->H1D("TTbarMuPlusJetsRefSelectionUnweighted")->Fill(-1, weight_);
	histMan_->H1D("TTbarMuPlusJetsRefSelectionUnweighted_singleCuts")->Fill(-1, weight_);

	for (unsigned int step = 0; step < TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = topMuPlusJetsRefSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = topMuPlusJetsRefSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarMuPlusJetsRefSelectionUnweighted")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarMuPlusJetsRefSelectionUnweighted_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::topMuPlusJetsReferenceSelection2011(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	histMan_->H1D("TTbarMuPlusJetsRefSelection2011")->Fill(-1, weight_);
	histMan_->H1D("TTbarMuPlusJetsRefSelection2011_singleCuts")->Fill(-1, weight_);

	for (unsigned int step = 0; step < TTbarMuPlusJetsReferenceSelection2011::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = topMuPlusJetsRefSelection2011_->passesSelectionStep(event, step);
		bool passesStepUpTo = topMuPlusJetsRefSelection2011_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarMuPlusJetsRefSelection2011")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarMuPlusJetsRefSelection2011_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::topEplusJetsPlusMETSelection(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	histMan_->H1D("TTbarEplusJetsRefAsymJetsSelection")->Fill(-1, weight_);
	histMan_->H1D("TTbarEplusJetsRefAsymJetsSelection_singleCuts")->Fill(-1, weight_);
	for (unsigned int step = 0; step < TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = topEplusAsymJetsSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = topEplusAsymJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarEplusJetsRefAsymJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarEplusJetsRefAsymJetsSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::topEplusJetsZprimeSelection(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	histMan_->H1D("TTbarEplusJetsRefAsymJetsMETSelection")->Fill(-1, weight_);
	histMan_->H1D("TTbarEplusJetsRefAsymJetsMETSelection_singleCuts")->Fill(-1, weight_);
	for (unsigned int step = 0; step < TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = topEplusAsymJetsMETSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = topEplusAsymJetsMETSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("TTbarEplusJetsRefAsymJetsMETSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("TTbarEplusJetsRefAsymJetsMETSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::qcdSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	histMan_->H1D("QCDNonIsoEplusJetsSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDNonIsoEplusJetsSelection_singleCuts")->Fill(-1, weight_);
	histMan_->H1D("QCDConversionSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDConversionSelection_singleCuts")->Fill(-1, weight_);
	histMan_->H1D("QCDPFRelIsoEPlusJetsSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDPFRelIsoEPlusJetsSelection_singleCuts")->Fill(-1, weight_);
	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsSelection_singleCuts")->Fill(step, weight_);

		passesStep = qcdConversionSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdConversionSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDConversionSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDConversionSelection_singleCuts")->Fill(step, weight_);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoEPlusJetsSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoEPlusJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::qcdMuPlusJetsSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	histMan_->H1D("QCDNonIsoMuPlusJetsSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDNonIsoMuPlusJetsSelection_singleCuts")->Fill(-1, weight_);
	histMan_->H1D("QCDPFRelIsoMuPlusJetsSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDPFRelIsoMuPlusJetsSelection_singleCuts")->Fill(-1, weight_);
	for (unsigned int step = 0; step < TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoMuonSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoMuonSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoMuPlusJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDNonIsoMuPlusJetsSelection_singleCuts")->Fill(step, weight_);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoMuPlusJetsSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoMuPlusJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoMuPlusJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoMuPlusJetsSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::qcdAsymJetsSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	histMan_->H1D("QCDNonIsoEplusJetsAsymJetsSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDNonIsoEplusJetsAsymJetsSelection_singleCuts")->Fill(-1, weight_);
	histMan_->H1D("QCDConversionAsymJetsSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDConversionAsymJetsSelection_singleCuts")->Fill(-1, weight_);
	histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsSelection_singleCuts")->Fill(-1, weight_);
	for (unsigned int step = 0; step < TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronAsymJetsSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronAsymJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsAsymJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsAsymJetsSelection_singleCuts")->Fill(step, weight_);

		passesStep = qcdConversionAsymJetsSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdConversionAsymJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDConversionAsymJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDConversionAsymJetsSelection_singleCuts")->Fill(step, weight_);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoAsymJetsSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoAsymJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::qcdAsymJetsMETSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	histMan_->H1D("QCDNonIsoEplusJetsAsymJetsMETSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDNonIsoEplusJetsAsymJetsMETSelection_singleCuts")->Fill(-1, weight_);
	histMan_->H1D("QCDConversionAsymJetsMETSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDConversionAsymJetsMETSelection_singleCuts")->Fill(-1, weight_);
	histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsMETSelection")->Fill(-1, weight_);
	histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsMETSelection_singleCuts")->Fill(-1, weight_);
	for (unsigned int step = 0; step < TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronAsymJetsMETSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronAsymJetsMETSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsAsymJetsMETSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsAsymJetsMETSelection_singleCuts")->Fill(step, weight_);

		passesStep = qcdConversionAsymJetsMETSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdConversionAsymJetsMETSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDConversionAsymJetsMETSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDConversionAsymJetsMETSelection_singleCuts")->Fill(step, weight_);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoAsymJetsMETSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoAsymJetsMETSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsMETSelection")->Fill(step, weight_);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsAsymJetsMETSelection_singleCuts")->Fill(step, weight_);
	}
}

void EventCountAnalyser::qcdNonIsoTriggerSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	unsigned int prescale(qcdNonIsoElectronNonIsoTriggerSelection_->prescale(event));

	histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerSelection")->Fill(-1, weight_ * prescale);
	histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerSelection_singleCuts")->Fill(-1, weight_ * prescale);
	histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerSelection")->Fill(-1, weight_ * prescale);
	histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerSelection_singleCuts")->Fill(-1, weight_ * prescale);

	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronNonIsoTriggerSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronNonIsoTriggerSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerSelection_singleCuts")->Fill(step, weight_ * prescale);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoNonIsoTriggerSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoNonIsoTriggerSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerSelection_singleCuts")->Fill(step, weight_ * prescale);
	}
}

void EventCountAnalyser::qcdNonIsoTriggerAsymJetsSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	unsigned int prescale(qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_->prescale(event));
	histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsSelection")->Fill(-1, weight_ * prescale);
	histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsSelection_singleCuts")->Fill(-1, weight_ * prescale);
	histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsSelection")->Fill(-1, weight_ * prescale);
	histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsSelection_singleCuts")->Fill(-1, weight_ * prescale);

	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsSelection_singleCuts")->Fill(step,
					weight_ * prescale);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsSelection_singleCuts")->Fill(step,
					weight_ * prescale);
	}
}

void EventCountAnalyser::qcdNonIsoTriggerAsymJetsMETSelections(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	unsigned int prescale(qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_->prescale(event));
	histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsMETSelection")->Fill(-1, weight_ * prescale);
	histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsMETSelection_singleCuts")->Fill(-1, weight_ * prescale);
	histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsMETSelection")->Fill(-1, weight_ * prescale);
	histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsMETSelection_singleCuts")->Fill(-1, weight_ * prescale);

	for (unsigned int step = 0; step < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++step) {
		bool passesStep = qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_->passesSelectionStep(event, step);
		bool passesStepUpTo = qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsMETSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDNonIsoEplusJetsNonIsoTriggerAsymJetsMETSelection_singleCuts")->Fill(step,
					weight_ * prescale);

		//selection for particle based relative isolation distribution
		passesStep = qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->passesSelectionStep(event, step);
		passesStepUpTo = qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->passesSelectionUpToStep(event, step);
		if (passesStepUpTo)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsMETSelection")->Fill(step, weight_ * prescale);
		if (passesStep)
			histMan_->H1D("QCDPFRelIsoEPlusJetsNonIsoTriggerAsymJetsMETSelection_singleCuts")->Fill(step,
					weight_ * prescale);
	}
}

void EventCountAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	const boost::array<string, 6> refSelections = { { "TTbarEplusJetsRef", "QCDNonIsoEplusJets",
			"QCDNonIsoEplusJetsNonIsoTrigger", "QCDConversion", "QCDPFRelIsoEPlusJets",
			"QCDPFRelIsoEPlusJetsNonIsoTrigger" } };

	for (unsigned int index = 0; index < refSelections.size(); ++index) {
		string selection = refSelections.at(index) + "Selection";
		histMan_->addH1D(selection, selection, TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "_singleCuts", selection + " (single cuts",
				TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

		histMan_->addH1D(selection + "Unweighted", selection, TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "Unweighted_singleCuts", selection + " (single cuts",
				TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

		selection = refSelections.at(index) + "AsymJetsSelection";

		histMan_->addH1D(selection, selection, TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "_singleCuts", selection + " (single cuts",
				TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarEPlusJetsRefAsymJetsSelection::NUMBER_OF_SELECTION_STEPS - 0.5);

		selection = refSelections.at(index) + "AsymJetsMETSelection";
		histMan_->addH1D(selection, selection, TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS + 1,
				-1.5, TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "_singleCuts", selection + " (single cuts",
				TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarEPlusJetsRefAsymJetsMETSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
	}
	//muons
	const boost::array<string, 6> refSelections_muons = { { "TTbarMuPlusJetsRef", "QCDNonIsoMuPlusJets",
			"QCDPFRelIsoMuPlusJets" } };
	for (unsigned int index = 0; index < refSelections_muons.size(); ++index) {
		string selection = refSelections_muons.at(index) + "Selection";
		histMan_->addH1D(selection, selection, TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "_singleCuts", selection + " (single cuts",
				TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "Unweighted", selection, TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "Unweighted_singleCuts", selection + " (single cuts",
				TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarMuPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS - 0.5);
	}
	const boost::array<string, 6> refSelections_muons2011 = { {"TTbarMuPlusJetsRef"} };
	for (unsigned int index = 0; index < refSelections_muons2011.size(); ++index) {
		string selection = refSelections_muons2011.at(index) + "Selection2011";
		histMan_->addH1D(selection, selection, TTbarMuPlusJetsReferenceSelection2011::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarMuPlusJetsReferenceSelection2011::NUMBER_OF_SELECTION_STEPS - 0.5);
		histMan_->addH1D(selection + "_singleCuts", selection + " (single cuts",
				TTbarMuPlusJetsReferenceSelection2011::NUMBER_OF_SELECTION_STEPS + 1, -1.5,
				TTbarMuPlusJetsReferenceSelection2011::NUMBER_OF_SELECTION_STEPS - 0.5);
	}
}

void EventCountAnalyser::setHistogramLabels() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	for (unsigned int index = 0; index < TTbarEPlusJetsReferenceSelection::NUMBER_OF_SELECTION_STEPS; ++index) {
		unsigned int bin(index + 1);
		string binLabel = TTbarEPlusJetsReferenceSelection::StringSteps[index];

		histMan_->H1D("TTbarEplusJetsSelection")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("TTbarEplusJetsSelection_singleCuts")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDNonIsoEplusJetsSelection")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDNonIsoEplusJetsSelection_singleCuts")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDConversionSelection")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDConversionSelection_singleCuts")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDPFRelIsoEPlusJetsSelection")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
		histMan_->H1D("QCDPFRelIsoEPlusJetsSelection_singleCuts")->GetXaxis()->SetBinLabel(bin, binLabel.c_str());
	}
}

EventCountAnalyser::EventCountAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		//signal selections
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		topEplusAsymJetsSelection_(new TopPairEplusJetsRefAsymJetsSelection()), //
		topEplusAsymJetsMETSelection_(new TopPairEPlusJetsRefAsymJetsMETSelection()), //
		topMuPlusJetsRefSelection_(new TopPairMuPlusJetsReferenceSelection()), //
		topMuPlusJetsRefSelection2011_(new TopPairMuPlusJetsReferenceSelection2011()), //
		//QCD selections with respect to reference selection
		qcdNonIsoElectronSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdNonIsoElectronNonIsoTriggerSelection_(new QCDNonIsolatedElectronSelection()), //
		qcdNonIsoMuonSelection_(new QCDNonIsolatedMuonSelection()), //
		qcdConversionSelection_(new QCDConversionsSelection()), //
		qcdPFRelIsoEPlusJetsSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		qcdPFRelIsoNonIsoTriggerSelection_(new QCDPFRelIsoEPlusJetsSelection()), //
		qcdPFRelIsoMuPlusJetsSelection_(new QCDPFRelIsoMuPlusJetsSelection()), //
		//QCD selections with respect to reference selection + asymmetric jet cuts
		qcdNonIsoElectronAsymJetsSelection_(new QCDNonIsolatedElectronAsymJetsSelection()), //
		qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_(new QCDNonIsolatedElectronAsymJetsSelection()), //
		qcdConversionAsymJetsSelection_(new QCDConversionsAsymJetsSelection()), //
		qcdPFRelIsoAsymJetsSelection_(new QCDPFRelIsoEPlusAsymJetsSelection()), //
		qcdPFRelIsoNonIsoTriggerAsymJetsSelection_(new QCDPFRelIsoEPlusAsymJetsSelection()), //
		//QCD selections with respect to reference selection + MET + asymmetric jet cuts
		qcdNonIsoElectronAsymJetsMETSelection_(new QCDNonIsolatedElectronAsymJetsMETSelection()), //
		qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_(new QCDNonIsolatedElectronAsymJetsMETSelection()), //
		qcdConversionAsymJetsMETSelection_(new QCDConversionsAsymJetsMETSelection()), //
		qcdPFRelIsoAsymJetsMETSelection_(new QCDPFRelIsoEPlusAsymJetsMET()), //
		qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_(new QCDPFRelIsoEPlusAsymJetsMET()) {
	qcdNonIsoElectronNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdNonIsoElectronAsymJetsNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdNonIsoElectronAsymJetsMETNonIsoTriggerSelection_->useNonIsoTrigger(true);

	qcdPFRelIsoNonIsoTriggerSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerAsymJetsSelection_->useNonIsoTrigger(true);
	qcdPFRelIsoNonIsoTriggerAsymJetsMETSelection_->useNonIsoTrigger(true);
}

EventCountAnalyser::~EventCountAnalyser() {
}

} /* namespace BAT */
