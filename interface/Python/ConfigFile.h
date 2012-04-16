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
    std::string bJetResoFile() const;
    std::string lightJetResoFile() const;
//    void LoadJetL7Resolutions(std::string bJetResoFile, std::string lightJetResoFile);
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

private:
	boost::program_options::variables_map programOptions;
	boost::python::object config;
	long maxEvents_;
	std::string datasetInfoFile_;
	std::string pileUpFile_;
    std::string bJetResoFile_;
    std::string lightJetResoFile_;
	bool useHitFit_;
	bool fitterOutputFlag_;
	std::vector<std::string> inputFiles_;
	std::string tqafPath_;
	double lumi_;
	unsigned int centerOfMassEnergy_;
//	PileUpReweightingMethod::value pileUpReweightingMethod_;

	boost::program_options::variables_map getParameters(int argc, char **argv);
	boost::shared_ptr<TH1D> getPileUpHistogram(std::string pileUpEstimationFile);
	boost::array<boost::shared_ptr<TF1>, 12> getL7Correction(std::string correctionFile);
};

} /* namespace BAT */
#endif /* CONFIGFILE_H_ */
