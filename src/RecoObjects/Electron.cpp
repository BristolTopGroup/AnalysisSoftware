/*
 * Electron.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Electron.h"
#include "../../interface/GlobalVariables.h"
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
		passConversionVeto_(false) {
}

Electron::Electron(double energy, double px, double py, double pz) :
		Lepton(energy, px, py, pz), usedAlgorithm_(ElectronAlgorithm::ParticleFlow), //
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
		passConversionVeto_(false) {

}

Electron::~Electron() {
}

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

double Electron::relativeIsolation() const {
	return (ecal_Isolation_DR03_ + hcal_Isolation_DR03_ + tracker_Isolation_DR03_) / this->et();
}

double Electron::relativeIsolationPUCorrected(double rho) const {
	return (ecalIsolationPUCorrected(rho) + hcalIsolationPUCorrected(rho) + tracker_Isolation_DR03_) / this->et();
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
	return pfIsolation(coneSize, deltaBetaCorrection) / et();
}

double Electron::pfRelativeIsolationRhoCorrected() const {
/*	//https://twiki.cern.ch/twiki/bin/view/CMS/EgammaEARhoCorrection#Isolation_cone_R_0_3
//	2.0<abs(eta)<2.2 	Aeff(NH) = 0.023 +/- 0.001 	Aeff(gamma) = 0.089 +/- 0.002 	Aeff(gamma+NH) = 0.11 +/- 0.003
//	2.2<abs(eta)<2.3 	Aeff(NH) = 0.023 +/- 0.002 	Aeff(gamma) = 0.092 +/- 0.004 	Aeff(gamma+NH) = 0.12 +/- 0.004
//	2.3<abs(eta)<2.4 	Aeff(NH) = 0.021 +/- 0.002 	Aeff(gamma) = 0.097 +/- 0.004 	Aeff(gamma+NH) = 0.12 +/- 0.005
//	abs(eta)>2.4 	Aeff(NH) = 0.021 +/- 0.003 	Aeff(gamma) = 0.11 +/- 0.004 	Aeff(gamma+NH) = 0.13 +/- 0.006

	if(coneSize != 0.3){
		//TODO: put exception or warning
		return 9999999;
	}


	double SCeta(fabs(superClusterEta()));
	double effArea(0);
	if (SCeta < 1.0)
		effArea = 0.10;
	else if (SCeta < 1.479)
		effArea = 0.12;
	else if (SCeta < 2.0)
		effArea = 0.085;
	else if (SCeta < 2.2)
		effArea = 0.11;
	else if (SCeta < 2.3)
		effArea = 0.12;
	else if (SCeta < 2.4)
		effArea = 0.12;
	else
		effArea = 0.13;

	double neutralIso = PFGammaIsolation(coneSize) + PFNeutralHadronIsolation(coneSize);
	double correctedIso = PFChargedHadronIsolation(coneSize) + max(neutralIso - rho * effArea, 0.);*/


	return PFRelativeIsolationRho_DR03_;
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

double Electron::mvaTrigV0() const {
	return mvaTrigV0_;
}

double Electron::mvaNonTrigV0() const {
	return mvaNonTrigV0_;
}

double Electron::getEfficiencyCorrection(bool qcd, int electron_scale_factor_systematic, int run_number) {
	double correction(1.);
	double electronEta(eta());

	if(Globals::energyInTeV == 7){
		// hard coded values since pickle file not available...not updated because I couldn't find any values on twikis!
		if (electronEta < -1.5)
			correction = 1.003;
		else if (electronEta >= -1.5 && electronEta < -1.2)
			correction = 0.980;
		else if (electronEta >= -1.2 && electronEta < -0.9)
			correction = 0.941;
		else if (electronEta >= -0.9 && electronEta < 0)
			correction = 0.974;
		else if (electronEta >= 0 && electronEta < 0.9)
			correction = 0.977;
		else if (electronEta >= 0.9 && electronEta < 1.2)
			correction = 0.939;
		else if (electronEta >= 1.2 && electronEta < 1.5)
			correction = 0.967;
		else if (electronEta >= 1.5)
			correction = 1.023;
	}
	//8TeV scale factors from https://twiki.cern.ch/twiki/bin/viewauth/CMS/KoPFAElectronTagAndProbe
	//Only factors from PromptReco available (in the "Efficiency for e+jet channel (promptreco)" section)
	//Specifically: ID & Iso: "ID/Isolation efficiency" sub-section
	//Specifically: Trigger: "Trigger efficiency" sub-section
	// These values are hard coded because, unlike for Muons, there is no pickle file provided.
	else if(qcd == false){ //corrections for (ID & Iso) and Trigger respectively
		//again, hard coded because no pickle file available.
		if(fabs(electronEta)<0.8) {
			if(20<=pt() && pt()<30) {  //Note: Trigger scale factors only provided down to electron pt of 30GeV in the link above, so I have used the same as for the 30GeV-40GeV range.
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.949-0.0007)*(0.987-0.017);
						break;
					case 1:
						correction = (0.949+0.007)*(0.987+0.012);
						break;
					default:
						correction = 0.949*0.987;
				}
			}
			else if(30<=pt() && pt()<40) {
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.939-0.003)*(0.987-0.017);
						break;
					case 1:
						correction = (0.939+0.003)*(0.987+0.012);
						break;
					default:
						correction = 0.939*0.987;
				}
			}
			else if(40<=pt() && pt()<50) {
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.950-0.001)*(0.997-0.001);
						break;
					case 1:
						correction = (0.950+0.001)*(0.997+0.001);
						break;
					default:
						correction = 0.950*0.997;
				}
			}
			else if(50<=pt() && pt()<=200) {
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.957-0.001)*(0.998-0.002);
						break;
					case 1:
						correction = (0.957+0.001)*(0.998+0.002);
						break;
					default:
						correction = 0.957*0.998;
				}
			}
		}
		else if(fabs(electronEta)>=0.8 && fabs(electronEta)<1.478) {
			if(20<=pt() && pt()<30) { //Note: Trigger scale factors only provided down to electron pt of 30GeV in the link above, so I have used the same as for the 30GeV-40GeV range.
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.900-0.009)*(0.964-0.001);
						break;
					case 1:
						correction = (0.900+0.010)*(0.964+0.002);
						break;
					default:
						correction = 0.900*0.964;
				}
			}
			if(30<=pt() && pt()<40) {
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.920-0.000)*(0.964-0.001);
						break;
					case 1:
						correction = (0.920+0.002)*(0.964+0.002);
						break;
					default:
						correction = 0.920*0.964;
				}
			}
			if(40<=pt() && pt()<50) {
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.949-0.002)*(0.980-0.001);
						break;
					case 1:
						correction = (0.949+0.002)*(0.980+0.001);
						break;
					default:
						correction = 0.949*0.980;
				}
			}
			if(50<=pt() && pt()<=200) {
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.959-0.003)*(0.988-0.002);
						break;
					case 1:
						correction = (0.959+0.003)*(0.988+0.002);
						break;
					default:
						correction = 0.954*0.988;
				}
			}
		}
		else if(fabs(electronEta)>=1.478 && fabs(electronEta)<2.5) {
			if(20<=pt() && pt()<30) { //Note: Trigger scale factors only provided down to electron pt of 30GeV in the link above, so I have used the same as for the 30GeV-40GeV range.
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.857-0.010)*(1.004-0.006);
						break;
					case 1:
						correction = (0.857+0.009)*(1.004+0.006);
						break;
					default:
						correction = 0.857*1.004;
				}
			}
			if(30<=pt() && pt()<40) {
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.907-0.005)*(1.004-0.006);
						break;
					case 1:
						correction = (0.907+0.005)*(1.004+0.006);
						break;
					default:
						correction = 0.907*1.004;
				}
			}
			if(40<=pt() && pt()<50) {
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.937-0.008)*(1.033-0.007);
						break;
					case 1:
						correction = (0.937+0.008)*(1.033+0.007);
						break;
					default:
						correction = 0.937*1.033;
				}
			}
			if(50<=pt() && pt()<=200) {
				switch (electron_scale_factor_systematic) {
					case -1:
						correction = (0.954-0.010)*(0.976-0.012);
						break;
					case 1:
						correction = (0.954+0.011)*(0.976+0.015);
						break;
					default:
						correction = 0.954*0.976;
				}
			}
		}
	}
//ID scale factor for QCD is technically irrelevant since we use conversion veto for QCD, but ID and isolation scale factors are provided as one so unable to split them to remove the ID scale factor.
//	else if(qcd == true){
//	}
	return correction;
}
}
