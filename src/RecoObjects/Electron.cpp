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

namespace BAT {

const double initialBigValue = 123456789;
Electron::Electron() :
    Lepton(),
    usedAlgorithm(ElectronAlgorithm::Calo),
    robustLooseId(false),
    robustTightId(false),
    superCluser_Eta(initialBigValue),
    innerLayerMissingHits_(initialBigValue),
    sigma_IEtaIEta(0),
    dPhi_In(0),
    dEta_In(0),
    hadOverEm(0),
    CiCElectronIDCompressed(0),
    gsfTrack(),
    closesTrackID(-1),
    sharedFractionInnerHits(0),
    dCotThetaToNextTrack(0),
    distToNextTrack(0) {
}

Electron::Electron(double energy, double px, double py, double pz) :
    Lepton(energy, px, py, pz),
    usedAlgorithm(ElectronAlgorithm::Calo),
    robustLooseId(false),
    robustTightId(false),
    superCluser_Eta(initialBigValue),
    innerLayerMissingHits_(initialBigValue),
    sigma_IEtaIEta(0),
    dPhi_In(0),
    dEta_In(0),
    hadOverEm(0),
    CiCElectronIDCompressed(0),
    gsfTrack(),
    closesTrackID(-1),
    sharedFractionInnerHits(0),
    dCotThetaToNextTrack(0),
    distToNextTrack(0) {

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
	if(isInBarrelRegion())
		effectiveArea = 0.101;
	else if(isInEndCapRegion())
		effectiveArea = 0.046;

	return ecalIsolation() - rho*effectiveArea;
}


/*
 * Values taken from
 * https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaRecipesFor2011
 */
double Electron::hcalIsolationPUCorrected(double rho) const {
	double effectiveArea = 0;
	if(isInBarrelRegion())
		effectiveArea = 0.021;
	else if(isInEndCapRegion())
		effectiveArea = 0.040;

	return hcalIsolation() - rho*effectiveArea;
}


double Electron::relativeIsolation() const {
    return (ecal_Isolation + hcal_Isolation + tracker_Isolation) / this->et();
}

double Electron::relativeIsolationPUCorrected(double rho) const {
	return (ecalIsolationPUCorrected(rho) + hcalIsolationPUCorrected(rho) + tracker_Isolation) / this->et();
}


bool Electron::isHEEPIsolated() const {
    if (isInBarrelRegion())
        return (ecal_Isolation + hcal_Isolation) < 2 + 0.03 * et();
    else if (isInEndCapRegion() && et() < 50)
        return (ecal_Isolation + hcal_Isolation) < 2.5;
    else if (isInEndCapRegion() && et() >= 50)
        return (ecal_Isolation + hcal_Isolation) < 2.5 + 0.03 * (et() - 50);
    else
        return false;
}

ElectronAlgorithm::value Electron::getUsedAlgorithm() const {
    return usedAlgorithm;
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

bool Electron::isLoose() const {
    bool passesEt = et() > 20;
    bool passesEta = fabs(eta()) < 2.5;
    return passesEt && passesEta && VBTF_W95_ElectronID();

}

bool Electron::isGood(short leptonID) const {
	bool passesID = passesElectronID(leptonID);
	bool passesEt = et() > Globals::minElectronET;
	bool passesEta = fabs(eta()) < Globals::MaxAbsoluteElectronEta && !isInCrack();

	// use d0 wrt primary vertex for
	bool passesD0 = fabs(d0()) < 0.02; //cm
	bool passesDistanceToPV = fabs(zDistanceToPrimaryVertex) < 1;
	//since H/E is used at trigger level, use the same cut here:
	bool passesHOverE = hadOverEm < 0.05; // same for EE and EB
	return passesEt && passesEta && passesD0 && passesID && passesDistanceToPV && passesHOverE;
}

bool Electron::passesElectronID(short leptonID) const {
	using namespace std;

	ElectronID::value electronID = Globals::electronID;
	try {
		electronID = (ElectronID::value) leptonID;
	} catch (exception& e) {
		cerr << "Electron ID with index '" << leptonID << "' not known." << endl;
	}

	switch (electronID) {
	case ElectronID::SimpleCutBasedWP70:
		return VBTF_W70_ElectronID();
	case ElectronID::SimpleCutBasedWP95:
		return VBTF_W95_ElectronID();
	default:
		if (electronID >= ElectronID::CiCVeryLooseMC)
			return CiC_ElectronID((CiCElectronID::value) electronID);
		else
			return false;
	}
}

bool Electron::isQCDElectron(short leptonID) const {
	bool passesID = passesElectronID(leptonID);
	bool passesEt = et() > Globals::minElectronET;
	bool passesEta = fabs(eta()) < Globals::MaxAbsoluteElectronEta && !isInCrack();

	// use d0 wrt primary vertex for
	bool passesD0 = fabs(d0()) < 0.02; //cm
	bool passesDistanceToPV = fabs(zDistanceToPrimaryVertex) < 1;
	//since H/E is used at trigger level, use the same cut here:
	bool passesHOverE = hadOverEm < 0.05; // same for EE and EB
	return passesEt && passesEta && passesD0 && !passesID && passesDistanceToPV && passesHOverE;
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

bool Electron::isFromConversion() const {
    return innerLayerMissingHits_ > 0;
}

bool Electron::isTaggedAsConversion(double maxDist, double maxDCotTheta) const {
    return fabs(distToNextTrack) < maxDist && fabs(dCotThetaToNextTrack) < maxDCotTheta;
}

/* Electron ID cuts (without isolation) from:
 * https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID#Cuts_for_use_on_2010_data
 */
bool Electron::VBTF_W70_ElectronID() const {
    if (isInBarrelRegion())
        return getVBTF_W70_ElectronID_Barrel();
    else if (isInEndCapRegion())
        return getVBTF_W70_ElectronID_Endcap();
    else
        // in crack
        return false;
}

bool Electron::getVBTF_W70_ElectronID_Barrel() const {
    bool passesSigmaIEta = sigma_IEtaIEta < 0.01;
    bool passesDPhiIn = fabs(dPhi_In) < 0.03;
    bool passesDEtaIn = fabs(dEta_In) < 0.004;
    bool passesHadOverEm = hadOverEm < 0.025;
    return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

bool Electron::getVBTF_W70_ElectronID_Endcap() const {
    bool passesSigmaIEta = sigma_IEtaIEta < 0.03;
    bool passesDPhiIn = fabs(dPhi_In) < 0.02;
    bool passesDEtaIn = fabs(dEta_In) < 0.005;
    bool passesHadOverEm = hadOverEm < 0.025;
    return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

bool Electron::VBTF_W95_ElectronID() const {
    if (isInBarrelRegion())
        return getVBTF_W95_ElectronID_Barrel();
    else if (isInEndCapRegion())
        return getVBTF_W95_ElectronID_Endcap();
    else
        // in crack
        return false;
}

bool Electron::getVBTF_W95_ElectronID_Barrel() const {
    bool passesSigmaIEta = sigma_IEtaIEta < 0.01;
    bool passesDPhiIn = fabs(dPhi_In) < 0.8;
    bool passesDEtaIn = fabs(dEta_In) < 0.007;
    bool passesHadOverEm = hadOverEm < 0.15;
    return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

bool Electron::getVBTF_W95_ElectronID_Endcap() const {
    bool passesSigmaIEta = sigma_IEtaIEta < 0.03;
    bool passesDPhiIn = fabs(dPhi_In) < 0.7;
    bool passesDEtaIn = fabs(dEta_In) < 0.01;
    bool passesHadOverEm = hadOverEm < 0.07;
    return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

bool Electron::QCD_AntiID_W70() const {
    if (isInBarrelRegion())
        return QCD_AntiID_W70_Barrel();
    else if (isInEndCapRegion())
        return QCD_AntiID_W70_Endcap();
    else
        return false;
}

bool Electron::QCD_AntiID_W70_Barrel() const {
    bool passesSigmaIEta = sigma_IEtaIEta < 0.01;
    bool passesDPhiIn = fabs(dPhi_In) > 0.03;
    bool passesDEtaIn = fabs(dEta_In) > 0.004;
    bool passesHadOverEm = hadOverEm < 0.025;
    return passesSigmaIEta && passesDPhiIn && passesDEtaIn && passesHadOverEm;
}

bool Electron::QCD_AntiID_W70_Endcap() const {
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
    usedAlgorithm = algo;
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
    distToNextTrack = dist;
}

void Electron::setDCotThetaToNextTrack(double dCotTheta) {
    dCotThetaToNextTrack = dCotTheta;
}


double Electron::pfIsolation() const {
    return (PFGamma_Isolation + PFChargedHadron_Isolation + PFNeutralHadron_Isolation) / et();
}

double Electron::ZDistanceToPrimaryVertex() const {
    return zDistanceToPrimaryVertex;
}

ElectronAlgorithm::value Electron::algorithm() const {
    return usedAlgorithm;
}

double Electron::innerLayerMissingHits() const {
    return innerLayerMissingHits_;
}

double Electron::distToClosestTrack() const {
    return distToNextTrack;
}

double Electron::dCotThetaToClosestTrack() const {
    return dCotThetaToNextTrack;
}

void Electron::setCompressedCiCElectronID(int electronID){
    CiCElectronIDCompressed = electronID;
}

bool Electron::CiC_ElectronID(CiCElectronID::value electronID) const{
    // compressedId bit-wise and with the mask
    return CiCElectronIDCompressed & (1 << (int) electronID);
}

bool Electron::isPFLepton() const {
	return usedAlgorithm == ElectronAlgorithm::ParticleFlow;
}

}
