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
#include "EventWeightProvider.h"
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
	ElectronCollection goodElectrons;
	ElectronCollection goodIsolatedElectrons;
	ElectronCollection goodPFIsolatedElectrons;
	ElectronCollection looseElectrons;
	ElectronCollection qcdElectrons;

	JetCollection allJets;
	JetCollection genJets;
	JetCollection goodJets;
	JetCollection goodBJets;
	JetCollection goodElectronCleanedJets;
	JetCollection goodElectronCleanedBJets;
	JetCollection goodMuonCleanedJets;
	JetCollection goodMuonCleanedBJets;

	MuonCollection allMuons;
	MuonCollection goodMuons;
	MuonCollection goodIsolatedMuons;
	MuonCollection goodPFIsolatedMuons;
	MuonCollection looseMuons;

	MCParticleCollection genParticles;

	METPointer met;

	DataType::value dataType;
	unsigned long runNumber;
	unsigned long eventNumber;
	unsigned long localEventNumber;
	unsigned long lumiBlock;
	double eventWeight;
	double pileUpWeight;

//    double jetCleaningEfficiency;
	unsigned int numberOfHighPurityTracks;
	bool isBeamScraping;

	std::vector<int> genNumberOfPileUpVertices;

	double ptdensityRho;

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
	void setMET(const METPointer met);
	void setHLTs(const boost::shared_ptr<std::vector<int> >);
	void setHLTPrescales(const boost::shared_ptr<std::vector<int> >);
	void setRunNumber(unsigned long number);
	void setEventNumber(unsigned long number);
	void setLocalEventNumber(unsigned long number);
	void setLumiBlock(unsigned long block);
	void setEventWeight(double weight);
	void setPileUpWeight(double weight);
	void setBeamScrapingVeto(bool isScraping);
	void setGenNumberOfPileUpVertices(std::vector<int> pileup);
	void setPtDensityRho(double rho);

	const VertexPointer PrimaryVertex() const;
	const VertexCollection& Vertices() const;
	const VertexCollection& GoodVertices() const;
	const TrackCollection& Tracks() const;
	const MCParticleCollection& GenParticles() const;
	const ElectronCollection& Electrons() const;
	const ElectronCollection& GoodElectrons() const;
	const ElectronCollection& GoodIsolatedElectrons() const;
	const ElectronCollection& GoodPFIsolatedElectrons() const;
	const ElectronCollection& QCDElectrons() const;
	const JetCollection& Jets() const;
	const JetCollection& GenJets() const;
	const JetCollection& GoodJets() const;
	const JetCollection& GoodElectronCleanedJets() const;
	const JetCollection& GoodMuonCleanedJets() const;
	const JetCollection& GoodBJets() const;
	const JetCollection& GoodElectronCleanedBJets() const;
	const JetCollection& GoodMuonCleanedBJets() const;
	const MuonCollection& Muons() const;
	const MuonCollection& GoodMuons() const;
	const MuonCollection& GoodIsolatedMuons() const;
	const MuonCollection& GoodPFIsolatedMuons() const;
	const METPointer MET() const;

	const ElectronPointer MostIsolatedElectron(const ElectronCollection&, bool usePFIso) const;
	const ElectronPointer MostIsolatedElectron(const ElectronCollection&) const;
	const ElectronPointer MostPFIsolatedElectron(const ElectronCollection&) const;

	const MuonPointer MostIsolatedMuon(const MuonCollection&, bool usePFIso) const;
	const MuonPointer MostIsolatedMuon(const MuonCollection& muons) const;
	const MuonPointer MostPFIsolatedMuon(const MuonCollection&) const;

	JetCollection GetBJetCollection(const JetCollection& jets, BtagAlgorithm::value btagAlgorithm,
			BtagAlgorithm::workingPoint WP = BtagAlgorithm::MEDIUM) const;
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
	static bool useCiCElectronID;

//	double averageNumberOfGeneratedPileUpVertices() const;
	const std::vector<int> GeneratedPileUpVertices() const;
	double numberOfGeneratedPileUpVertices(PileUpReweightingMethod::value method =
			Globals::pileUpReweightingMethod) const;
	double rho() const;

private:
	void selectElectronsByQuality();
	void cleanGoodJets();
	JetCollection cleanGoodJetsAgainstIsolatedElectrons(const ElectronCollection& electrons) const;
	JetCollection cleanGoodJetsAgainstIsolatedMuons(const MuonCollection& muons) const;
	JetCollection cleanGoodJetsAgainstMostIsolatedLepton(const LeptonPointer mostIsolatedLepton) const;
	void selectGoodJets();
	void selectMuonsByQuality();
	void selectVerticesByQuality();
};

}

#endif /* EVENT_H_ */
