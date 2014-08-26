/*
 * TestTrackReader.hh
 *
 *  Created on: 20 Sep 2010
 *      Author: kreczko
 */

#ifndef TESTTRACKREADER_HH_
#define TESTTRACKREADER_HH_
#include "cute/cute.h"
#include "cute/cute_suite.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "../interface/RecoObjects/Track.h"
#include "../interface/Readers/TrackReader.h"


struct TestTrackReader {
private:
	boost::shared_ptr<TChain> input;
	boost::scoped_ptr<BAT::TrackReader> trackReader;
	BAT::TrackCollection tracks;
	BAT::TrackPointer firstTrack;

public:
	TestTrackReader();
	void testReadTracksSize();
	void testReadFirstTrackPhi();
	void testFirstTrackCharge();
	void testFirstTrackD0();
	void testFirstTrackIsHighPurity();
	void testNumberOfHighPurityTracks();
};
extern cute::suite make_suite_TestTrackReader();

#endif /* TESTTRACKREADER_HH_ */
