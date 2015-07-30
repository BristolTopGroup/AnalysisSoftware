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

namespace BAT {

class ConfigFile {
public:
	ConfigFile(int argc, char **argv);
	virtual ~ConfigFile();

	void loadIntoMemory();
	long maxEvents() const;
	std::string datasetInfoFile() const;
	std::string PUFile() const;
	std::string TTbarLikelihoodFile() const;
	std::string MuonScaleFactorsFile() const;
	std::string ElectronTriggerScaleFactorsFile() const;
	std::string ElectronIdIsoScaleFactorsFile() const;
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
	std::string pileUpFile_;
	std::string ttbarLikelihoodFile_;
	bool getMuonScaleFactorsFromFile_;
	std::string muonScaleFactorsFile_;
	bool getElectronScaleFactorsFromFile_;
	std::string electronIdIsoScaleFactorsFile_;
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
	int electronScaleFactorSystematic_;
	int muonScaleFactorSystematic_;
	int jesSystematic_;
	int jetSmearingSystematic_;
	int btagSystematic_;
	int lightTagSystematic_;
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
	boost::shared_ptr<TH2F> getMuonIdIsoScaleFactorsHistogram(std::string muonScaleFactorsFile);
	boost::shared_ptr<TH3F> getMuonTriggerScaleFactorsHistogram(std::string muonScaleFactorsFile);
	boost::shared_ptr<TH1F> getElectronTriggerScaleFactorsHistogram(std::string electronTriggerScaleFactorsFile);
	boost::shared_ptr<TH2F> getElectronIdIsoScaleFactorsHistogram(std::string electronIdIsoScaleFactorsFile);
	boost::array<boost::shared_ptr<TF1>, 12> getL7Correction(std::string correctionFile);
	void getHadronTriggerLegHistogram(std::string hadronTriggerFile);
	

	void getCSVCorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getCSVIncorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getNuChiCorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getNuChiIncorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getHadronicRecoCorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getHadronicRecoIncorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getLeptonicRecoCorrectPermHistogram(std::string ttbarLikelihoodFile);
	void getLeptonicRecoIncorrectPermHistogram(std::string ttbarLikelihoodFile);
};

} /* namespace BAT */
#endif /* CONFIGFILE_H_ */
