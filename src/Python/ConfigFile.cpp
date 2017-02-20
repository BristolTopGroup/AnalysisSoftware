/*
 * ConfigFile.cpp
 *
 *  Created on: 20 Oct 2011
 *      Author: kreczko
 */

#include "../../interface/Python/ConfigFile.h"
#include "../../interface/Python/PythonParser.h"
#include "../../interface/GlobalVariables.h"

#include "TF1.h"
#include "TFile.h"
#include "TList.h"
// #include "TCollection.h"
// #include "TDirectory.h"

#include <iostream>
#include <iomanip>

#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>

using namespace std;

namespace BAT {
const string DEFAULT_CONFIG_PATH = "python/default_cfg.py";

ConfigFile::ConfigFile(int argc, char **argv) :
		programOptions(getParameters(argc, argv)), //
		config(PythonParser::parse_python_file(configPath())), //
		maxEvents_(PythonParser::getAttributeFromPyObject<long>(config, "maxEvents")), //
		datasetInfoFile_(PythonParser::getAttributeFromPyObject<string>(config, "datasetInfoFile")), //
		pileUpFile_(PythonParser::getAttributeFromPyObject<string>(config, "PUFile")), //
		pileUpFile_up_(PythonParser::getAttributeFromPyObject<string>(config, "PUFile_up")), //
		pileUpFile_down_(PythonParser::getAttributeFromPyObject<string>(config, "PUFile_down")), //
		ttbarLikelihoodFile_(PythonParser::getAttributeFromPyObject<string>(config, "TTbarLikelihoodFile")), //
		btagEfficiencyFile_(PythonParser::getAttributeFromPyObject<string>(config, "BTagEfficiencyFile")), //
		getMuonScaleFactorsFromFile_(PythonParser::getAttributeFromPyObject<bool>(config, "getMuonScaleFactorsFromFile")), //
		muonIdScaleFactorsFile_(PythonParser::getAttributeFromPyObject<string>(config, "MuonIdScaleFactorsFile")), //
		muonIsoScaleFactorsFile_(PythonParser::getAttributeFromPyObject<string>(config, "MuonIsoScaleFactorsFile")), //
		muonTriggerScaleFactorsFile_(PythonParser::getAttributeFromPyObject<string>(config, "MuonTriggerScaleFactorsFile")), //
		muonTrackingHIPScaleFactorsFile_(PythonParser::getAttributeFromPyObject<string>(config, "MuonTrackingHIPScaleFactorsFile")), //
		getElectronScaleFactorsFromFile_(PythonParser::getAttributeFromPyObject<bool>(config, "getElectronScaleFactorsFromFile")), //
		electronIdScaleFactorsFile_(PythonParser::getAttributeFromPyObject<string>(config, "ElectronIdScaleFactorsFile")), //
		electronRecoScaleFactorsFile_(PythonParser::getAttributeFromPyObject<string>(config, "ElectronRecoScaleFactorsFile")), //
		electronTriggerScaleFactorsFile_(PythonParser::getAttributeFromPyObject<string>(config, "ElectronTriggerScaleFactorsFile")), //
		bJetResoFile_(PythonParser::getAttributeFromPyObject<string>(config, "bJetResoFile")), //
		lightJetResoFile_(PythonParser::getAttributeFromPyObject<string>(config, "lightJetResoFile")), //
		getHadronTriggerFromFile_(PythonParser::getAttributeFromPyObject<bool>(config, "getHadronTriggerFromFile")), //
		hadronTriggerFile_(PythonParser::getAttributeFromPyObject<string>(config, "hadronTriggerFile")), //
		useHitFit_(PythonParser::getAttributeFromPyObject<bool>(config, "useHitFit")), //
		fitterOutputFlag_(PythonParser::getAttributeFromPyObject<bool>(config, "produceFitterASCIIoutput")), //
		inputFiles_(PythonParser::getVectorFromPythonObject(config, "inputFiles")), //
		tqafPath_(PythonParser::getAttributeFromPyObject<string>(config, "TQAFPath")), //
		lumi_(PythonParser::getAttributeFromPyObject<double>(config, "lumi")), //
		centerOfMassEnergy_(PythonParser::getAttributeFromPyObject<unsigned int>(config, "centerOfMassEnergy")), //
		nTupleVersion_(PythonParser::getAttributeFromPyObject<unsigned int>(config, "nTuple_version")), //
		sample_(PythonParser::getAttributeFromPyObject<string>(config, "sample")), //
		electronScaleFactorSystematic_(0), //
		muonScaleFactorSystematic_(0), //
		jesSystematic_(0), //
		jetSmearingSystematic_(0), //
		btagSystematic_(0), //
		lighttagSystematic_(0), //
		custom_file_suffix_(""), //
		pdfWeightNumber_(0), //
		applyMetSysShiftCorr_(0), //
		applyMetType0Corr_(0), //
		applyMetType1Corr_(0), //
		applyJetSmearing_(0), //
		applyTopPtReweighting_(0), //
		verbose_(0) {
	if (PythonParser::hasAttribute(config, "ElectronScaleFactorSystematic"))
		electronScaleFactorSystematic_ = PythonParser::getAttributeFromPyObject<int>(config, "ElectronScaleFactorSystematic");
	if (PythonParser::hasAttribute(config, "MuonScaleFactorSystematic"))
		muonScaleFactorSystematic_ = PythonParser::getAttributeFromPyObject<int>(config, "MuonScaleFactorSystematic");
	if (PythonParser::hasAttribute(config, "JESsystematic"))
		jesSystematic_ = PythonParser::getAttributeFromPyObject<int>(config, "JESsystematic");
	if (PythonParser::hasAttribute(config, "JetSmearingSystematic"))
		jetSmearingSystematic_ = PythonParser::getAttributeFromPyObject<int>(config, "JetSmearingSystematic");
	if (PythonParser::hasAttribute(config, "BTagSystematic"))
		btagSystematic_ = PythonParser::getAttributeFromPyObject<int>(config, "BTagSystematic");
	if (PythonParser::hasAttribute(config, "LightTagSystematic"))
		lighttagSystematic_ = PythonParser::getAttributeFromPyObject<int>(config, "LightTagSystematic");
	if (PythonParser::hasAttribute(config, "custom_file_suffix"))
		custom_file_suffix_ = PythonParser::getAttributeFromPyObject<string>(config, "custom_file_suffix");
	if (PythonParser::hasAttribute(config, "pdfWeightNumber"))
		pdfWeightNumber_ = PythonParser::getAttributeFromPyObject<unsigned int>(config, "pdfWeightNumber");
	if (PythonParser::hasAttribute(config, "applyMetSysShiftCorr"))
		applyMetSysShiftCorr_ = PythonParser::getAttributeFromPyObject<bool>(config, "applyMetSysShiftCorr");
	if (PythonParser::hasAttribute(config, "applyMetType0Corr"))
		applyMetType0Corr_ = PythonParser::getAttributeFromPyObject<bool>(config, "applyMetType0Corr");
	if (PythonParser::hasAttribute(config, "applyMetType1Corr"))
		applyMetType1Corr_ = PythonParser::getAttributeFromPyObject<bool>(config, "applyMetType1Corr");
	if (PythonParser::hasAttribute(config, "applyJetSmearing"))
		applyJetSmearing_ = PythonParser::getAttributeFromPyObject<bool>(config, "applyJetSmearing");
	if (PythonParser::hasAttribute(config, "applyTopPtReweighting"))
		applyTopPtReweighting_ = PythonParser::getAttributeFromPyObject<bool>(config, "applyTopPtReweighting");
}

boost::program_options::variables_map ConfigFile::getParameters(int argc, char** argv) {
	using namespace boost::program_options;
	variables_map vm;
	positional_options_description p;
	p.add("config-file", -1);

	// Declare the supported options.
	options_description desc("Allowed options");
	desc.add_options()("help,h", "produce help message");
	desc.add_options()("maxEvents", value<unsigned long>(), "set maximal number of events to be processed");
	desc.add_options()("config-file", value<std::string>(), "Configuration file for BAT");
	desc.add_options()("datasetInfoFile", value<std::string>(),
			"Dataset information file for event weight calculation");
	desc.add_options()("PUFile", value<std::string>(), "set input PU file for PU re-weighting");
	desc.add_options()("PUFile_up", value<std::string>(), "set input PU file for PU re-weighting up systematic");
	desc.add_options()("PUFile_down", value<std::string>(), "set input PU file for PU re-weighting down systematic");
	desc.add_options()("TTbarLikelihoodFile", value<std::string>(), "set input file for ttbar likelihood reconstruction");
	desc.add_options()("BTagEfficiencyFile", value<std::string>(), "set input file for b tag efficiency");
	desc.add_options()("getMuonScaleFactorsFromFile", value<bool>(), "state whether we are getting the muon scale factors from a file or not");
	desc.add_options()("MuonIdScaleFactorsFile", value<std::string>(), "set input file for id muon scale factors");
	desc.add_options()("MuonIsoScaleFactorsFile", value<std::string>(), "set input file for iso muon scale factors");
	desc.add_options()("MuonTriggerScaleFactorsFile", value<std::string>(), "set input file for trigger muon scale factors");
	desc.add_options()("MuonTrackingHIPScaleFactorsFile", value<std::string>(), "set input file for tracking HIP muon scale factors");
	desc.add_options()("getElectronScaleFactorsFromFile", value<bool>(), "state whether we are getting the electron scale factors from a file or not");
	desc.add_options()("ElectronTriggerScaleFactorsFile", value<std::string>(), "set input file for electron trigger scale factors");
	desc.add_options()("bJetResoFile", value<std::string>(), "set input root file for b-jet L7 resolutions");
	desc.add_options()("lightJetResoFile", value<std::string>(), "set input root file for light jet L7 resolutions");
	desc.add_options()("getHadronTriggerFromFile", value<bool>(), "state whether we are getting the electron trigger hadron leg efficiencies from a file or not");
	desc.add_options()("fitter", value<bool>(), "turn on the fitter (HitFit)");
	desc.add_options()("fitterASCIIoutput", value<bool>(), "produce full kinematic fit output in ASCII format");
	desc.add_options()("TQAFPath", value<std::string>(),
			"path to TopQuarkAnalysis folder (the folder itself not included).");
	desc.add_options()("lumi", value<std::string>(), "Integrated luminosity the MC simulation will be scaled to");
	desc.add_options()("ElectronScaleFactorSystematic", value<int>(),
			"Electron ID, Iso and Trigger scale factor systematic,  the +/- number of uncertainties to vary the scale factor with");
	desc.add_options()("MuonScaleFactorSystematic", value<int>(),
			"Muon ID, Iso and Trigger scale factor systematic,  the +/- number of uncertainties to vary the scale factor with");
	desc.add_options()("BTagSystematic", value<int>(),
			"B-tag scale factor systematic, the +/- number of uncertainties to vary the scale factor with");
	desc.add_options()("LightTagSystematic", value<int>(),
			"Light-tag scale factor systematic, the +/- number of uncertainties to vary the scale factor with");
	desc.add_options()("JESsystematic", value<int>(),
			"JES systematic, the +/- number of uncertainties to vary the jet with");
	desc.add_options()("JetSmearingSystematic", value<int>(),
			"Jet Smearing Systematic, the +/- number of uncertainties to vary the jet with");
	desc.add_options()("custom_file_suffix", value<std::string>(), "Custom file suffix, will be appended to file name");
	desc.add_options()("pdfWeightNumber", value<unsigned int>(),
			"Number of PDF weight to multiply the event weight with");
	desc.add_options()("applyMetSysShiftCorr", value<bool>(), "apply MET systematic shift correction for phi modulation");
	desc.add_options()("applyMetType0Corr", value<bool>(), "apply MET type-0 correction (for PU)");
	desc.add_options()("applyMetType1Corr", value<bool>(), "apply MET type-1 correction (JEC)");
	desc.add_options()("applyJetSmearing", value<bool>(), "apply Jet Smearing");
	desc.add_options()("applyTopPtReweighting", value<bool>(), "apply Top Pt Reweighting");

	desc.add_options()("verbose", value<bool>(), "enable verose output");

	store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	notify(vm);
	if (vm.count("help")) {
		cout << desc << "\n";
		exit(0);
		return 0;
	}

	return vm;
}

boost::array<boost::shared_ptr<TF1>, 12> ConfigFile::getL7Correction(std::string correctionFile) {
	boost::array<boost::shared_ptr<TF1>, 12> correctionsArray;

	boost::array<double, 12> towerBinning = { { 0., 0.174, 0.348, 0.522, 0.696, 0.87, 1.044, 1.218, 1.392, 1.566, 1.74,
			2.5 } };
	if (!boost::filesystem::exists(correctionFile)) {
		cerr << "ConfigFile::getL7Correction(" << correctionFile << "): could not find file" << endl;
		throw "Could not find L7 correction path in " + correctionFile;
	}

	boost::scoped_ptr<TFile> inputFile(TFile::Open(correctionFile.c_str()));

	for (unsigned int etaBin = 0; etaBin < towerBinning.size() - 1; ++etaBin) {
		stringstream graphname;
		graphname << "mean_relative_et_" << fixed << setprecision(3) << towerBinning[etaBin];
		graphname << "_" << fixed << setprecision(3) << towerBinning[etaBin + 1];
		boost::shared_ptr<TF1> corrections((TF1*) inputFile->Get(graphname.str().c_str()));
		correctionsArray[etaBin] = corrections;
	}

	inputFile->Close();
	return correctionsArray;
}

string ConfigFile::bJetResoFile() const {
	if (programOptions.count("bJetResoFile"))
		return programOptions["bJetResoFile"].as<std::string>();
	else
		return bJetResoFile_;
}

string ConfigFile::lightJetResoFile() const {
	if (programOptions.count("lightJetResoFile"))
		return programOptions["lightJetResoFile"].as<std::string>();
	else
		return lightJetResoFile_;
}

string ConfigFile::TQAFPath() const {
	if (programOptions.count("TQAFPath"))
		return programOptions["TQAFPath"].as<std::string>();
	else
		return tqafPath_;
}

string ConfigFile::configPath() const {
	if (programOptions.count("config-file"))
		return programOptions["config-file"].as<std::string>();
	else
		return DEFAULT_CONFIG_PATH;
}

string ConfigFile::PUFile() const {
	if (programOptions.count("PUFile"))
		return programOptions["PUFile"].as<std::string>();
	else
		return pileUpFile_;
}

string ConfigFile::PUFile_up() const {
	if (programOptions.count("PUFile_up"))
		return programOptions["PUFile_up"].as<std::string>();
	else
		return pileUpFile_up_;
}

string ConfigFile::PUFile_down() const {
	if (programOptions.count("PUFile_down"))
		return programOptions["PUFile_down"].as<std::string>();
	else
		return pileUpFile_down_;
}

string ConfigFile::TTbarLikelihoodFile() const {
	if (programOptions.count("TTbarLikelihoodFile"))
		return programOptions["TTbarLikelihoodFile"].as<std::string>();
	else
		return ttbarLikelihoodFile_;
}

string ConfigFile::BTagEfficiencyFile() const {
	if (programOptions.count("BTagEfficiencyFile"))
		return programOptions["BTagEfficiencyFile"].as<std::string>();
	else
		return btagEfficiencyFile_;
}

string ConfigFile::MuonIdScaleFactorsFile() const {
	if (programOptions.count("MuonIdScaleFactorsFile"))
		return programOptions["MuonIdScaleFactorsFile"].as<std::string>();
	else
		return muonIdScaleFactorsFile_;
}

string ConfigFile::MuonIsoScaleFactorsFile() const {
	if (programOptions.count("MuonIsoScaleFactorsFile"))
		return programOptions["MuonIsoScaleFactorsFile"].as<std::string>();
	else
		return muonIsoScaleFactorsFile_;
}

string ConfigFile::MuonTriggerScaleFactorsFile() const {
	if (programOptions.count("MuonTriggerScaleFactorsFile"))
		return programOptions["MuonTriggerScaleFactorsFile"].as<std::string>();
	else
		return muonTriggerScaleFactorsFile_;
}

string ConfigFile::MuonTrackingHIPScaleFactorsFile() const {
	if (programOptions.count("MuonTrackingHIPScaleFactorsFile"))
		return programOptions["MuonTrackingHIPScaleFactorsFile"].as<std::string>();
	else
		return muonTrackingHIPScaleFactorsFile_;
}

string ConfigFile::ElectronIdScaleFactorsFile() const {
	if (programOptions.count("ElectronIdScaleFactorsFile"))
		return programOptions["ElectronIdScaleFactorsFile"].as<std::string>();
	else
		return electronIdScaleFactorsFile_;
}

string ConfigFile::ElectronRecoScaleFactorsFile() const {
	if (programOptions.count("ElectronRecoScaleFactorsFile"))
		return programOptions["ElectronRecoScaleFactorsFile"].as<std::string>();
	else
		return electronRecoScaleFactorsFile_;
}

string ConfigFile::ElectronTriggerScaleFactorsFile() const {
	if (programOptions.count("ElectronTriggerScaleFactorsFile"))
		return programOptions["ElectronTriggerScaleFactorsFile"].as<std::string>();
	else
		return electronTriggerScaleFactorsFile_;
}

string ConfigFile::hadronTriggerFile() const {
	if (programOptions.count("hadronTriggerFile"))
		return programOptions["hadronTriggerFile"].as<std::string>();
	else
		return hadronTriggerFile_;
}

string ConfigFile::datasetInfoFile() const {
	if (programOptions.count("datasetInfoFile"))
		return programOptions["datasetInfoFile"].as<std::string>();
	else
		return datasetInfoFile_;
}

long ConfigFile::maxEvents() const {
	if (programOptions.count("maxEvents"))
		return programOptions["maxEvents"].as<unsigned long>();
	else
		return maxEvents_;
}

bool ConfigFile::useHitFit() const {
	if (programOptions.count("fitter"))
		return programOptions["fitter"].as<bool>();
	else
		return useHitFit_;
}

unsigned int ConfigFile::centerOfMassEnergy() const {
	if (programOptions.count("centerOfMassEnergy"))
		return programOptions["centerOfMassEnergy"].as<unsigned int>();
	else
		return centerOfMassEnergy_;
}

bool ConfigFile::fitterOutputFlag() const {
	if (programOptions.count("fitterASCIIoutput"))
		return programOptions["fitterASCIIoutput"].as<bool>();
	else
		return fitterOutputFlag_;
}

const vector<string>& ConfigFile::inputFiles() const {
	return inputFiles_;
}

double ConfigFile::lumi() const {
	if (programOptions.count("lumi"))
		return programOptions["lumi"].as<double>();
	else
		return lumi_;
}

int ConfigFile::electronScaleFactorSystematic() const {
	if (programOptions.count("ElectronScaleFactorSystematic"))
		return programOptions["ElectronScaleFactorSystematic"].as<int>();
	else
		return electronScaleFactorSystematic_;
}

int ConfigFile::muonScaleFactorSystematic() const {
	if (programOptions.count("MuonScaleFactorSystematic"))
		return programOptions["MuonScaleFactorSystematic"].as<int>();
	else
		return muonScaleFactorSystematic_;
}

int ConfigFile::jesSystematic() const {
	if (programOptions.count("JESsystematic"))
		return programOptions["JESsystematic"].as<int>();
	else
		return jesSystematic_;
}

int ConfigFile::jetSmearingSystematic() const {
	if (programOptions.count("JetSmearingSystematic"))
		return programOptions["JetSmearingSystematic"].as<int>();
	else
		return jetSmearingSystematic_;
}

int ConfigFile::BtagSystematic() const {
	if (programOptions.count("BTagSystematic"))
		return programOptions["BTagSystematic"].as<int>();
	else
		return btagSystematic_;
}
int ConfigFile::LightTagSystematic() const {
	if (programOptions.count("LightTagSystematic"))
		return programOptions["LightTagSystematic"].as<int>();
	else
		return lighttagSystematic_;
}

string ConfigFile::custom_file_suffix() const {
	if (programOptions.count("custom_file_suffix"))
		return programOptions["custom_file_suffix"].as<string>();
	else
		return custom_file_suffix_;
}

unsigned int ConfigFile::pdfWeightNumber() const {
	if (programOptions.count("pdfWeightNumber"))
		return programOptions["pdfWeightNumber"].as<unsigned int>();
	else
		return pdfWeightNumber_;
}

bool ConfigFile::applyMetSysShiftCorr() const {
	if (programOptions.count("applyMetSysShiftCorr"))
		return programOptions["applyMetSysShiftCorr"].as<bool>();
	else
		return applyMetSysShiftCorr_;
}

bool ConfigFile::applyMetType0Corr() const {
	if (programOptions.count("applyMetType0Corr"))
		return programOptions["applyMetType0Corr"].as<bool>();
	else
		return applyMetType0Corr_;
}

bool ConfigFile::applyMetType1Corr() const {
	if (programOptions.count("applyMetType1Corr"))
		return programOptions["applyMetType1Corr"].as<bool>();
	else
		return applyMetType1Corr_;
}

bool ConfigFile::applyJetSmearing() const {
	if (programOptions.count("applyJetSmearing"))
		return programOptions["applyJetSmearing"].as<bool>();
	else
		return applyJetSmearing_;
}

bool ConfigFile::applyTopPtReweighting() const {
	if (programOptions.count("applyTopPtReweighting"))
		return programOptions["applyTopPtReweighting"].as<bool>();
	else
		return applyTopPtReweighting_;
}

bool ConfigFile::verbose() const {
	if (programOptions.count("verbose"))
		return programOptions["verbose"].as<bool>();
	else
		return verbose_;
}

ConfigFile::~ConfigFile() {
}

PileUpReweightingMethod::value ConfigFile::PileUpReweightingMethod() const {
	std::string option = "";

	if (isOptionSetInConsole("pileUpReweightingMethod")) {
		option = programOptions["pileUpReweightingMethod"].as<std::string>();
	} else if (isOptionSetInConfig(option)) {
		option = PythonParser::getAttributeFromPyObject<std::string>(config, "pileUpReweightingMethod");
	} else
		return Globals::pileUpReweightingMethod;

	if (option == "averagePileUp")
		return PileUpReweightingMethod::averagePileUp;
	else if (option == "inTimePileUpOnly")
		return PileUpReweightingMethod::inTimePileUpOnly;
	else if (option == "threeDReweighting")
		return PileUpReweightingMethod::threeDReweighting;
	else {
		cout << "WARNING: Unknown or empty pile up reweighting option '" << option << "', using default" << endl;
		return Globals::pileUpReweightingMethod;
	}
}

bool ConfigFile::isOptionSet(std::string option) const {
	bool isInProgramOptions(isOptionSetInConsole(option));
	bool isInConfigFile(isOptionSetInConfig(option));

	return isInProgramOptions || isInConfigFile;
}

bool ConfigFile::isOptionSetInConsole(std::string option) const {
	return programOptions.count(option);
}

bool ConfigFile::isOptionSetInConfig(std::string option) const {
	return PyObject_HasAttrString(config.ptr(), option.c_str());
}

void ConfigFile::loadIntoMemory() {

	//always use function to access the variables
	//general
	Globals::luminosity = lumi();
	Globals::maxEvents = maxEvents();
	Globals::NTupleVersion = nTupleVersion();
	Globals::energyInTeV = centerOfMassEnergy();
	Globals::sample = sample();

	//kinematic fit
	Globals::TQAFPath = TQAFPath();
	Globals::useHitFit = useHitFit();
	Globals::produceFitterASCIIoutput = fitterOutputFlag();


	Globals::estimatedPileup = getPileUpHistogram(PUFile());
	Globals::estimatedPileup_up = getPileUpHistogram(PUFile_up());
	Globals::estimatedPileup_down = getPileUpHistogram(PUFile_down());

	//Lepton Scale Factors
	Globals::ElectronScaleFactorSystematic = electronScaleFactorSystematic();
	Globals::MuonScaleFactorSystematic = muonScaleFactorSystematic();

	if ( getMuonScaleFactorsFromFile_ 
		&& boost::filesystem::exists(MuonTriggerScaleFactorsFile()) 
		&& boost::filesystem::exists(MuonIdScaleFactorsFile()) 
		&& boost::filesystem::exists(MuonIsoScaleFactorsFile())
		// && boost::filesystem::exists(MuonTrackingHIPScaleFactorsFile())
		) {
		std::cout << "Getting muon scale factors from files :" << std::endl 
			<< MuonIdScaleFactorsFile() << std::endl
			<< MuonIsoScaleFactorsFile() << std::endl
			<< MuonTriggerScaleFactorsFile() << std::endl;
			// << MuonTrackingHIPScaleFactorsFile() <<std::endl;
		Globals::muonIdScaleFactorsHistogram = getMuonIdScaleFactorsHistogram(MuonIdScaleFactorsFile());
		Globals::muonIsoScaleFactorsHistogram = getMuonIsoScaleFactorsHistogram(MuonIsoScaleFactorsFile());
		Globals::muonTriggerScaleFactorsHistogram = getMuonTriggerScaleFactorsHistogram(MuonTriggerScaleFactorsFile());
		// Globals::muonTrackingHIPScaleFactorsHistogram = getMuonTrackingHIPScaleFactorsHistogram(MuonTrackingHIPScaleFactorsFile());
	}
	else{
		std::cout << "Unable to get muon scale factors from file - Check input file names"  << std::endl;
	}

	if ( getElectronScaleFactorsFromFile_ 
		&& boost::filesystem::exists(ElectronTriggerScaleFactorsFile()) 
		&& boost::filesystem::exists(ElectronIdScaleFactorsFile()) 
		&& boost::filesystem::exists(ElectronRecoScaleFactorsFile())
		) {
		std::cout << "Getting electron scale factors from file :"  << std::endl
			<< ElectronTriggerScaleFactorsFile()  << std::endl
			<< ElectronIdScaleFactorsFile() << std::endl
			<< ElectronRecoScaleFactorsFile() << std::endl;
		Globals::electronTriggerScaleFactorsHistogram = getElectronTriggerScaleFactorsHistogram(ElectronTriggerScaleFactorsFile());
		Globals::electronIdScaleFactorsHistogram = getElectronIdScaleFactorsHistogram(ElectronIdScaleFactorsFile());
		Globals::electronRecoScaleFactorsHistogram = getElectronRecoScaleFactorsHistogram(ElectronRecoScaleFactorsFile());

	}
	else{
		std::cout << "Unable to get electron scale factors from file - Check input file names"  << std::endl;
	}

	getCSVCorrectPermHistogram( TTbarLikelihoodFile() );
	getCSVIncorrectPermHistogram( TTbarLikelihoodFile() );

	getNuChiCorrectPermHistogram( TTbarLikelihoodFile() );
	getNuChiIncorrectPermHistogram( TTbarLikelihoodFile() );

	getHadronicRecoCorrectPermHistogram( TTbarLikelihoodFile() );
	getHadronicRecoIncorrectPermHistogram( TTbarLikelihoodFile() );
	getLeptonicRecoCorrectPermHistogram( TTbarLikelihoodFile() );
	getLeptonicRecoIncorrectPermHistogram( TTbarLikelihoodFile() );

	getbQuarkJet( BTagEfficiencyFile() );
	getcQuarkJet( BTagEfficiencyFile() );
	getudsgQuarkJet( BTagEfficiencyFile() );


	//JES systematic
	Globals::JESsystematic = jesSystematic();

	// Change tree name for JEC variations
	if ( Globals::JESsystematic == -1 ) Globals::treePrefix_ += "_JESDown";
	else if ( Globals::JESsystematic == 1 ) Globals::treePrefix_ += "_JESUp";

	//Jet Smearing systematic
	Globals::JetSmearingSystematic = jetSmearingSystematic();
	std::cout << "ConfigFile.cpp: Globals::JetSmearingSystematic = " << Globals::JetSmearingSystematic << std::endl;

	// Change tree name for JER variations
	if ( Globals::JetSmearingSystematic == -1 ) Globals::treePrefix_ += "_JERDown";
	else if ( Globals::JetSmearingSystematic == 1 ) Globals::treePrefix_ += "_JERUp";

	//b-tag systematics
	Globals::BJetSystematic = BtagSystematic();
	Globals::LightJetSystematic = LightTagSystematic();

	//Loading l7 JEC
	Globals::bL7Corrections = getL7Correction(bJetResoFile());
	Globals::lightL7Corrections = getL7Correction(lightJetResoFile());

	Globals::custom_file_suffix = custom_file_suffix();
	Globals::pdfWeightNumber = pdfWeightNumber();

	//MET corrections
	Globals::applySysShiftMetCorrection = applyMetSysShiftCorr();
	Globals::applyType0MetCorrection = applyMetType0Corr();
	Globals::applyType1MetCorrection = applyMetType1Corr();
	Globals::applyJetSmearing = applyJetSmearing();
	Globals::applyTopPtReweighting = applyTopPtReweighting();

	Globals::verbose = verbose();
	cout << "Verbose: " << verbose() << endl;
}

boost::shared_ptr<TH1D> ConfigFile::getPileUpHistogram(std::string pileUpEstimationFile) {
	using namespace std;

	if (!boost::filesystem::exists(pileUpEstimationFile)) {
		cerr << "ConfigFile::getPileUpHistogram(" << pileUpEstimationFile << "): could not find file" << endl;
		throw "Could not find pile-up histogram file in " + pileUpEstimationFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(pileUpEstimationFile.c_str()));
	boost::shared_ptr<TH1D> pileUp((TH1D*) file->Get("pileup")->Clone());
	file->Close();

	return pileUp;
}

boost::shared_ptr<TH2F> ConfigFile::getMuonIdScaleFactorsHistogram(std::string muonIdScaleFactorsFile) {
	using namespace std;

	if (!boost::filesystem::exists(muonIdScaleFactorsFile)) {
		cerr << "ConfigFile::getMuonIdScaleFactorsHistogram(" << muonIdScaleFactorsFile << "): could not find file" << endl;
		throw "Could not find muon ID scale factors histogram file in " + muonIdScaleFactorsFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(muonIdScaleFactorsFile.c_str()));
	boost::shared_ptr<TH2F> idHistogram((TH2F*) file->Get("abseta_pt_ratio")->Clone());
	file->Close();

	return idHistogram;
}

boost::shared_ptr<TH2F> ConfigFile::getMuonIsoScaleFactorsHistogram(std::string muonIsoScaleFactorsFile) {
	using namespace std;

	if (!boost::filesystem::exists(muonIsoScaleFactorsFile)) {
		cerr << "ConfigFile::getMuonIsoScaleFactorsHistogram(" << muonIsoScaleFactorsFile << "): could not find file" << endl;
		throw "Could not find muon iso scale factors histogram file in " + muonIsoScaleFactorsFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(muonIsoScaleFactorsFile.c_str()));
	boost::shared_ptr<TH2F> isoHistogram((TH2F*) file->Get("abseta_pt_ratio")->Clone());
	file->Close();

	return isoHistogram;
}

boost::shared_ptr<TH2F> ConfigFile::getMuonTriggerScaleFactorsHistogram(std::string muonTriggerScaleFactorsFile) {
	using namespace std;

	if (!boost::filesystem::exists(muonTriggerScaleFactorsFile)) {
		cerr << "ConfigFile::getMuonTriggerScaleFactorsHistogram(" << muonTriggerScaleFactorsFile << "): could not find file" << endl;
		throw "Could not find muon trigger scale factors histogram file in " + muonTriggerScaleFactorsFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(muonTriggerScaleFactorsFile.c_str()));
	boost::shared_ptr<TH2F> triggerHistogram((TH2F*) file->Get("abseta_pt_ratio")->Clone());
	file->Close();

	return triggerHistogram;
}

boost::shared_ptr<TH1F> ConfigFile::getMuonTrackingHIPScaleFactorsHistogram(std::string muonTrackingHIPScaleFactorsFile) {
	using namespace std;

	if (!boost::filesystem::exists(muonTrackingHIPScaleFactorsFile)) {
		cerr << "ConfigFile::getMuonTrackingHIPScaleFactorsHistogram(" << muonTrackingHIPScaleFactorsFile << "): could not find file" << endl;
		throw "Could not find muon tracking HIP scale factors histogram file in " + muonTrackingHIPScaleFactorsFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(muonTrackingHIPScaleFactorsFile.c_str()));
	boost::shared_ptr<TH1F> idHistogram((TH1F*) file->Get("ratio_eta")->Clone());
	file->Close();

	return idHistogram;
}

boost::shared_ptr<TH2F> ConfigFile::getElectronIdScaleFactorsHistogram(std::string electronIdScaleFactorsFile) {
	using namespace std;

	if (!boost::filesystem::exists(electronIdScaleFactorsFile)) {
		cerr << "ConfigFile::getElectronIdScaleFactorsHistogram(" << electronIdScaleFactorsFile << "): could not find file" << endl;
		throw "Could not find electron ID scale factors histogram file in " + electronIdScaleFactorsFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(electronIdScaleFactorsFile.c_str()));
	boost::shared_ptr<TH2F> idHistogram((TH2F*) file->Get("EGamma_SF2D")->Clone());
	file->Close();

	return idHistogram;
}

boost::shared_ptr<TH2F> ConfigFile::getElectronRecoScaleFactorsHistogram(std::string electronRecoScaleFactorsFile) {
	using namespace std;

	if (!boost::filesystem::exists(electronRecoScaleFactorsFile)) {
		cerr << "ConfigFile::getElectronRecoScaleFactorsHistogram(" << electronRecoScaleFactorsFile << "): could not find file" << endl;
		throw "Could not find electron Reco scale factors histogram file in " + electronRecoScaleFactorsFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(electronRecoScaleFactorsFile.c_str()));
	boost::shared_ptr<TH2F> recoHistogram((TH2F*) file->Get("EGamma_SF2D")->Clone());
	file->Close();

	return recoHistogram;
}

boost::shared_ptr<TH2F> ConfigFile::getElectronTriggerScaleFactorsHistogram(std::string electronTriggerScaleFactorsFile) {
	using namespace std;
	// although labelled abseta_pt_ratio due to formation from combineLeptonSF, it should be pt_abseta_ratio

	if (!boost::filesystem::exists(electronTriggerScaleFactorsFile)) {
		cerr << "ConfigFile::getElectronTriggerScaleFactorsHistogram(" << electronTriggerScaleFactorsFile << "): could not find file" << endl;
		throw "Could not find electron trigger scale factors histogram file in " + electronTriggerScaleFactorsFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(electronTriggerScaleFactorsFile.c_str()));
	boost::shared_ptr<TH2F> triggerHistogram((TH2F*) file->Get("abseta_pt_ratio"));
	
	file->Close();

	return triggerHistogram;
}

void ConfigFile::getCSVCorrectPermHistogram(std::string ttbarLikelihoodFile) {
	using namespace std;

	if (!boost::filesystem::exists(ttbarLikelihoodFile)) {
		cerr << "ConfigFile::getCSVCorrectPermHistogram(" << ttbarLikelihoodFile << "): could not find file" << endl;
		throw "Could not find file " + ttbarLikelihoodFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(ttbarLikelihoodFile.c_str()));
	Globals::csvCorrectPermHistogram = (boost::shared_ptr<TH1F>) (TH1F*) file->Get("CSV for BJets")->Clone();
	file->Close();
}

void ConfigFile::getCSVIncorrectPermHistogram(std::string ttbarLikelihoodFile) {
	using namespace std;

	if (!boost::filesystem::exists(ttbarLikelihoodFile)) {
		cerr << "ConfigFile::getCSVIncorrectPermHistogram(" << ttbarLikelihoodFile << "): could not find file" << endl;
		throw "Could not find file " + ttbarLikelihoodFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(ttbarLikelihoodFile.c_str()));
	Globals::csvIncorrectPermHistogram = (boost::shared_ptr<TH1F>) (TH1F*) file->Get("CSV for LightJets")->Clone();
	file->Close();
}

void ConfigFile::getNuChiCorrectPermHistogram(std::string ttbarLikelihoodFile) {
	using namespace std;

	if (!boost::filesystem::exists(ttbarLikelihoodFile)) {
		cerr << "ConfigFile::getNuChiCorrectPermHistogram(" << ttbarLikelihoodFile << "): could not find file" << endl;
		throw "Could not find file " + ttbarLikelihoodFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(ttbarLikelihoodFile.c_str()));
	Globals::NuChiCorrectPermHistogram = (boost::shared_ptr<TH1F>) (TH1F*) file->Get("NeutrinoChiSqLeptonicB")->Clone();
	file->Close();
}

void ConfigFile::getNuChiIncorrectPermHistogram(std::string ttbarLikelihoodFile) {
	using namespace std;

	if (!boost::filesystem::exists(ttbarLikelihoodFile)) {
		cerr << "ConfigFile::getNuChiIncorrectPermHistogram(" << ttbarLikelihoodFile << "): could not find file" << endl;
		throw "Could not find file " + ttbarLikelihoodFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(ttbarLikelihoodFile.c_str()));
	Globals::NuChiIncorrectPermHistogram = (boost::shared_ptr<TH1F>) (TH1F*) file->Get("NeutrinoChiSqFalseB")->Clone();
	file->Close();
}


void ConfigFile::getHadronicRecoCorrectPermHistogram(std::string ttbarLikelihoodFile) {
	using namespace std;

	if (!boost::filesystem::exists(ttbarLikelihoodFile)) {
		cerr << "ConfigFile::getHadronicRecoCorrectPermHistogram(" << ttbarLikelihoodFile << "): could not find file" << endl;
		throw "Could not find file " + ttbarLikelihoodFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(ttbarLikelihoodFile.c_str()));
	Globals::HadronicRecoCorrectPermHistogram = (boost::shared_ptr<TH2F>) (TH2F*) file->Get("MT vs MW True Reconstructed B(hadronic)")->Clone();
	file->Close();
}


void ConfigFile::getHadronicRecoIncorrectPermHistogram(std::string ttbarLikelihoodFile) {
	using namespace std;

	if (!boost::filesystem::exists(ttbarLikelihoodFile)) {
		cerr << "ConfigFile::getHadronicRecoIncorrectPermHistogram(" << ttbarLikelihoodFile << "): could not find file" << endl;
		throw "Could not find file " + ttbarLikelihoodFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(ttbarLikelihoodFile.c_str()));
	Globals::HadronicRecoIncorrectPermHistogram = (boost::shared_ptr<TH2F>) (TH2F*) file->Get("MT vs MW False Reconstructed B(hadronic)")->Clone();
	file->Close();
}


void ConfigFile::getLeptonicRecoCorrectPermHistogram(std::string ttbarLikelihoodFile) {
	using namespace std;

	if (!boost::filesystem::exists(ttbarLikelihoodFile)) {
		cerr << "ConfigFile::getLeptonicRecoCorrectPermHistogram(" << ttbarLikelihoodFile << "): could not find file" << endl;
		throw "Could not find file " + ttbarLikelihoodFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(ttbarLikelihoodFile.c_str()));
	Globals::LeptonicRecoCorrectPermHistogram = (boost::shared_ptr<TH2F>) (TH2F*) file->Get("MT vs MW True Reconstructed B(leptonic)")->Clone();
	file->Close();
}


void ConfigFile::getLeptonicRecoIncorrectPermHistogram(std::string ttbarLikelihoodFile) {
	using namespace std;

	if (!boost::filesystem::exists(ttbarLikelihoodFile)) {
		cerr << "ConfigFile::getLeptonicRecoIncorrectPermHistogram(" << ttbarLikelihoodFile << "): could not find file" << endl;
		throw "Could not find file " + ttbarLikelihoodFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(ttbarLikelihoodFile.c_str()));
	Globals::LeptonicRecoIncorrectPermHistogram = (boost::shared_ptr<TH2F>) (TH2F*) file->Get("MT vs MW False Reconstructed B(leptonic)")->Clone();
	file->Close();
}


void ConfigFile::getbQuarkJet(std::string btagEfficiencyFile) {
	using namespace std;

	std::string pathToBTagEff = getSampleBTagEffTag(sample_).append("bQuarkJets_Ratio_Hist");
	if (!boost::filesystem::exists(btagEfficiencyFile)) {
		cerr << "ConfigFile::getbQuarkJet(" << btagEfficiencyFile << "): could not find file" << endl;
		throw "Could not find file " + btagEfficiencyFile;
	}
	pathToBTagEff = checkEffFileExists(btagEfficiencyFile, pathToBTagEff);
	std::cout << "BTagEfficiencyFile in use : " << btagEfficiencyFile.c_str() << endl;
	std::cout << "MC BTag Efficiency in use : " << pathToBTagEff.c_str() << endl;
	boost::scoped_ptr<TFile> file(TFile::Open(btagEfficiencyFile.c_str()));

	Globals::bQuarkJet = (boost::shared_ptr<TH2F>) (TH2F*) file->Get(pathToBTagEff.c_str())->Clone();
	
	file->Close();
}

void ConfigFile::getcQuarkJet(std::string btagEfficiencyFile) {
	using namespace std;

	std::string pathToBTagEff = getSampleBTagEffTag(sample_).append("cQuarkJets_Ratio_Hist");

	if (!boost::filesystem::exists(btagEfficiencyFile)) {
		cerr << "ConfigFile::getcQuarkJet(" << btagEfficiencyFile << "): could not find file" << endl;
		throw "Could not find file " + btagEfficiencyFile;
	}
	pathToBTagEff = checkEffFileExists(btagEfficiencyFile, pathToBTagEff);
	boost::scoped_ptr<TFile> file(TFile::Open(btagEfficiencyFile.c_str()));
	Globals::cQuarkJet = (boost::shared_ptr<TH2F>) (TH2F*) file->Get(pathToBTagEff.c_str())->Clone();
	file->Close();
}


void ConfigFile::getudsgQuarkJet(std::string btagEfficiencyFile) {
	using namespace std;

	std::string pathToBTagEff = getSampleBTagEffTag(sample_).append("udsgQuarkJets_Ratio_Hist");

	if (!boost::filesystem::exists(btagEfficiencyFile)) {
		cerr << "ConfigFile::getudsgQuarkJet(" << btagEfficiencyFile << "): could not find file" << endl;
		throw "Could not find file " + btagEfficiencyFile;
	}
	pathToBTagEff = checkEffFileExists(btagEfficiencyFile, pathToBTagEff);
	boost::scoped_ptr<TFile> file(TFile::Open(btagEfficiencyFile.c_str()));
	Globals::udsgQuarkJet = (boost::shared_ptr<TH2F>) (TH2F*) file->Get(pathToBTagEff.c_str())->Clone();
	file->Close();
}

std::string ConfigFile::checkEffFileExists(std::string btagEfficiencyFile, std::string histpath) {
	boost::scoped_ptr<TFile> file(TFile::Open(btagEfficiencyFile.c_str()));
	std::vector<std::string> newpath;
  	std::stringstream ss(histpath);
  	std::string chunk, key;
  
	while(std::getline(ss, chunk, '/')) {
	    newpath.push_back(chunk);
	}
	if (file->GetListOfKeys()->Contains(newpath[0].c_str())){
		file->Close();
		return histpath;
	}
	else{
		std::cout << "Current TTJet sample " << sample_ << " is not available in BTagEfficiencyFile, switching to default PowhegPythia8/" << std::endl;
		newpath[0] = "PowhegPythia8/";
		histpath = newpath[0]+newpath[1];
		std::cout << "New Histogram Path : " << histpath << std::endl;
		file->Close();
		return histpath;
	}
}

std::string ConfigFile::getSampleBTagEffTag(std::string sample){
	// Use std::map here?
	if (sample == "TTJets_PowhegPythia8") return "PowhegPythia8/";
	else if( sample == "TTJets_PowhegHerwigpp") return "PowhegHerwigpp/";
	else if( sample == "TTJets_amcatnloFXFX") return "aMCatNLOPythia8/";
	else if( sample == "TTJets_madgraph") return "Madgraph/";
	else if( sample == "TTJets_PowhegPythia8_fsrdown") return "PowhegPythia8_fsrup/";
	else if( sample == "TTJets_PowhegPythia8_fsrup") return "PowhegPythia8_fsrdown/";
	else if( sample == "TTJets_PowhegPythia8_isrdown") return "PowhegPythia8_isrup/";
	else if( sample == "TTJets_PowhegPythia8_isrup") return "PowhegPythia8_isrdown/";
	else if( sample == "TTJets_PowhegPythia8_down") return "PowhegPythia8_up/";
	else if( sample == "TTJets_PowhegPythia8_up") return "PowhegPythia8_down/";
	else if( sample == "TTJets_PowhegPythia8_mtop1695") return "PowhegPythia8_mtop1755/";
	else if( sample == "TTJets_PowhegPythia8_mtop1755") return "PowhegPythia8_mtop1695/";
	else return "PowhegPythia8/";
}

unsigned int ConfigFile::nTupleVersion() const {
	return nTupleVersion_;
}

std::string ConfigFile::sample() const {
	return sample_;
}

} /* namespace BAT */
