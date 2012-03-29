/*
 * EventPrinter.h
 *
 *  Created on: 22 Aug 2010
 *      Author: kreczko
 */

#ifndef EventContentPrinter_H_
#define EventContentPrinter_H_
#include "../RecoObjects/MCParticle.h"
#include "../RecoObjects/Particle.h"
#include "../RecoObjects/Electron.h"
#include "../RecoObjects/Jet.h"
#include "../RecoObjects/Muon.h"

namespace BAT {

namespace EventContentPrinter {
    extern void printJets(const JetCollection jets);
    extern void printElectrons(const ElectronCollection electrons);
    extern void printMuons(const MuonCollection muons);

    extern void printJet(const JetPointer jet);
    extern void printPFJetExtra(const JetPointer jet);
    extern void printElectron(const ElectronPointer electron);
    extern void printMuon(const MuonPointer muon);
    extern void printParticle(const ParticlePointer particle);
    extern void printGenParticle(const MCParticlePointer genParticle);
    extern void printCutFlowTable();
    extern void printWeightedCutFlowTable();
}
}
#endif /* EVENTPRINTER_H_ */
