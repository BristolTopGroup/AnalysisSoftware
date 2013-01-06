/*
 * Electron.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef ELECTRON_H_
#define ELECTRON_H_
#include "Lepton.h"
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

namespace ElectronID {
enum value {
	SimpleCutBasedWP95 = -9,
	SimpleCutBasedWP80 = -8,
	SimpleCutBasedWP70 = -7,
	MVAIDTrigger = -6,
	MVAIDNonTrigger = -5
};



}  // namespace ElectronID

class Electron: public Lepton {
public:

    Electron();
    Electron(double energy, double px, double py, double pz);
    virtual ~Electron();
//    bool isGood(short leptonID) const;
    bool isHEEPIsolated() const;
//    bool isTaggedAsConversion(double maxDist = 0.02, double maxDCotTheta = 0.02) const;
//    bool isFromConversion() const;
    bool passConversionVeto() const;
//    bool isLoose() const;
//    bool isQCDElectron(short) const;

    ElectronAlgorithm::value algorithm() const;
    double ecalIsolationPUCorrected(double rho) const;
    double hcalIsolationPUCorrected(double rho) const;

    double superClusterEta() const;
    double sigmaIEtaIEta() const;
    double dPhiIn() const;
    double dEtaIn() const;
    double HadOverEm() const;
    double HEEPet() const;
    double innerLayerMissingHits() const;
    double dCotThetaToClosestTrack() const;
    double distToClosestTrack() const;
    bool VBTF_WP70_ElectronID() const;
    bool VBTF_WP95_ElectronID() const;
    bool QCD_AntiID_WP70() const;
    bool QCD_AntiID_WP70_Barrel() const;
    bool QCD_AntiID_WP70_Endcap() const;
    bool RobustLooseID() const;
    bool RobustTightID() const;
    bool passesElectronID(short leptonID) const;
    unsigned short getClosestJetIndex(const JetCollection& jets) const;
    ElectronAlgorithm::value getUsedAlgorithm() const;
    const TrackPointer GSFTrack() const;
    int closestCTFTrackID() const;
    double shFracInnerLayer() const;
    double mvaTrigV0() const;
    double mvaNonTrigV0() const;

    void setRobustLooseID(bool id);
    void setRobustTightID(bool id);

    void setSuperClusterEta(double eta);
    void setNumberOfMissingInnerLayerHits(double missingHits);
    void setUsedAlgorithm(ElectronAlgorithm::value algo);
    void setSigmaIEtaIEta(double sigma);
    void setDPhiIn(double dphi);
    void setDEtaIn(double deta);
    void setHadOverEm(double HoverE);
    void setGSFTrack(const TrackPointer track);
    void setClosestTrackID(const int trackID);
    void setSharedFractionInnerHits(double hits);
    void setDistToNextTrack(double dist);
    void setDCotThetaToNextTrack(double dCotTheta);
    void setMVATrigV0(double mva);
    void setMVANonTrigV0(double mva);
    void setPassConversionVeto(bool passes);

    double relativeIsolation() const;
    double relativeIsolationPUCorrected(double rho) const;
    virtual double pfRelativeIsolation(double coneSize = 0.3, bool deltaBetaCorrection = true) const;
    virtual double pfRelativeIsolationRhoCorrected() const;

    virtual bool isInBarrelRegion() const;
    bool isInCrack() const;
    bool isInEndCapRegion() const;
    bool isPFLepton() const;
    virtual std::string toString() const;

private:
    ElectronAlgorithm::value usedAlgorithm_;
    bool robustLooseId, robustTightId;
    double superCluser_Eta;
    double innerLayerMissingHits_;
    //used for electron ID
    double sigma_IEtaIEta, dPhi_In, dEta_In, hadOverEm;
    TrackPointer gsfTrack;
    int closesTrackID;
    double sharedFractionInnerHits;
    double dCotThetaToNextTrack_, distToNextTrack_;
    double mvaTrigV0_, mvaNonTrigV0_;
    bool passConversionVeto_;

    bool getVBTF_WP70_ElectronID_Barrel() const;
    bool getVBTF_WP70_ElectronID_Endcap() const;
    bool getVBTF_WP95_ElectronID_Barrel() const;
    bool getVBTF_WP95_ElectronID_Endcap() const;

};

typedef boost::shared_ptr<Electron> ElectronPointer;
typedef std::vector<ElectronPointer> ElectronCollection;

}

#endif /* ELECTRON_H_ */
