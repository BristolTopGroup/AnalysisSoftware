/*
 * Jet.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Jet.h"
#include "../../interface/GlobalVariables.h"
#include "../../interface/RecoObjects/TTGenInfo.h"
#include <algorithm>

namespace BAT {

Jet::Jet() :
		Particle(), //
		usedAlgorithm(JetAlgorithm::Calo_AntiKT_Cone05), //
		electromagneticFraction(0.), //
		numberOfRecHitsContaining90PercentOfTheJetEnergy(0.), //
		fractionOfEnergyIntheHottestHPDReadout(0.), //
		btag_discriminators(BtagAlgorithm::NUMBER_OF_BTAGALGORITHMS), //
		btagSF_(0), //
		btagSF_up_(0), //
		btagSF_down_(0), //
		tight_btagSF_(0), //
		tight_btagSF_up_(0), //
		tight_btagSF_down_(0), //
		numberOfDaughters(0), //
		chargedEmEnergyFraction(1), //
		neutralHadronEnergyFraction(1), //
		neutralEmEnergyFraction(1), //
		chargedHadronEnergyFraction(1), //
		chargedMultiplicity(0), //
		energyRaw_(-9999), //
		pxRaw_(-9999), //
		pyRaw_(-9999), //
		pzRaw_(-9999), //
		JEC_(1), //
		JECUncertainty_(-9999), //
		l1OffJEC(0), //
		l2l3ResJEC(0), //
		l2RelJEC(0), //
		l3AbsJEC(0), //
		partonFlavour_(0), //
		hadronFlavour_(0), //
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
		matchedParton(), //
		unsmearedJet(), //
		smearedJet(), //
		rawJet_(), //
		ttbar_decay_parton_(TTPartons::partonType::Undefined)
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
		btagSF_(0), //
		btagSF_up_(0), //
		btagSF_down_(0), //
		tight_btagSF_(0), //
		tight_btagSF_up_(0), //
		tight_btagSF_down_(0), //
		numberOfDaughters(0), //
		chargedEmEnergyFraction(1), //
		neutralHadronEnergyFraction(1), //
		neutralEmEnergyFraction(1), //
		chargedHadronEnergyFraction(1), //
		chargedMultiplicity(0), //
		energyRaw_(-9999), //
		pxRaw_(-9999), //
		pyRaw_(-9999), //
		pzRaw_(-9999), //
		JEC_(1), //
		JECUncertainty_(-9999), //
		l1OffJEC(0), //
		l2l3ResJEC(0), //
		l2RelJEC(0), //
		l3AbsJEC(0), //
		partonFlavour_(0), //
		hadronFlavour_(0), //
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
		matchedParton(), //
		unsmearedJet(), //
		smearedJet(), //
		rawJet_(),
		ttbar_decay_parton_(TTPartons::partonType::Undefined)
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

void Jet::set_matched_parton(const MCParticlePointer matchedparton) {
	matchedParton = matchedparton;
}

const MCParticlePointer Jet::matched_parton() {
	return matchedParton;
}

void Jet::set_ttbar_decay_parton( TTPartons::partonType ttbarDecayParton ) {
	ttbar_decay_parton_ = ttbarDecayParton;
}

const TTPartons::partonType Jet::ttbar_decay_parton() {
	return ttbar_decay_parton_;
}

void Jet::set_unsmeared_jet(const ParticlePointer unsmearedjet) {
	unsmearedJet = unsmearedjet;
}

const ParticlePointer Jet::unsmeared_jet() {
	return unsmearedJet;
}

void Jet::set_raw_jet(const ParticlePointer rawjet) {
	rawJet_ = rawjet;
}

const ParticlePointer Jet::raw_jet() {
	return rawJet_;
}

bool Jet::jet_matching(const ParticlePointer jet, const ParticlePointer gen_jet, double resolution) {

	bool is_matched = false;
	float dEta = jet->eta() - gen_jet->eta();
	float dPhi = jet->phi() - gen_jet->phi();
	float dR = sqrt(pow(dEta, 2)+pow(dPhi, 2));

	float pt = jet->pt();
	float dPt = jet->pt() - gen_jet->pt();

	// Matching in dR (0.4 = AK4)
	if ( dR <= 0.4/2 ){
		// Matching in pT (vs resolution)
		if ( dPt <= 3*resolution*pt){
			is_matched = true;
		}
	}
	return is_matched;
}

const ParticlePointer Jet::smear_jet(const ParticlePointer jet, const ParticlePointer gen_jet, double resolution, double scale_factor) {
	// Get the jet energy resolution scale factors, depending on the jet eta, from 
	// https://twiki.cern.ch/twiki/bin/view/CMS/JetResolution
	bool well_matched_jet = jet_matching(jet, gen_jet, resolution);
	float smearing_factor;

	// Truncated at zero so can only get worse.
	if ( well_matched_jet ){
		// scaling method
		smearing_factor = std::max(0., 1 + (scale_factor - 1)*(jet->pt() - gen_jet->pt())/jet->pt() );
	}
	else{
		// stochastic method
		float RGN = gRandom->Gaus(0,resolution);
		smearing_factor = std::max(0., 1 + RGN*sqrt(std::max(0., pow(scale_factor,2)-1) ) );
	}
	
	//get the unsmeared reconstructed values
	double energy_unsmeared = jet->energy();
	double px_unsmeared 	= jet->px();
	double py_unsmeared 	= jet->py();
	double pz_unsmeared 	= jet->pz();

	//correct the reconstructed jet values
	double energy_smeared 	= smearing_factor * energy_unsmeared;
	double px_smeared 		= smearing_factor * px_unsmeared;
	double py_smeared 		= smearing_factor * py_unsmeared;
	double pz_smeared 		= pz_unsmeared;

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
	return energyRaw_;
}

double Jet::PxRaw() const {
	return pxRaw_;
}

double Jet::PyRaw() const {
	return pyRaw_;
}

double Jet::PzRaw() const {
	return pzRaw_;
}

double Jet::JEC() const {
	return JEC_;
}

double Jet::JECUnc() const {
	return JECUncertainty_;
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
	energyRaw_ = energy;
}

void Jet::setPxRaw(double px) {
	pxRaw_ = px;
}

void Jet::setPyRaw(double py) {
	pyRaw_ = py;
}

void Jet::setPzRaw(double pz) {
	pzRaw_ = pz;
}

void Jet::setJEC(double newJEC) {
	JEC_ = newJEC;
}

void Jet::setJECUnc(double JECUnc) {
	JECUncertainty_ = JECUnc;
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

void Jet::setIsLoose( bool isLoose ) {
	isLoose_ = isLoose;
}

void Jet::setIsMedium( bool isMedium ) {
	isMedium_ = isMedium;
}

void Jet::setIsTight( bool isTight ) {
	isTight_ = isTight;
}

void Jet::setBTagSF( double sf ) {
	btagSF_ = sf;
}

void Jet::setBTagSFUp( double sf ) {
	btagSF_up_ = sf;
}

void Jet::setBTagSFDown( double sf ) {
	btagSF_down_ = sf;
}

void Jet::setTightBTagSF( double sf ) {
	tight_btagSF_ = sf;
}

void Jet::setTightBTagSFUp( double sf ) {
	tight_btagSF_up_ = sf;
}

void Jet::setTightBTagSFDown( double sf ) {
	tight_btagSF_down_ = sf;
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

void Jet::setHadronFlavour(int flavour) {
	hadronFlavour_ = flavour;
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
 * https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagRecommendation74X50ns
 * The b-tag POG has defined working points "Loose " (L), "Medium" (M) and "Tight" (T) for 10%, 1% and 0.1% light mis tag :
 * more details on these working points can be found on https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagPerformanceOP
 */
bool Jet::isBJet(BtagAlgorithm::value type, BtagAlgorithm::workingPoint wp) const {
	double cut(9998);
	switch (type) {

	case BtagAlgorithm::CombinedSecondaryVertexV2:
		if (wp == BtagAlgorithm::LOOSE)
			cut = 0.605;
		else if (wp == BtagAlgorithm::MEDIUM)
			cut = 0.890;
		else if (wp == BtagAlgorithm::TIGHT)
			cut = 0.970;
		break;

	default:
		return false;

	}
	return btag_discriminators[type] > cut;
}

bool Jet::isBJet() const {
	return isBJet_;
}

bool Jet::isLoose() const {
	return isLoose_;
}

bool Jet::isMedium() const {
	return isMedium_;
}

bool Jet::isTight() const {
	return isTight_;
}

double Jet::getBTagSF( int systematic ) const {
	if ( systematic == 0 ) {
		return btagSF_;
	}
	else if ( systematic == 1 ) {
		return btagSF_up_;
	}
	else if ( systematic == -1 ) {
		return btagSF_down_;
	}

	return -1;
}

double Jet::getTightBTagSF( int systematic ) const {
	if ( systematic == 0 ) {
		return tight_btagSF_;
	}
	else if ( systematic == 1 ) {
		return tight_btagSF_up_;
	}
	else if ( systematic == -1 ) {
		return tight_btagSF_down_;
	}

	return -1;
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

int Jet::hadronFlavour() const {
	return hadronFlavour_;
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


