/*
 * DiffVariablesAnalyser.cpp
 *
 *  Created on: Oct 10, 2012
 *      Author: phzss
 */

#include "../../interface/Analysers/DiffVariablesAnalyser.h"
//signal selections
#include "../../interface/Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../../interface/Selections/TopPairMuPlusJetsReferenceSelection.h"

namespace BAT {

void DiffVariablesAnalyser::analyse(const EventPtr event) {

	ePlusJetsSignalAnalysis(event);
	muPlusJetsSignalAnalysis(event);

}

void DiffVariablesAnalyser::muPlusJetsSignalAnalysis(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/MuPlusJets/");
	weight_ = event->weight();
	if (topMuPlusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(topMuPlusJetsRefSelection_->cleanedJets(event));
		const JetCollection bJets(topMuPlusJetsRefSelection_->cleanedBJets(event));
		const LeptonPointer signalLepton = topMuPlusJetsRefSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		const METPointer met(event->MET());
		ParticlePointer leptonic_W;
		leptonic_W = ParticlePointer(new Particle(*met + *signalLepton));

		unsigned int numberOfJets(jets.size());
		unsigned int numberOfBJets(bJets.size());

		//HT calculation
		double HT = 0;
		double HT_lepton = 0;
		double HT_lepton_MET = 0;
		for (unsigned int i = 0; i < numberOfJets; ++i) {
			HT = HT + jets.at(i)->pt();
		}
		HT_lepton = HT+signalLepton->pt();
		HT_lepton_MET = HT_lepton + met->pt();


		//M3 calculation
		double M3 = 0;
		double max_pt = 0;
		if (jets.size() >= 3) {
				for (unsigned int index1 = 0; index1 < jets.size() - 2; ++index1) {
					for (unsigned int index2 = index1 + 1; index2 < jets.size() - 1; ++index2) {
						for (unsigned int index3 = index2 + 1; index3 < jets.size(); ++index3) {
							FourVector m3Vector(
									jets.at(index1)->getFourVector() + jets.at(index2)->getFourVector()
											+ jets.at(index3)->getFourVector());
							double currentPt = m3Vector.Pt();
							if (currentPt > max_pt) {
								max_pt = currentPt;
								M3 = m3Vector.M();
							}
						}
					}
				}
		}

		histMan_->H1D("HT")->Fill(HT, weight_);
		histMan_->H1D("HT_lepton")->Fill(HT_lepton, weight_);
		histMan_->H1D("HT_lepton_MET")->Fill(HT_lepton_MET, weight_);

		histMan_->H1D("MET_phi")->Fill(met->phi(), weight_);
		histMan_->H1D("MET_pt")->Fill(met->pt(), weight_);
		histMan_->H1D("MET_1st_jet_pt")->Fill(met->et()+jets.at(0)->pt(), weight_);
		histMan_->H1D("leptonic_W_pt")->Fill(leptonic_W->pt(), weight_);
		histMan_->H1D("leptons_invariant_mass")->Fill(signalLepton->invariantMass(met), weight_);
		histMan_->H1D("deltaR_lepton_MET")->Fill(leptonic_W->deltaR(met), weight_);
		histMan_->H1D("M3")->Fill(M3, weight_);
		histMan_->H1D("deltaPhi_lepton_2jets")->Fill(signalLepton->deltaPhi(jets.at(0))+signalLepton->deltaPhi(jets.at(1)), weight_);
		if (bJets.size()>0) {
			histMan_->H1D("invariant_mass_lepton_1bjet")->Fill(signalLepton->invariantMass(bJets.at(0)), weight_);
			double deltaR_lepton_bjet = 999.0;
			unsigned int closest_bjet_index = 0;
			for (unsigned int i = 0; i < numberOfBJets; ++i) {
				if (signalLepton->deltaR(bJets.at(i))<deltaR_lepton_bjet) {
					deltaR_lepton_bjet = signalLepton->deltaR(bJets.at(i));
					closest_bjet_index = i;
				}
			}
			histMan_->H1D("deltaPhi_lepton_closest_bjet")->Fill(signalLepton->deltaPhi(bJets.at(closest_bjet_index)));
		}
		if (bJets.size()>1) {
			histMan_->H1D("invariant_mass_lepton_2bjet")->Fill(signalLepton->invariantMass(bJets.at(1)), weight_);
			histMan_->H1D("invariant_mass_2bjets")->Fill(bJets.at(0)->invariantMass(bJets.at(1)), weight_);
			histMan_->H1D("deltaPhi_lepton_2bjets")->Fill(signalLepton->deltaPhi(bJets.at(0))+signalLepton->deltaPhi(bJets.at(1)), weight_);
			histMan_->H1D("deltaPhi_2bjets")->Fill(bJets.at(0)->deltaPhi(bJets.at(1)), weight_);
			histMan_->H1D("MET_bjets_pt")->Fill(met->et()+bJets.at(0)->pt()+bJets.at(1)->pt(), weight_);
			histMan_->H1D("deltaPhi_MET_2bjets")->Fill(met->deltaPhi(bJets.at(0))+met->deltaPhi(bJets.at(1)), weight_);
		}
		histMan_->H1D("deltaPhi_lepton_MET")->Fill(signalLepton->deltaPhi(met), weight_);
	}
}


void DiffVariablesAnalyser::ePlusJetsSignalAnalysis(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/EPlusJets/");
	weight_ = event->weight();
	if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		const JetCollection jets(topEplusJetsRefSelection_->cleanedJets(event));
		const JetCollection bJets(topEplusJetsRefSelection_->cleanedBJets(event));
		const LeptonPointer signalLepton = topEplusJetsRefSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
		const METPointer met(event->MET());
		ParticlePointer leptonic_W;
		leptonic_W = ParticlePointer(new Particle(*met + *signalLepton));

		unsigned int numberOfJets(jets.size());
		unsigned int numberOfBJets(bJets.size());

		//HT calculation
		double HT = 0;
		double HT_lepton = 0;
		double HT_lepton_MET = 0;
		for (unsigned int i = 0; i < numberOfJets; ++i) {
			HT = HT + jets.at(i)->pt();
		}
		HT_lepton = HT+signalLepton->pt();
		HT_lepton_MET = HT_lepton + met->pt();


		//M3 calculation
		double M3 = 0;
		double max_pt = 0;
		if (jets.size() >= 3) {
				for (unsigned int index1 = 0; index1 < jets.size() - 2; ++index1) {
					for (unsigned int index2 = index1 + 1; index2 < jets.size() - 1; ++index2) {
						for (unsigned int index3 = index2 + 1; index3 < jets.size(); ++index3) {
							FourVector m3Vector(
									jets.at(index1)->getFourVector() + jets.at(index2)->getFourVector()
											+ jets.at(index3)->getFourVector());
							double currentPt = m3Vector.Pt();
							if (currentPt > max_pt) {
								max_pt = currentPt;
								M3 = m3Vector.M();
							}
						}
					}
				}
		}

		histMan_->H1D("HT")->Fill(HT, weight_);
		histMan_->H1D("HT_lepton")->Fill(HT_lepton, weight_);
		histMan_->H1D("HT_lepton_MET")->Fill(HT_lepton_MET, weight_);

		histMan_->H1D("MET_phi")->Fill(met->phi(), weight_);
		histMan_->H1D("MET_pt")->Fill(met->pt(), weight_);
		histMan_->H1D("MET_1st_jet_pt")->Fill(met->et()+jets.at(0)->pt(), weight_);
		histMan_->H1D("leptonic_W_pt")->Fill(leptonic_W->pt(), weight_);
		histMan_->H1D("leptons_invariant_mass")->Fill(signalLepton->invariantMass(met), weight_);
		histMan_->H1D("deltaR_lepton_MET")->Fill(leptonic_W->deltaR(met), weight_);
		histMan_->H1D("M3")->Fill(M3, weight_);
		histMan_->H1D("deltaPhi_lepton_2jets")->Fill(signalLepton->deltaPhi(jets.at(0))+signalLepton->deltaPhi(jets.at(1)), weight_);
		if (bJets.size()>0) {
			histMan_->H1D("invariant_mass_lepton_1bjet")->Fill(signalLepton->invariantMass(bJets.at(0)), weight_);
			double deltaR_lepton_bjet = 999.0;
			unsigned int closest_bjet_index = 0;
			for (unsigned int i = 0; i < numberOfBJets; ++i) {
				if (signalLepton->deltaR(bJets.at(i))<deltaR_lepton_bjet) {
					deltaR_lepton_bjet = signalLepton->deltaR(bJets.at(i));
					closest_bjet_index = i;
				}
			}
			histMan_->H1D("deltaPhi_lepton_closest_bjet")->Fill(signalLepton->deltaPhi(bJets.at(closest_bjet_index)));
		}
		if (bJets.size()>1) {
			histMan_->H1D("invariant_mass_lepton_2bjet")->Fill(signalLepton->invariantMass(bJets.at(1)), weight_);
			histMan_->H1D("invariant_mass_2bjets")->Fill(bJets.at(0)->invariantMass(bJets.at(1)), weight_);
			histMan_->H1D("deltaPhi_lepton_2bjets")->Fill(signalLepton->deltaPhi(bJets.at(0))+signalLepton->deltaPhi(bJets.at(1)), weight_);
			histMan_->H1D("deltaPhi_2bjets")->Fill(bJets.at(0)->deltaPhi(bJets.at(1)), weight_);
			histMan_->H1D("MET_bjets_pt")->Fill(met->et()+bJets.at(0)->pt()+bJets.at(1)->pt(), weight_);
			histMan_->H1D("deltaPhi_MET_2bjets")->Fill(met->deltaPhi(bJets.at(0))+met->deltaPhi(bJets.at(1)), weight_);
		}
		histMan_->H1D("deltaPhi_lepton_MET")->Fill(signalLepton->deltaPhi(met), weight_);
	}
}

void DiffVariablesAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/EPlusJets/");

	float HT_bins[] = { 50, 150, 250, 350, 450, 650, 1100 };
	float HT_lepton_bins[] = { 100, 200, 300, 400, 500, 700, 1200 };
	float HT_lepton_MET_bins[] = { 150, 250, 350, 450, 550, 750, 1250 };
	histMan_->addH1D("HT", "HT (jets);HT (jets);Events", 6, HT_bins);
	histMan_->addH1D("HT_lepton", "HT (jets+lepton);HT (jets+lepton);Events", 6, HT_lepton_bins);
	histMan_->addH1D("HT_lepton_MET", "HT (jets+lepton+#slash{E}_{T});HT (jets+lepton+#slash{E}_{T});Events", 6, HT_lepton_MET_bins);

	histMan_->addH1D("MET_phi", "#phi(Missing transverse energy);#phi(#slash{E}_{T});Events", 80, -3, 3);
	histMan_->addH1D("MET_pt", "pt (Missing transverse energy);p_t(#slash{E}_{T});Events", 100, 0, 300);
	histMan_->addH1D("MET_1st_jet_pt", "#slash{E}_{T}+1st jet pt;#slash{E}_{T}+pt(1st jet);Events", 100, 0, 500);
	histMan_->addH1D("MET_bjets_pt", "#slash{E}_{T}+2 b-jets pt's;#slash{E}_{T}+bjets pts;Events", 100, 0, 600);

	histMan_->addH1D("leptonic_W_pt", "pt (leptonic W);p_t(leptonic W);Events", 100, 0, 350);
	histMan_->addH1D("leptons_invariant_mass", "Inv.mass (lepton, #slash{E}_{T});Inv.mass (lepton, #slash{E}_{T});Events", 100, 0, 350);
	histMan_->addH1D("invariant_mass_lepton_1bjet", "Inv.mass (lepton, 1 b-jet);Inv.mass (lepton,1b-jet)", 100, 0, 400);
	histMan_->addH1D("invariant_mass_lepton_2bjet", "Inv.mass (lepton, 2 b-jet);Inv.mass (lepton,2b-jet)", 100, 0, 400);
	histMan_->addH1D("invariant_mass_2bjets", "Inv.mass (1 b-jet, 2 b-jet);Inv.mass (2 b-jets)", 100, 0, 500);
	histMan_->addH1D("M3", "M3;M3;Events", 100, 100, 1000);
	histMan_->addH1D("deltaR_lepton_MET", "#Delta R (lepton, #slash{E}_{T});#Delta R (lepton, #slash{E}_{T});Events", 100, 0, 5);
	histMan_->addH1D("deltaPhi_lepton_2jets", "#Delta #phi (lepton, 1st jet) + #Delta #phi (lepton, 2nd jet);#Delta #phi (l, jet 1) + #Delta #phi(l, jet 2)", 100, -6, 6);
	histMan_->addH1D("deltaPhi_lepton_2bjets", "#Delta #phi (lepton, 1st b-jet) + #Delta #phi (lepton, 2nd b-jet);#Delta #phi (l, bjet 1) + #Delta #phi(l, bjet 2)", 100, -6, 6);
	histMan_->addH1D("deltaPhi_lepton_MET","#Delta #phi (lepton, #slash{E}_{T});#Delta #phi (l, #slash{E}_{T})", 100, -4, 4);
	histMan_->addH1D("deltaPhi_lepton_closest_bjet","#Delta #phi (lepton, closest b-jet);#Delta #phi (l, b-jet)", 100, -4, 4);
	histMan_->addH1D("deltaPhi_2bjets","#Delta #phi (1 b-jet, 2 b-jet);#Delta #phi (2 b-jets)", 100, -4, 4);
	histMan_->addH1D("deltaPhi_MET_2bjets", "#Delta #phi (#slash{E}_{T}, 1st b-jet) + #Delta #phi (#slash{E}_{T}, 2nd b-jet);#Delta #phi (#slash{E}_{T}, bjet 1) + #Delta #phi(#slash{E}_{T}, bjet 2)", 100, -6, 6);

	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/MuPlusJets/");

	histMan_->addH1D("HT", "HT (jets);HT (jets);Events", 6, HT_bins);
	histMan_->addH1D("HT_lepton", "HT (jets+lepton);HT (jets+lepton);Events", 6, HT_lepton_bins);
	histMan_->addH1D("HT_lepton_MET", "HT (jets+lepton+#slash{E}_{T});HT (jets+lepton+#slash{E}_{T});Events", 6, HT_lepton_MET_bins);

	histMan_->addH1D("MET_phi", "#phi(Missing transverse energy);#phi(#slash{E}_{T});Events", 80, -3, 3);
	histMan_->addH1D("MET_pt", "pt (Missing transverse energy);p_t(#slash{E}_{T});Events", 100, 0, 300);
	histMan_->addH1D("MET_1st_jet_pt", "#slash{E}_{T}+1st jet pt;#slash{E}_{T}+pt(1st jet);Events", 100, 0, 500);
	histMan_->addH1D("MET_bjets_pt", "#slash{E}_{T}+2 b-jets pt's;#slash{E}_{T}+bjets pts;Events", 100, 0, 600);

	histMan_->addH1D("leptonic_W_pt", "pt (leptonic W);p_t(leptonic W);Events", 100, 0, 350);
	histMan_->addH1D("leptons_invariant_mass", "Inv.mass (lepton, #slash{E}_{T});Inv.mass (lepton, #slash{E}_{T});Events", 100, 0, 350);
	histMan_->addH1D("invariant_mass_lepton_1bjet", "Inv.mass (lepton, 1 b-jet);Inv.mass (lepton,1b-jet)", 100, 0, 400);
	histMan_->addH1D("invariant_mass_lepton_2bjet", "Inv.mass (lepton, 2 b-jet);Inv.mass (lepton,2b-jet)", 100, 0, 400);
	histMan_->addH1D("invariant_mass_2bjets", "Inv.mass (1 b-jet, 2 b-jet);Inv.mass (2 b-jets)", 100, 0, 500);
	histMan_->addH1D("M3", "M3;M3;Events", 100, 100, 1000);
	histMan_->addH1D("deltaR_lepton_MET", "#Delta R (lepton, #slash{E}_{T});#Delta R (lepton, #slash{E}_{T});Events", 100, 0, 5);
	histMan_->addH1D("deltaPhi_lepton_2jets", "#Delta #phi (lepton, 1st jet) + #Delta #phi (lepton, 2nd jet);#Delta #phi (l, jet 1) + #Delta #phi(l, jet 2)", 100, -6, 6);
	histMan_->addH1D("deltaPhi_lepton_2bjets", "#Delta #phi (lepton, 1st b-jet) + #Delta #phi (lepton, 2nd b-jet);#Delta #phi (l, bjet 1) + #Delta #phi(l, bjet 2)", 100, -6, 6);
	histMan_->addH1D("deltaPhi_lepton_MET","#Delta #phi (lepton, #slash{E}_{T});#Delta #phi (l, #slash{E}_{T})", 100, -4, 4);
	histMan_->addH1D("deltaPhi_lepton_closest_bjet","#Delta #phi (lepton, closest b-jet);#Delta #phi (l, b-jet)", 100, -4, 4);
	histMan_->addH1D("deltaPhi_2bjets","#Delta #phi (1 b-jet, 2 b-jet);#Delta #phi (2 b-jets)", 100, -4, 4);
	histMan_->addH1D("deltaPhi_MET_2bjets", "#Delta #phi (#slash{E}_{T}, 1st b-jet) + #Delta #phi (#slash{E}_{T}, 2nd b-jet);#Delta #phi (#slash{E}_{T}, bjet 1) + #Delta #phi(#slash{E}_{T}, bjet 2)", 100, -6, 6);


}





DiffVariablesAnalyser::DiffVariablesAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		topMuPlusJetsRefSelection_(new TopPairMuPlusJetsReferenceSelection()) {

}

DiffVariablesAnalyser::~DiffVariablesAnalyser() {
}

} /* namespace BAT */
