/*
 * BinnedVariableanalyser.cpp
 *
 *  Created on: 17 Jan 2013
 *      Author: kreczko
 */

#include "../../interface/Analysers/Binned_variable_analyser.h"

using namespace std;

namespace BAT {

Binned_variable_analyser::Binned_variable_analyser(HistogramManagerPtr histMan, string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		variable_(), //
		fit_variables_(){

}

Binned_variable_analyser::~Binned_variable_analyser() {
}

void Binned_variable_analyser::analyse(const EventPtr event) {

}

void Binned_variable_analyser::analyse(double variable, double fit_variable_value,
		double weight) {
	if (!have_histograms_been_created_)
		createHistograms();
	analyse(variable, fit_variable_value, weight, fit_variables_.at(0).name);
}

void Binned_variable_analyser::analyse(double variable,
		std::vector<double> fit_variable_values, double weight) {
	assert(fit_variable_values.size() == fit_variables_.size());
	if (!have_histograms_been_created_)
		createHistograms();

	for (unsigned int i = 0; i < fit_variable_values.size(); ++i) {
		analyse(variable, fit_variable_values.at(i), weight,
				fit_variables_.at(i).name);
	}

	analyse_correlations(fit_variable_values, weight);
}

void Binned_variable_analyser::analyse(double variable,
		double fit_variable_value, double weight, string fit_variable_name) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = weight * prescale_ * scale_;
	for (unsigned int index = 0; index < variable_.bins.size(); ++index) {
		double upperCut =
				index < variable_.bins.size() - 1 ?
						variable_.bins.at(index + 1) : 999999.;
		double lowerCut = variable_.bins.at(index);

		if (variable >= lowerCut && variable < upperCut) { //find right bin
			string lower_edge = boost::lexical_cast<string>(variable_.bins.at(index));
			string upper_edge =
					index < variable_.bins.size() - 1 ? boost::lexical_cast<string>(variable_.bins.at(index + 1)) :
							"inf";
			string folder = variable_.name + "_bin_" + lower_edge + "-" + upper_edge;
			histMan_->setCurrentHistogramFolder(
					histogramFolder_ + "/" + folder);
			histMan_->H1D_BJetBinned(fit_variable_name)->Fill(
					fit_variable_value, weight_);
		}
	}
}

void Binned_variable_analyser::analyse_correlations(
		vector<double> fit_variable_values, double weight) {
	if (fit_variable_values.size() < 2)
		return;
	// only if we have more than one variable
	for (unsigned int i = 0; i < fit_variables_.size(); ++i) {
		histMan_->setCurrentHistogramFolder(
				histogramFolder_ + "/" + "fit_variable_correlations");
		for (unsigned int j = i + 1; j < fit_variables_.size(); ++j) {
			Variable a = fit_variables_.at(i);
			Variable b = fit_variables_.at(j);
			double value_a = fit_variable_values.at(i);
			double value_b = fit_variable_values.at(j);
			histMan_->H2D(b.name + "_vs_" + a.name)->Fill(value_a, value_b,
					weight);
		}
	}
}

void Binned_variable_analyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);

	for (unsigned int index = 0; index < variable_.bins.size(); ++index) {
		string lower_edge = boost::lexical_cast<string>(variable_.bins.at(index));
		string upper_edge =
				index < variable_.bins.size() - 1 ? boost::lexical_cast<string>(variable_.bins.at(index + 1)) : "inf";

		string folder = variable_.name + "_bin_" + lower_edge + "-" + upper_edge;
		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + folder);
		// create all fit variable histograms
		for (unsigned int i = 0; i < fit_variables_.size(); ++i) {
			Variable fit_variable = fit_variables_.at(i);
			if ((fit_variable.bins.empty() && fit_variable.n_bins <= 1)
					|| variable_.bins.empty())
				throw "Binned_Variable_analyser: fit_variable_bins and/or variable_bins have not been specified!";

			if (fit_variable.bins.empty())
				histMan_->addH1D_BJetBinned(fit_variable.name,
						fit_variable.name, fit_variable.n_bins,
						fit_variable.min, fit_variable.max);
			//no alternative at the moment
		}
	}

	// correlation histograms
	for (unsigned int i = 0; i < fit_variables_.size(); ++i) {
		histMan_->setCurrentHistogramFolder(
				histogramFolder_ + "/" + "fit_variable_correlations");
		for (unsigned int j = i + 1; j < fit_variables_.size(); ++j) {
		// no b-tag binning
			Variable a = fit_variables_.at(i);
			Variable b = fit_variables_.at(j);
			// name, 'title; y-axis title; x-axis title', etc
			histMan_->addH2D(b.name + "_vs_" + a.name, b.name + "_vs_" + a.name + "; " + a.name + "; " + b.name, a.n_bins, a.min, a.max, b.n_bins, b.min, b.max);
		}

	}
	have_histograms_been_created_ = true;
}

void Binned_variable_analyser::set_variables(string variable_name, vector<double> variable_bins,
		string fit_variable_name, vector<double> fit_variable_bins) {
	variable_ = Variable(variable_name, variable_bins);
	variable_.bins = variable_bins;

	fit_variables_.push_back(Variable(fit_variable_name, fit_variable_bins));
}

void Binned_variable_analyser::set_variables(string variable_name,
		vector<double> variable_bins, string fit_variable_name,
		unsigned int fit_variable_n_bins, double fit_variable_min,
		double fit_variable_max) {
	variable_ = Variable(variable_name, variable_bins);

	fit_variables_.push_back(
			Variable(fit_variable_name, fit_variable_n_bins, fit_variable_min,
					fit_variable_max));
}

void Binned_variable_analyser::set_variables(Variable variable,
		Variable fit_variable) {
	variable_ = variable;
	fit_variables_.push_back(fit_variable);
}

void Binned_variable_analyser::set_variables(Variable variable,
		vector<Variable> fit_variables) {
	assert(fit_variables.size() > 0);
	variable_ = variable;
	fit_variables_ = fit_variables;
}

}

