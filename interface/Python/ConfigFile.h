/*
 * ConfigFile.h
 *
 *  Created on: 20 Oct 2011
 *      Author: kreczko
 */

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_
#include <boost/python.hpp>
#include <string>
#include <vector>
#include <boost/program_options.hpp>
#include <boost/program_options/options_description.hpp>
#include "../EventWeightProvider.h"
#include "TH1D.h"
#include "TH2F.h"
#include "TH3F.h"
#include "TCanvas.h"
#include "TEfficiency.h"
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include "TGraphAsymmErrors.h"

namespace BAT {

class ConfigFile {
public:
	ConfigFile(int argc, char **argv);
	virtual ~ConfigFile();

	void loadIntoMemory();
	long maxEvents() const;
	std::string datasetInfoFile() const;
	std::string PUFile() const;
	std::string PUFile_up() const;
	std::string PUFile_down() const;
	std::string TTbarLikelihoodFile() const;
	std::string BTagEfficiencyFile() const;
	std::string MuonIdScaleFactorsFile() const;
	std::string MuonIsoScaleFactorsFile() const;
	std::string MuonTriggerScaleFactorsFile() const;
	std::string MuonTrackingScaleFactorsFile() const;
	std::string ElectronTriggerScaleFactorsFile() const;
	std::string ElectronIdScaleFactorsFile() const;
	std::string ElectronRecoScaleFactorsFile() const;
	std::string bJetResoFile() const;
    std::string lightJetResoFile() const;
    std::string hadronTriggerFile() const;
	bool useHitFit() const;
	bool fitterOutputFlag() const;
	const std::vector<std::string>& inputFiles() const;
	std::string configPath() const;
	std::string TQAFPath() const;
	double lumi() const;
	PileUpReweightingMethod::value PileUpReweightingMethod() const;
	bool isOptionSet(std::string option) const;
	bool isOptionSetInConsole(std::string option) const;
	bool isOptionSetInConfig(std::string option) const;
	unsigned int centerOfMassEnergy() const;
	unsigned int nTupleVersion() const;
	std::string sample() const;
	std::string mode() const;
	int electronScaleFactorSystematic() const;
	int muonScaleFactorSystematic() const;
	int jesSystematic() const;
	int jetSmearingSystematic() const;
	int LightTagSystematic() const;
	int BtagSystematic() const;
	std::string custom_file_suffix() const;
	unsigned int pdfWeightNumber() const;
	bool applyMetSysShiftCorr() const;
	bool applyMetType0Corr() const;
	bool applyMetType1Corr() const;
	bool applyJetSmearing() const;
	bool applyTopPtReweighting() const;

	bool verbose() const;

private:
	boost::program_options::variables_map programOptions;
	boost::python::object config;
	long maxEvents_;
	std::string datasetInfoFile_;
	std::string pileUpFile_, pileUpFile_up_, pileUpFile_down_;
	std::string ttbarLikelihoodFile_;
	std::string btagEfficiencyFile_;
	bool getMuonScaleFactorsFromFile_;
	std::string muonIdScaleFactorsFile_;
	std::string muonIsoScaleFactorsFile_;
	std::string muonTriggerScaleFactorsFile_;
	std::string muonTrackingScaleFactorsFile_;
	bool getElectronScaleFactorsFromFile_;
	std::string electronIdScaleFactorsFile_;
	std::string electronRecoScaleFactorsFile_;
	std::string electronTriggerScaleFactorsFile_;
	std::string bJetResoFile_;
    std::string lightJetResoFile_;
    bool getHadronTriggerFromFile_;
    std::string hadronTriggerFile_;
	bool useHitFit_;
	bool fitterOutputFlag_;
	std::vector<std::string> inputFiles_;
	std::string tqafPath_;
	double lumi_;
	unsigned int centerOfMassEnergy_;
	unsigned int nTupleVersion_;
	std::string sample_, mode_;
	int electronScaleFactorSystematic_;
	int muonScaleFactorSystematic_;
	int jesSystematic_;
	int jetSmearingSystematic_;
	int btagSystematic_;
	int lighttagSystematic_;
	std::string custom_file_suffix_;
	unsigned int pdfWeightNumber_;
//	PileUpReweightingMethod::value pileUpReweightingMethod_;
	bool applyMetSysShiftCorr_;
	bool applyMetType0Corr_;
	bool applyMetType1Corr_;
	bool applyJetSmearing_;
	bool applyTopPtReweighting_;

	bool verbose_;

	boost::program_options::variables_map getParameters(int argc, char **argv);
	boost::shared_ptr<TH1D> getPileUpHistogram(std::string pileUpEstimationFile);
	boost::shared_ptr<TH2F> getMuonIdScaleFactorsHistogram(std::string muonIdScaleFactorsFile);
	boost::shared_ptr<TH2F> getMuonIsoScaleFactorsHistogram(std::string muonIsoScaleFactorsFile);
	boost::shared_ptr<TH2F> getMuonTriggerScaleFactorsHistogram(std::string muonScaleFactorsFile);
	boost::shared_ptr<TH1F> getMuonTrackingScaleFactorsHistogram(std::string muonTrackingScaleFactorsFile);
	boost::shared_ptr<TH2F> getElectronTriggerScaleFactorsHistogram(std::string electronTriggerScaleFactorsFile);
	boost::shared_ptr<TH2F> getElectronIdScaleFactorsHistogram(std::string electronIdScaleFactorsFile);
	boost::shared_ptr<TH2F> getElectronRecoScaleFactorsHistogram(std::string electronRecoScaleFactorsFile);
	boost::array<boost::shared_ptr<TF1>, 12> getL7Correction(std::string correctionFile);
	

	void getCSVCorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getCSVIncorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getNuChiCorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getNuChiIncorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getHadronicRecoCorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getHadronicRecoIncorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getLeptonicRecoCorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getLeptonicRecoIncorrectPermHistogram(std::string ttbarLikelihoodFile);

	void getbQuarkJet(std::string btagEfficiencyFile);
	void getcQuarkJet(std::string btagEfficiencyFile);
	void getudsgQuarkJet(std::string btagEfficiencyFile);
	std::string checkEffFileExists(std::string btagEfficiencyFile, std::string histpath);
	std::string getSampleBTagEffTag(std::string sample, std::string mode);

};

} /* namespace BAT */
#endif /* CONFIGFILE_H_ */
