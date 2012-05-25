/*
 * NeutrinoReconstructionAnalyser.cpp
 *
 *  Created on: Apr 2, 2012
 *      Author: lkreczko
 */

#include "../../interface/Analysers/NeutrinoReconstructionAnalyser.h"
#include <iostream>
#include "../../interface/ReconstructionModules/BasicNeutrinoReconstruction.h"
#include "../../interface/ReconstructionModules/NeutrinoReconstructionByApproximation.h"

using namespace std;

namespace BAT {

void NeutrinoReconstructionAnalyser::analyse(const EventPtr event) {
	if (event->getDataType() != DataType::TTJets)
		return;

	if (!truthMatchEvent.get()) {
		cerr << "NeutrinoReconstructionAnalyser::analyse >> MC Truth Event is not set!" << endl;
		throw "NeutrinoReconstructionAnalyser::analyse >> MC Truth Event is not set!";
	}

	//TODO: accept mu+jets events as well
	if (truthMatchEvent->decayChannel != Decay::electronPlusJets)
		return;

	histMan_->setCurrentHistogramFolder(histogramFolder_);
	double weight(event->weight());

	const ParticlePointer neutrino = truthMatchEvent->neutrinoFromW;
	const METPointer met = event->MET();
	const LeptonPointer lepton = truthMatchEvent->leptonFromW;

	histMan_->H1D("Neutrino_px")->Fill(neutrino->px(), weight);
	histMan_->H1D("Neutrino_py")->Fill(neutrino->py(), weight);
	histMan_->H1D("Neutrino_pz")->Fill(neutrino->pz(), weight);

	histMan_->H1D("Neutrino_phi")->Fill(neutrino->phi(), weight);
	histMan_->H1D("Neutrino_phi_resolution")->Fill(neutrino->phi() - met->phi(), weight);

	histMan_->H2D("Neutrino_px_vs_MET_px")->Fill(met->px(), neutrino->px(), weight);
	histMan_->H2D("Neutrino_py_vs_MET_py")->Fill(met->py(), neutrino->py(), weight);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/BasicReco");
	BasicNeutrinoReconstruction basicReco(lepton, met);
	boost::array<ParticlePointer, 2> basicNeutrinos = basicReco.getNeutrinos(0);
	double resolution = (neutrino->pz() - basicNeutrinos.at(0)->pz()) / neutrino->pz();
	histMan_->H1D("Neutrino_pz_resolution")->Fill(resolution, weight);
	resolution = (neutrino->pz() - basicNeutrinos.at(1)->pz()) / neutrino->pz();
	histMan_->H1D("Neutrino_pz_resolution")->Fill(resolution, weight);
	histMan_->H2D("Neutrino_pz_vs_MET_pz")->Fill(basicNeutrinos.at(0)->pz(), neutrino->pz(), weight);
	histMan_->H2D("Neutrino_pz_vs_MET_pz")->Fill(basicNeutrinos.at(1)->pz(), neutrino->pz(), weight);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/ScalingMETApproximation");
	NeutrinoReconstructionByApproximation approxReco(lepton, met);
	ParticlePointer approxNeutrino = approxReco.scalingMETApproximation();

	resolution = (neutrino->pz() - approxNeutrino->pz()) / neutrino->pz();

	histMan_->H1D("Neutrino_pz_resolution")->Fill(resolution, weight);
	histMan_->H2D("Neutrino_pz_vs_MET_pz")->Fill(approxNeutrino->pz(), neutrino->pz(), weight);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/SameEtaApproximation");
	approxNeutrino = approxReco.sameEtaApproximation();
	resolution = (neutrino->pz() - approxNeutrino->pz()) / neutrino->pz();

	histMan_->H1D("Neutrino_pz_resolution")->Fill(resolution, weight);
	histMan_->H2D("Neutrino_pz_vs_MET_pz")->Fill(approxNeutrino->pz(), neutrino->pz(), weight);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/ColinearApproximation");
	approxNeutrino = approxReco.colinearApproximation();
	resolution = (neutrino->pz() - approxNeutrino->pz()) / neutrino->pz();

	histMan_->H1D("Neutrino_pz_resolution")->Fill(resolution, weight);
	histMan_->H2D("Neutrino_pz_vs_MET_pz")->Fill(approxNeutrino->pz(), neutrino->pz(), weight);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/NullDeltaApproximation");
	approxNeutrino = approxReco.nullDeltaApproximation();
	resolution = (neutrino->pz() - approxNeutrino->pz()) / neutrino->pz();

	histMan_->H1D("Neutrino_pz_resolution")->Fill(resolution, weight);
	histMan_->H2D("Neutrino_pz_vs_MET_pz")->Fill(approxNeutrino->pz(), neutrino->pz(), weight);
}

void NeutrinoReconstructionAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	histMan_->addH1D("Neutrino_px", "Neutrino_px", 1000, -500., 500);
	histMan_->addH1D("Neutrino_py", "Neutrino_py", 1000, -500., 500);
	histMan_->addH1D("Neutrino_pz", "Neutrino_pz", 1000, -500., 500);

	histMan_->addH1D("Neutrino_phi", "Neutrino_phi", 800, -4., 4);
	histMan_->addH1D("Neutrino_phi_resolution", "Neutrino_phi resolution; #phi_{#nu} - #phi_{MET}; events/0.01", 800,
			-4., 4);

	histMan_->addH2D("Neutrino_px_vs_MET_px", "p_{x}(#nu) vs p_{x}(MET);p_{x}(MET) (GeV); p_{x}(#nu) (GeV)", 100, 0.,
			500, 500, 0., 500);
	histMan_->addH2D("Neutrino_py_vs_MET_py", "p_{y}(#nu) vs p_{y}(MET);p_{y}(MET) (GeV); p_{y}(#nu) (GeV)", 100, 0.,
			500, 500, 0., 500);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/BasicReco");
	histMan_->addH1D("Neutrino_pz_resolution", "Neutrino p_{z}; (p_{z}(#nu) - p_{z}(reco))/p_{z}(#nu); events/0.1", 200, -10, 10);
	histMan_->addH2D("Neutrino_pz_vs_MET_pz", "p_{z}(#nu) vs p_{z}(reco #nu);p_{z}(reco #nu) (GeV); p_{z}(#nu) (GeV)",
			500, 0., 500, 500, 0., 500);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/ScalingMETApproximation");
	histMan_->addH1D("Neutrino_pz_resolution", "Neutrino p_{z}; (p_{z}(#nu) - p_{z}(reco))/p_{z}(#nu); events/0.1", 200, -10, 10);
	histMan_->addH2D("Neutrino_pz_vs_MET_pz", "p_{z}(#nu) vs p_{z}(reco #nu);p_{z}(reco #nu) (GeV); p_{z}(#nu) (GeV)",
			500, 0., 500, 500, 0., 500);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/SameEtaApproximation");
	histMan_->addH1D("Neutrino_pz_resolution", "Neutrino p_{z}; (p_{z}(#nu) - p_{z}(reco))/p_{z}(#nu); events/0.1", 200, -10, 10);
	histMan_->addH2D("Neutrino_pz_vs_MET_pz", "p_{z}(#nu) vs p_{z}(reco #nu);p_{z}(reco #nu) (GeV); p_{z}(#nu) (GeV)",
			500, 0., 500, 500, 0., 500);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/ColinearApproximation");
	histMan_->addH1D("Neutrino_pz_resolution", "Neutrino p_{z}; (p_{z}(#nu) - p_{z}(reco))/p_{z}(#nu); events/0.1", 200, -10, 10);
	histMan_->addH2D("Neutrino_pz_vs_MET_pz", "p_{z}(#nu) vs p_{z}(reco #nu);p_{z}(reco #nu) (GeV); p_{z}(#nu) (GeV)",
			500, 0., 500, 500, 0., 500);

	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/NullDeltaApproximation");
	histMan_->addH1D("Neutrino_pz_resolution", "Neutrino p_{z}; (p_{z}(#nu) - p_{z}(reco))/p_{z}(#nu); events/0.1", 200, -10, 10);
	histMan_->addH2D("Neutrino_pz_vs_MET_pz", "p_{z}(#nu) vs p_{z}(reco #nu);p_{z}(reco #nu) (GeV); p_{z}(#nu) (GeV)",
			500, 0., 500, 500, 0., 500);
}

void NeutrinoReconstructionAnalyser::setMCTTbarHypothesis(const TtbarHypothesisPointer mcEvent) {
	truthMatchEvent = mcEvent;
}

NeutrinoReconstructionAnalyser::NeutrinoReconstructionAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
	BasicAnalyser(histMan, histogramFolder) {

}

NeutrinoReconstructionAnalyser::~NeutrinoReconstructionAnalyser() {
}

}
