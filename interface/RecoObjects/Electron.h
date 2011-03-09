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
#include "../Constants.h"

namespace BAT {

class Electron: public Particle {
public:

    Electron();
    Electron(float energy, float px, float py, float pz);
    virtual ~Electron();
    bool isGood(const float minEt = 30) const;
    bool isIsolated() const;
    bool isPFIsolated() const;
    bool isHEEPIsolated() const;
    bool isTaggedAsConversion(float maxDist, float maxDCotTheta) const;
    bool isFromConversion() const;
    bool isLoose() const;
    bool isQCDElectron(const float minEt = 30) const;

    ElectronAlgorithm::value algorithm() const;
    float ecalIsolation() const;
    float hcalIsolation() const;
    float trackerIsolation() const;
    float PFGammaIsolation() const;
    float PFChargedHadronIsolation() const;
    float PFNeutralHadronIsolation() const;
    float superClusterEta() const;
    float sigmaIEtaIEta() const;
    float dPhiIn() const;
    float dEtaIn() const;
    float HadOverEm() const;
    float HEEPet() const;
    bool VBTF_W70_ElectronID() const;
    bool VBTF_W95_ElectronID() const;
    bool QCD_AntiID_W70() const;
    bool QCD_AntiID_W70_Barrel() const;
    bool QCD_AntiID_W70_Endcap() const;
    bool RobustLooseID() const;
    bool RobustTightID() const;
    unsigned short getClosestJetID(const JetCollection& jets) const;
    ElectronAlgorithm::value getUsedAlgorithm() const;
    const TrackPointer GSFTrack() const;
    int closestCTFTrackID() const;
    float shFracInnerLayer() const;
    float ZDistanceToPrimaryVertex() const;

    void setRobustLooseID(bool id);
    void setRobustTightID(bool id);

    void setSuperClusterEta(float eta);
    void setEcalIsolation(float isolation);
    void setHcalIsolation(float isolation);
    void setTrackerIsolation(float isolation);
    void setNumberOfMissingInnerLayerHits(float missingHits);
    void setUsedAlgorithm(ElectronAlgorithm::value algo);
    void setSigmaIEtaIEta(float sigma);
    void setDPhiIn(float dphi);
    void setDEtaIn(float deta);
    void setHadOverEm(float HoverE);
    void setGSFTrack(const TrackPointer track);
    void setClosestTrackID(const int trackID);
    void setSharedFractionInnerHits(float hits);
//    void setElectronVertexZPosition(float z);
    void setZDistanceToPrimaryVertex(float dist);
    void setDistToNextTrack(float dist);
    void setDCotThetaToNextTrack(float dCotTheta);
    void setPFGammaIsolation(float pfGammaIso);
    void setPFChargedHadronIsolation(float chargedHadronIso);
    void setPFNeutralHadronIsolation(float neutralHadronIso);

    float relativeIsolation() const;
    float pfIsolation() const;

    bool isInBarrelRegion() const;
    bool isInCrack() const;
    bool isInEndCapRegion() const;

//    float vz() const;

private:
    ElectronAlgorithm::value usedAlgorithm;
    bool robustLooseId, robustTightId;
    float superCluser_Eta;
    float ecal_Isolation, hcal_Isolation, tracker_Isolation;
    float innerLayerMissingHits;
    //used for HEEP
    float sigma_IEtaIEta, dPhi_In, dEta_In, hadOverEm;
    TrackPointer gsfTrack;
    int closesTrackID;
    float sharedFractionInnerHits;
    float zDistanceToPrimaryVertex, dCotThetaToNextTrack, distToNextTrack;
    float PFGamma_Isolation, PFChargedHadron_Isolation, PFNeutralHadron_Isolation;


    bool getVBTF_W70_ElectronID_Barrel() const;
    bool getVBTF_W70_ElectronID_Endcap() const;
    bool getVBTF_W95_ElectronID_Barrel() const;
    bool getVBTF_W95_ElectronID_Endcap() const;

};

typedef boost::shared_ptr<Electron> ElectronPointer;
typedef std::vector<ElectronPointer> ElectronCollection;

}

#endif /* ELECTRON_H_ */
