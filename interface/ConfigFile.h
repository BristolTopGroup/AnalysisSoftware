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

namespace BAT {

class ConfigFile {
public:
	ConfigFile(int argc, char **argv);
	virtual ~ConfigFile();

	unsigned long maxEvents() const;
	std::string PUFile() const;
    std::string bJetResoFile() const;
    std::string lightJetResoFile() const;
    void LoadJetL7Resolutions(std::string bJetResoFile, std::string lightJetResoFile);
	bool useHitFit() const;
	bool fitterOutputFlag() const;
	const std::vector<std::string>& inputFiles() const;
	std::string configPath() const;
	std::string TQAFPath() const;
	double lumi() const;

private:
	boost::program_options::variables_map programOptions;
	boost::python::object config;
	unsigned long maxEvents_;
	std::string pileUpFile_;
    std::string bJetResoFile_;
    std::string lightJetResoFile_;
	bool useHitFit_;
	bool fitterOutputFlag_;
	std::vector<std::string> inputFiles_;
	std::string tqafPath_;
	double lumi_;

	boost::python::object parse_config(const std::string configPath);
	std::string parse_python_exception();
	template<typename T> const T get(const std::string attribute);
	const std::vector<std::string> getVector(const std::string attribute);
	boost::program_options::variables_map getParameters(int argc, char **argv);
};

} /* namespace BAT */
#endif /* CONFIGFILE_H_ */
