#include "cute/cute.h"
#include "cute/cute_suite.h"

#include "../interface/Readers/METReader.h"
#include "../interface/RecoObjects/MET.h"
#include "InputFiles.h"
using namespace BAT;

struct TestMETReader {
private:
    boost::shared_ptr<TChain> input;
    boost::scoped_ptr<METReader> reader, PFreader;
    METPointer met, pfmet;

public:
    TestMETReader() :
        input(new TChain(NTupleEventReader::EVENT_CHAIN)),
        reader(new METReader(input)),
        PFreader(new METReader(input, METAlgorithm::ParticleFlowMET)),
        met(),
        pfmet() {
        input->Add(InputFile::ttbar);
        input->SetBranchStatus("*", 0);
        reader->initialise();
        PFreader->initialise();
        input->GetEntry(0);
        met = reader->getMET();
        pfmet = PFreader->getMET();
    }

    void testMET() {
        ASSERT_EQUAL_DELTA(69.2572, met->et(), 0.0001);
    }

    void testPFMET() {
        ASSERT_EQUAL_DELTA(86.9846, pfmet->et(), 0.0001);
    }


    void testAreMETsGood(){
        ASSERT_EQUAL(true, met->isGood());
        ASSERT_EQUAL(true, pfmet->isGood());
    }

};

extern cute::suite make_suite_TestMETReader() {
    cute::suite s;

    s.push_back(CUTE_SMEMFUN(TestMETReader, testMET));
    s.push_back(CUTE_SMEMFUN(TestMETReader, testPFMET));
    s.push_back(CUTE_SMEMFUN(TestMETReader, testAreMETsGood));

    return s;
}
