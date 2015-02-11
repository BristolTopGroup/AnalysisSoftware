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
    quarkBarGenJetIndexReader() {

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
    quarkBarGenJetIndexReader(input, "Event.hadronicDecayQuarkBarGenJetIndex") {

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

    ttGenInfo_ = TTGenInfoPointer( new TTGenInfo( quark, quarkBar, qGenJetIndex, qBarGenJetIndex ) );

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
}

}
