/*
 * ConfigFile.cpp
 *
 *  Created on: 20 Oct 2011
 *      Author: kreczko
 */

#include "../interface/ConfigFile.h"
#include <iostream>

using namespace std;

namespace BAT {
const string DEFAULT_CONFIG_PATH = "python/default_cfg.py";

ConfigFile::ConfigFile(int argc, char **argv) :
		programOptions(getParameters(argc, argv)),
		config(parse_config(configPath())),
		maxEvents_(get<unsigned long>("maxEvents")),
		pileUpFile_(get<string>("PUFile")),
		useHitFit_(get<bool>("useHitFit")),
		inputFiles_(getVector("inputFiles")),
		tqafPath_(get<string>("TQAFPath")),
		lumi_(get<double>("lumi")){

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
	desc.add_options()("fitter", value<bool>(), "turn on the fitter (HitFit)");
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
		throw "ConfigParser: Terminating. Could not initilise configuration.";
	}
	return configuration;
}

template<typename T>
const T ConfigFile::get(const string attribute) {
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

unsigned long ConfigFile::maxEvents() const{
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

string ConfigFile::parse_python_exception() {
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
} /* namespace BAT */
