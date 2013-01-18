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
		variable_name_("variable"), //
		fit_variable_name_("fit_variable"), //
		variable_bins_(), //
		fit_variable_bins_(), //
		fit_variable_n_bins_(1), //
		fit_variable_min_(0), //
		fit_variable_max_(100) {

}

Binned_variable_analyser::~Binned_variable_analyser() {
}

void Binned_variable_analyser::analyse(const EventPtr event) {

}

void Binned_variable_analyser::analyse(double variable, double fit_variable, double weight) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = weight * prescale_ * scale_;
	for (unsigned int index = 0; index < variable_bins_.size() + 1; ++index) {
		double upperCut = index < variable_bins_.size() ? variable_bins_.at(index) : 999999.;
		double lowerCut = index == 0 ? 0. : variable_bins_.at(index - 1);

		if (variable >= lowerCut && variable < upperCut) {//find right bin
			string bin =
					index < variable_bins_.size() ? boost::lexical_cast<string>(variable_bins_.at(index)) : "inf";
			string previousBin = index == 0 ? "0" : boost::lexical_cast<string>(variable_bins_.at(index - 1));
			string folder = variable_name_ + "_bin_" + previousBin + "-" + bin;
			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + folder);
			histMan_->H1D_BJetBinned(fit_variable_name_)->Fill(fit_variable, weight_);
		}
	}
}

void Binned_variable_analyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	if ((fit_variable_bins_.empty() && fit_variable_n_bins_ <= 1) || variable_bins_.empty())
		throw "Binned_Variable_analyser: fit_variable_bins and/or variable_bins have not been specified!";

	for (unsigned int index = 0; index < variable_bins_.size() + 1; ++index) {
		string bin = index < variable_bins_.size() ? boost::lexical_cast<string>(variable_bins_.at(index)) : "inf";
		string previousBin = index == 0 ? "0" : boost::lexical_cast<string>(variable_bins_.at(index - 1));
		string folder = variable_name_ + "_bin_" + previousBin + "-" + bin;
		histMan_->setCurrentHistogramFolder(histogramFolder_ + "/" + folder);
		if (fit_variable_bins_.empty())
			histMan_->addH1D_BJetBinned(fit_variable_name_, fit_variable_name_, fit_variable_n_bins_, fit_variable_min_,
					fit_variable_max_);
		//no alternative at the moment
	}
}

void Binned_variable_analyser::set_variables(string variable_name, vector<double> variable_bins,
		string fit_variable_name, vector<double> fit_variable_bins) {
	variable_name_ = variable_name;
	fit_variable_name_ = fit_variable_name;
	variable_bins_ = variable_bins;
	fit_variable_bins_ = fit_variable_bins;
}

void Binned_variable_analyser::set_variables(string variable_name, vector<double> variable_bins,
		string fit_variable_name, unsigned int fit_variable_n_bins, double fit_variable_min,
		double fit_variable_max) {
	variable_name_ = variable_name;
	fit_variable_name_ = fit_variable_name;
	variable_bins_ = variable_bins;
	fit_variable_n_bins_ = fit_variable_n_bins;
	fit_variable_min_ = fit_variable_min;
	fit_variable_max_ = fit_variable_max;
}

}

