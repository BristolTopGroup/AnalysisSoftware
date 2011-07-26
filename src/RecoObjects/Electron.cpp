/*
 * Electron.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Electron.h"
#include <assert.h>

namespace BAT {

const float initialBigValue = 123456789;
Electron::Electron() :
    Particle(),
    usedAlgorithm(ElectronAlgorithm::Calo),
    robustLooseId(false),
    robustTightId(false),
    superCluser_Eta(initialBigValue),
    ecal_Isolation(initialBigValue),
    hcal_Isolation(initialBigValue),
    tracker_Isolation(initialBigValue),
    innerLayerMissingHits_(initialBigValue),
    sigma_IEtaIEta(0),
    dPhi_In(0),
    dEta_In(0),
    hadOverEm(0),
    CiCElectronIDCompressed(0),
    gsfTrack(),
    closesTrackID(-1),
    sharedFractionInnerHits(0),
    zDistanceToPrimaryVertex(initialBigValue),
    dCotThetaToNextTrack(0),
    distToNextTrack(0),
    PFGamma_Isolation(initialBigValue),
    PFChargedHadron_Isolation(initialBigValue),
    PFNeutralHadron_Isolation(initialBigValue){
}

Electron::Electron(float energy, float px, float py, float pz) :
    Particle(energy, px, py, pz),
    usedAlgorithm(ElectronAlgorithm::Calo),
    robustLooseId(false),
    robustTightId(false),
    superCluser_Eta(initialBigValue),
    ecal_Isolation(initialBigValue),
    hcal_Isolation(initialBigValue),
    tracker_Isolation(initialBigValue),
    innerLayerMissingHits_(initialBigValue),
    sigma_IEtaIEta(0),
    dPhi_In(0),
    dEta_In(0),
    hadOverEm(0),
    CiCElectronIDCompressed(0),
    gsfTrack(),
    closesTrackID(-1),
    sharedFractionInnerHits(0),
    zDistanceToPrimaryVertex(initialBigValue),
    dCotThetaToNextTrack(0),
    distToNextTrack(0),
    PFGamma_Isolation(initialBigValue),
    PFChargedHadron_Isolation(initialBigValue),
    PFNeutralHadron_Isolation(initialBigValue){
}

Electron::~Electron() {
}

float Electron::superClusterEta() const {
    return superCluser_Eta;
}
float Electron::ecalIsolation() const {
    return ecal_Isolation;
}

/*
 * Values taken from
 * https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaRecipesFor2011
 */
float Electron::ecalIsolationPUCorrected(float rho) const {
	float effectiveArea = 0;
	if(isInBarrelRegion())
		effectiveArea = 0.101;
	else if(isInEndCapRegion())
		effectiveArea = 0.046;

	return ecalIsolation() - rho*effectiveArea;
}

float Electron::hcalIsolation() const {
    return hcal_Isolation;
}

/*
 * Values taken from
 * https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaRecipesFor2011
 */
float Electron::hcalIsolationPUCorrected(float rho) const {
	float effectiveArea = 0;
	if(isInBarrelRegion())
		effectiveArea = 0.021;
	else if(isInEndCapRegion())
		effectiveArea = 0.040;

	return hcalIsolation() - rho*effectiveArea;
}

float Electron::trackerIsolation() const {
    return tracker_Isolation;
}

float Electron::relativeIsolation() const {
    return (ecal_Isolation + hcal_Isolation + tracker_Isolation) / this->et();
}

float Electron::relativeIsolationPUCorrected(float rho) const {
	return (ecalIsolationPUCorrected(rho) + hcalIsolationPUCorrected(rho) + tracker_Isolation) / this->et();
}

bool Electron::isIsolated() const {
    return relativeIsolation() < 0.1;
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

void Electron::setEcalIsolation(float isolation) {
    ecal_Isolation = isolation;
}

void Electron::setHcalIsolation(float isolation) {
    hcal_Isolation = isolation;
}

void Electron::setTrackerIsolation(float isolation) {
    tracker_Isolation = isolation;
}

void Electron::setSuperClusterEta(float eta) {
    superCluser_Eta = eta;
}

void Electron::setRobustLooseID(bool id) {
    robustLooseId = id;
}

void Electron::setRobustTightID(bool id) {
    robustTightId = id;
}

void Electron::setSigmaIEtaIEta(float sigma) {
    sigma_IEtaIEta = sigma;
}

void Electron::setDPhiIn(float dphi) {
    dPhi_In = dphi;
}

void Electron::setDEtaIn(float deta) {
    dEta_In = deta;
}

void Electron::setHadOverEm(float HoverE) {
    hadOverEm = HoverE;
}

bool Electron::isLoose() const {
    bool passesEt = et() > 20;
    bool passesEta = fabs(eta()) < 2.5;
    return passesEt && passesEta && VBTF_W95_ElectronID();

}

bool Electron::isGood(bool useCiCElectronID) const {
    bool passesEt = et() > 30;
    bool passesEta = fabs(eta()) < 2.5 && !isInCrack();

    // use d0 wrt primary vertex for
    bool passesD0 = fabs(d0()) < 0.02;//cm

    bool passesDistanceToPV = fabs(zDistanceToPrimaryVertex) < 1;
    bool passesID = false;
    if(useCiCElectronID)
    	passesID = CiC_ElectronID(CiCElectronID::eidHyperTight1MC);
    else
    	passesID = VBTF_W70_ElectronID();

    return passesEt && passesEta && passesD0 && passesID && passesDistanceToPV;
}

//bool Electron::isGoodCiCElectronID(const float minEt) const {
//    bool passesEt = et() > minEt;
//    bool passesEta = fabs(eta()) < 2.5 && !isInCrack();
//
//    // use d0 wrt primary vertex for
//    bool passesD0 = fabs(d0()) < 0.02;//cm
//
//    bool passesDistanceToPV = fabs(zDistanceToPrimaryVertex) < 1;
//    bool passesID = CiC_ElectronID(CiCElectronID::eidHyperTight1MC);
//
//    return passesEt && passesEta && passesD0 && passesID && passesDistanceToPV;
//}

bool Electron::isQCDElectron(bool useCiCElectronID) const {
    bool passesEt = et() > 30;
    bool passesEta = fabs(eta()) < 2.5 && !isInCrack();
    bool passesD0 = fabs(d0()) < 0.02;//cm
    bool passesDistanceToPV = fabs(zDistanceToPrimaryVertex) < 1;
    bool passesID(false);
    if (useCiCElectronID)
		passesID = CiC_ElectronID(CiCElectronID::eidHyperTight1MC);
	else
		passesID = VBTF_W70_ElectronID();
    
    return passesEt && passesEta && passesD0 && passesID && passesDistanceToPV;
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

bool Electron::isTaggedAsConversion(float maxDist, float maxDCotTheta) const {
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

float Electron::sigmaIEtaIEta() const {
    return sigma_IEtaIEta;
}

float Electron::dPhiIn() const {
    return dPhi_In;
}

float Electron::dEtaIn() const {
    return dEta_In;
}

float Electron::HadOverEm() const {
    return hadOverEm;
}

float Electron::HEEPet() const {
    return energy() * sin(fourvector.Theta());
}

bool Electron::RobustLooseID() const {
    return robustLooseId;
}

bool Electron::RobustTightID() const {
    return robustTightId;
}

void Electron::setNumberOfMissingInnerLayerHits(float missingHits) {
    innerLayerMissingHits_ = missingHits;
}

unsigned short Electron::getClosestJetIndex(const JetCollection& jets) const {
    unsigned short idOfClosest = 999;
    float closestDR = 999.;
    for (unsigned short index = 0; index < jets.size(); ++index) {
        float DR = deltaR(jets.at(index));
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

void Electron::setSharedFractionInnerHits(float hits) {
    sharedFractionInnerHits = hits;
}

float Electron::shFracInnerLayer() const {
    return sharedFractionInnerHits;
}

void Electron::setZDistanceToPrimaryVertex(float dist) {
    zDistanceToPrimaryVertex = dist;
}

void Electron::setDistToNextTrack(float dist) {
    distToNextTrack = dist;
}

void Electron::setDCotThetaToNextTrack(float dCotTheta) {
    dCotThetaToNextTrack = dCotTheta;
}

void Electron::setPFGammaIsolation(float pfGammaIso) {
    PFGamma_Isolation = pfGammaIso;
}

void Electron::setPFChargedHadronIsolation(float chargedHadronIso) {
    PFChargedHadron_Isolation = chargedHadronIso;
}

void Electron::setPFNeutralHadronIsolation(float neutralHadronIso) {
    PFNeutralHadron_Isolation = neutralHadronIso;
}

float Electron::PFGammaIsolation() const {
    return PFGamma_Isolation;
}

float Electron::PFChargedHadronIsolation() const {
    return PFChargedHadron_Isolation;
}

float Electron::PFNeutralHadronIsolation() const {
    return PFNeutralHadron_Isolation;
}

float Electron::pfIsolation() const {
    return (PFGamma_Isolation + PFChargedHadron_Isolation + PFNeutralHadron_Isolation) / et();
}

bool Electron::isPFIsolated() const {
    return pfIsolation() < 0.1;
}

float Electron::ZDistanceToPrimaryVertex() const {
    return zDistanceToPrimaryVertex;
}

ElectronAlgorithm::value Electron::algorithm() const {
    return usedAlgorithm;
}

float Electron::innerLayerMissingHits() const {
    return innerLayerMissingHits_;
}

float Electron::distToClosestTrack() const {
    return distToNextTrack;
}

float Electron::dCotThetaToClosestTrack() const {
    return dCotThetaToNextTrack;
}

void Electron::setCompressedCiCElectronID(int electronID){
    CiCElectronIDCompressed = electronID;
}

bool Electron::CiC_ElectronID(CiCElectronID::value electronID) const{
    // compressedId bit-wise and with the mask
    return CiCElectronIDCompressed & (1 << (int) electronID);
}

}
