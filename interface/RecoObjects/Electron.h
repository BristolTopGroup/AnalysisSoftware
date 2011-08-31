/*
 * Electron.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef ELECTRON_H_
#define ELECTRON_H_
#include "Particle.h"
#include <vector>
#include <string>
#include <boost/static_assert.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include "Jet.h"
#include "Track.h"

namespace BAT {

namespace ElectronAlgorithm {
enum value {
    Calo,
    ParticleFlow,
    NUMBER_OF_ELECTRONALGORITHMS
};

const boost::array<std::string, ElectronAlgorithm::NUMBER_OF_ELECTRONALGORITHMS> prefixes = { {
        "selectedPatElectrons",
        "selectedPatElectronsLoosePFlow"  } };

const boost::array<std::string, ElectronAlgorithm::NUMBER_OF_ELECTRONALGORITHMS> names = { {
        "CaloElectron",
        "PFElectron"} };
}

namespace ElectronIsolation{
enum value {
    RelativeIsolation,
    ParticleFlowIsolation,
    NUMBER_OF_ELECTRONISOLATIONS
};
}

namespace CiCElectronID {
enum value {
    eidVeryLooseMC,
    eidLooseMC,
    eidMediumMC,
    eidTightMC,
    eidSuperTightMC,
    eidHyperTight1MC,
    eidHyperTight2MC,
    eidHyperTight3MC,
    eidHyperTight4MC,
    NUMBER_OF_CiCIds
};
}

class Electron: public Particle {
public:

    Electron();
    Electron(double energy, double px, double py, double pz);
    virtual ~Electron();
    bool isGood(bool useCiCElectronID) const;
//    bool isGoodCiCElectronID(const double minEt = 30) const;
    bool isIsolated() const;
    bool isPFIsolated() const;
    bool isHEEPIsolated() const;
    bool isTaggedAsConversion(double maxDist, double maxDCotTheta) const;
    bool isFromConversion() const;
    bool isLoose() const;
    bool isQCDElectron(bool useCiCElectronID) const;

    ElectronAlgorithm::value algorithm() const;
    double ecalIsolation() const;
    double ecalIsolationPUCorrected(double rho) const;
    double hcalIsolation() const;
    double hcalIsolationPUCorrected(double rho) const;
    double trackerIsolation() const;
    double PFGammaIsolation() const;
    double PFChargedHadronIsolation() const;
    double PFNeutralHadronIsolation() const;

    double superClusterEta() const;
    double sigmaIEtaIEta() const;
    double dPhiIn() const;
    double dEtaIn() const;
    double HadOverEm() const;
    double HEEPet() const;
    double innerLayerMissingHits() const;
    double dCotThetaToClosestTrack() const;
    double distToClosestTrack() const;
    bool VBTF_W70_ElectronID() const;
    bool VBTF_W95_ElectronID() const;
    bool CiC_ElectronID(CiCElectronID::value id) const;
    bool QCD_AntiID_W70() const;
    bool QCD_AntiID_W70_Barrel() const;
    bool QCD_AntiID_W70_Endcap() const;
    bool RobustLooseID() const;
    bool RobustTightID() const;
    unsigned short getClosestJetIndex(const JetCollection& jets) const;
    ElectronAlgorithm::value getUsedAlgorithm() const;
    const TrackPointer GSFTrack() const;
    int closestCTFTrackID() const;
    double shFracInnerLayer() const;
    double ZDistanceToPrimaryVertex() const;

    void setRobustLooseID(bool id);
    void setRobustTightID(bool id);

    void setSuperClusterEta(double eta);
    void setEcalIsolation(double isolation);
    void setHcalIsolation(double isolation);
    void setTrackerIsolation(double isolation);
    void setNumberOfMissingInnerLayerHits(double missingHits);
    void setUsedAlgorithm(ElectronAlgorithm::value algo);
    void setSigmaIEtaIEta(double sigma);
    void setDPhiIn(double dphi);
    void setDEtaIn(double deta);
    void setHadOverEm(double HoverE);
    void setGSFTrack(const TrackPointer track);
    void setClosestTrackID(const int trackID);
    void setSharedFractionInnerHits(double hits);
//    void setElectronVertexZPosition(double z);
    void setZDistanceToPrimaryVertex(double dist);
    void setDistToNextTrack(double dist);
    void setDCotThetaToNextTrack(double dCotTheta);
    void setPFGammaIsolation(double pfGammaIso);
    void setPFChargedHadronIsolation(double chargedHadronIso);
    void setPFNeutralHadronIsolation(double neutralHadronIso);
    void setCompressedCiCElectronID(int electronID);

    double relativeIsolation() const;
    double relativeIsolationPUCorrected(double rho) const;
    double pfIsolation() const;

    bool isInBarrelRegion() const;
    bool isInCrack() const;
    bool isInEndCapRegion() const;

//    double vz() const;

private:
    ElectronAlgorithm::value usedAlgorithm;
    bool robustLooseId, robustTightId;
    double superCluser_Eta;
    double ecal_Isolation, hcal_Isolation, tracker_Isolation;
    double innerLayerMissingHits_;
    //used for electron ID
    double sigma_IEtaIEta, dPhi_In, dEta_In, hadOverEm;
    int CiCElectronIDCompressed;
    TrackPointer gsfTrack;
    int closesTrackID;
    double sharedFractionInnerHits;
    double zDistanceToPrimaryVertex, dCotThetaToNextTrack, distToNextTrack;
    double PFGamma_Isolation, PFChargedHadron_Isolation, PFNeutralHadron_Isolation;

    bool getVBTF_W70_ElectronID_Barrel() const;
    bool getVBTF_W70_ElectronID_Endcap() const;
    bool getVBTF_W95_ElectronID_Barrel() const;
    bool getVBTF_W95_ElectronID_Endcap() const;

};

typedef boost::shared_ptr<Electron> ElectronPointer;
typedef std::vector<ElectronPointer> ElectronCollection;

}

#endif /* ELECTRON_H_ */
