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

struct Variable {
	std::string name;
	std::vector<double> bins;
	unsigned int n_bins;
	double min;
	double max;

	Variable() :
			name("none"), //
			bins(), //
			n_bins(0), //
			min(0), //
			max(0) {
	}

	Variable(std::string name, std::vector<double> bins) :
			name(name), //
			bins(bins), //
			n_bins(bins.size()), //
			min(bins.at(0)), //
			max(bins.at(bins.size() - 1)) {
	}

	Variable(std::string name, unsigned int n_bins, double min, double max) :
			name(name), //
			bins(), //
			n_bins(n_bins), //
			min(min), //
			max(max) {
	}
};

class Binned_variable_analyser: public BAT::BasicAnalyser {
public:
	Binned_variable_analyser(HistogramManagerPtr histMan, std::string histogramFolder);
	virtual ~Binned_variable_analyser();
	virtual void analyse(const EventPtr);
	void analyse(double variable, double fit_variable_value, double weight);
	void analyse(double variable, double fit_variable_value, double weight, std::string fit_variable_name);
	void analyse(double variable, std::vector<double> fit_variable_values, double weight);
	void set_variables(std::string variable_name, std::vector<double> variable_bins, std::string fit_variable_name,
			std::vector<double> fit_variable_bins);

	void set_variables(std::string variable_name, std::vector<double> variable_bins, std::string fit_variable_name,
				unsigned int fit_variable_n_bins, double fit_variable_min, double fit_variable_max);

	void set_variables(Variable variable, Variable fit_variable);
	void set_variables(Variable variable, std::vector<Variable> fit_variable);

	virtual void createHistograms();

private:
	void analyse_correlations(std::vector<double> fit_variable_values, double weight);
	Variable variable_;
	std::vector<Variable> fit_variables_;

};

typedef boost::scoped_ptr<Binned_variable_analyser> Binned_Variable_analyser_local_ptr;
typedef boost::shared_ptr<Binned_variable_analyser> Binned_Variable_analyser_ptr;
}
#endif /* BINNEDVARIABLEANALYSER_H_ */
