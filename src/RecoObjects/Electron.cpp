/*
 * Electron.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Electron.h"
#include "../../interface/GlobalVariables.h"
#include "../../interface/Python/ConfigFile.h"
#include <exception>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

namespace BAT {

const double initialBigValue = 123456789;
Electron::Electron() :
		Lepton(), //
		usedAlgorithm_(ElectronAlgorithm::ParticleFlow), //
		// PFRelIsoWithEA_(initialBigValue), //
		robustLooseId(false), //
		robustTightId(false), //
		superCluser_Eta(initialBigValue), //
		innerLayerMissingHits_(initialBigValue), //
		sigma_IEtaIEta(initialBigValue), //
		dPhi_In(initialBigValue), //
		dEta_In(initialBigValue), //
		hadOverEm(initialBigValue), //
		gsfTrack(), //
		closesTrackID(-1), //
		sharedFractionInnerHits(0), //
		dCotThetaToNextTrack_(0), //
		distToNextTrack_(0), //
		mvaTrigV0_(-initialBigValue), //
		mvaNonTrigV0_(-initialBigValue), //
		passConversionVeto_(false),
		isTightElectron_(false),
		isTightConversionElectron_(false),
		isTightNonIsoElectron_(false) {
}

Electron::Electron(double energy, double px, double py, double pz) :
		Lepton(energy, px, py, pz), usedAlgorithm_(ElectronAlgorithm::ParticleFlow), //
		// PFRelIsoWithEA_(initialBigValue), //
		robustLooseId(false), //
		robustTightId(false), //
		superCluser_Eta(initialBigValue), //
		innerLayerMissingHits_(initialBigValue), //
		sigma_IEtaIEta(initialBigValue), //
		dPhi_In(initialBigValue), //
		dEta_In(initialBigValue), //
		hadOverEm(initialBigValue), //
		gsfTrack(), //
		closesTrackID(-1), //
		sharedFractionInnerHits(0), //
		dCotThetaToNextTrack_(0), //
		distToNextTrack_(0), //
		mvaTrigV0_(-initialBigValue), //
		mvaNonTrigV0_(-initialBigValue), //
		passConversionVeto_(false),
		isTightElectron_(false),
		isTightConversionElectron_(false),
		isTightNonIsoElectron_(false)  {
}

Electron::~Electron() {
}

// void Electron::setPFRelIsoWithEA( double isolation ) {
// 	PFRelIsoWithEA_ = isolation;
// }

double Electron::superClusterEta() const {
	return superCluser_Eta;
}

/*
 * Values taken from
 * https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaRecipesFor2011
 */
double Electron::ecalIsolationPUCorrected(double rho) const {
	double effectiveArea = 0;
	if (isInBarrelRegion())
		effectiveArea = 0.101;
	else if (isInEndCapRegion())
		effectiveArea = 0.046;

	return ecalIsolation() - rho * effectiveArea;
}

/*
 * Values taken from
 * https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaRecipesFor2011
 */
double Electron::hcalIsolationPUCorrected(double rho) const {
	double effectiveArea = 0;
	if (isInBarrelRegion())
		effectiveArea = 0.021;
	else if (isInEndCapRegion())
		effectiveArea = 0.040;

	return hcalIsolation() - rho * effectiveArea;
}

bool Electron::isHEEPIsolated() const {
	if (isInBarrelRegion())
		return (ecal_Isolation_DR03_ + hcal_Isolation_DR03_) < 2 + 0.03 * et();
	else if (isInEndCapRegion() && et() < 50)
		return (ecal_Isolation_DR03_ + hcal_Isolation_DR03_) < 2.5;
	else if (isInEndCapRegion() && et() >= 50)
		return (ecal_Isolation_DR03_ + hcal_Isolation_DR03_) < 2.5 + 0.03 * (et() - 50);
	else
		return false;
}

ElectronAlgorithm::value Electron::getUsedAlgorithm() const {
	return usedAlgorithm_;
}

void Electron::setSuperClusterEta(double eta) {
	superCluser_Eta = eta;
}

void Electron::setRobustLooseID(bool id) {
	robustLooseId = id;
}

void Electron::setRobustTightID(bool id) {
	robustTightId = id;
}

void Electron::setSigmaIEtaIEta(double sigma) {
	sigma_IEtaIEta = sigma;
}

void Electron::setDPhiIn(double dphi) {
	dPhi_In = dphi;
}

void Electron::setDEtaIn(double deta) {
	dEta_In = deta;
}

void Electron::setHadOverEm(double HoverE) {
	hadOverEm = HoverE;
}

// double Electron::PFRelIsoWithEA() const {
// 	return PFRelIsoWithEA_;
// }

bool Electron::passesElectronID(short leptonID) const {
	using namespace std;

	ElectronID::value electronID(ElectronID::MVAIDTrigger);
	try {
		electronID = (ElectronID::value) leptonID;
	} catch (exception& e) {
		cerr << "Electron ID with index '" << leptonID << "' not known." << endl;
	}

	switch (electronID) {
	case ElectronID::SimpleCutBasedWP70:
		return VBTF_WP70_ElectronID();
	case ElectronID::SimpleCutBasedWP95:
		return VBTF_WP95_ElectronID();
	case ElectronID::MVAIDTrigger:
		return mvaTrigV0() > 0.5;
	case ElectronID::MVAIDNonTrigger:
		return mvaNonTrigV0() > 0.0;
	default:
		return mvaTrigV0() > 0.5;
	}
}

bool Electron::isTightElectron() const{
	return isTightElectron_;
}

bool Electron::isTightConversionElectron() const{
	return isTightConversionElectron_;
}

bool Electron::isTightNonIsoElectron() const{
	return isTightNonIsoElectron_;
}

bool Electron::isInBarrelRegion() const {
	return fabs(superClusterEta()) < 1.4442;
}

bool Electron::isInCrack() const {
	return !isInBarrelRegion() && !isInEndCapRegion();
}

bool Electron::isInEndCapRegion() const {
	return fabs(superClusterEta()) > 1.5660;
}

bool Electron::passConversionVeto() const {
	return passConversionVeto_;
}

/* Electron ID cuts (without isolation) from:
 * https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID#Cuts_for_use_on_2010_data
 */
bool Electron::VBTF_WP70_ElectronID() const {
	if (isInBarrelRegion())
		return getVBTF_WP70_ElectronID_Barrel();
	else if (isInEndCapRegion())
		return getVBTF_WP70_ElectronID_Endcap();
	else
		// in crack
		return false;
}

bool Electron::getVBTF_WP70_ElectronID_Barrel() const {
	bool passesSigmaIEta = sigma_IEtaIEta < 0.01;
	bool passesDPhiIn = fabs(dPhi_In) < 0.03;
	bool passesDEtaIn = fabs(dEta_In) < 0.004;
	bool passesHadOverEm = hadOverEm < 0.025;
	return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

bool Electron::getVBTF_WP70_ElectronID_Endcap() const {
	bool passesSigmaIEta = sigma_IEtaIEta < 0.03;
	bool passesDPhiIn = fabs(dPhi_In) < 0.02;
	bool passesDEtaIn = fabs(dEta_In) < 0.005;
	bool passesHadOverEm = hadOverEm < 0.025;
	return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

bool Electron::VBTF_WP95_ElectronID() const {
	if (isInBarrelRegion())
		return getVBTF_WP95_ElectronID_Barrel();
	else if (isInEndCapRegion())
		return getVBTF_WP95_ElectronID_Endcap();
	else
		// in crack
		return false;
}

bool Electron::getVBTF_WP95_ElectronID_Barrel() const {
	bool passesSigmaIEta = sigma_IEtaIEta < 0.01;
	bool passesDPhiIn = fabs(dPhi_In) < 0.8;
	bool passesDEtaIn = fabs(dEta_In) < 0.007;
	bool passesHadOverEm = hadOverEm < 0.15;
	return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

bool Electron::getVBTF_WP95_ElectronID_Endcap() const {
	bool passesSigmaIEta = sigma_IEtaIEta < 0.03;
	bool passesDPhiIn = fabs(dPhi_In) < 0.7;
	bool passesDEtaIn = fabs(dEta_In) < 0.01;
	bool passesHadOverEm = hadOverEm < 0.07;
	return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

bool Electron::QCD_AntiID_WP70() const {
	if (isInBarrelRegion())
		return QCD_AntiID_WP70_Barrel();
	else if (isInEndCapRegion())
		return QCD_AntiID_WP70_Endcap();
	else
		return false;
}

bool Electron::QCD_AntiID_WP70_Barrel() const {
	bool passesSigmaIEta = sigma_IEtaIEta < 0.01;
	bool passesDPhiIn = fabs(dPhi_In) > 0.03;
	bool passesDEtaIn = fabs(dEta_In) > 0.004;
	bool passesHadOverEm = hadOverEm < 0.025;
	return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

bool Electron::QCD_AntiID_WP70_Endcap() const {
	bool passesSigmaIEta = sigma_IEtaIEta < 0.03;
	bool passesDPhiIn = fabs(dPhi_In) > 0.02;
	bool passesDEtaIn = fabs(dEta_In) > 0.005;
	bool passesHadOverEm = hadOverEm < 0.025;
	return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

double Electron::sigmaIEtaIEta() const {
	return sigma_IEtaIEta;
}

double Electron::dPhiIn() const {
	return dPhi_In;
}

double Electron::dEtaIn() const {
	return dEta_In;
}

double Electron::HadOverEm() const {
	return hadOverEm;
}

double Electron::HEEPet() const {
	return energy() * sin(fourvector.Theta());
}

bool Electron::RobustLooseID() const {
	return robustLooseId;
}

bool Electron::RobustTightID() const {
	return robustTightId;
}

void Electron::setNumberOfMissingInnerLayerHits(double missingHits) {
	innerLayerMissingHits_ = missingHits;
}

unsigned short Electron::getClosestJetIndex(const JetCollection& jets) const {
	unsigned short idOfClosest = 999;
	double closestDR = 999.;
	for (unsigned short index = 0; index < jets.size(); ++index) {
		double DR = deltaR(jets.at(index));
		if (DR < closestDR) {
			closestDR = DR;
			idOfClosest = index;
		}
	}
	return idOfClosest;
}

void Electron::setUsedAlgorithm(ElectronAlgorithm::value algo) {
	usedAlgorithm_ = algo;
}

void Electron::setGSFTrack(const TrackPointer track) {
	gsfTrack = track;
}

const TrackPointer Electron::GSFTrack() const {
	return gsfTrack;
}

void Electron::setClosestTrackID(int trackID) {
	closesTrackID = trackID;
}

int Electron::closestCTFTrackID() const {
	return closesTrackID;
}

void Electron::setSharedFractionInnerHits(double hits) {
	sharedFractionInnerHits = hits;
}

double Electron::shFracInnerLayer() const {
	return sharedFractionInnerHits;
}

void Electron::setDistToNextTrack(double dist) {
	distToNextTrack_ = dist;
}

void Electron::setDCotThetaToNextTrack(double dCotTheta) {
	dCotThetaToNextTrack_ = dCotTheta;
}

double Electron::pfRelativeIsolation(double coneSize, bool deltaBetaCorrection) const {
	if ( coneSize == 0.3 ) {
		return PFRelIso03();
	}
	else if ( coneSize == 0.4 ) {
		return PFRelIso04();
	}

	return 999;
}

ElectronAlgorithm::value Electron::algorithm() const {
	return usedAlgorithm_;
}

double Electron::innerLayerMissingHits() const {
	return innerLayerMissingHits_;
}

double Electron::distToClosestTrack() const {
	return distToNextTrack_;
}

double Electron::dCotThetaToClosestTrack() const {
	return dCotThetaToNextTrack_;
}

bool Electron::isPFLepton() const {
	return usedAlgorithm_ == ElectronAlgorithm::ParticleFlow;
}

string Electron::toString() const {
	stringstream out;
	out << Lepton::toString();
	out << "Electron information" << "\n";
	return out.str();
}

void Electron::setMVATrigV0(double mva) {
	mvaTrigV0_ = mva;
}

void Electron::setMVANonTrigV0(double mva) {
	mvaNonTrigV0_ = mva;
}

void Electron::setPassConversionVeto(bool passes) {
	passConversionVeto_ = passes;
}

void Electron::setIsTightElectron(bool passes) {
	isTightElectron_ = passes;
}

void Electron::setIsTightConversionElectron(bool passes) {
	isTightConversionElectron_ = passes;
}

void Electron::setIsTightNonIsoElectron(bool passes) {
	isTightNonIsoElectron_ = passes;
}

double Electron::mvaTrigV0() const {
	return mvaTrigV0_;
}

double Electron::mvaNonTrigV0() const {
	return mvaNonTrigV0_;
}

// double Electron::getEfficiencyCorrection(int electron_scale_factor_systematic) const {
// 	if ( electron_scale_factor_systematic == 0 ) {
// 		return 1.;
// 	}
// 	else if ( electron_scale_factor_systematic == -1 ) {
// 		return 0.99;
// 	}
// 	else if ( electron_scale_factor_systematic == 1 ) {
// 		return 1.01;
// 	}

// 	return 0.;
// }


double Electron::getEfficiencyCorrection(int electron_scale_factor_systematic) const {

	double triggerEfficiency(1.);
	double triggerEfficiencyRelativeError(0.1);
	// boost::shared_ptr<TH1F> electronTriggerScaleFactorsHistogram(Globals::electronTriggerScaleFactorsHistogram);
	double electronPt = pt();
	double electronEta = fabs(eta());
	// double maxPt = electronTriggerScaleFactorsHistogram->GetXaxis()->GetXmax();
	// unsigned int bin = 0;
	// if ( electronPt <= maxPt ) {
	// 	bin = electronTriggerScaleFactorsHistogram->FindBin( electronPt );
	// }
	// else {
	// 	bin = electronTriggerScaleFactorsHistogram->GetNbinsX();
	// }

	// triggerEfficiency = electronTriggerScaleFactorsHistogram->GetBinContent( bin );

	// ID scalefactor
	double idSF(1.);
	double idSFError(0.);
	boost::shared_ptr<TH2F> electronIDScaleFactorsHistogram(Globals::electronIdScaleFactorsHistogram);
	double maxPt = electronIDScaleFactorsHistogram->GetYaxis()->GetXmax();

	unsigned int bin = 0;
	if ( electronPt <= maxPt ) {
		bin = electronIDScaleFactorsHistogram->FindBin( electronEta, electronPt );
	}
	else {
		double lastPtBinCentre = electronIDScaleFactorsHistogram->GetYaxis()->GetBinCenter( electronIDScaleFactorsHistogram->GetNbinsY() );
		bin = electronIDScaleFactorsHistogram->FindBin( electronEta, lastPtBinCentre );
	}
	idSF = electronIDScaleFactorsHistogram->GetBinContent( bin );
	idSFError = electronIDScaleFactorsHistogram->GetBinError( bin );


	// Reco scalefactor
	double recoSF(1.);
	double recoSFError(0.);
	boost::shared_ptr<TH2F> electronRecoScaleFactorsHistogram(Globals::electronRecoScaleFactorsHistogram);
	maxPt = electronRecoScaleFactorsHistogram->GetYaxis()->GetXmax();

	bin = 0;
	if ( electronPt <= maxPt ) {
		bin = electronRecoScaleFactorsHistogram->FindBin( electronEta, electronPt );
	}
	else {
		double lastPtBinCentre = electronRecoScaleFactorsHistogram->GetYaxis()->GetBinCenter( electronRecoScaleFactorsHistogram->GetNbinsY() );
		bin = electronRecoScaleFactorsHistogram->FindBin( electronEta, lastPtBinCentre );
	}
	recoSF = electronRecoScaleFactorsHistogram->GetBinContent( bin );
	recoSFError = electronRecoScaleFactorsHistogram->GetBinError( bin );

	if (electron_scale_factor_systematic == -1 ) {
		idSF -= idSFError;
		recoSF -= recoSFError;
		triggerEfficiency = triggerEfficiency * ( 1 - triggerEfficiencyRelativeError );

	}
	else if ( electron_scale_factor_systematic == 1 ) {
		idSF += idSFError;
		recoSF += recoSFError;
		triggerEfficiency = triggerEfficiency * ( 1 + triggerEfficiencyRelativeError );
	}

	return idSF * recoSF;
	// return triggerEfficiency * idSF * recoSF;

}


}
