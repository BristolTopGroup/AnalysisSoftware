/*
 * ElectronAnalyser.cpp
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */
#include "../../interface/Analysers/ElectronAnalyser.h"
namespace BAT {

void ElectronAnalyser::analyse(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;
	const ElectronCollection electrons = event->Electrons();

	if (singleElectronOnly_)
		return;
	histMan_->H1D_BJetBinned("Number_Of_Electrons")->Fill(electrons.size(), weight_);
	for (unsigned int index = 0; index < electrons.size(); ++index) {
		const ElectronPointer electron(electrons.at(index));

		histMan_->H1D_BJetBinned("All_Electron_Pt")->Fill(electron->pt(), weight_);
		histMan_->H1D_BJetBinned("All_Electron_Eta")->Fill(electron->eta(), weight_);
		histMan_->H1D_BJetBinned("All_Electron_AbsEta")->Fill(fabs(electron->eta()), weight_);
		histMan_->H1D_BJetBinned("All_Electron_Phi")->Fill(electron->phi(), weight_);
		histMan_->H1D_BJetBinned("All_Electron_pfIsolation_03")->Fill(electron->pfRelativeIsolation(0.3), weight_);
		histMan_->H1D_BJetBinned("All_Electron_pfIsolation_04")->Fill(electron->pfRelativeIsolation(0.4), weight_);
		histMan_->H1D_BJetBinned("All_Electron_pfIsolation_05")->Fill(electron->pfRelativeIsolation(0.5), weight_);
		histMan_->H1D_BJetBinned("All_Electron_rhoCorrectedIso_03")->Fill(electron->pfRelativeIsolationRhoCorrected(), weight_);

		histMan_->H1D_BJetBinned("All_Electron_sigma_ietaieta")->Fill(electron->sigmaIEtaIEta(), weight_);
		histMan_->H1D_BJetBinned("All_Electron_dPhi_in")->Fill(electron->dPhiIn(), weight_);
		histMan_->H1D_BJetBinned("All_Electron_dEta_in")->Fill(electron->dEtaIn(), weight_);
		histMan_->H1D_BJetBinned("All_Electron_HadOverEM")->Fill(electron->HadOverEm(), weight_);
		histMan_->H1D_BJetBinned("All_Electron_mvaTrigV0")->Fill(electron->mvaTrigV0(), weight_);
		histMan_->H1D_BJetBinned("All_Electron_mvaNonTrigV0")->Fill(electron->mvaNonTrigV0(), weight_);
		histMan_->H1D_BJetBinned("All_Electron_dB")->Fill(electron->d0(), weight_);
	}
}

void ElectronAnalyser::analyseElectron(const ElectronPointer electron, double weight) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = weight * prescale_ * scale_;
	if (!singleElectronOnly_)
		return;
	histMan_->H1D_BJetBinned("electron_eta")->Fill(electron->eta(), weight_);
	histMan_->H1D_BJetBinned("electron_AbsEta")->Fill(fabs(electron->eta()), weight_);
	histMan_->H1D_BJetBinned("electron_pfIsolation_03")->Fill(electron->pfRelativeIsolation(0.3), weight_);

	if (!ttbarPlusMETAnalysisSetup_) {
		histMan_->H1D_BJetBinned("electron_pT")->Fill(electron->pt(), weight_);
		histMan_->H1D_BJetBinned("electron_phi")->Fill(electron->phi(), weight_);

		histMan_->H1D_BJetBinned("electron_pfIsolation_04")->Fill(electron->pfRelativeIsolation(0.4), weight_);
		histMan_->H1D_BJetBinned("electron_pfIsolation_05")->Fill(electron->pfRelativeIsolation(0.5), weight_);
		histMan_->H1D_BJetBinned("electron_rhoCorrectedIso_03")->Fill(electron->pfRelativeIsolationRhoCorrected(), weight_);

		histMan_->H1D_BJetBinned("electron_sigma_ietaieta")->Fill(electron->sigmaIEtaIEta(), weight_);
		histMan_->H1D_BJetBinned("electron_dPhi_in")->Fill(electron->dPhiIn(), weight_);
		histMan_->H1D_BJetBinned("electron_dEta_in")->Fill(electron->dEtaIn(), weight_);
		histMan_->H1D_BJetBinned("electron_HadOverEM")->Fill(electron->HadOverEm(), weight_);
		histMan_->H1D_BJetBinned("electron_mvaTrigV0")->Fill(electron->mvaTrigV0(), weight_);
		histMan_->H1D_BJetBinned("electron_mvaNonTrigV0")->Fill(electron->mvaNonTrigV0(), weight_);
		histMan_->H1D_BJetBinned("electron_dB")->Fill(electron->d0(), weight_);
	}
}

ElectronAnalyser::ElectronAnalyser(HistogramManagerPtr histMan, std::string histogramFolder, bool singleElectronOnly) :
		BasicAnalyser(histMan, histogramFolder), //
		singleElectronOnly_(singleElectronOnly), //
		ttbarPlusMETAnalysisSetup_(false) {

}

ElectronAnalyser::~ElectronAnalyser() {
}

void ElectronAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	if (!ttbarPlusMETAnalysisSetup_) {
		//analyse all electrons
		if (!singleElectronOnly_) {
			histMan_->addH1D_BJetBinned("Number_Of_Electrons", "Number of electrons;N(e);Events ", 6, -0.5, 5.5);
			histMan_->addH1D_BJetBinned("All_Electron_Pt", "Electron p_{T};p_{T}(e)/GeV;Events/2GeV ", 500, 0, 1000);
			histMan_->addH1D_BJetBinned("All_Electron_Eta", "Electron #eta; #eta(e); Events/(0.02)", 300, -3, 3);
			histMan_->addH1D_BJetBinned("All_Electron_AbsEta", "Electron |#eta|; |#eta(e)|; Events/(0.01)", 300, 0, 3);
			histMan_->addH1D_BJetBinned("All_Electron_Phi", "Electron #phi; #phi(e); Events/(0.02)", 400, -4, 4);
			histMan_->addH1D_BJetBinned("All_Electron_pfIsolation_03",
					"Electron relative pf isolation (DR=0.3); PF relative isolation; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D_BJetBinned("All_Electron_pfIsolation_04",
					"Electron relative pf isolation (DR=0.4); PF relative isolation; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D_BJetBinned("All_Electron_pfIsolation_05",
					"Electron relative pf isolation (DR=0.5); PF relative isolation; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D_BJetBinned("All_Electron_rhoCorrectedIso_03",
					"Electron rho-corrected isolation (DR=0.3); rho-corrected isolation; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D_BJetBinned("All_Electron_sigma_ietaieta",
					"Electron #sigma_{i#etai#eta}; #sigma_{i#etai#eta}; Events/(0.001)", 100, 0, 0.1);
			histMan_->addH1D_BJetBinned("All_Electron_dPhi_in", "Electron #Delta#Phi_{in}; #Delta#Phi_{in}; Events/(0.01)", 200,
					-1, 1);
			histMan_->addH1D_BJetBinned("All_Electron_dEta_in", "Electron #Delta#eta_{in}; #Delta#eta_{in}; Events/(0.001)", 1000,
					-0.5, 0.5);
			histMan_->addH1D_BJetBinned("All_Electron_HadOverEM", "Electron HadronicOverEM; HadOverEM; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D_BJetBinned("All_Electron_mvaTrigV0", "Electron mvaTrigV0; mvaTrigV0; Events/(0.01)", 240, -1.2, 1.2);
			histMan_->addH1D_BJetBinned("All_Electron_mvaNonTrigV0", "Electron mvaNonTrigV0; mvaNonTrigV0; Events/(0.01)", 240,
					-1.2, 1.2);
			histMan_->addH1D_BJetBinned("All_Electron_dB", "Electron dB(PV); dB/cm; Events/(0.001)", 200, 0, 0.2);
		} else {
			//single electron histograms for analyseElectron
			histMan_->addH1D_BJetBinned("electron_pT", "Electron p_{T}; p_{T} (GeV); Events/(1 GeV)", 1000, 0, 1000);
			histMan_->addH1D_BJetBinned("electron_phi", "Electron #phi; #phi(e); Events/(0.05)", 400, -4, 4);

			//b-jet binning useful for QCD estimates

			histMan_->addH1D_BJetBinned("electron_pfIsolation_04",
					"Electron relative pf isolation (DR=0.4); PF relative isolation; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D_BJetBinned("electron_pfIsolation_05",
					"Electron relative pf isolation (DR=0.5); PF relative isolation; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D_BJetBinned("electron_rhoCorrectedIso_03",
					"Electron rho-corrected isolation (DR=0.3); rho-corrected isolation; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D_BJetBinned("electron_sigma_ietaieta",
					"Electron #sigma_{i#etai#eta}; #sigma_{i#etai#eta}; Events/(0.001)", 100, 0, 0.1);
			histMan_->addH1D_BJetBinned("electron_dPhi_in", "Electron #Delta#Phi_{in}; #Delta#Phi_{in}; Events/(0.01)", 200, -1,
					1);
			histMan_->addH1D_BJetBinned("electron_dEta_in", "Electron #Delta#eta_{in}; #Delta#eta_{in}; Events/(0.001)", 1000,
					-0.5, 0.5);
			histMan_->addH1D_BJetBinned("electron_HadOverEM", "Electron HadronicOverEM; HadOverEM; Events/(0.01)", 500, 0, 5);
			histMan_->addH1D_BJetBinned("electron_mvaTrigV0", "Electron mvaTrigV0; mvaTrigV0; Events/(0.01)", 240, -1.2,
					1.2);
			histMan_->addH1D_BJetBinned("electron_mvaNonTrigV0", "Electron mvaNonTrigV0; mvaNonTrigV0; Events/(0.01)", 240, -1.2,
					1.2);
			histMan_->addH1D_BJetBinned("electron_dB", "Electron dB(PV); dB/cm; Events/(0.001 cm)", 200, 0, 0.2);
		}
	}
	if (singleElectronOnly_) {
		histMan_->addH1D_BJetBinned("electron_eta", "Electron #eta; #eta(e); Events/(0.02)", 300, -3, 3);
		histMan_->addH1D_BJetBinned("electron_AbsEta", "Electron |#eta|; |#eta(e)|; Events/(0.01)", 300, 0, 3);
		histMan_->addH1D_BJetBinned("electron_pfIsolation_03",
				"Electron relative pf isolation (DR=0.3); PF relative isolation; Events/(0.01)", 500, 0, 5);
	}

}

void ElectronAnalyser::useTTbarPlusMETSetup(bool use) {
	ttbarPlusMETAnalysisSetup_ = use;
}

}
