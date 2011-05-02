/*
 * ElectronReader.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/Readers/ElectronReader.h"
namespace BAT {

//const std::string ElectronReader::algorithmPrefixes[Electron::NUMBER_OF_ELECTRONALGORITHMS] = { "els", "PFElsAll" };

ElectronReader::ElectronReader() :
//    numberOfElectronsReader(),
    energyReader(),
    pxReader(),
    pyReader(),
    pzReader(),
    chargeReader(),
    superClusterEtaReader(),
    d0_BS_Reader(),
    d0_PV_Reader(),
    numberOfInnerLayerMissingHitsReader(),
    ecalIsolationReader(),
    hcalIsolationReader(),
    trackerIsolationReader(),
    PFGammaIsolationReader(),
    PFChargedHadronIsolationReader(),
    PFNeutralHadronIsolationReader(),
//    robustLooseIDReader(),
//    robustTightIDReader(),
    sigmaIEtaIEtaReader(),
    dPhiInReader(),
    dEtaInReader(),
    hadOverEmReader(),
    sharedFractionInnerHits(),
    trackIDReader(),
    track_phi(),
    track_eta(),
    track_pt(),
    track_theta(),
    track_charge(),
    vertex_dist_z(),
    dist(),
    dCotTheta(),
    algorithm(ElectronAlgorithm::Calo),
    electrons() {

}

ElectronReader::ElectronReader(TChainPointer input, ElectronAlgorithm::value algo) :
//    numberOfElectronsReader(input, "N" + ElectronAlgorithm::prefixes.at(algo)),
    energyReader(input, ElectronAlgorithm::prefixes.at(algo) + ".Energy"),
    pxReader(input, ElectronAlgorithm::prefixes.at(algo) + ".Px"),
    pyReader(input, ElectronAlgorithm::prefixes.at(algo) + ".Py"),
    pzReader(input, ElectronAlgorithm::prefixes.at(algo) + ".Pz"),
    chargeReader(input, ElectronAlgorithm::prefixes.at(algo) + ".Charge"),
    superClusterEtaReader(input, ElectronAlgorithm::prefixes.at(algo) + ".SCEta"),
    d0_BS_Reader(input, ElectronAlgorithm::prefixes.at(algo) + ".dBBS"),
    d0_PV_Reader(input, ElectronAlgorithm::prefixes.at(algo) + ".dB"),
    numberOfInnerLayerMissingHitsReader(input, ElectronAlgorithm::prefixes.at(algo) + ".MissingHits"),
    ecalIsolationReader(input, ElectronAlgorithm::prefixes.at(algo) + ".EcalIso03"),
    hcalIsolationReader(input, ElectronAlgorithm::prefixes.at(algo) + ".HcalIso03"),
    trackerIsolationReader(input,ElectronAlgorithm::prefixes.at(algo) + ".TrkIso03"),
    PFGammaIsolationReader(input,ElectronAlgorithm::prefixes.at(algo) + ".PFGammaIso"),
    PFChargedHadronIsolationReader(input,ElectronAlgorithm::prefixes.at(algo) + ".PfChargedHadronIso"),
    PFNeutralHadronIsolationReader(input,ElectronAlgorithm::prefixes.at(algo) + ".PfNeutralHadronIso"),
//    robustLooseIDReader(input, ElectronAlgorithm::prefixes.at(algo) + ".robustLooseId"),
//    robustTightIDReader(input, ElectronAlgorithm::prefixes.at(algo) + ".robustTightId"),
    sigmaIEtaIEtaReader(input, ElectronAlgorithm::prefixes.at(algo) + ".SigmaIEtaIEta"),
    dPhiInReader(input, ElectronAlgorithm::prefixes.at(algo) + ".DeltaPhiTrkSC"),
    dEtaInReader(input, ElectronAlgorithm::prefixes.at(algo) + ".DeltaEtaTrkSC"),
    hadOverEmReader(input, ElectronAlgorithm::prefixes.at(algo) + ".HoE"),
    sharedFractionInnerHits(input, ElectronAlgorithm::prefixes.at(algo) + ".shFracInnerHits"),
    trackIDReader(input, ElectronAlgorithm::prefixes.at(algo) + ".closestCtfTrackRef"),
    track_phi(input, ElectronAlgorithm::prefixes.at(algo) + ".GSFTrack.Phi"),
    track_eta(input, ElectronAlgorithm::prefixes.at(algo) + ".GSFTrack.Eta"),
    track_pt(input, ElectronAlgorithm::prefixes.at(algo) + ".GSFTrack.Pt"),
    track_theta(input, ElectronAlgorithm::prefixes.at(algo) + ".GSFTrack.Theta"),
    track_charge(input, ElectronAlgorithm::prefixes.at(algo) + ".GSFTrack.Charge"),
    vertex_dist_z(input, ElectronAlgorithm::prefixes.at(algo) + ".VtxDistZ"),
    dist(input, ElectronAlgorithm::prefixes.at(algo) + ".Dist"),
    dCotTheta(input, ElectronAlgorithm::prefixes.at(algo) + ".DCotTheta"),
    algorithm(algo),
    electrons() {

}

ElectronReader::~ElectronReader() {
}

const ElectronCollection& ElectronReader::getElectrons() {
    if (electrons.empty() == false)
        electrons.clear();
    readElectrons();
    return electrons;
}

void ElectronReader::readElectrons() {
//    unsigned int numberOfElectrons = numberOfElectronsReader.getVariable();
    for (unsigned int index = 0; index < energyReader.size(); index++) {
        float energy = energyReader.getVariableAt(index);
        float px = pxReader.getVariableAt(index);
        float py = pyReader.getVariableAt(index);
        float pz = pzReader.getVariableAt(index);
        ElectronPointer electron(new Electron(energy, px, py, pz));
        electron->setUsedAlgorithm(algorithm);
        electron->setCharge(chargeReader.getIntVariableAt(index));
        if(d0_BS_Reader.doesVariableExist() && algorithm == ElectronAlgorithm::Calo)
            electron->setD0_wrtBeamSpot(d0_BS_Reader.getVariableAt(index));
        electron->setD0(d0_PV_Reader.getVariableAt(index));
        electron->setZDistanceToPrimaryVertex(vertex_dist_z.getVariableAt(index));
        electron->setNumberOfMissingInnerLayerHits(numberOfInnerLayerMissingHitsReader.getIntVariableAt(index));
        electron->setEcalIsolation(ecalIsolationReader.getVariableAt(index));
        electron->setHcalIsolation(hcalIsolationReader.getVariableAt(index));
        electron->setTrackerIsolation(trackerIsolationReader.getVariableAt(index));
        electron->setSuperClusterEta(superClusterEtaReader.getVariableAt(index));
        electron->setSigmaIEtaIEta(sigmaIEtaIEtaReader.getVariableAt(index));
        electron->setDPhiIn(dPhiInReader.getVariableAt(index));
        electron->setDEtaIn(dEtaInReader.getVariableAt(index));
        electron->setHadOverEm(hadOverEmReader.getVariableAt(index));
//        electron->setRobustLooseID(robustLooseIDReader.getVariableAt(index) == 1);
//        electron->setRobustTightID(robustTightIDReader.getVariableAt(index) == 1);
        electron->setDistToNextTrack(dist.getVariableAt(index));
        electron->setDCotThetaToNextTrack(dCotTheta.getVariableAt(index));

        electron->setSharedFractionInnerHits(sharedFractionInnerHits.getVariableAt(index));
        electron->setClosestTrackID(trackIDReader.getIntVariableAt(index));
        float trackPhi = track_phi.getVariableAt(index);
        float trackEta = track_eta.getVariableAt(index);
        float trackPt = track_pt.getVariableAt(index);
        float trackTheta = track_theta.getVariableAt(index);
        float trackCharge = track_charge.getIntVariableAt(index);
        TrackPointer track = TrackPointer(new Track(trackPhi, trackEta, trackPt, trackTheta));
        track->setCharge(trackCharge);
        if(algorithm == ElectronAlgorithm::Calo)
            track->setD0(electron->d0_wrtBeamSpot());
        else
            track->setD0(electron->d0());

        if(algorithm == ElectronAlgorithm::ParticleFlow){
            electron->setPFGammaIsolation(PFGammaIsolationReader.getVariableAt(index));
            electron->setPFChargedHadronIsolation(PFChargedHadronIsolationReader.getVariableAt(index));
            electron->setPFNeutralHadronIsolation(PFNeutralHadronIsolationReader.getVariableAt(index));
        }
        electron->setGSFTrack(track);
        electrons.push_back(electron);
    }
}

void ElectronReader::initialise() {
//    numberOfElectronsReader.initialise();
    energyReader.initialise();
    pxReader.initialise();
    pyReader.initialise();
    pzReader.initialise();
    chargeReader.initialise();
    superClusterEtaReader.initialise();

    if(d0_BS_Reader.doesVariableExist() && algorithm == ElectronAlgorithm::Calo)
        d0_BS_Reader.initialise();
    d0_PV_Reader.initialise();
    numberOfInnerLayerMissingHitsReader.initialise();

    ecalIsolationReader.initialise();
    hcalIsolationReader.initialise();
    trackerIsolationReader.initialise();

//    robustLooseIDReader.initialise();
//    robustTightIDReader.initialise();
    sigmaIEtaIEtaReader.initialise();
    dPhiInReader.initialise();
    dEtaInReader.initialise();
    hadOverEmReader.initialise();

    sharedFractionInnerHits.initialise();
    trackIDReader.initialise();
    track_phi.initialise();
    track_eta.initialise();
    track_pt.initialise();
    track_theta.initialise();
    track_charge.initialise();
    vertex_dist_z.initialise();
    dist.initialise();
    dCotTheta.initialise();
    if(algorithm == ElectronAlgorithm::ParticleFlow){
        PFGammaIsolationReader.initialise();
        PFChargedHadronIsolationReader.initialise();
        PFNeutralHadronIsolationReader.initialise();
    }
}

}

