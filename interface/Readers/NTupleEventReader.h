/*
 * NTupleEventReader.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef NTUPLEEVENTREADER_H_
#define NTUPLEEVENTREADER_H_
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include "TChain.h"
#include "../Event.h"
#include "ElectronReader.h"
#include "JetReader.h"
#include "GenJetReader.h"
#include "MuonReader.h"
#include "VariableReader.h"
#include "VertexReader.h"
#include "METReader.h"
#include "METCorrReader.h"
#include "GenMETReader.h"
#include "TrackReader.h"
#include "GenParticleReader.h"
#include "PseudoTopReader.h"
#include "SelectionOutputReader.h"
#include "TTGenReader.h"
#include "../BTagWeight.h"
#include "../DataTypes.h"
#include <string>

namespace BAT {
struct NoFileFoundException: public std::exception {
	TString msg;
	NoFileFoundException(TString message) :
			msg(message) {
	}
	~NoFileFoundException() throw () {
	}

	const char* what() const throw () {
		return msg;
	}
};

class NTupleEventReader {
public:
	static const char * EVENT_CHAIN;

	static bool loadTracks;

	NTupleEventReader();
	virtual ~NTupleEventReader();
	const EventPtr getNextEvent();
	bool hasNextEvent();
	void addInputFile(const TString fileName);
	// without check for unit tests -> faster to start, no difference in long analysis
	void addInputFileWithoutCheck(const char* fileName);
	void addInputFile(const char* fileName, DataType::value type);
	void skipNumberOfEvents(unsigned long skipNextNEvents);
	unsigned long getNumberOfProccessedEvents() const;
	unsigned long getCurrentLocalEventNumber() const;
	void setMaximumNumberOfEvents(long maxNumberOfEvents);
	const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& getSeenDatatypes();
	const char* getCurrentFile() const;
private:
	unsigned long processedEvents;
	long maximalNumberOfEvents;
	long currentEventEntry;
	unsigned long numberOfFiles;
	boost::shared_ptr<TChain> input;
	// boost::shared_ptr<VariableReader<MultiIntPointer> > hltReader;
	// boost::shared_ptr<VariableReader<MultiIntPointer> > hltPrescaleReader;
	boost::scoped_ptr<VertexReader> vertexReader;
	boost::scoped_ptr<TrackReader> trackReader;
	boost::scoped_ptr<ElectronReader> electronReader;
	boost::scoped_ptr<GenParticleReader> genParticleReader;
	boost::scoped_ptr<PseudoTopReader> pseudoTopReader;
	boost::scoped_ptr<JetReader> jetReader;
	boost::scoped_ptr<GenJetReader> genJetReader;
	boost::scoped_ptr<MuonReader> muonReader;
//	boost::scoped_ptr<GenMETReader> genMetReader;
	std::vector<boost::shared_ptr<METReader> > metReaders;
	// std::vector<boost::shared_ptr<METCorrReader> > metCorrReaders;


	boost::scoped_ptr<VariableReader<bool> > passesElectronChannelTriggerReader;
	boost::scoped_ptr<VariableReader<bool> > passesMuonChannelTriggerReader;
	boost::scoped_ptr<VariableReader<bool> > passesTkMuonChannelTriggerReader;
	boost::scoped_ptr<VariableReader<bool> > passesElectronChannelMCTriggerReader;
	boost::scoped_ptr<VariableReader<bool> > passesMuonChannelMCTriggerReader;
	boost::scoped_ptr<VariableReader<bool> > passesTkMuonChannelMCTriggerReader;
	boost::scoped_ptr<VariableReader<bool> > passesElectronChannelQCDTriggerReader;
	boost::scoped_ptr<VariableReader<bool> > passesMuonChannelQCDTriggerReader;

	boost::scoped_ptr<VariableReader<MultiUIntPointer> > passesOfflineSelectionReader;
	boost::scoped_ptr<VariableReader<MultiUIntPointer> > passesGenSelectionReader;
	boost::scoped_ptr<SelectionOutputReader> selectionOutputReader_electron;
	boost::scoped_ptr<SelectionOutputReader> selectionOutputReader_muon;
	boost::scoped_ptr<SelectionOutputReader> selectionOutputReader_electronQCDNonisolated;
	boost::scoped_ptr<SelectionOutputReader> selectionOutputReader_electronQCDConversion;
	boost::scoped_ptr<SelectionOutputReader> selectionOutputReader_muonQCDNonisolated1p5to3;
	boost::scoped_ptr<SelectionOutputReader> selectionOutputReader_muonQCDNonisolated3toInf;

	boost::scoped_ptr<TTGenReader> ttGenInfoReader;

	boost::scoped_ptr<VariableReader<unsigned int> > runNumberReader;
	boost::scoped_ptr<VariableReader<unsigned int> > eventNumberReader;
	boost::scoped_ptr<VariableReader<unsigned int> > lumiBlockReader;
	boost::scoped_ptr<VariableReader<bool> > passesMetFilterReader_;

	boost::scoped_ptr<VariableReader<double> > generatorWeightReader_;
	boost::scoped_ptr<VariableReader<double> > centralLHEWeightReader_;
	boost::scoped_ptr<VariableReader<MultiDoublePointer> > systematicWeightsReader_;
	boost::scoped_ptr<VariableReader<MultiIntPointer> > systematicWeightsReaderID_;
	boost::scoped_ptr<VariableReader<MultiIntPointer> > PileupInfoReader;
	boost::scoped_ptr<VariableReader<MultiIntPointer> > TruePileupInfoReader;
	boost::scoped_ptr<VariableReader<double> > PUWeightInTimeOnly_, PUWeight3BX_, // PUWeight3D_,
			PUWeightShiftUp_, PUWeightShiftDown_;
	boost::scoped_ptr<VariableReader<double> > sumETReader_;
	boost::scoped_ptr<VariableReader<bool> > HCALLaserFilter, ECALDeadCellFilter, ECALDeadCellTPFilter, TrackingFailureFilter,
								CSCTightHaloId, EEBadSCFilter, ECALLaserCorrFilter, TrackingPOGFilters;
	bool areReadersSet, areDatatypesKnown;
	EventPtr currentEvent;
	boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenDataTypes;

	void selectNextNtupleEvent();
	void initiateReadersIfNotSet();
	void readDataTypes();
};

typedef boost::scoped_ptr<NTupleEventReader> NTupleEventReaderLocalPtr;

} //end namespace BAT

#endif /* NTUPLEEVENTREADER_H_ */
