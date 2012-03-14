/*
 * PythonParser.cpp
 *
 *  Created on: 12 Mar 2012
 *      Author: kreczko
 */

#include "../../interface/Python/PythonParser.h"
using namespace std;

namespace BAT {

PythonParser::PythonParser(){

}

PythonParser::~PythonParser() {
}

boost::python::object PythonParser::parse_python_file(const string configPath) {
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

const vector<string> PythonParser::getVectorFromPythonObject(const boost::python::object pyObject, const string vectorName) {
	using namespace boost::python;
	vector<string> ret;
	try {
		list myList = extract<list>(pyObject.attr(vectorName.c_str()));
		unsigned int length = extract<unsigned int>(myList.attr("__len__")());

		for (unsigned int index = 0; index < length; ++index) {
			ret.push_back(extract<string>(myList[index]));
		}
	} catch (boost::python::error_already_set const &) {
		string perror_str = PythonParser::parse_python_exception();
		cout << "Error during configuration parsing: " << perror_str << endl;
		throw "Configparser: error when parsing '" + vectorName + "'.";
	}
	return ret;
}

string PythonParser::parse_python_exception() {
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
