/*
 * Event.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef EVENT_H_
#define EVENT_H_
#include "RecoObjects/Electron.h"
#include "RecoObjects/Jet.h"
#include "RecoObjects/Muon.h"
#include "RecoObjects/Vertex.h"
#include "RecoObjects/MET.h"
#include "RecoObjects/MCParticle.h"
#include "DataTypes.h"
#include "Printers/EventContentPrinter.h"
#include "HighLevelTriggers.h"

#include <boost/shared_ptr.hpp>
#include <string>

//forward definition of event needed before BasicSelection inclusion.
namespace BAT {
class Event;
typedef boost::shared_ptr<Event> EventPtr;
}
//include BasicSelection to have access to the selections.
#include "Selections/BasicSelection.h"
#include "GlobalVariables.h"
namespace BAT {

class Event {
protected:
	boost::shared_ptr<std::vector<int> > HLTs;
	boost::shared_ptr<std::vector<int> > HLTPrescales;
	VertexCollection vertices;
	VertexCollection goodVertices;
	TrackCollection tracks;
	ElectronCollection allElectrons;

	JetCollection allJets;
	JetCollection genJets;

	MuonCollection allMuons;

	MCParticleCollection genParticles;

	METCollection mets_;
	METPointer genMet_;

	DataType::value dataType;
	unsigned long runNumber;
	unsigned long eventNumber;
	unsigned long localEventNumber;
	unsigned long lumiBlock;
	double eventWeight;
	double pileUpWeight_;
	std::vector<double> bjetWeights_;
	//These values are calculated with PileUp_2011_truth_finebin.root
	double PUWeightInTimeOnly_, PUWeight3BX_, // PUWeight3D_,
			PUWeightShiftUp_, PUWeightShiftDown_;

	unsigned int numberOfHighPurityTracks_;
	bool isBeamScraping_;

	std::vector<int> genNumberOfPileUpVertices;
	std::vector<int> trueNumberOfPileUpVertices_;
	std::vector<double> pdfWeights;

	double ptdensityRho;
	std::string file_;

	bool passesCSCTightBeamHaloFilter_, passesHBHENoiseFilter_, passesHCALLaserFilter_, passesECALDeadCellFilter_, passesECALDeadCellTPFilter_,
			passesTrackingFailureFilter_, passesEEBadSCFilter_, passesECALLaserCorrFilter_, passesTrackingPOGFilters_;

public:
	Event();
	virtual ~Event();
	bool isRealData() const;
	const DataType::value getDataType() const;
	void setDataType(DataType::value type);
	void setVertices(VertexCollection vertices);
	void setTracks(TrackCollection tracks);
	void setGenParticles(MCParticleCollection genParticles);
	void setElectrons(ElectronCollection electrons);
	void setJets(JetCollection jets);
	void setGenJets(JetCollection genJets);
	void setMuons(MuonCollection muons);
	void setMETs(const std::vector<METPointer> mets);
	void setGenMET(const METPointer met);
	void setHLTs(const boost::shared_ptr<std::vector<int> >);
	void setHLTPrescales(const boost::shared_ptr<std::vector<int> >);
	void setFile(std::string file);
	void setRunNumber(unsigned long number);
	void setEventNumber(unsigned long number);
	void setLocalEventNumber(unsigned long number);
	void setLumiBlock(unsigned long block);
	void setEventWeight(double weight);
	void setPileUpWeight(double weight);
	void setBeamScrapingVeto(bool isScraping);
	void setGenNumberOfPileUpVertices(std::vector<int> pileup);
	void setTrueNumberOfPileUpVertices(std::vector<int> pileup);
	void setPDFWeights(std::vector<double> pdfWeights);
	void setPtDensityRho(double rho);
	void setPUWeightInTimeOnly(double puweight);
	void setPUWeight3BX(double puweight);
	void setPUWeightShiftUp(double puweight);
	void setPUWeightShiftDown(double puweight);
	void setCSCTightBeamHaloFilter(bool result);
	void setHBHENoiseFilter(bool result);
	void setHCALLaserFilter(bool result);
	void setECALDeadCellFilter(bool result);
	void setECALDeadCellTPFilter(bool result);
	void setTrackingFailureFilter(bool result);
	void setEEBadSCFilter(bool result);
	void setECALLaserCorrFilter(bool result);
	void setTrackingPOGFilters(bool result);

	const VertexPointer PrimaryVertex() const;
	const VertexCollection& Vertices() const;
	const VertexCollection& GoodVertices() const;
	const TrackCollection& Tracks() const;
	const MCParticleCollection& GenParticles() const;
	const ElectronCollection& Electrons() const;
	const ElectronCollection& QCDElectrons() const;
	const JetCollection& Jets() const;
	const JetCollection& GenJets() const;
	const MuonCollection& Muons() const;
	const METPointer MET() const;
	const METPointer MET(METAlgorithm::value type) const;
	const METPointer GenMET() const;

	const ElectronPointer MostIsolatedElectron(const ElectronCollection&, bool usePFIso) const;
	const ElectronPointer MostIsolatedElectron(const ElectronCollection&) const;
	const ElectronPointer MostPFIsolatedElectron(const ElectronCollection&) const;

	const MuonPointer MostIsolatedMuon(const MuonCollection&, bool usePFIso) const;
	const MuonPointer MostIsolatedMuon(const MuonCollection& muons) const;
	const MuonPointer MostPFIsolatedMuon(const MuonCollection&) const;

	JetCollection GetBJetCollection(const JetCollection& jets, BtagAlgorithm::value btagAlgorithm,
			BtagAlgorithm::workingPoint WP = BtagAlgorithm::MEDIUM) const;
	std::string file() const;
	unsigned long runnumber() const;
	unsigned long eventnumber() const;
	unsigned long localnumber() const;
	unsigned long lumiblock() const;
	double weight() const;
	double PileUpWeight() const;
	void inspect() const;
	bool HLT(HLTriggers::value trigger) const;
	int HLTPrescale(HLTriggers::value trigger) const;
	static bool useCustomConversionTagger;
	static bool usePFIsolation;

	const std::vector<int> GeneratedPileUpVertices() const;
	const std::vector<double> PDFWeights() const;
	double averageNumberOfVertices() const;
	double inTimeOnlyNumberOfVertices() const;
	double rho() const;
	unsigned int numberOfHighPurityTracks() const;
	bool isBeamScraping() const;
	const std::vector<int>& getTrueNumberOfVertices() const;

	double PUWeightInTimeOnly() const;
	double PUWeight3BX() const;
	double PUWeightShiftUp() const;
	double PUWeightShiftDown() const;

	bool passesCSCTightBeamHaloFilter() const;
	bool passesHBHENoiseFilter() const;
	bool passesHCALLaserFilter() const;
	bool passesECALDeadCellFilter() const;
	bool passesECALDeadCellTPFilter() const;
	bool passesTrackingFailureFilter() const;
	bool passesEEBadSCFilter() const;
	bool passesECALLaserCorrFilter() const;
	bool passesTrackingPOGFilters() const;

	static double HT(const JetCollection jets);
	static double ST(const JetCollection, const ParticlePointer, const METPointer);
	static double MT(const ParticlePointer, const METPointer);
	static double WPT(const ParticlePointer, const METPointer);

};

}

#endif /* EVENT_H_ */
