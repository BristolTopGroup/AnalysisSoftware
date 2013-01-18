/*
 * BinnedVariableanalyser.h
 *
 *  Created on: 17 Jan 2013
 *      Author: kreczko
 */

#ifndef BINNEDVARIABLEANALYSER_H_
#define BINNEDVARIABLEANALYSER_H_

#include "BasicAnalyser.h"
#include "../Event.h"
#include <vector>

namespace BAT {

class Binned_variable_analyser: public BAT::BasicAnalyser {
public:
	Binned_variable_analyser(HistogramManagerPtr histMan, std::string histogramFolder);
	virtual ~Binned_variable_analyser();
	virtual void analyse(const EventPtr);
	void analyse(double variable, double fit_variable, double weight);
	void set_variables(std::string variable_name, std::vector<double> variable_bins, std::string fit_variable_name,
			std::vector<double> fit_variable_bins);

	void set_variables(std::string variable_name, std::vector<double> variable_bins, std::string fit_variable_name,
				unsigned int fit_variable_n_bins, double fit_variable_min, double fit_variable_max);

	virtual void createHistograms();

private:
	std::string variable_name_, fit_variable_name_;
	std::vector<double> variable_bins_, fit_variable_bins_;
	unsigned int fit_variable_n_bins_;
	double fit_variable_min_, fit_variable_max_;

};

typedef boost::scoped_ptr<Binned_variable_analyser> Binned_Variable_analyser_local_ptr;
typedef boost::shared_ptr<Binned_variable_analyser> Binned_Variable_analyser_ptr;
}
#endif /* BINNEDVARIABLEANALYSER_H_ */
