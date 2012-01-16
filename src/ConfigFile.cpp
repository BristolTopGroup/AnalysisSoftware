/*
 * ConfigFile.cpp
 *
 *  Created on: 20 Oct 2011
 *      Author: kreczko
 */

#include "../interface/ConfigFile.h"
#include "../interface/GlobalVariables.h"
#include "TF1.h"
#include "TFile.h"
#include <iostream>
#include <boost/scoped_ptr.hpp>
#include <sstream>
#include <iomanip>

using namespace std;

namespace BAT {
const string DEFAULT_CONFIG_PATH = "python/default_cfg.py";

ConfigFile::ConfigFile(int argc, char **argv) :
		programOptions(getParameters(argc, argv)),
		config(parse_config(configPath())),
		maxEvents_(get<long>("maxEvents")),
		pileUpFile_(get<string>("PUFile")),
        bJetResoFile_(get<string>("bJetResoFile")),
        lightJetResoFile_(get<string>("lightJetResoFile")),
		useHitFit_(get<bool>("useHitFit")),
		fitterOutputFlag_(get<bool>("produceFitterASCIIoutput")),
		inputFiles_(getVector("inputFiles")),
		tqafPath_(get<string>("TQAFPath")),
		lumi_(get<double>("lumi")),
		pileUpReweightingMethod_(Globals::pileUpReweightingMethod){

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
	desc.add_options()("PUfile", value<std::string>(), "set input PU file for PU re-weighting");
    desc.add_options()("bJetResoFile", value<std::string>(), "set input root file for b-jet L7 resolutions");
    desc.add_options()("lightJetResoFile", value<std::string>(), "set input root file for light jet L7 resolutions");
	desc.add_options()("fitter", value<bool>(), "turn on the fitter (HitFit)");
	desc.add_options()("fitterASCIIoutput", value<bool>(), "produce full kinematic fit output in ASCII format");
	desc.add_options()("TQAFPath", value<std::string>(), "path to TopQuarkAnalysis folder (the folder itself not included).");
	desc.add_options()("lumi", value<std::string>(), "Integrated luminosity the MC simulation will be scaled to.");

	store(command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	notify(vm);
	if (vm.count("help")) {
		cout << desc << "\n";
		exit(0);
		return 0;
	}

	return vm;
}

boost::python::object ConfigFile::parse_config(const string configPath) {
	Py_Initialize();
	boost::python::object configuration;
	try {
		boost::python::object main_module = boost::python::import("__main__");
		boost::python::object main_namespace = main_module.attr("__dict__");

		boost::python::exec("import imp", main_namespace);
		boost::python::exec("import os", main_namespace);
		boost::python::object result = boost::python::exec("def load_from_file(filepath):                   \n"
				"    obj = None                   \n"
				"    expected_object = 'config'   \n"
				"    mod_name, file_ext = os.path.splitext(os.path.split(filepath)[-1]) \n"
				"    if file_ext.lower() == '.py': \n"
				"        py_mod = imp.load_source(mod_name, filepath) \n"
				"    elif file_ext.lower() == '.pyc': \n"
				"        py_mod = imp.load_compiled(mod_name, filepath) \n"
//				"    if expected_object in dir(py_mod):\n"
//				"        obj = py_mod.config \n"
				"    obj = py_mod \n"
				"    return obj \n", main_namespace, main_namespace);

		// Create a reference to it.
		boost::python::object load_from_file = main_namespace["load_from_file"];

		configuration = load_from_file(configPath);
	} catch (boost::python::error_already_set const &) {
		string perror_str = parse_python_exception();
		cout << "Error during configuration parsing: " << perror_str << endl;
		throw "ConfigParser: Terminating. Could not initialise configuration.";
	}
	return configuration;
}

template<typename T>
const T ConfigFile::get(const string attribute) const {
	using namespace boost::python;
	T ret;
	try {
		ret = extract<T>(config.attr(attribute.c_str()));
	} catch (error_already_set const &) {
		string perror_str = parse_python_exception();
		cout << "Error during configuration parsing: " << perror_str << endl;
		throw "Configparser: error when parsing '" + attribute + "'.";
	}
	return ret;
}

const vector<string> ConfigFile::getVector(const string attribute) {
	using namespace boost::python;
	vector<string> ret;
	try {
		list myList = extract<list>(config.attr(attribute.c_str()));
		unsigned int length = extract<unsigned int>(myList.attr("__len__")());

		for (unsigned int index = 0; index < length; ++index) {
			ret.push_back(extract<string>(myList[index]));
		}
	} catch (boost::python::error_already_set const &) {
		string perror_str = parse_python_exception();
		cout << "Error during configuration parsing: " << perror_str << endl;
		throw "Configparser: error when parsing '" + attribute + "'.";
	}
	return ret;
}
//
//void ConfigFile::LoadJetL7Resolutions(std::string bJetResoFile, std::string lightJetResoFile){
//    unsigned int nEtaBins_ = 11;
//    Float_t towerBinning[] = {0.0, 0.174, 0.348, 0.522, 0.696, 0.87, 1.044, 1.218, 1.392, 1.566, 1.74, 2.5};
//    TFile *bFile = new TFile (bJetResoFile.c_str(), "READ");
//    TFile *lFile = new TFile (lightJetResoFile.c_str(), "READ");
//
//    bFile->cd();
//    lFile->cd();
//
//    for(UInt_t iEta = 0; iEta < nEtaBins_; iEta++) {
//        TString loweretastrg;
//        TString upperetastrg;
//        Char_t etachar[10];
//        sprintf(etachar,"%1.3f",towerBinning[iEta]);
//        loweretastrg = etachar;
//        sprintf(etachar,"%1.3f",towerBinning[iEta+1]);
//        upperetastrg = etachar;
//        Globals::bL7Corrections[iEta] = boost::shared_ptr<TF1>((TF1*) bFile->Get("mean_relative_et_"+loweretastrg+"_"+upperetastrg));
//        Globals::lightL7Corrections[iEta] = boost::shared_ptr<TF1>((TF1*) lFile->Get("mean_relative_et_"+loweretastrg+"_"+upperetastrg));
//    }
//    bFile->Close();
//    lFile->Close();
//}

boost::array<boost::shared_ptr<TF1>, 12> ConfigFile::getL7Correction(std::string correctionFile) {
	//TODO: add check if file exists (boost::filesystem)
	boost::array<boost::shared_ptr<TF1>, 12> correctionsArray;

	boost::array<double, 12> towerBinning = {{0., 0.174, 0.348, 0.522, 0.696, 0.87, 1.044, 1.218, 1.392, 1.566, 1.74, 2.5}};
	boost::scoped_ptr<TFile> inputFile(TFile::Open(correctionFile.c_str()));

	for(unsigned int etaBin = 0; etaBin < towerBinning.size() - 1; ++ etaBin){
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
	if(programOptions.count("bJetResoFile"))
		return programOptions["bJetResoFile"].as<std::string>();
        else
	        return bJetResoFile_;
}

string ConfigFile::lightJetResoFile() const {
        if(programOptions.count("lightJetResoFile"))
                return programOptions["lightJetResoFile"].as<std::string>();
        else
                return lightJetResoFile_;
}

string ConfigFile::TQAFPath() const{
	if(programOptions.count("TQAFPath"))
		return programOptions["TQAFPath"].as<std::string>();
	else
		return tqafPath_;
}


string ConfigFile::configPath() const{
	if(programOptions.count("config-file"))
		return programOptions["config-file"].as<std::string>();
	else
		return DEFAULT_CONFIG_PATH;
}
string ConfigFile::PUFile() const {
	if(programOptions.count("PUfile"))
		return programOptions["PUfile"].as<std::string>();
	else
		return pileUpFile_;
}

long ConfigFile::maxEvents() const{
	if(programOptions.count("maxEvents"))
		return programOptions["maxEvents"].as<unsigned long>();
	else
		return maxEvents_;
}

bool ConfigFile::useHitFit() const{
	if(programOptions.count("fitter"))
		return programOptions["fitter"].as<bool>();
	else
		return useHitFit_;
}

bool ConfigFile::fitterOutputFlag() const{
	if(programOptions.count("fitterASCIIoutput"))
		return programOptions["fitterASCIIoutput"].as<bool>();
	else
		return fitterOutputFlag_;
}

const vector<string>& ConfigFile::inputFiles() const{
	return inputFiles_;
}

double ConfigFile::lumi() const {
	if(programOptions.count("lumi"))
		return programOptions["lumi"].as<double>();
	else
		return lumi_;
}

ConfigFile::~ConfigFile() {
}

string ConfigFile::parse_python_exception() const {
	PyObject *type_ptr = NULL, *value_ptr = NULL, *traceback_ptr = NULL;
	PyErr_Fetch(&type_ptr, &value_ptr, &traceback_ptr);
	string ret("Unfetchable Python error");

	if (type_ptr != NULL) {
		boost::python::handle<> h_type(type_ptr);
		boost::python::str type_pstr(h_type);
		boost::python::extract<string> e_type_pstr(type_pstr);
		if (e_type_pstr.check())
			ret = e_type_pstr();
		else
			ret = "Unknown exception type";
	}

	if (value_ptr != NULL) {
		boost::python::handle<> h_val(value_ptr);
		boost::python::str a(h_val);
		boost::python::extract<string> returned(a);
		if (returned.check())
			ret += ": " + returned();
		else
			ret += string(": Unparseable Python error: ");
	}

	if (traceback_ptr != NULL) {
		boost::python::handle<> h_tb(traceback_ptr);
		boost::python::object tb(boost::python::import("traceback"));
		boost::python::object fmt_tb(tb.attr("format_tb"));
		boost::python::object tb_list(fmt_tb(h_tb));
		boost::python::object tb_str(boost::python::str("\n").join(tb_list));
		boost::python::extract<string> returned(tb_str);
		if (returned.check())
			ret += ": " + returned();
		else
			ret += string(": Unparseable Python traceback");
	}
	return ret;
}

PileUpReweightingMethod::value ConfigFile::PileUpReweightingMethod() const {
	std::string option = "";

	if (isOptionSetInConsole("pileUpReweightingMethod")) {
		option = programOptions["pileUpReweightingMethod"].as<std::string> ();
	} else if (isOptionSetInConfig(option)) {
		option = get<std::string> ("pileUpReweightingMethod");
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

bool ConfigFile::isOptionSet(std::string option) const{
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

	//kinematic fit
	Globals::TQAFPath = TQAFPath();
	Globals::useHitFit = useHitFit();
	Globals::produceFitterASCIIoutput = fitterOutputFlag();

	//Loading l7 JEC
//	LoadJetL7Resolutions(bJetResoFile(), lightJetResoFile());

	Globals::estimatedPileup = getPileUpHistogram(PUFile());

	//Loading l7 JEC
	Globals::bL7Corrections = getL7Correction(bJetResoFile());
	Globals::lightL7Corrections = getL7Correction(lightJetResoFile());
}

boost::shared_ptr<TH1D> ConfigFile::getPileUpHistogram(std::string pileUpEstimationFile) {
	using namespace std;
	boost::scoped_ptr<TFile> file(TFile::Open(pileUpEstimationFile.c_str()));
	boost::shared_ptr<TH1D> pileUp((TH1D*) file->Get("pileup")->Clone());
	file->Close();

	return pileUp;
}
} /* namespace BAT */
