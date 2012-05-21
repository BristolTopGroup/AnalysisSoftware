#include "TestTopPairEventCandidate.h"
#include "TestObjectFactory.h"

#include <iostream>

using namespace BAT;
using namespace std;

TestTopPairEventCandidate::TestTopPairEventCandidate() : //
		ttbarEvent(), //
		goodZEvent(), //
		poorZEvent(), //
		DiJetEvent(), //
		DiJetEventWithConversion(), //
		muonEvent(), //
		emptyEvent(), //
		customEvent(), //
		goodIsolatedElectron(TestObjectFactory::goodIsolatedElectron()), //
		goodIsolatedElectron2(TestObjectFactory::goodIsolatedElectron2()), //
		goodLooseElectron(TestObjectFactory::goodLooseElectron()), //
		badElectron(TestObjectFactory::badElectronNoID()), //
		electronFromConversion(TestObjectFactory::electronWithMissingInnerLayerHit()), //
		goodJet(TestObjectFactory::goodCaloJet()), //
		goodBJet(TestObjectFactory::goodCaloBJetSSVHEM()), //
		badJet(TestObjectFactory::badEtCaloJet()), //
		goodJetCloseToElectron(TestObjectFactory::goodCaloJetCloseToCaloElectron()), //
		goodVertex(TestObjectFactory::goodVertex()), //
		badVertex(TestObjectFactory::badFakeVertex()), //
		goodIsolatedMuon(TestObjectFactory::goodIsolatedMuon()), //
		badMuon(TestObjectFactory::badEtaMuon()), //
		met(TestObjectFactory::goodMET()), //
		lessThan10Tracks(), //
		moreThan10TracksHighPurity(), //
		moreThan10TracksLowPurity(), //
		moreThan10TracksMixedPurity_H(), //
		moreThan10TracksMixedPurity_L(), //
		HLTs(new std::vector<int>()) {
	setUpTracks();

	for (unsigned int i = 0; i < HLTriggers::NUMBER_OF_TRIGGERS; ++i) {
		HLTs->push_back(true);
	}

	setUpTTbarEvent();
	setUpGoodZEvent();
	setUpPoorZEvent();
	setUpDiJetEvent();
	setUpMuonEvent();
	setUpCustomEvent();

}

void TestTopPairEventCandidate::setUpTTbarEvent() {
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
	METCollection mets;
	mets.resize(METAlgorithm::NUMBER_OF_METALGORITHMS);
	mets.at(METAlgorithm::patMETsPFlow) = met;
	ttbarEvent.setMETs(mets);
	ttbarEvent.setDataType(DataType::TTJets);
	ttbarEvent.setTracks(moreThan10TracksHighPurity);
	ttbarEvent.setBeamScrapingVeto(false);
	ttbarEvent.setHLTs(HLTs);

	assert(ttbarEvent.passesScrapingFilter());
	assert(ttbarEvent.passesHighLevelTrigger());
	assert(ttbarEvent.hasOneGoodPrimaryVertex());
	assert(ttbarEvent.GoodIsolatedElectrons().size() > 0);
	assert(ttbarEvent.GoodPFIsolatedElectrons().size() > 0);
	assert(ttbarEvent.hasOnlyOneGoodIsolatedElectron());
	assert(ttbarEvent.hasAtLeastFourGoodJets());
	assert(ttbarEvent.isolatedElectronDoesNotComeFromConversion());
	assert(ttbarEvent.isolatedElectronNotTaggedAsFromConversion());
}

void TestTopPairEventCandidate::setUpGoodZEvent() {
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

void TestTopPairEventCandidate::setUpPoorZEvent() {
	VertexCollection vertices;
	vertices.push_back(goodVertex);
	poorZEvent.setVertices(vertices);
	ElectronCollection electrons;
	electrons.push_back(goodIsolatedElectron);
	electrons.push_back(goodLooseElectron);
	electrons.push_back(badElectron);
	poorZEvent.setElectrons(electrons);
	poorZEvent.setHLTs(HLTs);
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

void TestTopPairEventCandidate::setUpDiJetEvent() {
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

void TestTopPairEventCandidate::setUpDiJetEventWithConversion() {
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

void TestTopPairEventCandidate::setUpMuonEvent() {
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

void TestTopPairEventCandidate::setUpTracks() {
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

void TestTopPairEventCandidate::setUpCustomEvent() {
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
	customEvent.passesMET = true;
	customEvent.passesAsymJets = true;
	customEvent.atLeastOneBtag = true;
	customEvent.atLeastTwoBtags = true;

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

void TestTopPairEventCandidate::testEventConstructor() {
	TopPairEventCandidate candidate = TopPairEventCandidate(ttbarEvent);
	ASSERT_EQUAL(2, candidate.Electrons().size());
	ASSERT_EQUAL(1, candidate.GoodIsolatedElectrons().size());
	ASSERT_EQUAL(5, candidate.Jets().size());
	ASSERT_EQUAL(4, candidate.GoodJets().size());
}

void TestTopPairEventCandidate::testPassesHLT() {
	ASSERT(ttbarEvent.passesHighLevelTrigger());
}

//    void TestTopPairEventCandidate::testDoesNotPassHLT() {
//        DiJetEvent.setDataType(DataType::DATA);
//        ASSERT_EQUAL(false, DiJetEvent.passesHighLevelTrigger());
//    }

void TestTopPairEventCandidate::testDoesHaveGoodPV() {
	ASSERT(ttbarEvent.hasOneGoodPrimaryVertex());
}

void TestTopPairEventCandidate::testDoesntHaveGoodPV() {
	ASSERT(DiJetEvent.hasOneGoodPrimaryVertex() == false);
}

void TestTopPairEventCandidate::testHasOnlyOneGoodIsolatedElectron() {
	ASSERT(ttbarEvent.hasOnlyOneGoodIsolatedElectron());
}

void TestTopPairEventCandidate::testHasNotOnlyOneGoodIsolatedElectron() {
	ASSERT(goodZEvent.hasOnlyOneGoodIsolatedElectron() == false);
	ASSERT(DiJetEvent.hasOnlyOneGoodIsolatedElectron() == false);
}

void TestTopPairEventCandidate::testIsolatedElectronNotFromConversion() {
	ASSERT(ttbarEvent.isolatedElectronDoesNotComeFromConversion());
}

void TestTopPairEventCandidate::testIsolatedElectronFromConversion() {
	ASSERT(DiJetEventWithConversion.isolatedElectronDoesNotComeFromConversion() == false);
}

void TestTopPairEventCandidate::testNoElectronInEventReturnsFalse() {
	ASSERT(DiJetEvent.isolatedElectronDoesNotComeFromConversion() == false);
}

void TestTopPairEventCandidate::testHasAtLeastOneGoodJet() {
	ASSERT(ttbarEvent.hasAtLeastOneGoodJet());
}

void TestTopPairEventCandidate::testHasNoGoodJets() {
	ASSERT(emptyEvent.hasAtLeastOneGoodJet() == false);
}

void TestTopPairEventCandidate::testHasAtLeastTwoGoodjet() {
	ASSERT(ttbarEvent.hasAtLeastTwoGoodJets());
}

void TestTopPairEventCandidate::testHasNoTwoGoodJets() {
	ASSERT(emptyEvent.hasAtLeastTwoGoodJets() == false);
}

void TestTopPairEventCandidate::testHasAtLeastThreeGoodJets() {
	ASSERT(ttbarEvent.hasAtLeastThreeGoodJets());
}

void TestTopPairEventCandidate::testHasNoThreeGoodJets() {
	ASSERT(DiJetEvent.hasAtLeastThreeGoodJets() == false);
}

void TestTopPairEventCandidate::testHasAtFourThreeGoodJets() {
	ASSERT(ttbarEvent.hasAtLeastThreeGoodJets());
}

void TestTopPairEventCandidate::testHasNoFourThreeGoodJets() {
	ASSERT(DiJetEvent.hasAtLeastThreeGoodJets() == false);
}

void TestTopPairEventCandidate::testTTbarIsNotAZBosonEvent() {
	ASSERT(ttbarEvent.isNotAZBosonEvent());
}

void TestTopPairEventCandidate::testGoodZIsAZBosonEvent() {
	ASSERT_EQUAL(false, goodZEvent.isNotAZBosonEvent());
}

void TestTopPairEventCandidate::testPoorZIsAZBosonEvent() {
	ASSERT_EQUAL(false, poorZEvent.isNotAZBosonEvent());
}

void TestTopPairEventCandidate::testDiJetIsNotAZBosonEvent() {
	ASSERT_EQUAL(true, DiJetEvent.isNotAZBosonEvent());
}

void TestTopPairEventCandidate::testTTbarEventPassesMuonVeto() {
	ASSERT_EQUAL(true, ttbarEvent.hasNoIsolatedMuon());
}

void TestTopPairEventCandidate::testMuonEventDoesnPassMuonVeto() {
	ASSERT_EQUAL(false, muonEvent.hasNoIsolatedMuon());
}

void TestTopPairEventCandidate::testEventPasses0Step() {
	ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
}

void TestTopPairEventCandidate::testEventPasses0StepWithLowNumberOfTracks() {
	ttbarEvent.setTracks(lessThan10Tracks);
	ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
}

void TestTopPairEventCandidate::testEventPasses0StepWithMixedTracks_H() {
	ttbarEvent.setTracks(moreThan10TracksMixedPurity_H);
	ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
}

void TestTopPairEventCandidate::testEventPasses0StepWithMixedTracks_L() {
	ttbarEvent.setTracks(moreThan10TracksMixedPurity_L);
	ASSERT_EQUAL(false, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
}

void TestTopPairEventCandidate::testEventFails0StepWithLowPurityTracks() {
	ttbarEvent.setTracks(moreThan10TracksLowPurity);
	ASSERT_EQUAL(false, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::FilterOutScraping));
}

void TestTopPairEventCandidate::testEventPasses1stStep() {
	ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::HighLevelTrigger));
}

void TestTopPairEventCandidate::testEventPasses2ndStep() {
	ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::GoodPrimaryvertex));
}

void TestTopPairEventCandidate::testEventPasses3ndStep() {
	ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::OneIsolatedElectron));
}

void TestTopPairEventCandidate::testEventPasses4thStep() {
	ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::ConversionRejection));
}

void TestTopPairEventCandidate::testEventPasses5thStep() {
	ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::LooseMuonVeto));
}

void TestTopPairEventCandidate::testEventPasses6thStep() {
	ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::AtLeastFourGoodJets));
}

void TestTopPairEventCandidate::testEventPasses7thStep() {
	ASSERT_EQUAL(true, ttbarEvent.passesSelectionStep(TTbarEPlusJetsSelection::DileptonVeto));
}

void TestTopPairEventCandidate::testTTbarEventPassesNStep() {
	ASSERT_EQUAL(true, ttbarEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::DileptonVeto));
}

void TestTopPairEventCandidate::testPoorZEventPassesUpToStep() {
	assert(poorZEvent.passesScrapingFilter());
	assert(poorZEvent.passesHighLevelTrigger());
	assert(poorZEvent.hasOneGoodPrimaryVertex());
	cout << poorZEvent.GoodElectrons().size() << endl;
	cout << poorZEvent.GoodIsolatedElectrons().size() << endl;
	assert(poorZEvent.hasOnlyOneGoodIsolatedElectron());
	assert(poorZEvent.isolatedElectronDoesNotComeFromConversion());
	ASSERT_EQUAL(true, poorZEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::LooseMuonVeto));
}

void TestTopPairEventCandidate::testPoorZEventDoesntPassUpToStep() {
	ASSERT_EQUAL(false, poorZEvent.passesEPlusJetsSelectionStepUpTo(TTbarEPlusJetsSelection::DileptonVeto));
}

void TestTopPairEventCandidate::testPassesFullTTbarSelection() {
	ASSERT_EQUAL(true, ttbarEvent.passesFullTTbarEPlusJetSelection());
}

void TestTopPairEventCandidate::testFailsFullTTbarSelection() {
	ASSERT_EQUAL(false, DiJetEvent.passesFullTTbarEPlusJetSelection());
}

void TestTopPairEventCandidate::testComputeNeutrinoPzsWithoutMETThrowsExpeption() {
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
	ASSERT_THROWS(cand.computeNeutrinoPz(), ReconstructionException);
}

void TestTopPairEventCandidate::testComputeNeutrinoPzsIsolatedElectronThrowsExpeption() {
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
	ASSERT_THROWS(cand.computeNeutrinoPz(), ReconstructionException);
}

void TestTopPairEventCandidate::testComputeNeutrinoPzs() {
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
	METCollection mets;
	mets.resize(METAlgorithm::NUMBER_OF_METALGORITHMS);
	mets.at(METAlgorithm::patMETsPFlow) = met;
	cand.setMETs(mets);
	cand.setElectronFromW(electron);
	ASSERT_EQUAL(sqrt(80*80-80.389*80.389), cand.computeNeutrinoPz().at(0));
	ASSERT_EQUAL(sqrt(80*80-80.389*80.389), cand.computeNeutrinoPz().at(1));
}

void TestTopPairEventCandidate::testReconstructTopEventUsingChiWithNotEnoughJetsThrowsException() {
	METPointer met(new MET(40, 0));
	ElectronCollection eCollection;
	eCollection.push_back(goodIsolatedElectron);
	TopPairEventCandidate cand = TopPairEventCandidate();
	VertexCollection vertices;
	vertices.push_back(goodVertex);
	cand.setVertices(vertices);
	METCollection mets;
	mets.resize(METAlgorithm::NUMBER_OF_METALGORITHMS);
	mets.at(METAlgorithm::patMETsPFlow) = met;

	cand.setMETs(mets);
	cand.setElectrons(eCollection);
	ASSERT_THROWS(cand.reconstructTTbarToEPlusJets(goodIsolatedElectron), ReconstructionException);
}

void TestTopPairEventCandidate::testNMinus1CutsPositive() {
	ASSERT_EQUAL(true, customEvent.passesNMinus1(TTbarEPlusJetsSelection::ConversionRejection));
}

void TestTopPairEventCandidate::testNMinus1CutsNegative() {
	ASSERT_EQUAL(false, customEvent.passesNMinus1(TTbarEPlusJetsSelection::HighLevelTrigger));
}

cute::suite make_suite_TestTopPairEventCandidate() {
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

//TODO: test hasBeenReconstructed function
}
