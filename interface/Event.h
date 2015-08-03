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
#include "RecoObjects/PseudoTopParticles.h"
#include "RecoObjects/SelectionOutputInfo.h"
#include "RecoObjects/TTGenInfo.h"
#include "DataTypes.h"
#include "Printers/EventContentPrinter.h"
#include "HighLevelTriggers.h"
#include "TtbarHypothesis.h"

#include <boost/shared_ptr.hpp>
#include <string>

//forward definition of event needed before BasicSelection inclusion.
namespace BAT {
class Event;
typedef boost::shared_ptr<Event> EventPtr;
}
#include "GlobalVariables.h"
namespace BAT {

namespace SelectionCriteria {
enum selection {
	MuonPlusJetsReference = 1,
	ElectronPlusJetsReference = 2,
	MuonPlusJetsQCDNonIsolated = 3,
	ElectronPlusJetsQCDNonIsolated = 4,
	ElectronPlusJetsQCDConversion = 5,
	NUMBER_OF_SELECTION_STEPS
};
}

class Event {
protected:
	boost::shared_ptr<std::vector<int> > HLTs;
	boost::shared_ptr<std::vector<int> > HLTPrescales;
	VertexCollection vertices;
	VertexCollection goodVertices;
	TrackCollection tracks;
	ElectronCollection allElectrons;

	JetCollection allJets;
	JetCollection cleanedJets_;
	JetCollection cleanedBJets_;
	JetCollection genJets;

	MuonCollection allMuons;

	bool passesElectronChannelTrigger_;
	bool passesElectronChannelQCDTrigger_;
	bool passesMuonChannelTrigger_;
	bool passesMuonChannelQCDTrigger_;

	bool passesElectronSelection_;
	bool passesElectronQCDSelection_;
	bool passesElectronConversionSelection_;
	bool passesMuonSelection_;
	bool passesMuonQCDSelection_;

	bool passesElectronSelectionNoB_;
	bool passesElectronQCDSelectionNoB_;
	bool passesElectronConversionSelectionNoB_;
	bool passesMuonSelectionNoB_;
	bool passesMuonQCDSelectionNoB_;

	bool isSemiLeptonicElectron_;
	bool isSemiLeptonicMuon_;
	SelectionOutputInfo selectionOutputInfo_electron;
	SelectionOutputInfo selectionOutputInfo_muon;
	SelectionOutputInfo selectionOutputInfo_electronQCDNonisolated;
	SelectionOutputInfo selectionOutputInfo_electronQCDConversion;
	SelectionOutputInfo selectionOutputInfo_muonQCDNonisolated;

	TTGenInfoPointer ttGenInfo_;

	PseudoTopParticlesPointer pseudoTopParticles_;
	MCParticleCollection genParticles;

	METCollection mets_;
	METPointer genMet_;

	TtbarHypothesis ttbarHypothesis_;

	DataType::value dataType;
	unsigned long runNumber;
	unsigned long eventNumber;
	unsigned long localEventNumber;
	unsigned long lumiBlock;
	double eventWeight;
	double pileUpWeight_;
	double bJetWeight_;
	//These values are calculated with PileUp_2011_truth_finebin.root
	double PUWeightInTimeOnly_, PUWeight3BX_, // PUWeight3D_,
			PUWeightShiftUp_, PUWeightShiftDown_;

	unsigned int numberOfHighPurityTracks_;
	bool isBeamScraping_;

	std::vector<int> genNumberOfPileUpVertices;
	std::vector<int> trueNumberOfPileUpVertices_;
	double generatorWeight_;
	double centralLHEWeight_;
	std::vector<double> generatorSystematicWeights_;

	double ptdensityRho;
	std::string file_;

	bool passesCSCTightBeamHaloFilter_, passesHBHENoiseFilter_, passesHCALLaserFilter_, passesECALDeadCellFilter_, passesECALDeadCellTPFilter_,
			passesTrackingFailureFilter_, passesEEBadSCFilter_, passesECALLaserCorrFilter_, passesTrackingPOGFilters_;

public:
	Event();
	virtual ~Event();
	bool isRealData() const;
	const DataType::value getDataType() const;
	bool isTTJet(DataType::value type) const;
	void setDataType(DataType::value type);
	void setVertices(VertexCollection vertices);
	void setTracks(TrackCollection tracks);
	void setGenParticles(MCParticleCollection genParticles);
	void setElectrons(ElectronCollection electrons);
	void setJets(JetCollection jets);
	void setJetTTBarPartons();
	void setCleanedJets(JetCollection jets);
	void setCleanedBJets(JetCollection bjets);
	void setGenJets(JetCollection genJets);
	void setMuons(MuonCollection muons);
	void setPassesElectronChannelTrigger( bool passesTrigger );
	void setPassesElectronChannelQCDTrigger( bool passesTrigger );
	void setPassesMuonChannelTrigger( bool passesTrigger );
	void setPassesMuonChannelQCDTrigger( bool passesTrigger );
	void setPassesElectronSelection(bool passesElectronSelection);
	void setPassesElectronQCDSelection(bool passesElectronQCDSelection);
	void setPassesElectronConversionSelection(bool passesElectronConversionSelection);
	void setPassesMuonSelection(bool passesMuonSelection);
	void setPassesMuonQCDSelection(bool passesMuonQCDSelection);
	void setPassesElectronSelectionNoB(bool passesElectronSelectionNoB);
	void setPassesElectronQCDSelectionNoB(bool passesElectronQCDSelectionNoB);
	void setPassesElectronConversionSelectionNoB(bool passesElectronConversionSelectionNoB);
	void setPassesMuonSelectionNoB(bool passesMuonSelectionNoB);
	void setPassesMuonQCDSelectionNoB(bool passesMuonQCDSelectionNoB);
	void setPassOfflineSelectionInfo( std::vector<unsigned int> );
	const bool passesJetSelection( const unsigned int selectionCriteria );
	const bool passesBJetSelection( const unsigned int selectionCriteria );
	const bool passesSignalLeptonSelection( const unsigned int selectionCriteria );
	void setPassGenSelectionInfo( std::vector<unsigned int> );
	void setIsSemiLeptonicElectron( bool isSemiLeptonicElectron );
	void setIsSemiLeptonicMuon( bool isSemiLeptonicMuon );
	void setElectronSelectionOutputInfo(SelectionOutputInfo newSelectionOutputInfo);
	void setMuonSelectionOutputInfo(SelectionOutputInfo newSelectionOutputInfo);
	void setElectronQCDNonisolatedSelectionOutputInfo(SelectionOutputInfo newSelectionOutputInfo);
	void setElectronConversionSelectionOutputInfo(SelectionOutputInfo newSelectionOutputInfo);
	void setMuonQCDNonisolatedSelectionOutputInfo(SelectionOutputInfo newSelectionOutputInfo);
	void setTTGenInfo(TTGenInfoPointer ttGenInfo );
	void setPseudoTopParticles(PseudoTopParticlesPointer newPseudoParticles );
	void setMETs(const std::vector<METPointer> mets);
	void setGenMET(const METPointer met);
	void setTTbarHypothesis(const TtbarHypothesis newHypo);
	void setHLTs(const boost::shared_ptr<std::vector<int> >);
	void setHLTPrescales(const boost::shared_ptr<std::vector<int> >);
	void setFile(std::string file);
	void setRunNumber(unsigned long number);
	void setEventNumber(unsigned long number);
	void setLocalEventNumber(unsigned long number);
	void setLumiBlock(unsigned long block);
	void setEventWeight(double weight);
	void setPileUpWeight(double weight);
	void setBJetWeight(double weight);
	void setBeamScrapingVeto(bool isScraping);
	void setGenNumberOfPileUpVertices(std::vector<int> pileup);
	void setTrueNumberOfPileUpVertices(std::vector<int> pileup);
	void setGeneratorWeight( double generatorWeight );
	void setCentralLHEWeight( double centralLHEWeight );
	void setGeneratorSystematicWeights( std::vector<double> generatorSystematicWeights );
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
	const JetCollection& CleanedJets() const;
	const JetCollection& CleanedBJets() const;
	const JetCollection& GenJets() const;
	const MuonCollection& Muons() const;
	const METPointer MET() const;
	const METPointer MET(METAlgorithm::value type) const;
	const METPointer GenMET() const;

	const TtbarHypothesis ttbarHypothesis() const;

	const bool PassesElectronChannelTrigger() const;
	const bool PassesMuonChannelTrigger() const;

	const bool PassesElectronSelection() const;
	const bool PassesElectronQCDSelection() const;
	const bool PassesElectronConversionSelection() const;
	const bool PassesMuonSelection() const;
	const bool PassesMuonQCDSelection() const;

	const bool PassesElectronTriggerAndSelection() const;
	const bool PassesMuonTriggerAndSelection() const;
	const bool PassesElectronTriggerAndQCDSelection() const;
	const bool PassesElectronTriggerAndConversionSelection() const;
	const bool PassesMuonTriggerAndQCDSelection() const;
	
	const bool PassesElectronSelectionNoB() const;
	const bool PassesElectronQCDSelectionNoB() const;
	const bool PassesElectronConversionSelectionNoB() const;
	const bool PassesMuonSelectionNoB() const;
	const bool PassesMuonQCDSelectionNoB() const;

	const bool PassesElectronTriggerAndSelectionNoB() const;
	const bool PassesMuonTriggerAndSelectionNoB() const;
	const bool PassesElectronTriggerAndQCDSelectionNoB() const;
	const bool PassesElectronTriggerAndConversionSelectionNoB() const;
	const bool PassesMuonTriggerAndQCDSelectionNoB() const;

	const bool isSemiLeptonicElectron() const;
	const bool isSemiLeptonicMuon() const;
	
	const TTGenInfoPointer TTGenInfo() const;

	const PseudoTopParticlesPointer PseudoTopParticles() const;

	const LeptonPointer getSignalLepton( unsigned int selectionCriteria ) const;
	const JetCollection getCleanedJets( unsigned int selectionCriteria ) const;
	const JetCollection getCleanedBJets( unsigned int selectionCriteria ) const;
	const unsigned int getNBJets( unsigned int selectionCriteria ) const;

	JetCollection GetBJetCollection(const JetCollection& jets, BtagAlgorithm::value btagAlgorithm,
			BtagAlgorithm::workingPoint WP = BtagAlgorithm::MEDIUM) const;
	std::string file() const;
	unsigned long runnumber() const;
	unsigned long eventnumber() const;
	unsigned long localnumber() const;
	unsigned long lumiblock() const;
	double weight() const;
	double PileUpWeight() const;
	double BJetWeight() const;
	void inspect() const;
	bool HLT(HLTriggers::value trigger) const;
	int HLTPrescale(HLTriggers::value trigger) const;
	static bool useCustomConversionTagger;
	static bool usePFIsolation;

	const std::vector<int> GeneratedPileUpVertices() const;
	const double generatorWeight() const;
	const double centralLHEWeight() const;
	const std::vector<double> generatorSystematicWeights() const;
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
	static double M3(const JetCollection jets);
	static double M_bl(const JetCollection, const ParticlePointer);
	static double angle_bl(const JetCollection, const ParticlePointer);
	static unsigned int NJets(const JetCollection);
	static double ST(const JetCollection, const ParticlePointer, const METPointer);
	static double MT(const ParticlePointer, const METPointer);
	static double WPT(const ParticlePointer, const METPointer);

	static double const minJetPt_;
	static unsigned int const minNJets_;
	static unsigned int const minNBJets_;

	static double const minSignalMuonPt_;
	static double const minSignalElectronPt_;

};

}

#endif /* EVENT_H_ */
