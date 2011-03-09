#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/Readers/METReader.h"
#include "../interface/RecoObjects/MET.h"
#include "InputFiles.h"
using namespace BAT;

struct TestMETReader {
private:
    boost::shared_ptr<TChain> input;
    boost::scoped_ptr<METReader> reader, PFreader;//, TCreader, KT4reader, KT6Reader;
    METPointer met, pfmet;//, tcmet;//, kt4met, kt6met;
public:
    TestMETReader() :
        input(new TChain(NTupleEventReader::EVENT_CHAIN)),
        reader(new METReader(input)),
        PFreader(new METReader(input, METAlgorithm::ParticleFlowMET)),
//        TCreader(new METReader(input, METAlgorithm::tcMET)),
//        KT4reader(new METReader(input, MET::KT4)),
//        KT6Reader(new METReader(input, MET::KT6)),
        met(),
        pfmet() {
        input->Add(InputFile::ttbar);
        input->SetBranchStatus("*", 0);
        reader->initialise();
        PFreader->initialise();
//        TCreader->initialise();
//        KT4reader->initialise();
//        KT6Reader->initialise();
        input->GetEntry(0);
        met = reader->getMET();
        pfmet = PFreader->getMET();
//        tcmet = TCreader->getMET();
//        kt4met = KT4reader->getMET();
//        kt6met = KT6Reader->getMET();
    }

    void testMET() {
        ASSERT_EQUAL_DELTA(69.2572, met->et(), 0.0001);
    }

    void testPFMET() {
        ASSERT_EQUAL_DELTA(86.9846, pfmet->et(), 0.0001);
    }

//    void testTCMET() {
//        ASSERT_EQUAL_DELTA(40.0313, tcmet->et(), 0.0001);
//    }

//    void testKT4MET() {
//        ASSERT_EQUAL_DELTA(47.1134, kt4met->et(), 0.001);
//    }
//
//    void testKT6MET() {
//        ASSERT_EQUAL_DELTA(48.828, kt6met->et(), 0.001);
//    }

    void testAreMETsGood(){
        ASSERT_EQUAL(true, met->isGood());
        ASSERT_EQUAL(true, pfmet->isGood());
//        ASSERT_EQUAL(true, tcmet->isGood());
//        ASSERT_EQUAL(true, kt4met->isGood());
//        ASSERT_EQUAL(true, kt6met->isGood());
    }

};

extern cute::suite make_suite_TestMETReader() {
    cute::suite s;
    s.push_back(CUTE_SMEMFUN(TestMETReader, testMET));
    s.push_back(CUTE_SMEMFUN(TestMETReader, testPFMET));
//    s.push_back(CUTE_SMEMFUN(TestMETReader, testTCMET));
//    s.push_back(CUTE_SMEMFUN(TestMETReader, testKT4MET));
//    s.push_back(CUTE_SMEMFUN(TestMETReader, testKT6MET));
    s.push_back(CUTE_SMEMFUN(TestMETReader, testAreMETsGood));
    return s;
}
