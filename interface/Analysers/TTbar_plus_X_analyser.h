/*
 * TTbar_plus_X_analyser.h
 *
 *  Created on: 22 Mar 2012
 *      Author: kreczko
 */

#ifndef TTBARPLUSMETANALYSER_H_
#define TTBARPLUSMETANALYSER_H_

#include "BasicAnalyser.h"
#include "METAnalyser.h"
#include "ElectronAnalyser.h"
#include "MuonAnalyser.h"
#include "VertexAnalyser.h"
#include "JetAnalyser.h"
#include "Binned_variable_analyser.h"
#include "../Selections/BasicSelection.h"
#include "../../interface/Selections/QCDNoIsoNoIDSelection.h"

#include <string>

namespace BAT {

class TTbar_plus_X_analyser: public BAT::BasicAnalyser {
public:
	TTbar_plus_X_analyser(HistogramManagerPtr histMan, std::string histogramFolder = "TTbar_plus_X_analysis");
	virtual ~TTbar_plus_X_analyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();
	void ePlusJetsQcdAnalysis(const EventPtr);
	void muPlusJetsQcdAnalysis(const EventPtr);
	void ePlusJetsSignalAnalysis(const EventPtr);
	void muPlusJetsSignalAnalysis(const EventPtr);

private:
	//signal selections
	SelectionPointer topEplusJetsRefSelection_, topMuplusJetsRefSelection_;
	//QCD selections with respect to reference selection
	SelectionPointer qcdNonIsoElectronSelection_;
	SelectionPointer qcdConversionSelection_;
	SelectionPointer qcdPFRelIsoEPlusJetsSelection_, qcdPFRelIsoMuPlusJetsSelection_,
			qcd_noniso_muon_plus_jets_selection_, qcd_noiso_muon_plus_jets_selection_;

	/**
	 * Analysers
	 */
	//signal regions
	METAnalyserLocalPtr metAnalyserEPlusJetsRefSelection_, metAnalyserMuPlusJetsRefSelection_;
	ElectronAnalyserLocalPtr electronAnalyserRefSelection_;
	MuonAnalyserLocalPtr muonAnalyserRefSelection_;
	BasicAnalyserLocalPtr vertexAnalyserEPlusJetsRefSelection_, vertexAnalyserMuPlusJetsRefSelection_;

	//QCD region Non-isolated electrons
	METAnalyserLocalPtr metAnalyserqcdNonIsoElectronSelection_, metAnalyserqcdNonIsoMuonSelection_;
	ElectronAnalyserLocalPtr qcdNonIsoElectronAnalyser_;
	MuonAnalyserLocalPtr qcdNonIsoMuonAnalyser_;
	//QCD region electrons from conversions
	METAnalyserLocalPtr metAnalyserqcdConversionSelection_;
	ElectronAnalyserLocalPtr qcdConversionsElectronAnalyser_;
	//No iso
	ElectronAnalyserLocalPtr qcdEPlusjetsPFRelIsoElectronAnalyser_;
	MuonAnalyserLocalPtr qcdMuPlusjetsPFRelIsoMuonAnalyser_, qcdNoIsolationMuonAnalyser_;

	std::vector<double> metBins_, ht_bins_, st_bins_, mt_bins_, wpt_bins_;
	//MET analysers electron
	std::vector<Binned_Variable_analyser_ptr> ref_selection_binned_MET_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_conversion_binned_MET_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_noniso_binned_MET_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_MET_analyser_electron_isolation_; //for QCD rate estimation
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_MET_analyser_electron_rhocorrected_isolation_; //for QCD rate estimation
	//MET analysers muon
	std::vector<Binned_Variable_analyser_ptr> ref_selection_binned_MET_analyser_muon_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_noniso_binned_MET_analyser_muon_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_MET_analyser_muon_eta_;

	//HT analysers electron
	Binned_Variable_analyser_ptr ref_selection_binned_HT_analyser_electron_eta_;
	Binned_Variable_analyser_ptr qcd_conversion_binned_HT_analyser_electron_eta_;
	Binned_Variable_analyser_ptr qcd_noniso_binned_HT_analyser_electron_eta_;
	Binned_Variable_analyser_ptr qcd_PFRelIso_binned_HT_analyser_electron_isolation_; //for QCD rate estimation
	Binned_Variable_analyser_ptr qcd_PFRelIso_binned_HT_analyser_electron_rhocorrected_isolation_; //for QCD rate estimation
	//HT analysers muon
	Binned_Variable_analyser_ptr ref_selection_binned_HT_analyser_muon_eta_;
	Binned_Variable_analyser_ptr qcd_noniso_binned_HT_analyser_muon_eta_;
	Binned_Variable_analyser_ptr qcd_PFRelIso_binned_HT_analyser_muon_eta_;

	//ST analysers electron
	std::vector<Binned_Variable_analyser_ptr> ref_selection_binned_ST_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_conversion_binned_ST_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_noniso_binned_ST_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_ST_analyser_electron_isolation_; //for QCD rate estimation
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_ST_analyser_electron_rhocorrected_isolation_; //for QCD rate estimation
	//ST analysers muon
	std::vector<Binned_Variable_analyser_ptr> ref_selection_binned_ST_analyser_muon_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_noniso_binned_ST_analyser_muon_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_ST_analyser_muon_eta_;

	//MT analysers electron
	std::vector<Binned_Variable_analyser_ptr> ref_selection_binned_MT_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_conversion_binned_MT_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_noniso_binned_MT_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_MT_analyser_electron_isolation_; //for QCD rate estimation
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_MT_analyser_electron_rhocorrected_isolation_; //for QCD rate estimation
	//MT analysers muon
	std::vector<Binned_Variable_analyser_ptr> ref_selection_binned_MT_analyser_muon_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_noniso_binned_MT_analyser_muon_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_MT_analyser_muon_eta_;

	//WPT analysers electron
	std::vector<Binned_Variable_analyser_ptr> ref_selection_binned_WPT_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_conversion_binned_WPT_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_noniso_binned_WPT_analyser_electron_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_WPT_analyser_electron_isolation_; //for QCD rate estimation
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_WPT_analyser_electron_rhocorrected_isolation_; //for QCD rate estimation
	//WPT analysers muon
	std::vector<Binned_Variable_analyser_ptr> ref_selection_binned_WPT_analyser_muon_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_noniso_binned_WPT_analyser_muon_eta_;
	std::vector<Binned_Variable_analyser_ptr> qcd_PFRelIso_binned_WPT_analyser_muon_eta_;

	JetAnalyserLocalPtr jetAnalyserEPlusJetsRefSelection_, jetAnalyserMuPlusJetsRefSelection_;

	void make_binned_MET_analysers();
	void make_binned_HT_analysers();
	void make_binned_ST_analysers();
	void make_binned_MT_analysers();
	void make_binned_WPT_analysers();
};

typedef boost::scoped_ptr<BAT::TTbar_plus_X_analyser> TTbar_plus_X_analyser_local_ptr;
} /* namespace BAT */
#endif /* TTBARPLUSMETANALYSER_H_ */
