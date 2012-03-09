/*
 * TestTrackReader.hh
 *
 *  Created on: 20 Sep 2010
 *      Author: kreczko
 */

#include "TestTrackReader.h"
#include "../interface/Readers/NTupleEventReader.h"
#include "InputFiles.h"

using namespace BAT;

TestTrackReader::TestTrackReader() :
		input(new TChain(NTupleEventReader::EVENT_CHAIN)), //
		trackReader(new TrackReader(input)), //
		tracks(), //
		firstTrack() {

	input->Add(InputFile::TTJets);

	input->LoadTree(1);
	input->SetBranchStatus("*", 0);
	trackReader->initialise();
	input->GetEntry(1);
	tracks = trackReader->getTracks();
	firstTrack = tracks.front();
}

void TestTrackReader::testReadTracksSize() {
	ASSERT_EQUAL(121, tracks.size());
}

void TestTrackReader::testReadFirstTrackPhi() {
	ASSERT_EQUAL(true, fabs(firstTrack->phi()) < 2*TMath::Pi()+0.1);
}

void TestTrackReader::testFirstTrackCharge() {
	ASSERT_EQUAL(1, fabs(firstTrack->charge()));
}

void TestTrackReader::testFirstTrackD0() {
	ASSERT_EQUAL_DELTA(0.359524, firstTrack->d0(), 0.000001);
}

void TestTrackReader::testFirstTrackIsHighPurity() {
	ASSERT_EQUAL(true, firstTrack->isHighPurity());
}

void TestTrackReader::testNumberOfHighPurityTracks() {
	unsigned int numberOfHighPurityTracks = 0;
	for (unsigned int index = 0; index < tracks.size(); ++index) {
		if (tracks.at(index)->isHighPurity())
			numberOfHighPurityTracks++;
	}
	ASSERT_EQUAL(111, numberOfHighPurityTracks);
}

cute::suite make_suite_TestTrackReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestTrackReader, testReadTracksSize));
	s.push_back(CUTE_SMEMFUN(TestTrackReader, testReadFirstTrackPhi));
	s.push_back(CUTE_SMEMFUN(TestTrackReader, testFirstTrackCharge));
	s.push_back(CUTE_SMEMFUN(TestTrackReader, testFirstTrackD0));
	s.push_back(CUTE_SMEMFUN(TestTrackReader, testFirstTrackIsHighPurity));
	s.push_back(CUTE_SMEMFUN(TestTrackReader, testNumberOfHighPurityTracks));

	return s;
}
