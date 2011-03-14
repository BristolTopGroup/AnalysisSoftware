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
#include "TrackReader.h"
#include "GenParticleReader.h"
#include <string>
#include "../Constants.h"
#include "../CrossSections.h"

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

	static JetAlgorithm::value jetAlgorithm;
	static ElectronAlgorithm::value electronAlgorithm;
	static METAlgorithm::value metAlgorithm;
	static MuonAlgorithm::value muonAlgorithm;
	static bool loadTracks;

	NTupleEventReader();
	virtual ~NTupleEventReader();
	const Event& getNextEvent();
	bool hasNextEvent();
	void addInputFile(const char* fileName);
	// without check for unit tests -> faster to start, no difference in long analysis
	void addInputFileWithoutCheck(const char* fileName);
	void addInputFile(const char* fileName, DataType::value type);
	void skipNumberOfEvents(unsigned long skipNextNEvents);
	unsigned long getNumberOfProccessedEvents() const;
	unsigned long getCurrentLocalEventNumber() const;
	void setMaximumNumberOfEvents(unsigned long maxNumberOfEvents);
	const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& getSeenDatatypes();
	const char* getCurrentFile() const;
	Double_t test;
private:
	unsigned long processedEvents;
	unsigned long maximalNumberOfEvents;
	unsigned long currentEventEntry;
	unsigned long numberOfFiles;
	boost::shared_ptr<TChain> input;
	boost::shared_ptr<VariableReader<MultiIntPointer> > hltReader;
	boost::scoped_ptr<VertexReader> primaryReader;
	boost::scoped_ptr<TrackReader> trackReader;
	boost::scoped_ptr<ElectronReader> electronReader;
	boost::scoped_ptr<GenParticleReader> genParticleReader;
	boost::scoped_ptr<JetReader> jetReader;
	boost::scoped_ptr<GenJetReader> genJetReader;
	boost::scoped_ptr<MuonReader> muonReader;
	boost::scoped_ptr<METReader> metReader;

	boost::scoped_ptr<VariableReader<unsigned int> > runNumberReader;
	boost::scoped_ptr<VariableReader<unsigned int> > eventNumberReader;
	boost::scoped_ptr<VariableReader<unsigned int> > lumiBlockReader;
	boost::scoped_ptr<VariableReader<bool> > beamScrapingReader;
	bool areReadersSet, areDatatypesKnown;
	Event currentEvent;
	boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenDataTypes;

	void selectNextNtupleEvent();
	void initiateReadersIfNotSet();
	DataType::value getDataType(const std::string filename);
	void readDataTypes();
};
}

#endif /* NTUPLEEVENTREADER_H_ */
