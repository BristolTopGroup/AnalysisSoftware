#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>

#include "../interface/Event.h"
#include "../interface/OnlyForTests/DummyTTbarEvent.h"
#include "TestObjectFactory.h"
#include <iostream>

using namespace BAT;
using namespace std;

struct TestTopPairEventCandidate {
    DummyTTbarEvent ttbarEvent, goodZEvent, poorZEvent, DiJetEvent, DiJetEventWithConversion, muonEvent;
    DummyTTbarEvent emptyEvent, customEvent;

    ElectronPointer goodIsolatedElectron, goodIsolatedElectron2;
    ElectronPointer goodLooseElectron;
    ElectronPointer badElectron;
    ElectronPointer electronFromConversion;
    JetPointer goodJet, goodBJet, badJet, goodJetCloseToElectron;
    VertexPointer goodVertex;
    VertexPointer badVertex;
    MuonPointer goodIsolatedMuon;
    MuonPointer badMuon;
    METPointer met;
    TrackCollection lessThan10Tracks;
    TrackCollection moreThan10TracksHighPurity;
    TrackCollection moreThan10TracksLowPurity;
    TrackCollection moreThan10TracksMixedPurity_H;
    TrackCollection moreThan10TracksMixedPurity_L;

    TestTopPairEventCandidate() :
        ttbarEvent(),
        goodZEvent(),
        poorZEvent(),
        DiJetEvent(),
        DiJetEventWithConversion(),
        muonEvent(),
        emptyEvent(),
        customEvent(),
        goodIsolatedElectron(TestObjectFactory::goodIsolatedElectron()),
        goodIsolatedElectron2(TestObjectFactory::goodIsolatedElectron2()),
        goodLooseElectron(new Electron(100., 79., -13., -5.)),
        badElectron(new Electron(20, 14., 15., 0)),
        electronFromConversion(new Electron(*goodIsolatedElectron)),
        goodJet(new Jet(100, 13, 99, 5)),
        goodBJet(new Jet(*goodJet)),
        badJet(new Jet(20, 19, 0, 0)),
        goodJetCloseToElectron(new Jet(100., 98., 13., 5.)),
        goodVertex(TestObjectFactory::goodVertex()),
        badVertex(TestObjectFactory::badFakeVertex()),
        goodIsolatedMuon(new Muon(100., 99., 13., 5.)),
        badMuon(new Muon(100., 99., 13., 5.)),
        met(new MET(40, 30)) {
        setUpGoodLooseElectron();
        setUpBadElectron();
        setUpGoodIsolatedElectronFromConversion();
        setUpGoodJet();
        setUpGoodBJet();
        setUpGoodJetCloseToElectron();
        setUpIsolatedGoodMuon();
        setUpBadMuon();
        setUpTracks();

        setUpTTbarEvent();
        setUpGoodZEvent();
        setUpPoorZEvent();
        setUpDiJetEvent();
        setUpMuonEvent();
        setUpCustomEvent();
    }

private:

    void setUpBadElectron() {
        badElectron->setHcalIsolation(4);
        badElectron->setEcalIsolation(44);
        badElectron->setTrackerIsolation(1);
        badElectron->setZDistanceToPrimaryVertex(0);
    }

    void setUpGoodLooseElectron() {
        goodLooseElectron->setHcalIsolation(5);
        goodLooseElectron->setEcalIsolation(3);
        goodLooseElectron->setTrackerIsolation(4);
        goodLooseElectron->setSigmaIEtaIEta(0.009);
        goodLooseElectron->setDPhiIn(0.7);
        goodLooseElectron->setDEtaIn(0.006);
        goodLooseElectron->setHadOverEm(0.14);
        goodLooseElectron->setRobustLooseID(true);
        goodLooseElectron->setNumberOfMissingInnerLayerHits(0);
        goodLooseElectron->setSuperClusterEta(0);
        goodLooseElectron->setZDistanceToPrimaryVertex(0);
        TrackPointer track = TrackPointer(new Track(goodLooseElectron->phi(), goodLooseElectron->eta(),
                goodLooseElectron->pt(), goodLooseElectron->theta()));
        track->setD0(goodLooseElectron->d0_wrtBeamSpot());
        track->setCharge(goodLooseElectron->charge());
        assert(goodLooseElectron->isGood() == false);
        assert(goodLooseElectron->isLoose() && goodLooseElectron->relativeIsolation() < 1.);
    }

    void setUpGoodIsolatedElectronFromConversion() {
        electronFromConversion->setHcalIsolation(0.5);
        electronFromConversion->setEcalIsolation(0.3);
        electronFromConversion->setTrackerIsolation(0.4);
        electronFromConversion->setNumberOfMissingInnerLayerHits(3);
        electronFromConversion->setZDistanceToPrimaryVertex(0);
    }
    void setUpGoodJet() {
        goodJet->setEMF(0.2);
        goodJet->setFHPD(0.5);
        goodJet->setN90Hits(2);
    }

    void setUpGoodBJet() {
        goodBJet->setEMF(0.2);
        goodBJet->setFHPD(0.5);
        goodBJet->setN90Hits(2);
        goodBJet->setDiscriminatorForBtagType(2.5, BtagAlgorithm::SimpleSecondaryVertexHighEffBTag);
    }

    void setUpGoodJetCloseToElectron() {
        goodJetCloseToElectron->setEMF(0.2);
        goodJetCloseToElectron->setFHPD(0.5);
        goodJetCloseToElectron->setN90Hits(2);
    }


    void setUpIsolatedGoodMuon() {
        goodIsolatedMuon->makeGlobal(true);
        goodIsolatedMuon->setEcalIsolation(1);
        goodIsolatedMuon->setHcalIsolation(1);
        goodIsolatedMuon->setTrackerIsolation(1);
    }

    void setUpBadMuon() {
        badMuon->makeGlobal(false);
    }

    void setUpTTbarEvent() {
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        ttbarEvent.setVertices(vertices);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badElectron);
        ttbarEvent.setElectrons(electrons);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        jets.push_back(goodBJet);
        jets.push_back(badJet);
        ttbarEvent.setJets(jets);

        MuonCollection muons;
        muons.push_back(badMuon);
        ttbarEvent.setMuons(muons);
        ttbarEvent.setMET(met);
        ttbarEvent.setDataType(DataType::ttbar);
        ttbarEvent.setTracks(moreThan10TracksHighPurity);
        ttbarEvent.setBeamScrapingVeto(false);
        assert(ttbarEvent.passesScrapingFilter());
        assert(ttbarEvent.passesHighLevelTrigger());
        assert(ttbarEvent.hasOneGoodPrimaryVertex());
        assert(ttbarEvent.hasOnlyOneGoodIsolatedElectron());
        assert(ttbarEvent.hasAtLeastFourGoodJets());
        assert(ttbarEvent.isolatedElectronDoesNotComeFromConversion());
        assert(ttbarEvent.isolatedElectronNotTaggedAsFromConversion());
    }

    void setUpGoodZEvent() {
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        goodZEvent.setVertices(vertices);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(goodIsolatedElectron2);
        electrons.push_back(badElectron);
        goodZEvent.setElectrons(electrons);

        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        goodZEvent.setJets(jets);
//        goodZEvent.setHLT_Emulated_Photon15(true);

        goodZEvent.setDataType(DataType::Zjets);
        goodZEvent.setBeamScrapingVeto(false);
    }

    void setUpPoorZEvent() {
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        poorZEvent.setVertices(vertices);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(goodLooseElectron);
        electrons.push_back(badElectron);
        poorZEvent.setElectrons(electrons);

        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        poorZEvent.setJets(jets);
//        poorZEvent.setHLT_Emulated_Photon15(true);

        poorZEvent.setDataType(DataType::Zjets);
        poorZEvent.setBeamScrapingVeto(false);
    }

    void setUpDiJetEvent() {
        VertexCollection vertices;
        vertices.push_back(badVertex);
        DiJetEvent.setVertices(vertices);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        DiJetEvent.setJets(jets);
//        DiJetEvent.setHLT_Emulated_Photon15(false);

        DiJetEvent.setDataType(DataType::QCD_BCtoE_Pt80to170);
        DiJetEvent.setBeamScrapingVeto(false);
    }

    void setUpDiJetEventWithConversion() {
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        DiJetEventWithConversion.setVertices(vertices);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        DiJetEventWithConversion.setJets(jets);
//        DiJetEventWithConversion.setHLT_Emulated_Photon15(false);

        ElectronCollection electrons;
        electrons.push_back(electronFromConversion);
        DiJetEventWithConversion.setElectrons(electrons);
        DiJetEventWithConversion.setDataType(DataType::QCD_EMEnriched_Pt20to30);
        DiJetEventWithConversion.setBeamScrapingVeto(false);
    }

    void setUpMuonEvent() {
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        muonEvent.setVertices(vertices);
        ElectronCollection electrons;
        electrons.push_back(goodIsolatedElectron);
        electrons.push_back(badElectron);
        muonEvent.setElectrons(electrons);
        JetCollection jets;
        jets.push_back(goodJet);
        jets.push_back(goodJet);
        jets.push_back(goodBJet);
        jets.push_back(goodBJet);
        jets.push_back(badJet);
        muonEvent.setJets(jets);
//        muonEvent.setHLT_Emulated_Photon15(true);

        MuonCollection muons;
        muons.push_back(goodIsolatedMuon);
        muonEvent.setMuons(muons);
        muonEvent.setDataType(DataType::Zjets);
    }

    void setUpTracks() {
        TrackPointer trackHighPurity = TrackPointer(new Track(1., 0., 20., 6.));
        TrackPointer trackLowPurity = TrackPointer(new Track(1., 0., 20., 6.));
        trackHighPurity->setHighPurity(true);
        trackLowPurity->setHighPurity(false);
        for (unsigned int index = 0; index < 20; ++index) {
            if (index < 8)
                lessThan10Tracks.push_back(trackHighPurity);
            if (index < 2) {
                moreThan10TracksMixedPurity_H.push_back(trackLowPurity);
                moreThan10TracksMixedPurity_L.push_back(trackHighPurity);
            } else {
                moreThan10TracksMixedPurity_H.push_back(trackHighPurity);
                moreThan10TracksMixedPurity_L.push_back(trackLowPurity);
            }

            moreThan10TracksHighPurity.push_back(trackHighPurity);
            moreThan10TracksLowPurity.push_back(trackLowPurity);
        }
    }

    void setUpCustomEvent() {
        customEvent.useCustomReturnValues = true;
        customEvent.passConversion = false;
        customEvent.passElectronCut = true;
        customEvent.passHLT = true;
        customEvent.passPV = true;
        customEvent.passScraping = true;
        customEvent.passes1JetCut = true;
        customEvent.passes2JetCut = true;
        customEvent.passes3JetCut = true;
        customEvent.passes4JetCut = true;
        customEvent.passesMuon = true;
        customEvent.passesZveto = true;
        customEvent.passConversionPartnerTrack = true;
        assert(customEvent.passesScrapingFilter());
        assert(customEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
        assert(customEvent.passesHighLevelTrigger());
        assert(customEvent.passesSelectionStep(TTbarEPlusJetsSelection::HighLevelTrigger));
        assert(customEvent.isolatedElectronDoesNotComeFromConversion() == false);
        assert(customEvent.isolatedElectronNotTaggedAsFromConversion());
        assert(customEvent.hasOneGoodPrimaryVertex());
        assert(customEvent.hasOnlyOneGoodIsolatedElectron());
        assert(customEvent.hasNoIsolatedMuon());
        assert(customEvent.hasAtLeastOneGoodJet());
        assert(customEvent.hasAtLeastTwoGoodJets());
        assert(customEvent.hasAtLeastThreeGoodJets());
        assert(customEvent.hasAtLeastFourGoodJets());
        assert(customEvent.isNotAZBosonEvent());

        assert(customEvent.useCustomReturnValues);
    }

public:
    void testEventConstructor() {
        TopPairEventCandidate candidate = TopPairEventCandidate(ttbarEvent);
        ASSERT_EQUAL(2, candidate.Electrons().size());
        ASSERT_EQUAL(1, candidate.GoodIsolatedElectrons().size());
        ASSERT_EQUAL(5, candidate.Jets().size());
        ASSERT_EQUAL(4, candidate.GoodJets().size());
    }

    void testPassesHLT() {
        ASSERT(ttbarEvent.passesHighLevelTrigger());
    }

//    void testDoesNotPassHLT() {
//        DiJetEvent.setDataType(DataType::DATA);
//        ASSERT_EQUAL(false, DiJetEvent.passesHighLevelTrigger());
//    }

    void testDoesHaveGoodPV() {
        ASSERT(ttbarEvent.hasOneGoodPrimaryVertex());
    }

    void testDoesntHaveGoodPV() {
        ASSERT(DiJetEvent.hasOneGoodPrimaryVertex() == false);
    }

    void testHasOnlyOneGoodIsolatedElectron() {
        ASSERT(ttbarEvent.hasOnlyOneGoodIsolatedElectron());
    }

    void testHasNotOnlyOneGoodIsolatedElectron() {
        ASSERT(goodZEvent.hasOnlyOneGoodIsolatedElectron() == false);
        ASSERT(DiJetEvent.hasOnlyOneGoodIsolatedElectron() == false);
    }

    void testIsolatedElectronNotFromConversion() {
        ASSERT(ttbarEvent.isolatedElectronDoesNotComeFromConversion());
    }

    void testIsolatedElectronFromConversion() {
        ASSERT(DiJetEventWithConversion.isolatedElectronDoesNotComeFromConversion() == false);
    }

    void testNoElectronInEventReturnsFalse() {
        ASSERT(DiJetEvent.isolatedElectronDoesNotComeFromConversion() == false);
    }

    void testHasAtLeastOneGoodJet() {
        ASSERT(ttbarEvent.hasAtLeastOneGoodJet());
    }

    void testHasNoGoodJets() {
        ASSERT(emptyEvent.hasAtLeastOneGoodJet() == false);
    }

    void testHasAtLeastTwoGoodjet() {
        ASSERT(ttbarEvent.hasAtLeastTwoGoodJets());
    }

    void testHasNoTwoGoodJets() {
        ASSERT(emptyEvent.hasAtLeastTwoGoodJets() == false);
    }

    void testHasAtLeastThreeGoodJets() {
        ASSERT(ttbarEvent.hasAtLeastThreeGoodJets());
    }

    void testHasNoThreeGoodJets() {
        ASSERT(DiJetEvent.hasAtLeastThreeGoodJets() == false);
    }

    void testHasAtFourThreeGoodJets() {
        ASSERT(ttbarEvent.hasAtLeastThreeGoodJets());
    }

    void testHasNoFourThreeGoodJets() {
        ASSERT(DiJetEvent.hasAtLeastThreeGoodJets() == false);
    }

    void testTTbarIsNotAZBosonEvent() {
        ASSERT(ttbarEvent.isNotAZBosonEvent());
    }

    void testGoodZIsAZBosonEvent() {
        ASSERT_EQUAL(false, goodZEvent.isNotAZBosonEvent());
    }

    void testPoorZIsAZBosonEvent() {
        ASSERT_EQUAL(false, poorZEvent.isNotAZBosonEvent());
    }

    void testDiJetIsNotAZBosonEvent() {
        ASSERT_EQUAL(true, DiJetEvent.isNotAZBosonEvent());
    }

    void testTTbarEventPassesMuonVeto() {
        ASSERT_EQUAL(true, ttbarEvent.hasNoIsolatedMuon());
    }

    void testMuonEventDoesnPassMuonVeto() {
        ASSERT_EQUAL(false, muonEvent.hasNoIsolatedMuon());
    }

    void testEventPasses0Step() {
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
    }

    void testEventPasses0StepWithLowNumberOfTracks() {
        ttbarEvent.setTracks(lessThan10Tracks);
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
    }

    void testEventPasses0StepWithMixedTracks_H() {
        ttbarEvent.setTracks(moreThan10TracksMixedPurity_H);
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
    }

    void testEventPasses0StepWithMixedTracks_L() {
        ttbarEvent.setTracks(moreThan10TracksMixedPurity_L);
        ASSERT_EQUAL(false, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
    }

    void testEventFails0StepWithLowPurityTracks() {
        ttbarEvent.setTracks(moreThan10TracksLowPurity);
        ASSERT_EQUAL(false, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
    }

    void testEventPasses1stStep() {
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::HighLevelTrigger));
    }

    void testEventPasses2ndStep() {
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::GoodPrimaryvertex));
    }

    void testEventPasses3ndStep() {
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::OneIsolatedElectron));
    }

    void testEventPasses4thStep() {
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::ConversionRejection));
    }

    void testEventPasses5thStep() {
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::LooseMuonVeto));
    }

    void testEventPasses6thStep() {
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::AtLeastFourGoodJets));
    }

    void testEventPasses7thStep() {
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::Zveto));
    }

    void testTTbarEventPassesNStep() {
        ASSERT_EQUAL(true, ttbarEvent.passesSelectionStepUpTo(TTbarEPlusJetsSelection::Zveto));
    }

    void testPoorZEventPassesUpToStep() {
        assert(poorZEvent.passesScrapingFilter());
        assert(poorZEvent.passesHighLevelTrigger());
        assert(poorZEvent.hasOneGoodPrimaryVertex());
        cout << poorZEvent.GoodElectrons().size() << endl;
        cout << poorZEvent.GoodIsolatedElectrons().size() << endl;
        assert(poorZEvent.hasOnlyOneGoodIsolatedElectron());
        assert(poorZEvent.isolatedElectronDoesNotComeFromConversion());
        ASSERT_EQUAL(true, poorZEvent.passesSelectionStepUpTo(TTbarEPlusJetsSelection::LooseMuonVeto));
    }

    void testPoorZEventDoesntPassUpToStep() {
        ASSERT_EQUAL(false, poorZEvent.passesSelectionStepUpTo(TTbarEPlusJetsSelection::Zveto));
    }

    void testPassesFullTTbarSelection() {
        ASSERT_EQUAL(true, ttbarEvent.passesFullTTbarEPlusJetSelection());
    }

    void testFailsFullTTbarSelection() {
        ASSERT_EQUAL(false, DiJetEvent.passesFullTTbarEPlusJetSelection());
    }

    void testComputeNeutrinoPzsWithoutMETThrowsExpeption() {
        TopPairEventCandidate cand = TopPairEventCandidate();
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        cand.setVertices(vertices);
        ElectronPointer electron(new Electron(40, -40, 0, 0));
        electron->setHcalIsolation(0);
        electron->setEcalIsolation(0);
        electron->setTrackerIsolation(0);
        electron->setNumberOfMissingInnerLayerHits(0);
        ElectronCollection eCollection;
        eCollection.push_back(electron);
        cand.setElectrons(eCollection);
        ASSERT_THROWS(cand.computeNeutrinoPz(),ReconstructionException);
    }

    void testComputeNeutrinoPzsIsolatedElectronThrowsExpeption() {
        TopPairEventCandidate cand = TopPairEventCandidate();
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        cand.setVertices(vertices);
        ElectronPointer electron(new Electron(40, -40, 0, 0));
        electron->setHcalIsolation(50);
        electron->setEcalIsolation(50);
        electron->setTrackerIsolation(50);
        electron->setNumberOfMissingInnerLayerHits(0);
        ElectronCollection eCollection;
        eCollection.push_back(electron);
        cand.setElectrons(eCollection);
        ASSERT_THROWS(cand.computeNeutrinoPz(),ReconstructionException);
    }

    void testComputeNeutrinoPzs() {
        METPointer met(new MET(40, 0));
        ElectronPointer electron(new Electron(40, -40, 0, 0));
        electron->setHcalIsolation(0);
        electron->setEcalIsolation(0);
        electron->setTrackerIsolation(0);
        electron->setNumberOfMissingInnerLayerHits(0);
        electron->setD0_wrtBeamSpot(0);
        electron->setSuperClusterEta(0);
        electron->setDEtaIn(0);
        electron->setDPhiIn(0);
        electron->setSigmaIEtaIEta(0);
        electron->setHadOverEm(0);

        DummyTTbarEvent cand = DummyTTbarEvent();
        cand.setMET(met);
        cand.setElectronFromW(electron);
        ASSERT_EQUAL(sqrt(80*80-80.389*80.389), cand.computeNeutrinoPz().at(0));
        ASSERT_EQUAL(sqrt(80*80-80.389*80.389), cand.computeNeutrinoPz().at(1));
    }

    void testReconstructTopEventUsingChiWithNotEnoughJetsThrowsException() {
        METPointer met(new MET(40, 0));
        ElectronCollection eCollection;
        eCollection.push_back(goodIsolatedElectron);
        TopPairEventCandidate cand = TopPairEventCandidate();
        VertexCollection vertices;
        vertices.push_back(goodVertex);
        cand.setVertices(vertices);
        cand.setMET(met);
        cand.setElectrons(eCollection);
        ASSERT_THROWS(cand.reconstructTTbar(goodIsolatedElectron),ReconstructionException);
    }

    void testNMinus1CutsPositive() {
        ASSERT_EQUAL(true, customEvent.passesNMinus1(TTbarEPlusJetsSelection::ConversionRejection));
    }

    void testNMinus1CutsNegative() {
        ASSERT_EQUAL(false, customEvent.passesNMinus1(TTbarEPlusJetsSelection::HighLevelTrigger));
    }
};

extern cute::suite make_suite_TestTopPairEventCandidate() {
    cute::suite s;
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testNMinus1CutsPositive));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testNMinus1CutsNegative));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventConstructor));

    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testPassesHLT));
//    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testDoesNotPassHLT));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testDoesHaveGoodPV));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testDoesntHaveGoodPV));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testHasOnlyOneGoodIsolatedElectron));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testHasNotOnlyOneGoodIsolatedElectron));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testIsolatedElectronNotFromConversion));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testIsolatedElectronFromConversion));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testNoElectronInEventReturnsFalse));

    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testHasAtLeastOneGoodJet));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testHasNoGoodJets));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testHasAtLeastTwoGoodjet));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testHasNoTwoGoodJets));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testHasAtLeastThreeGoodJets));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testHasNoThreeGoodJets));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testHasAtFourThreeGoodJets));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testHasNoFourThreeGoodJets));

    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testTTbarIsNotAZBosonEvent));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testGoodZIsAZBosonEvent));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testPoorZIsAZBosonEvent));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testDiJetIsNotAZBosonEvent));

    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testTTbarEventPassesMuonVeto));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testMuonEventDoesnPassMuonVeto));

    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses0Step));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses0StepWithLowNumberOfTracks));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventFails0StepWithLowPurityTracks));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses0StepWithMixedTracks_H));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses0StepWithMixedTracks_L));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses1stStep));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses2ndStep));
//    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses2ndStepInRealData));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses3ndStep));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses4thStep));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses5thStep));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses6thStep));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testEventPasses7thStep));

    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testTTbarEventPassesNStep));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testPoorZEventPassesUpToStep));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testPoorZEventDoesntPassUpToStep));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testPassesFullTTbarSelection));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testFailsFullTTbarSelection));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testComputeNeutrinoPzsWithoutMETThrowsExpeption));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testComputeNeutrinoPzsIsolatedElectronThrowsExpeption));
    s.push_back(CUTE_SMEMFUN(TestTopPairEventCandidate, testComputeNeutrinoPzs));
    s.push_back(
            CUTE_SMEMFUN(TestTopPairEventCandidate, testReconstructTopEventUsingChiWithNotEnoughJetsThrowsException));

    return s;
}
