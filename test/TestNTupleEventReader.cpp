#include "TestNTupleEventReader.h"
#include "InputFiles.h"
#include <iostream>

using namespace BAT;
using namespace std;

TestNTupleEventReader::TestNTupleEventReader() :
		TTbarReader(new NTupleEventReader()), //
		QCDenri1Reader(new NTupleEventReader()), //
		QCDenri2Reader(new NTupleEventReader()), //
		QCDenri3Reader(new NTupleEventReader()), //
		QCDbce1Reader(new NTupleEventReader()), //
		QCDbce2Reader(new NTupleEventReader()), //
		QCDbce3Reader(new NTupleEventReader()), //
		WjetsReader(new NTupleEventReader()), //
		ZJetsReader(new NTupleEventReader()), //
		TWReader(new NTupleEventReader()), //
		TChanReader(new NTupleEventReader()), //
		DataReader(new NTupleEventReader()), //
		DataReader2(new NTupleEventReader()), //
		AllMCReader(new NTupleEventReader()) {

	TTbarReader->addInputFile(InputFile::TTJets);
	QCDenri1Reader->addInputFile(InputFile::QCD_EMEnriched_Pt20to30);
	QCDenri2Reader->addInputFile(InputFile::QCD_EMEnriched_Pt30to80);
	QCDenri3Reader->addInputFile(InputFile::QCD_EMEnriched_Pt80to170);

	QCDbce1Reader->addInputFile(InputFile::QCD_BCtoE_Pt20to30);
	QCDbce2Reader->addInputFile(InputFile::QCD_BCtoE_Pt30to80);
	QCDbce3Reader->addInputFile(InputFile::QCD_BCtoE_Pt80to170);

	WjetsReader->addInputFile(InputFile::WJets);
	ZJetsReader->addInputFile(InputFile::ZJets);
	TWReader->addInputFile(InputFile::singleTop_And_W);
	TChanReader->addInputFile(InputFile::singleTopTChannel);
	DataReader->addInputFile(InputFile::data);
	DataReader->addInputFile(InputFile::data2);
	DataReader2->addInputFile(InputFile::data2);
	DataReader2->addInputFile(InputFile::data);

	AllMCReader->addInputFile(InputFile::TTJets);
	AllMCReader->addInputFile(InputFile::QCD_EMEnriched_Pt20to30);
	AllMCReader->addInputFile(InputFile::QCD_EMEnriched_Pt30to80);
	AllMCReader->addInputFile(InputFile::QCD_EMEnriched_Pt80to170);
	AllMCReader->addInputFile(InputFile::QCD_BCtoE_Pt20to30);
	AllMCReader->addInputFile(InputFile::QCD_BCtoE_Pt30to80);
	AllMCReader->addInputFile(InputFile::QCD_BCtoE_Pt80to170);
	AllMCReader->addInputFile(InputFile::WJets);
	AllMCReader->addInputFile(InputFile::ZJets);
	AllMCReader->addInputFile(InputFile::singleTop_And_W);
	AllMCReader->addInputFile(InputFile::singleTopTChannel);
	AllMCReader->addInputFile(InputFile::singleTopSChannel);
	AllMCReader->addInputFile(InputFile::singleAntiTop_And_W);
	AllMCReader->addInputFile(InputFile::singleAntiTopTChannel);
	AllMCReader->addInputFile(InputFile::singleAntiTopSChannel);
	AllMCReader->addInputFile(InputFile::GJets_Pt40to100);
	AllMCReader->addInputFile(InputFile::GJets_Pt100to200);
	AllMCReader->addInputFile(InputFile::GJets_Pt200toInf);

}

void TestNTupleEventReader::testTTbarType() {
	EventPtr currentEvent = TTbarReader->getNextEvent();
	ASSERT_EQUAL(short(DataType::TTJets), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testQCD_EMEnriched_80_to_170Type() {
	EventPtr currentEvent = QCDenri3Reader->getNextEvent();
	ASSERT_EQUAL(short(DataType::QCD_EMEnriched_Pt80to170), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testQCD_EMEnriched_30_to_80Type() {
	EventPtr currentEvent = QCDenri2Reader->getNextEvent();
	ASSERT_EQUAL(short(DataType::QCD_EMEnriched_Pt30to80), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testQCD_EMEnriched_20_to_30Type() {
	EventPtr currentEvent = QCDenri1Reader->getNextEvent();
	ASSERT_EQUAL(short(DataType::QCD_EMEnriched_Pt20to30), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testQCD_BCtoE_80_to_170Type() {
	EventPtr currentEvent = QCDbce3Reader->getNextEvent();
	ASSERT_EQUAL(short(DataType::QCD_BCtoE_Pt80to170), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testQCD_BCtoE_30_to_80Type() {
	EventPtr currentEvent = QCDbce2Reader->getNextEvent();
	ASSERT_EQUAL(short(DataType::QCD_BCtoE_Pt30to80), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testQCD_BCtoE_20_to_30Type() {
	EventPtr currentEvent = QCDbce1Reader->getNextEvent();
	ASSERT_EQUAL(short(DataType::QCD_BCtoE_Pt20to30), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testWjetsType() {
	EventPtr currentEvent = WjetsReader->getNextEvent();
	ASSERT_EQUAL(short(DataType::Wjets), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testZjetsType() {
	ASSERT(ZJetsReader->hasNextEvent());
	EventPtr currentEvent = ZJetsReader->getNextEvent();
	ASSERT_EQUAL(short(DataType::Zjets), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testTWType() {
	ASSERT(TWReader->hasNextEvent());
	EventPtr currentEvent = TWReader->getNextEvent();
	ASSERT_EQUAL(short(DataType::singleTop_And_W), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testTChanType() {
	EventPtr currentEvent = TChanReader->getNextEvent();
	ASSERT_EQUAL(short(DataType::singleTopTChannel), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testDataType() {
	EventPtr currentEvent = DataReader->getNextEvent();
	ASSERT_EQUAL(short(DataType::ElectronHad), short(currentEvent->getDataType()));
}

void TestNTupleEventReader::testReadDataType() {
	ASSERT(DataReader->hasNextEvent());
	EventPtr currentEvent = DataReader->getNextEvent();
	ASSERT_EQUAL(true, currentEvent->isRealData());
}

void TestNTupleEventReader::testNumberOfTracksInEvent1() {
	ASSERT(TTbarReader->hasNextEvent());
	EventPtr currentEvent = TTbarReader->getNextEvent();
	ASSERT_EQUAL(0, currentEvent->Tracks().size());
	//Tracks deactivated
}

void TestNTupleEventReader::testNumberOfElectronsInEvent1() {
	EventPtr currentEvent = TTbarReader->getNextEvent();
	cout << TTbarReader->getCurrentLocalEventNumber() << endl;
	ASSERT_EQUAL(1, currentEvent->Electrons().size());
}

void TestNTupleEventReader::testNumberOfJetsInEvent1() {
	EventPtr currentEvent = TTbarReader->getNextEvent();
	cout << TTbarReader->getCurrentLocalEventNumber() << endl;
	ASSERT_EQUAL(5, currentEvent->Jets().size());
}

void TestNTupleEventReader::testNumberOfMuonsInEvent1() {
	EventPtr currentEvent = TTbarReader->getNextEvent();
	ASSERT_EQUAL(0, currentEvent->Muons().size());
}

void TestNTupleEventReader::testHasNextEvent() {
	ASSERT_EQUAL(true, TTbarReader->hasNextEvent());
}

void TestNTupleEventReader::testHasNotNextEvent() {
	TTbarReader->skipNumberOfEvents(123456789);
	ASSERT(TTbarReader->hasNextEvent() == false);
}

void TestNTupleEventReader::testGetProccessedNumberOfEvents() {
	TTbarReader->getNextEvent();
	ASSERT_EQUAL(1, TTbarReader->getNumberOfProccessedEvents());
	TTbarReader->getNextEvent();
	ASSERT_EQUAL(2, TTbarReader->getNumberOfProccessedEvents());
}

void TestNTupleEventReader::testGetProccessedNumberOfEventsWithSkippedEvents() {
	TTbarReader->getNextEvent();
	TTbarReader->skipNumberOfEvents(100);
	ASSERT_EQUAL(1, TTbarReader->getNumberOfProccessedEvents());
}
void TestNTupleEventReader::testSkipEvents() {
	TTbarReader->skipNumberOfEvents(10);
	ASSERT_EQUAL(10, TTbarReader->getCurrentLocalEventNumber());
}

void TestNTupleEventReader::testGetCurrentLocalEventNumber() {
	ASSERT_EQUAL(0, TTbarReader->getCurrentLocalEventNumber());
	TTbarReader->getNextEvent();
	ASSERT_EQUAL(1, TTbarReader->getCurrentLocalEventNumber());
}

void TestNTupleEventReader::testSetMaximumNumberOfEventsWithSkippedEvents() {
	TTbarReader->setMaximumNumberOfEvents(5);
	TTbarReader->skipNumberOfEvents(10);
	ASSERT(TTbarReader->hasNextEvent() == false);
}

void TestNTupleEventReader::testSetMaximumNumberOfEvents() {
	unsigned int maxEvents = 5;
	TTbarReader->setMaximumNumberOfEvents(maxEvents);
	while (TTbarReader->hasNextEvent())
		TTbarReader->getNextEvent();
	ASSERT_EQUAL(maxEvents, TTbarReader->getNumberOfProccessedEvents());
}

void TestNTupleEventReader::testMCRunNumber() {
	ASSERT_EQUAL(1, TTbarReader->getNextEvent()->runnumber());
}

void TestNTupleEventReader::testMCLumiBlock() {
	ASSERT_EQUAL(521065, TTbarReader->getNextEvent()->lumiblock());
}

void TestNTupleEventReader::testLocalEventNumber() {
	ASSERT_EQUAL(1, TTbarReader->getNextEvent()->localnumber());
}

void TestNTupleEventReader::testInitialEventWeigth() {
	ASSERT(TTbarReader->hasNextEvent());
	ASSERT_EQUAL(1., TTbarReader->getNextEvent()->weight());
}

void TestNTupleEventReader::testPrimaryVertex() {
	ASSERT_EQUAL(true, TTbarReader->getNextEvent()->PrimaryVertex()->isGood());
}

void TestNTupleEventReader::testEventChainConstant() {
	ASSERT_EQUAL(0, strcmp("rootTupleTree/tree", NTupleEventReader::EVENT_CHAIN));
}

void TestNTupleEventReader::testTTbarEventMET() {
	EventPtr event = TTbarReader->getNextEvent();
	ASSERT_EQUAL_DELTA(7.69286, event->MET()->et(), 0.001);
}

void TestNTupleEventReader::testSeenTTbar() {
	const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenTypes = TTbarReader->getSeenDatatypes();

	for (unsigned short index = 0; index < DataType::NUMBER_OF_DATA_TYPES; ++index) {
		if (index == DataType::TTJets)
			ASSERT_EQUAL(true, seenTypes.at(index));
		else
			ASSERT_EQUAL(false, seenTypes.at(index));
	}
}

void TestNTupleEventReader::testSeenAllMC() {
	const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenTypes = AllMCReader->getSeenDatatypes();

	for (unsigned short index = 0; index < DataType::NUMBER_OF_DATA_TYPES; ++index) {
		if (index == DataType::ElectronHad || index == DataType::QCD_MuEnrichedPt15_Pt20 || index == DataType::MuHad
				|| index >= DataType::Zprime_M500GeV_W5GeV || index == DataType::VQQ) {
			if (seenTypes.at(index))
				cout << index << endl;
			ASSERT_EQUAL(false, seenTypes.at(index));
		}

		else {
			std::string msg("index ");
			std::stringstream stream;
			stream << index;
			msg += stream.str();
			ASSERT_EQUALM(msg.c_str(), true, seenTypes.at(index));
		}

	}
}

void TestNTupleEventReader::testAddInputFileNoFileThrowsException() {
	NTupleEventReaderLocalPtr reader(new NTupleEventReader());
	ASSERT_THROWS(reader->addInputFile("dfjndkjvnvh"), NoFileFoundException);
}

void TestNTupleEventReader::testGenJetsMC() {
	EventPtr event = TTbarReader->getNextEvent();
	ASSERT_EQUAL(7, event->GenJets().size());
}

void TestNTupleEventReader::testGenJetsDATA() {
	EventPtr event = DataReader->getNextEvent();
	ASSERT_EQUAL(0, event->GenJets().size());
}

cute::suite make_suite_TestNTupleEventReader() {
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testTTbarType));

	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testQCD_EMEnriched_20_to_30Type));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testQCD_EMEnriched_30_to_80Type));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testQCD_EMEnriched_80_to_170Type));

	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testQCD_BCtoE_20_to_30Type));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testQCD_BCtoE_30_to_80Type));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testQCD_BCtoE_80_to_170Type));

	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testWjetsType));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testZjetsType));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testTWType));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testTChanType));

	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testDataType));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testReadDataType));

	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testNumberOfTracksInEvent1));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testNumberOfElectronsInEvent1));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testNumberOfJetsInEvent1));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testNumberOfMuonsInEvent1));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testHasNextEvent));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testHasNotNextEvent));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testGetProccessedNumberOfEvents));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testGetProccessedNumberOfEventsWithSkippedEvents));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testSkipEvents));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testGetCurrentLocalEventNumber));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testSetMaximumNumberOfEventsWithSkippedEvents));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testSetMaximumNumberOfEvents));

	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testMCRunNumber));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testMCLumiBlock));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testLocalEventNumber));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testInitialEventWeigth));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testPrimaryVertex));
//	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testHLTTrigger));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testEventChainConstant));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testTTbarEventMET));

	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testSeenTTbar));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testSeenAllMC));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testAddInputFileNoFileThrowsException));

	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testGenJetsMC));
	s.push_back(CUTE_SMEMFUN(TestNTupleEventReader, testGenJetsDATA));

	return s;
}
