#ifndef TESTELECTRONREADER_H_
#define TESTELECTRONREADER_H_
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../interface/RecoObjects/Electron.h"
#include "../interface/Readers/ElectronReader.h"
#include "../interface/Readers/NTupleEventReader.h"
#include <iostream>

using namespace BAT;

struct TestElectronReader {
private:
    boost::shared_ptr<TChain> input;
    boost::scoped_ptr<ElectronReader> electronReader;
    boost::scoped_ptr<VariableReader<MultiFloatPointer> > swissCrossReader;
    ElectronCollection electrons;
    ElectronPointer firstElectron;

public:
    TestElectronReader();

    void testReadElectronsSize();
    void testReadFirstElectronEnergy();
    void testReadFirstElectronIsIsolated();
    void testFirstElectronSigmaEtaEta();
    void testFirstElectronDPhiIn();
    void testFirstElectronDEtaIn();
    void testFirstElectronHadOverEm();
    void testFirstElectronCharge();
    void testFirstElectronD0();
    void testShFracInnerHits();
    void testCiCElectronIDVeryLooseMC();
        void testCiCElectronIDLooseMC();
        void testCiCElectronIDMediumMC();
        void testCiCElectronIDTightMC();
        void testCiCElectronIDSuperTightMC();
        void testCiCElectronIDHyperTight1MC();
        void testCiCElectronIDHyperTight2MC();
        void testCiCElectronIDHyperTight3MC();
        void testCiCElectronIDHyperTight4MC();
};
extern cute::suite make_suite_TestElectronReader();

#endif
