/*
 * ElectronAnalyser.cpp
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */
#include "../../interface/Analysers/ElectronAnalyser.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {

void ElectronAnalyser::analyse(const TopPairEventCandidate& ttbarEvent) {
    histMan->setCurrentHistogramFolder("ElectronAnalysis");

    const ElectronCollection electrons = ttbarEvent.Electrons();
    float weight = ttbarEvent.weight();
    ElectronCollection goodElectronsNoID;

    if (ttbarEvent.passesFullTTbarEPlusJetSelection()) {
    	const ElectronPointer electron = ttbarEvent.MostPFIsolatedElectron(ttbarEvent.GoodElectrons());
		histMan->H1D("electronEta")->Fill(electron->eta(), weight);
		histMan->H1D("electronAbsEta")->Fill(abs(electron->eta()), weight);
		histMan->H1D("electronPt")->Fill(electron->pt(), weight);
		histMan->H1D("electronPFIsolation")->Fill(electron->pfIsolation(), weight);
		histMan->H1D("electronRelIso")->Fill(electron->relativeIsolation(), weight);
    }

    for (unsigned index = 0; index < electrons.size(); ++index) {
        const ElectronPointer electron = electrons.at(index);
        bool passesEt = electron->et() > 30;
        bool passesEta = fabs(electron->eta()) < 2.5 && !electron->isInCrack();

        // use d0 wrt primary vertex for
        bool passesD0 = fabs(electron->d0()) < 0.02;//cm

        bool passesDistanceToPV = fabs(electron->ZDistanceToPrimaryVertex()) < 1;

        if (passesEt && passesEta && passesD0 && passesDistanceToPV)
            goodElectronsNoID.push_back(electron);
    }

    for (unsigned int id = 0; id < CiCElectronID::NUMBER_OF_CiCIds; ++id) {
        for (unsigned int index = 0; index < goodElectronsNoID.size(); ++index) {
            const ElectronPointer electron = goodElectronsNoID.at(index);

            if (electron->CiC_ElectronID((CiCElectronID::value) id)) {
                histMan->H1D("nEventsPassingCiCId")->Fill(id + 1, weight);

                if (electron->isPFLepton() && electron->pfIsolation() < Globals::maxElectronPFIsolation)
                    histMan->H1D("nEventsPassingCiCIdIso")->Fill(id + 1, weight);

                if (!electron->isPFLepton() && electron->relativeIsolation() < Globals::maxElectronRelativeIsolation)
                    histMan->H1D("nEventsPassingCiCIdIso")->Fill(id + 1, weight);

            } else {
                histMan->H1D("nEventsPassingCiCId")->Fill(0., weight);
                if (electron->isPFLepton() && electron->pfIsolation() < Globals::maxElectronPFIsolation)
                    histMan->H1D("nEventsPassingCiCIdIso")->Fill(0., weight);
                if (!electron->isPFLepton() && electron->relativeIsolation() < Globals::maxElectronRelativeIsolation)
                    histMan->H1D("nEventsPassingCiCIdIso")->Fill(0., weight);
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

                    if (!electron->isPFLepton() && electron->relativeIsolation() < Globals::maxElectronRelativeIsolation)
                        ++goodIsoElectrons;
                }

            }

            histMan->H1D("nElectronsCiCHyperTight4MC")->Fill(goodElectrons, weight);
            histMan->H1D("nElectronsCiCHyperTight4MCIso")->Fill(goodIsoElectrons, weight);

        }
    }
    unsigned int goodElectrons = 0;
    unsigned int goodIsoElectrons = 0;

    for (unsigned int index = 0; index < goodElectronsNoID.size(); ++index) {
        const ElectronPointer electron = goodElectronsNoID.at(index);
        if (electron->VBTF_WP70_ElectronID()) {
            ++goodElectrons;

            if (electron->isPFLepton() && electron->pfIsolation() < Globals::maxElectronPFIsolation)
                ++goodIsoElectrons;

            if (!electron->isPFLepton() && electron->relativeIsolation() < Globals::maxElectronRelativeIsolation)
                ++goodIsoElectrons;
        }

    }

    histMan->H1D("nElectronsWP70")->Fill(goodElectrons, weight);
    histMan->H1D("nElectronsWP70Iso")->Fill(goodIsoElectrons, weight);

    //TODO:Add H/E for electrons which pass the id


}

ElectronAnalyser::ElectronAnalyser(boost::shared_ptr<HistogramManager> histMan) :
    BasicAnalyser(histMan) {

}

ElectronAnalyser::~ElectronAnalyser() {
}

void ElectronAnalyser::createHistograms() {
    histMan->setCurrentHistogramFolder("ElectronAnalysis");

    histMan->addH1D("nEventsPassingCiCId", "nEventsPassingCiCId", CiCElectronID::NUMBER_OF_CiCIds + 1, 0,
            CiCElectronID::NUMBER_OF_CiCIds);
    histMan->addH1D("nEventsPassingCiCIdIso", "nEventsPassingCiCIdIso", CiCElectronID::NUMBER_OF_CiCIds + 1, 0,
            CiCElectronID::NUMBER_OF_CiCIds);
    histMan->addH1D("nElectronsCiCHyperTight4MC", "nElectronsCiCHyperTight4MC", 6, 0, 5);
    histMan->addH1D("nElectronsCiCHyperTight4MCIso", "nElectronsCiCHyperTight4MCIso", 6, 0, 5);
    histMan->addH1D("nElectronsWP70", "nElectronsWP70", 6, 0, 5);
    histMan->addH1D("nElectronsWP70Iso", "nElectronsWP70Iso", 6, 0, 5);
	histMan->addH1D("electronEta", "Electron eta", 100, -2.4, 2.4);
	histMan->addH1D("electronAbsEta", "Electron abs(eta)", 100, 0, 2.6);
	histMan->addH1D("electronPt", "Electron Pt", 100, 0, 200);
	histMan->addH1D("electronPFIsolation", "Electron PFIso", 100, 0, 5);
	histMan->addH1D("electronRelIso", "Electron RelIso", 100, 0, 5);
}

}
