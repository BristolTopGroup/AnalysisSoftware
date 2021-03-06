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
    singleneutrinoEReader(),
    leptonicWPxReader(),
    leptonicWPyReader(),
    leptonicWPzReader(),
    leptonicWEReader(),
    leptonicTopPxReader(),
    leptonicTopPyReader(),
    leptonicTopPzReader(),
    leptonicTopEReader(),
    hadronicTopPxReader(),
    hadronicTopPyReader(),
    hadronicTopPzReader(),
    hadronicTopEReader()
{

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
    singleneutrinoEReader(input, "Event.SingleNeutrinoEnergy"),

    leptonicWPxReader(input, "Event.leptonicWPx"),
    leptonicWPyReader(input, "Event.leptonicWPy"),
    leptonicWPzReader(input, "Event.leptonicWPz"),
    leptonicWEReader(input, "Event.leptonicWEnergy"),

    leptonicTopPxReader(input, "Event.leptonicTopPx"),
    leptonicTopPyReader(input, "Event.leptonicTopPy"),
    leptonicTopPzReader(input, "Event.leptonicTopPz"),
    leptonicTopEReader(input, "Event.leptonicTopEnergy"),

    hadronicTopPxReader(input, "Event.hadronicTopPx"),
    hadronicTopPyReader(input, "Event.hadronicTopPy"),
    hadronicTopPzReader(input, "Event.hadronicTopPz"),
    hadronicTopEReader(input, "Event.hadronicTopEnergy"){

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

    // Leptonic W
    double leptonicWPx = leptonicWPxReader.getVariable();
    double leptonicWPy = leptonicWPyReader.getVariable();
    double leptonicWPz = leptonicWPzReader.getVariable();
    double leptonicWE = leptonicWEReader.getVariable();
    ParticlePointer leptonicW(new Particle(leptonicWE, leptonicWPx, leptonicWPy, leptonicWPz));    

    // Leptonic Top
    double leptonicTopPx = leptonicTopPxReader.getVariable();
    double leptonicTopPy = leptonicTopPyReader.getVariable();
    double leptonicTopPz = leptonicTopPzReader.getVariable();
    double leptonicTopE = leptonicTopEReader.getVariable();
    ParticlePointer leptonicTop(new Particle(leptonicTopE, leptonicTopPx, leptonicTopPy, leptonicTopPz));    

    // Hadronic Top
    double hadronicTopPx = hadronicTopPxReader.getVariable();
    double hadronicTopPy = hadronicTopPyReader.getVariable();
    double hadronicTopPz = hadronicTopPzReader.getVariable();
    double hadronicTopE = hadronicTopEReader.getVariable();
    ParticlePointer hadronicTop(new Particle(hadronicTopE, hadronicTopPx, hadronicTopPy, hadronicTopPz));   

    ttGenInfo_ = TTGenInfoPointer( new TTGenInfo( leptonicTop, hadronicTop, leptonicW, quark, quarkBar, leptonicB, hadronicB, singlelepton, singleneutrino, qGenJetIndex, qBarGenJetIndex, leptonicBGenJetIndex, hadronicBGenJetIndex ) );
}

void TTGenReader::initialise() {

    quarkPxReader.initialiseBlindly();
    quarkPyReader.initialiseBlindly();
    quarkPzReader.initialiseBlindly();
    quarkEReader.initialiseBlindly();
    quarkGenJetIndexReader.initialiseBlindly();
    quarkBarPxReader.initialiseBlindly();
    quarkBarPyReader.initialiseBlindly();
    quarkBarPzReader.initialiseBlindly();
    quarkBarEReader.initialiseBlindly();
    quarkBarGenJetIndexReader.initialiseBlindly();
    leptonicBPxReader.initialiseBlindly();
    leptonicBPyReader.initialiseBlindly();
    leptonicBPzReader.initialiseBlindly();
    leptonicBEReader.initialiseBlindly();
    leptonicBGenJetIndexReader.initialiseBlindly();
    hadronicBPxReader.initialiseBlindly();
    hadronicBPyReader.initialiseBlindly();
    hadronicBPzReader.initialiseBlindly();
    hadronicBEReader.initialiseBlindly();
    hadronicBGenJetIndexReader.initialiseBlindly();
    singleleptonPxReader.initialiseBlindly();
    singleleptonPyReader.initialiseBlindly();
    singleleptonPzReader.initialiseBlindly();
    singleleptonEReader.initialiseBlindly();
    singleneutrinoPxReader.initialiseBlindly();
    singleneutrinoPyReader.initialiseBlindly();
    singleneutrinoPzReader.initialiseBlindly();
    singleneutrinoEReader.initialiseBlindly();

    leptonicWPxReader.initialiseBlindly();
    leptonicWPyReader.initialiseBlindly();
    leptonicWPzReader.initialiseBlindly();
    leptonicWEReader.initialiseBlindly();

    leptonicTopPxReader.initialiseBlindly();
    leptonicTopPyReader.initialiseBlindly();
    leptonicTopPzReader.initialiseBlindly();
    leptonicTopEReader.initialiseBlindly();
    hadronicTopPxReader.initialiseBlindly();
    hadronicTopPyReader.initialiseBlindly();
    hadronicTopPzReader.initialiseBlindly();
    hadronicTopEReader.initialiseBlindly();
}

}
