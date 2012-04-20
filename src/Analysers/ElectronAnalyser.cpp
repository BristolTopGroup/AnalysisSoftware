/*
 * ElectronAnalyser.cpp
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */
#include "../../interface/Analysers/ElectronAnalyser.h"
#include "../../interface/GlobalVariables.h"
#include "../../interface/Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../../interface/TopPairEventCandidate.h"
namespace BAT {

void ElectronAnalyser::analyse(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_;
	const ElectronCollection electrons = event->Electrons();
	const ElectronCollection goodElectrons = event->GoodElectrons();
	ElectronCollection goodElectronsNoID;

	if (goodElectrons.size() > 0) {
		const ElectronPointer electron = event->MostPFIsolatedElectron(goodElectrons);
		histMan_->H1D("AllElectronEta")->Fill(electron->eta(), weight_);
		histMan_->H1D("AllElectronAbsEta")->Fill(abs(electron->eta()), weight_);
		histMan_->H1D("AllElectronPt")->Fill(electron->pt(), weight_);
		histMan_->H1D("AllElectronPFIsolation")->Fill(electron->pfIsolation(), weight_);
		histMan_->H1D("AllElectronRelIso")->Fill(electron->relativeIsolation(), weight_);
	}

	for (unsigned index = 0; index < electrons.size(); ++index) {
		const ElectronPointer electron = electrons.at(index);
		bool passesEt = electron->et() > 30;
		bool passesEta = fabs(electron->eta()) < 2.5 && !electron->isInCrack();

		// use d0 wrt primary vertex for
		bool passesD0 = fabs(electron->d0()) < 0.02; //cm

		bool passesDistanceToPV = fabs(electron->ZDistanceToPrimaryVertex()) < 1;

		if (passesEt && passesEta && passesD0 && passesDistanceToPV)
			goodElectronsNoID.push_back(electron);
	}

	for (unsigned int id = 0; id < CiCElectronID::NUMBER_OF_CiCIds; ++id) {
		for (unsigned int index = 0; index < goodElectronsNoID.size(); ++index) {
			const ElectronPointer electron = goodElectronsNoID.at(index);

			if (electron->CiC_ElectronID((CiCElectronID::value) id)) {
				histMan_->H1D("nEventsPassingCiCId")->Fill(id + 1, weight_);

				if (electron->isPFLepton() && electron->pfIsolation() < Globals::maxElectronPFIsolation)
					histMan_->H1D("nEventsPassingCiCIdIso")->Fill(id + 1, weight_);

				if (!electron->isPFLepton() && electron->relativeIsolation() < Globals::maxElectronRelativeIsolation)
					histMan_->H1D("nEventsPassingCiCIdIso")->Fill(id + 1, weight_);

			} else {
				histMan_->H1D("nEventsPassingCiCId")->Fill(0., weight_);
				if (electron->isPFLepton() && electron->pfIsolation() < Globals::maxElectronPFIsolation)
					histMan_->H1D("nEventsPassingCiCIdIso")->Fill(0., weight_);
				if (!electron->isPFLepton() && electron->relativeIsolation() < Globals::maxElectronRelativeIsolation)
					histMan_->H1D("nEventsPassingCiCIdIso")->Fill(0., weight_);
			}
		}

		if (id == CiCElectronID::eidHyperTight4MC) {
			unsigned int goodElectrons = 0;
			unsigned int goodIsoElectrons = 0;

			for (unsigned int index = 0; index < goodElectronsNoID.size(); ++index) {
				const ElectronPointer electron = goodElectronsNoID.at(index);
				if (electron->CiC_ElectronID((CiCElectronID::value) id)) {
					++goodElectrons;

					if (electron->isPFLepton() && electron->pfIsolation() < Globals::maxElectronPFIsolation)
						++goodIsoElectrons;

					if (!electron->isPFLepton()
							&& electron->relativeIsolation() < Globals::maxElectronRelativeIsolation)
						++goodIsoElectrons;
				}

			}

			histMan_->H1D("nElectronsCiCHyperTight4MC")->Fill(goodElectrons, weight_);
			histMan_->H1D("nElectronsCiCHyperTight4MCIso")->Fill(goodIsoElectrons, weight_);

		}
	}
	unsigned int nGoodElectrons = 0;
	unsigned int nGoodIsoElectrons = 0;

	for (unsigned int index = 0; index < goodElectronsNoID.size(); ++index) {
		const ElectronPointer electron = goodElectronsNoID.at(index);
		if (electron->VBTF_WP70_ElectronID()) {
			++nGoodElectrons;

			if (electron->isPFLepton() && electron->pfIsolation() < Globals::maxElectronPFIsolation)
				++nGoodIsoElectrons;

			if (!electron->isPFLepton() && electron->relativeIsolation() < Globals::maxElectronRelativeIsolation)
				++nGoodIsoElectrons;
		}

	}

	histMan_->H1D("nElectronsWP70")->Fill(nGoodElectrons, weight_);
	histMan_->H1D("nElectronsWP70Iso")->Fill(nGoodIsoElectrons, weight_);

	//TODO:Add H/E for electrons which pass the id

}

//TODO: move to LeptonAnalyser
void ElectronAnalyser::analyseElectron(const ElectronPointer electron, double weight){
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = weight * prescale_;
	histMan_->H1D("pT")->Fill(electron->pt(), weight_);
	histMan_->H1D("eta")->Fill(electron->eta(), weight_);
	histMan_->H1D("phi")->Fill(electron->phi(), weight_);
	histMan_->H1D("PFIsolation_03")->Fill(electron->pfRelativeIsolation(0.3), weight_);
	histMan_->H1D("PFIsolation_04")->Fill(electron->pfRelativeIsolation(0.4), weight_);
	histMan_->H1D("PFIsolation_05")->Fill(electron->pfRelativeIsolation(0.5), weight_);
}

ElectronAnalyser::ElectronAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder) {

}

ElectronAnalyser::~ElectronAnalyser() {
}

void ElectronAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	histMan_->addH1D("nEventsPassingCiCId", "nEventsPassingCiCId", CiCElectronID::NUMBER_OF_CiCIds + 1, 0,
			CiCElectronID::NUMBER_OF_CiCIds);
	histMan_->addH1D("nEventsPassingCiCIdIso", "nEventsPassingCiCIdIso", CiCElectronID::NUMBER_OF_CiCIds + 1, 0,
			CiCElectronID::NUMBER_OF_CiCIds);
	histMan_->addH1D("nElectronsCiCHyperTight4MC", "nElectronsCiCHyperTight4MC", 6, 0, 5);
	histMan_->addH1D("nElectronsCiCHyperTight4MCIso", "nElectronsCiCHyperTight4MCIso", 6, 0, 5);
	histMan_->addH1D("nElectronsWP70", "nElectronsWP70", 6, 0, 5);
	histMan_->addH1D("nElectronsWP70Iso", "nElectronsWP70Iso", 6, 0, 5);
	histMan_->addH1D("AllElectronEta", "Electron eta", 100, -2.4, 2.4);
	histMan_->addH1D("AllElectronAbsEta", "Electron abs(eta)", 100, 0, 2.6);
	histMan_->addH1D("AllElectronPt", "Electron Pt", 100, 0, 200);
	histMan_->addH1D("AllElectronPFIsolation", "Electron PFIso", 100, 0, 5);
	histMan_->addH1D("AllElectronRelIso", "Electron RelIso", 100, 0, 5);

	//single electron histograms for analyseElectron
	histMan_->addH1D("PFIsolation_03", "Electron PFIso (DR=0.3); PF relative isolation; Events/(0.01)", 500, 0, 5);
	histMan_->addH1D("PFIsolation_04", "Electron PFIso (DR=0.4); PF relative isolation; Events/(0.01)", 500, 0, 5);
	histMan_->addH1D("PFIsolation_05", "Electron PFIso (DR=0.5); PF relative isolation; Events/(0.01)", 500, 0, 5);
	histMan_->addH1D("pT", "Electron pT; p_{T} (GeV); Events/(1 GeV)", 2000, 0, 2000);
	histMan_->addH1D("eta", "Electron #eta; #eta(e); Events/(0.01)", 600, -3, 3);
	histMan_->addH1D("phi", "Electron #phi; #phi(e); Events/(0.01)", 800, -4, 4);
}

}
