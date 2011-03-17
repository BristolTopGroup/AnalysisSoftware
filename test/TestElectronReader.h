#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../interface/RecoObjects/Electron.h"
#include "../interface/Readers/ElectronReader.h"
#include "../interface/Readers/NTupleEventReader.h"
#include "InputFiles.h"
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
    TestElectronReader() :
        input(new TChain(NTupleEventReader::EVENT_CHAIN)),
        electronReader(new ElectronReader(input)),
        electrons(),
        firstElectron()
    {

        input->Add(InputFile::ttbar);

        input->LoadTree(1);
        input->SetBranchStatus("*", 0);
        electronReader->initialise();
        input->GetEntry(1);
        electrons = electronReader->getElectrons();
        firstElectron = electrons.front();
    }

    void testReadElectronsSize() {
        ASSERT_EQUAL(1, electrons.size());
    }

    void testReadFirstElectronEnergy() {
        ASSERT_EQUAL_DELTA(108.714, firstElectron->energy(), 0.001);
    }

    void testReadFirstElectronIsIsolated() {
        ASSERT_EQUAL(false, firstElectron->isIsolated());
    }

    void testFirstElectronSigmaEtaEta() {
        ASSERT_EQUAL_DELTA(0.0436961, firstElectron->sigmaIEtaIEta(), 0.0000001);
    }

    void testFirstElectronDPhiIn() {
        ASSERT_EQUAL_DELTA(0.00436783, firstElectron->dPhiIn(), 0.0000001);
    }

    void testFirstElectronDEtaIn() {
        ASSERT_EQUAL_DELTA(-0.00934207, firstElectron->dEtaIn(), 0.0000001);
    }

    void testFirstElectronHadOverEm() {
        ASSERT_EQUAL_DELTA(0.0301635, firstElectron->HadOverEm(), 0.0000001);
    }

    void testFirstElectronCharge() {
        ASSERT_EQUAL(1, fabs(firstElectron->charge()));
    }

    void testFirstElectronD0() {
        ASSERT_EQUAL_DELTA(0.0725705, firstElectron->d0(), 0.0000001);
    }

//    void testEcalSpikeBranchNumberOfElectrons() {
//        ASSERT_EQUAL(numberOfElectronsReader->getVariable(), electrons.size());
//    }

    void testShFracInnerHits() {
        ASSERT_EQUAL(1, firstElectron->shFracInnerLayer());
    }

    void testGSFTrack() {
        const TrackPointer track = firstElectron->GSFTrack();
        ASSERT_EQUAL(track->charge(), firstElectron->charge());
    }

    void testGSFTrackPhi() {
        const TrackPointer track = firstElectron->GSFTrack();
        ASSERT_EQUAL_DELTA(2.56433, track->phi(), 0.00001);
    }

    void testGSFTrackEta() {
        const TrackPointer track = firstElectron->GSFTrack();
        ASSERT_EQUAL_DELTA(1.84104, track->eta(), 0.00001);
    }

    void testGSFTrackPt() {
        const TrackPointer track = firstElectron->GSFTrack();
        ASSERT_EQUAL_DELTA(13.729, track->pt(), 0.0001);
    }

    void testGSFTrackTheta() {
        const TrackPointer track = firstElectron->GSFTrack();
        ASSERT_EQUAL_DELTA(0.314682, track->theta(), 0.00001);
    }

    void testClosestCTFTrackID() {
        ASSERT_EQUAL(34, firstElectron->closestCTFTrackID());
    }

    void testTrackd0(){
        const TrackPointer track = firstElectron->GSFTrack();
        ASSERT_EQUAL(firstElectron->d0_wrtBeamSpot(), track->d0());
    }
};
extern cute::suite make_suite_TestElectronReader() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestElectronReader, testReadElectronsSize));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testReadFirstElectronEnergy));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testReadFirstElectronIsIsolated));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronSigmaEtaEta));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronDPhiIn));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronDEtaIn));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronHadOverEm));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronCharge));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testFirstElectronD0));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testShFracInnerHits));

    s.push_back(CUTE_SMEMFUN(TestElectronReader, testGSFTrack));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testGSFTrackPhi));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testGSFTrackEta));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testGSFTrackPt));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testGSFTrackTheta));

    s.push_back(CUTE_SMEMFUN(TestElectronReader, testClosestCTFTrackID));
    s.push_back(CUTE_SMEMFUN(TestElectronReader, testTrackd0));

    return s;
}
