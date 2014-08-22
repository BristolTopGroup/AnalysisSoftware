#include "cute/cute.h"
#include "cute/cute_suite.h"
#include <boost/scoped_ptr.hpp>

#include "../interface/Event.h"
#include "../interface/TopPairEventCandidate.h"
#include "../interface/Readers/NTupleEventReader.h"
#include "InputFiles.h"

class TestNTupleEventReader {
private:
	BAT::NTupleEventReaderLocalPtr TTbarReader;
	BAT::NTupleEventReaderLocalPtr QCDenri1Reader;
	BAT::NTupleEventReaderLocalPtr QCDenri2Reader;
	BAT::NTupleEventReaderLocalPtr QCDenri3Reader;

	BAT::NTupleEventReaderLocalPtr QCDbce1Reader;
	BAT::NTupleEventReaderLocalPtr QCDbce2Reader;
	BAT::NTupleEventReaderLocalPtr QCDbce3Reader;

	BAT::NTupleEventReaderLocalPtr WjetsReader;
	BAT::NTupleEventReaderLocalPtr ZJetsReader;
	BAT::NTupleEventReaderLocalPtr TWReader;
	BAT::NTupleEventReaderLocalPtr TChanReader;
	BAT::NTupleEventReaderLocalPtr DataReader;
	BAT::NTupleEventReaderLocalPtr DataReader2;
	BAT::NTupleEventReaderLocalPtr AllMCReader;

public:
	TestNTupleEventReader();

	void testTTbarType();
	void testQCD_EMEnriched_80_to_170Type();
	void testQCD_EMEnriched_30_to_80Type();
	void testQCD_EMEnriched_20_to_30Type();
	void testQCD_BCtoE_80_to_170Type();
	void testQCD_BCtoE_30_to_80Type();
	void testQCD_BCtoE_20_to_30Type();
	void testWjetsType();
	void testZjetsType();
	void testTWType();
	void testTChanType();
	void testDataType();
	void testReadDataType();

	void testNumberOfTracksInEvent1();
	void testNumberOfElectronsInEvent1();
	void testNumberOfJetsInEvent1();
	void testNumberOfMuonsInEvent1();
	void testHasNextEvent();
	void testHasNotNextEvent();
	void testGetProccessedNumberOfEvents();
	void testGetProccessedNumberOfEventsWithSkippedEvents();
	void testSkipEvents();
	void testGetCurrentLocalEventNumber();
	void testSetMaximumNumberOfEventsWithSkippedEvents();
	void testSetMaximumNumberOfEvents();
	void testMCRunNumber();
	void testMCLumiBlock();
	void testLocalEventNumber();
	void testInitialEventWeigth();
	void testPrimaryVertex();
	void testEventChainConstant();
	void testTTbarEventMET();
	void testSeenTTbar();
	void testSeenAllMC();
	void testAddInputFileNoFileThrowsException();
	void testGenJetsMC();
	void testGenJetsDATA();

};

extern cute::suite make_suite_TestNTupleEventReader();
