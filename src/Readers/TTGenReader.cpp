/*
 * TTGenReader.cpp
 *
 *  Created on: 6 Feb 2015
 *      Author: ejclemen
 */

#include "../../interface/Readers/TTGenReader.h"

namespace BAT {

TTGenReader::TTGenReader() :
    quarkPxReader(),
    quarkPyReader(),
    quarkPzReader(),
    quarkEReader(),
    quarkGenJetIndexReader(),
    quarkBarPxReader(),
    quarkBarPyReader(),
    quarkBarPzReader(),
    quarkBarEReader(),
    quarkBarGenJetIndexReader(), 
    leptonicBPxReader(),
    leptonicBPyReader(),
    leptonicBPzReader(),
    leptonicBEReader(),
    leptonicBGenJetIndexReader(),
    hadronicBPxReader(),
    hadronicBPyReader(),
    hadronicBPzReader(),
    hadronicBEReader(),
    hadronicBGenJetIndexReader(),
    singleleptonPxReader(),
    singleleptonPyReader(),
    singleleptonPzReader(),
    singleleptonEReader(),
    singleneutrinoPxReader(),
    singleneutrinoPyReader(),
    singleneutrinoPzReader(),
    singleneutrinoEReader() {

}

TTGenReader::TTGenReader(TChainPointer input) :
    quarkPxReader(input, "Event.hadronicdecayquarkPx"),
    quarkPyReader(input, "Event.hadronicdecayquarkPy"),
    quarkPzReader(input, "Event.hadronicdecayquarkPz"),
    quarkEReader(input, "Event.hadronicdecayquarkEnergy"),
    quarkGenJetIndexReader(input, "Event.hadronicDecayQuarkGenJetIndex"),
    quarkBarPxReader(input, "Event.hadronicdecayquarkbarPx"),
    quarkBarPyReader(input, "Event.hadronicdecayquarkbarPy"),
    quarkBarPzReader(input, "Event.hadronicdecayquarkbarPz"),
    quarkBarEReader(input, "Event.hadronicdecayquarkbarEnergy"),
    quarkBarGenJetIndexReader(input, "Event.hadronicDecayQuarkBarGenJetIndex"), 
    leptonicBPxReader(input, "Event.leptonicBPx"),
    leptonicBPyReader(input, "Event.leptonicBPy"),
    leptonicBPzReader(input, "Event.leptonicBPz"),
    leptonicBEReader(input, "Event.leptonicBEnergy"),
    leptonicBGenJetIndexReader(input, "Event.leptonicBGenJetIndex"), 
    hadronicBPxReader(input, "Event.hadronicBPx"),
    hadronicBPyReader(input, "Event.hadronicBPy"),
    hadronicBPzReader(input, "Event.hadronicBPz"),
    hadronicBEReader(input, "Event.hadronicBEnergy"),
    hadronicBGenJetIndexReader(input, "Event.hadronicBGenJetIndex"),
    singleleptonPxReader(input, "Event.SingleLeptonPx"),
    singleleptonPyReader(input, "Event.SingleLeptonPy"),
    singleleptonPzReader(input, "Event.SingleLeptonPz"),
    singleleptonEReader(input, "Event.SingleLeptonEnergy"),
    singleneutrinoPxReader(input, "Event.SingleNeutrinoPx"),
    singleneutrinoPyReader(input, "Event.SingleNeutrinoPy"),
    singleneutrinoPzReader(input, "Event.SingleNeutrinoPz"),
    singleneutrinoEReader(input, "Event.SingleNeutrinoEnergy") {

}

TTGenReader::~TTGenReader() {
}

const TTGenInfoPointer& TTGenReader::getTTGenInfo() {
    readTTGenInfo();
    return ttGenInfo_;
}

void TTGenReader::readTTGenInfo() {

    // Make quark particle
    double quarkPx = quarkPxReader.getVariable();
    double quarkPy = quarkPyReader.getVariable();
    double quarkPz = quarkPzReader.getVariable();
    double quarkE = quarkEReader.getVariable();
    ParticlePointer quark(new Particle(quarkE, quarkPx, quarkPy, quarkPz));

    // Get index of gen jet associated with quark parton
    int qGenJetIndex = quarkGenJetIndexReader.getVariable();

    // Make quark bar particle
    double quarkBarPx = quarkBarPxReader.getVariable();
    double quarkBarPy = quarkBarPyReader.getVariable();
    double quarkBarPz = quarkBarPzReader.getVariable();
    double quarkBarE = quarkBarEReader.getVariable();
    ParticlePointer quarkBar(new Particle(quarkBarE, quarkBarPx, quarkBarPy, quarkBarPz));

    // Get index of gen jet associated with quark bar parton
    int qBarGenJetIndex = quarkBarGenJetIndexReader.getVariable();

    // Make leptonic b quark particle
    double leptonicBPx = leptonicBPxReader.getVariable();
    double leptonicBPy = leptonicBPyReader.getVariable();
    double leptonicBPz = leptonicBPzReader.getVariable();
    double leptonicBE = leptonicBEReader.getVariable();
    ParticlePointer leptonicB(new Particle(leptonicBE, leptonicBPx, leptonicBPy, leptonicBPz));

    // Get index of gen jet associated with leptonic B
    int leptonicBGenJetIndex = leptonicBGenJetIndexReader.getVariable();

    // Make hadronic b quark particle
    double hadronicBPx = hadronicBPxReader.getVariable();
    double hadronicBPy = hadronicBPyReader.getVariable();
    double hadronicBPz = hadronicBPzReader.getVariable();
    double hadronicBE = hadronicBEReader.getVariable();
    ParticlePointer hadronicB(new Particle(hadronicBE, hadronicBPx, hadronicBPy, hadronicBPz));

    // Get index of gen jet associated with hadronic B
    int hadronicBGenJetIndex = hadronicBGenJetIndexReader.getVariable();

    // Make Single Lepton particle
    double singleleptonPx = singleleptonPxReader.getVariable();
    double singleleptonPy = singleleptonPyReader.getVariable();
    double singleleptonPz = singleleptonPzReader.getVariable();
    double singleleptonE = singleleptonEReader.getVariable();
    ParticlePointer singlelepton(new Particle(singleleptonE, singleleptonPx, singleleptonPy, singleleptonPz));
    // Make Single Neutrino particle
    double singleneutrinoPx = singleneutrinoPxReader.getVariable();
    double singleneutrinoPy = singleneutrinoPyReader.getVariable();
    double singleneutrinoPz = singleneutrinoPzReader.getVariable();
    double singleneutrinoE = singleneutrinoEReader.getVariable();
    ParticlePointer singleneutrino(new Particle(singleneutrinoE, singleneutrinoPx, singleneutrinoPy, singleneutrinoPz));

    ttGenInfo_ = TTGenInfoPointer( new TTGenInfo( quark, quarkBar, leptonicB, hadronicB, singlelepton, singleneutrino, qGenJetIndex, qBarGenJetIndex, leptonicBGenJetIndex, hadronicBGenJetIndex ) );

}

void TTGenReader::initialise() {

    quarkPxReader.initialise();
    quarkPyReader.initialise();
    quarkPzReader.initialise();
    quarkEReader.initialise();
    quarkGenJetIndexReader.initialise();
    quarkBarPxReader.initialise();
    quarkBarPyReader.initialise();
    quarkBarPzReader.initialise();
    quarkBarEReader.initialise();
    quarkBarGenJetIndexReader.initialise();
    leptonicBPxReader.initialise();
    leptonicBPyReader.initialise();
    leptonicBPzReader.initialise();
    leptonicBEReader.initialise();
    leptonicBGenJetIndexReader.initialise();
    hadronicBPxReader.initialise();
    hadronicBPyReader.initialise();
    hadronicBPzReader.initialise();
    hadronicBEReader.initialise();
    hadronicBGenJetIndexReader.initialise();
    singleleptonPxReader.initialise();
    singleleptonPyReader.initialise();
    singleleptonPzReader.initialise();
    singleleptonEReader.initialise();
    singleneutrinoPxReader.initialise();
    singleneutrinoPyReader.initialise();
    singleneutrinoPzReader.initialise();
    singleneutrinoEReader.initialise();
}

}
