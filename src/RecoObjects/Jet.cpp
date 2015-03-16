/*
 * Jet.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Jet.h"
#include "../../interface/GlobalVariables.h"
#include <algorithm>

namespace BAT {

Jet::Jet() :
		Particle(), //
		usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05), //
		electromagneticFraction(0.), //
		numberOfRecHitsContaining90PercentOfTheJetEnergy(0.), //
		fractionOfEnergyIntheHottestHPDReadout(0.), //
		btag_discriminators(BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS), //
		numberOfDaughters(0), //
		chargedEmEnergyFraction(1), //
		neutralHadronEnergyFraction(1), //
		neutralEmEnergyFraction(1), //
		chargedHadronEnergyFraction(1), //
		chargedMultiplicity(0), //
		energyRaw(-9999), //
		pxRaw(-9999), //
		pyRaw(-9999), //
		pzRaw(-9999), //
		JEC(1), //
		JECUncertainty(-9999), //
		l1OffJEC(0), //
		l2l3ResJEC(0), //
		l2RelJEC(0), //
		l3AbsJEC(0), //
		partonFlavour_(0), //
		//
		energy_unsmeared(0.), //
		pt_unsmeared(0.), //
		px_unsmeared(0.), //
		py_unsmeared(0.), //
		pz_unsmeared(0.), //
		phi_unsmeared(0.), //
		eta_unsmeared(0.), //
		//
		energy_smeared(0.), //
		pt_smeared(0.), //
		px_smeared(0.), //
		py_smeared(0.), //
		pz_smeared(0.), //
		phi_smeared(0.), //
		eta_smeared(0.), //
		//
		matchedGeneratedJet(), //
		unsmearedJet(), //
		smearedJet(), //
		rawJet()
{
	for (unsigned int btag = 0; btag < btag_discriminators.size(); ++btag) {
		btag_discriminators[btag] = -9999;
	}
}

Jet::Jet(double energy, double px, double py, double pz) :
		Particle(energy, px, py, pz), //
		usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05), //
		electromagneticFraction(0.), //
		numberOfRecHitsContaining90PercentOfTheJetEnergy(0.), //
		fractionOfEnergyIntheHottestHPDReadout(0.), //
		btag_discriminators(BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS), //
		numberOfDaughters(0), //
		chargedEmEnergyFraction(1), //
		neutralHadronEnergyFraction(1), //
		neutralEmEnergyFraction(1), //
		chargedHadronEnergyFraction(1), //
		chargedMultiplicity(0), //
		energyRaw(-9999), //
		pxRaw(-9999), //
		pyRaw(-9999), //
		pzRaw(-9999), //
		JEC(1), //
		JECUncertainty(-9999), //
		l1OffJEC(0), //
		l2l3ResJEC(0), //
		l2RelJEC(0), //
		l3AbsJEC(0), //
		partonFlavour_(0), //
		//
		energy_unsmeared(0.), //
		pt_unsmeared(0.), //
		px_unsmeared(0.), //
		py_unsmeared(0.), //
		pz_unsmeared(0.), //
		phi_unsmeared(0.), //
		eta_unsmeared(0.), //
		//
		energy_smeared(0.), //
		pt_smeared(0.), //
		px_smeared(0.), //
		py_smeared(0.), //
		pz_smeared(0.), //
		phi_smeared(0.), //
		eta_smeared(0.), //
		//
		matchedGeneratedJet(), //
		unsmearedJet(), //
		smearedJet(), //
		rawJet()
{
	for (unsigned int btag = 0; btag < btag_discriminators.size(); ++btag) {
		btag_discriminators[btag] = -9999;
	}

}

Jet::~Jet() {
}

JetAlgorithm::value Jet::getUsedAlgorithm() const {
	return usedAlgorithm;
}

void Jet::set_matched_generated_jet(const ParticlePointer matchedgeneratedjet) {
	matchedGeneratedJet = matchedgeneratedjet;
}

const ParticlePointer Jet::matched_generated_jet() {
	return matchedGeneratedJet;
}

void Jet::set_unsmeared_jet(const ParticlePointer unsmearedjet) {
	unsmearedJet = unsmearedjet;
}

const ParticlePointer Jet::unsmeared_jet() {
	return unsmearedJet;
}

void Jet::set_raw_jet(const ParticlePointer rawjet) {
	rawJet = rawjet;
}

const ParticlePointer Jet::raw_jet() {
	return rawJet;
}

const ParticlePointer Jet::smear_jet(const ParticlePointer jet, const ParticlePointer gen_jet, int jet_smearing_systematic) {
	// Get the jet energy resolution scale factors, depending on the jet eta, from 
	// https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetResolution#Recommendations_for_7_and_8_TeV
	double scaleFactor(0.);
	if (fabs(jet->eta()) >= 0.0 && fabs(jet->eta()) < 0.5) {
		switch (jet_smearing_systematic) {
			case -1:
				scaleFactor = 0.990;
				break;
			case 1:
				scaleFactor = 1.115;
				break;
			default:
				scaleFactor = 1.052;
				break;
		}
	}
	if (fabs(jet->eta()) >= 0.5 && fabs(jet->eta()) < 1.1) {
		switch (jet_smearing_systematic) {
			case -1:
				scaleFactor = 1.001;
				break;
			case 1:
				scaleFactor = 1.114;
				break;
			default:
				scaleFactor = 1.057;
				break;
		}
	}
	if (fabs(jet->eta()) >= 1.1 && fabs(jet->eta()) < 1.7) {
		switch (jet_smearing_systematic) {
			case -1:
				scaleFactor = 1.032;
				break;
			case 1:
				scaleFactor = 1.161;
				break;
			default:
				scaleFactor = 1.096;
				break;
		}
	}
	if (fabs(jet->eta()) >= 1.7 && fabs(jet->eta()) < 2.3) {
		switch (jet_smearing_systematic) {
			case -1:
				scaleFactor = 1.042;
				break;
			case 1:
				scaleFactor = 1.228;
				break;
			default:
				scaleFactor = 1.134;
				break;
		}
	}
	if (fabs(jet->eta()) >= 2.3 && fabs(jet->eta()) < 5.0) {
		switch (jet_smearing_systematic) {
			case -1:
				scaleFactor = 1.089;
				break;
			case 1:
				scaleFactor = 1.488;
				break;
			default:
				scaleFactor = 1.288;
				break;
		}
	}
	//use raw scaleFactors from above to calculate the final factors to apply
	double matchedGeneratedJetpt = gen_jet->pt();
	double jetPt = jet->pt();
	double factor = 1-scaleFactor;
	double deltaPt = factor * (jetPt - matchedGeneratedJetpt);
	double ptScale = std::max(0.0, ((jetPt + deltaPt)/jetPt));
	
	//get the unsmeared reconstructed values
	double energy_unsmeared = jet->energy();
	double px_unsmeared = jet->px();
	double py_unsmeared = jet->py();
	double pz_unsmeared = jet->pz();

	//correct the reconstructed jet values
	double energy_smeared = ptScale * energy_unsmeared;
	double px_smeared = ptScale * px_unsmeared;
	double py_smeared = ptScale * py_unsmeared;
	double pz_smeared = ptScale * pz_unsmeared;

	//make new jet to be a new variable to store the final smeared jet
	const ParticlePointer smearedJet(new Particle(energy_smeared, px_smeared, py_smeared, pz_smeared));
	return smearedJet;
}

double Jet::emf() const {
	return electromagneticFraction;
}

double Jet::n90Hits() const {
	return numberOfRecHitsContaining90PercentOfTheJetEnergy;
}

double Jet::fHPD() const {
	return fractionOfEnergyIntheHottestHPDReadout;
}

double Jet::NOD() const {
	return numberOfDaughters;
}

double Jet::CEF() const {
	return chargedEmEnergyFraction;
}

double Jet::NHF() const {
	return neutralHadronEnergyFraction;
}

double Jet::NEF() const {
	return neutralEmEnergyFraction;
}

double Jet::CHF() const {
	return chargedHadronEnergyFraction;
}

double Jet::NCH() const {
	return chargedMultiplicity;
}

double Jet::EnergyRaw() const {
	return energyRaw;
}

double Jet::PxRaw() const {
	return pxRaw;
}

double Jet::PyRaw() const {
	return pyRaw;
}

double Jet::PzRaw() const {
	return pzRaw;
}

double Jet::getJEC() const {
	return JEC;
}

double Jet::JECUnc() const {
	return JECUncertainty;
}

double Jet::L1OffJEC() const {
	return l1OffJEC;
}

double Jet::L2L3ResJEC() const {
	return l2l3ResJEC;
}

double Jet::L2RelJEC() const {
	return l2RelJEC;
}

double Jet::L3AbsJEC() const {
	return l3AbsJEC;
}

void Jet::setEnergyRaw(double energy) {
	energyRaw = energy;
}

void Jet::setPxRaw(double px) {
	pxRaw = px;
}

void Jet::setPyRaw(double py) {
	pyRaw = py;
}

void Jet::setPzRaw(double pz) {
	pzRaw = pz;
}

void Jet::setJEC(double newJEC) {
	JEC = newJEC;
}

void Jet::setJECUnc(double JECUnc) {
	JECUncertainty = JECUnc;
}

void Jet::setL1OffJEC(double JEC) {
	l1OffJEC = JEC;
}

void Jet::setL2L3ResJEC(double JEC) {
	l2l3ResJEC = JEC;
}

void Jet::setL2RelJEC(double JEC) {
	l2RelJEC = JEC;
}

void Jet::setL3AbsJEC(double JEC) {
	l3AbsJEC = JEC;
}

void Jet::setUsedAlgorithm(JetAlgorithm::value algo) {
	usedAlgorithm = algo;
}

void Jet::setEMF(double emf) {
	electromagneticFraction = emf;
}

void Jet::setN90Hits(int n90Hits) {
	numberOfRecHitsContaining90PercentOfTheJetEnergy = n90Hits;
}

void Jet::setFHPD(double fHPD) {
	fractionOfEnergyIntheHottestHPDReadout = fHPD;
}

void Jet::setDiscriminatorForBtagType(double discriminator, BtagAlgorithm::value type) {
	btag_discriminators[type] = discriminator;
}

void Jet::setIsBJet( bool isItAB ) {
	isBJet_ = isItAB;
}

void Jet::setNOD(int nod) {
	numberOfDaughters = nod;
}
void Jet::setCEF(double cef) {
	chargedEmEnergyFraction = cef;
}
void Jet::setNHF(double nhf) {
	neutralHadronEnergyFraction = nhf;
}
void Jet::setNEF(double nef) {
	neutralEmEnergyFraction = nef;
}
void Jet::setCHF(double chf) {
	chargedHadronEnergyFraction = chf;
}
void Jet::setNCH(double nch) {
	chargedMultiplicity = nch;
}

void Jet::setPartonFlavour(int flavour) {
	partonFlavour_ = flavour;
}

bool Jet::isGood() const {
	bool passesPt = pt() > 30;
	bool passesEta = fabs(eta()) < 2.4;
	bool jetID = false;
	if (usedAlgorithm == JetAlgorithm::CA08PF || usedAlgorithm == JetAlgorithm::PF2PAT)
		jetID = FirstDataLoosePFJetID();
	else
		jetID = FirstDataLooseCaloJetID();

	return passesPt && passesEta && jetID;
}

bool Jet::FirstDataLoosePFJetID() const {
	bool passNOD = NOD() > 1;
	bool passCEF = CEF() < 0.99;
	bool passNHF = NHF() < 0.99;
	bool passNEF = NEF() < 0.99;
	bool passCHF = true;
	bool passNCH = true;
	if (fabs(eta()) < 2.4) {
		passCHF = CHF() > 0;
		passNCH = NCH() > 0;
	}
	return passNOD && passCEF && passNHF && passNEF && passCHF && passNCH;
}

bool Jet::FirstDataLooseCaloJetID() const {
	bool passesEMF = emf() > 0.01;
	bool passesN90Hits = n90Hits() > 1;
	bool passesFHPD = fHPD() < 0.98;
	return passesEMF && passesN90Hits && passesFHPD;
}

/*
 * Values taken from
 * https://twiki.cern.ch/twiki/bin/view/CMSPublic/SWGuideBTagPerformance
 * The b-tag POG has defined working points "Loose " (L), "Medium" (M) and "Tight" (T) for 10%, 1% and 0.1% light mis tag :
 * more details on these working points can be found on https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
 */
bool Jet::isBJet(BtagAlgorithm::value type, BtagAlgorithm::workingPoint wp) const {
	double cut(9998);
	switch (type) {

	case BtagAlgorithm::CombinedSecondaryVertexV2:
		if (wp == BtagAlgorithm::LOOSE)
			cut = 0.423;
		else if (wp == BtagAlgorithm::MEDIUM)
			cut = 0.814;
		else if (wp == BtagAlgorithm::TIGHT)
			cut = 0.941;
		break;

	default:
		return false;

	}
	return btag_discriminators[type] > cut;
}

bool Jet::isBJet() const {
	return isBJet_;
}

double Jet::btagSSVHE() const {
	return -1;
	// return btag_discriminators[BtagAlgorithm::SimpleSecondaryVertexHighEfficiency];
}

double Jet::getBJetL7EtCorrection() const {
	Float_t towerBinning[] = { 0.0, 0.174, 0.348, 0.522, 0.696, 0.87, 1.044, 1.218, 1.392, 1.566, 1.74, 2.5 };
	unsigned int nEtaBins_ = 11;
	double res = -1;
	for (unsigned int iEta = 0; iEta < nEtaBins_; ++iEta)
		if (fabs(eta()) >= towerBinning[iEta] && fabs(eta()) < towerBinning[iEta + 1])
			res = Globals::bL7Corrections[iEta]->Eval(pt());
	return res;
}

double Jet::getLightJetL7EtCorrection() const {
	Float_t towerBinning[] = { 0.0, 0.174, 0.348, 0.522, 0.696, 0.87, 1.044, 1.218, 1.392, 1.566, 1.74, 2.5 };
	unsigned int nEtaBins_ = 11;
	double res = -1;
	for (unsigned int iEta = 0; iEta < nEtaBins_; ++iEta)
		if (fabs(eta()) >= towerBinning[iEta] && fabs(eta()) < towerBinning[iEta + 1])
			res = Globals::lightL7Corrections[iEta]->Eval(pt());
	return res;
}

int Jet::partonFlavour() const {
	return partonFlavour_;
}

double Jet::getBTagDiscriminator(BtagAlgorithm::value type) const {
	return btag_discriminators.at((unsigned int) type);
}

double Jet::getEfficiencyCorrection( int scale_factor_systematic ) const {

	if (Globals::energyInTeV == 7) {
		double correction(1.);
		double jetPt = pt();

		if ( jetPt >= 100 ) return 1.0;

		unsigned int binNumber = Globals::hadronTriggerLegEfficiencyHistogram_nonIsoJets->FindFixBin( jetPt );

		double correction_nonIsoJets = 1.;
		double lumi_nonIsoJets = Globals::luminosity * 0.076;
		double correction_isoJets = 1.;
		double lumi_isoJets = Globals::luminosity * 0.764;
		double correction_isoPFJets = 1.;
		double lumi_isoPFJets = Globals::luminosity * 0.16;


		switch (scale_factor_systematic) {
			case -1:
				correction_nonIsoJets = Globals::hadronTriggerLegEfficiencyHistogram_nonIsoJets->GetEfficiency( binNumber ) - Globals::hadronTriggerLegEfficiencyHistogram_nonIsoJets->GetEfficiencyErrorLow( binNumber );
				correction_isoJets = Globals::hadronTriggerLegEfficiencyHistogram_isoJets->GetEfficiency( binNumber ) - Globals::hadronTriggerLegEfficiencyHistogram_isoJets->GetEfficiencyErrorLow( binNumber );
				correction_isoPFJets = Globals::hadronTriggerLegEfficiencyHistogram_isoPFJets->GetEfficiency( binNumber ) - Globals::hadronTriggerLegEfficiencyHistogram_isoPFJets->GetEfficiencyErrorLow( binNumber );
				break;
			case 1:
				correction_nonIsoJets = Globals::hadronTriggerLegEfficiencyHistogram_nonIsoJets->GetEfficiency( binNumber ) + Globals::hadronTriggerLegEfficiencyHistogram_nonIsoJets->GetEfficiencyErrorUp( binNumber );
				correction_isoJets = Globals::hadronTriggerLegEfficiencyHistogram_isoJets->GetEfficiency( binNumber ) + Globals::hadronTriggerLegEfficiencyHistogram_isoJets->GetEfficiencyErrorUp( binNumber );
				correction_isoPFJets = Globals::hadronTriggerLegEfficiencyHistogram_isoPFJets->GetEfficiency( binNumber ) + Globals::hadronTriggerLegEfficiencyHistogram_isoPFJets->GetEfficiencyErrorUp( binNumber );
				break;
			default:
				correction_nonIsoJets = Globals::hadronTriggerLegEfficiencyHistogram_nonIsoJets->GetEfficiency( binNumber );
				correction_isoJets = Globals::hadronTriggerLegEfficiencyHistogram_isoJets->GetEfficiency( binNumber );
				correction_isoPFJets = Globals::hadronTriggerLegEfficiencyHistogram_isoPFJets->GetEfficiency( binNumber );				
		}

		correction = ((correction_nonIsoJets * lumi_nonIsoJets) + (correction_isoJets * lumi_isoJets) + (correction_isoPFJets * lumi_isoPFJets)) / (Globals::luminosity);

		return correction;
	}
	else {
		return 1.;
	}
}

}


