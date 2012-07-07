/*
 * PythonParser.h
 *
 * Code from
 * http://thejosephturner.com/blog/2011/07/18/embedding-python-in-c-applications-with-boostpython-part-4/
 *
 * Modified:
 *  Created on: 12 Mar 2012
 *      Author: kreczko
 */

#ifndef PYTHONPARSER_H_
#define PYTHONPARSER_H_
#include <boost/python.hpp>
#include <vector>
#include <string>
#include <iostream>

namespace BAT {

class PythonParser {
public:
	PythonParser();
	virtual ~PythonParser();
	static boost::python::object parse_python_file(const std::string configPath);
	static std::string parse_python_exception();
	template<typename T> static T getAttributeFromPyObject(const boost::python::object pyObject,
			const std::string attribute);
	static const std::vector<std::string> getVectorFromPythonObject(const boost::python::object pyObject,
			const std::string attribute);

	template<typename T> static T getObjectFromPyDictionary(const boost::python::dict pydict,
			const std::string key);

	static bool hasAttribute(const boost::python::object pyObject, const std::string attribute);
};

template<typename T>
T PythonParser::getAttributeFromPyObject(const boost::python::object pyObject, const std::string attribute) {
	using namespace boost::python;
	T ret;
	try {
		ret = extract<T>(pyObject.attr(attribute.c_str()));
	} catch (error_already_set const &) {
		std::string perror_str = PythonParser::parse_python_exception();
		std::cout << "Error during Python file parsing: " << perror_str << std::endl;
		std::cout << "PythonParser: error when parsing '" + attribute + "'." << std::endl;
		throw "PythonParser: error when parsing '" + attribute + "'.";
	}
	return ret;
}

template<typename T>
T PythonParser::getObjectFromPyDictionary(const boost::python::dict pydict, const std::string key) {
	using namespace boost::python;
	T ret;
	try {
		ret = extract<T>(pydict.get(key));
	} catch (error_already_set const &) {
		std::string perror_str = PythonParser::parse_python_exception();
		std::cout << "Error during python file parsing: " << perror_str << std::endl;
		std::cout << "PythonParser: error when accessing dictionary key '" + key + "'." << std::endl;
		throw "PythonParser: error when accessing dictionary key '" + key + "'.";
	}
	return ret;
}
} /* namespace BAT */
#endif /* PYTHONPARSER_H_ */
