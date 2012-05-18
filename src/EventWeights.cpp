/*
 * EventWeights.cpp
 *
 *  Created on: 24 Mar 2012
 *      Author: kreczko
 */

#include "../interface/EventWeights.h"
#include "TFile.h"

#include <boost/filesystem.hpp>
#include <boost/scoped_ptr.hpp>

#include <iostream>

using namespace std;

namespace BAT {

EventWeights::EventWeights(string datasetInfo, string DataPUFile, string MCPUFile) :
		datasetInfo_(datasetInfo), //
		xsection(datasetInfo_.getCrossSections()), //
		numberOfProcessedEvents(datasetInfo_.getArrayOfProcessedEvents()), //
		dataPUDistribution_(
				EventWeights::getPUDistributionFromHistogram(EventWeights::getPUHistogramFromFile(DataPUFile))), //
		mcPUDistribution_(EventWeights::getPUDistributionFromHistogram(EventWeights::getPUHistogramFromFile(MCPUFile))) {

}

EventWeights::EventWeights(string datasetInfo, TH1DPtr DataPUHistogram, TH1DPtr MCPUHistogram) :
		datasetInfo_(datasetInfo), //
		dataPUDistribution_(EventWeights::getPUDistributionFromHistogram(DataPUHistogram)), //
		mcPUDistribution_(EventWeights::getPUDistributionFromHistogram(MCPUHistogram)) {

}

EventWeights::EventWeights(string datasetInfo, vector<double> DataPUDistribution, vector<double> MCPUDistribution) :
		datasetInfo_(datasetInfo), //
		dataPUDistribution_(DataPUDistribution), //
		mcPUDistribution_(MCPUDistribution) {

}

const TH1DPtr EventWeights::getPUHistogramFromFile(const string pileUpEstimationFile) {
	if (!boost::filesystem::exists(pileUpEstimationFile)) {
		cerr << "EventWeights::getPUHistogramFromFile(" << pileUpEstimationFile << "): could not find file" << endl;
		throw "Could not find pile-up histogram file in " + pileUpEstimationFile;
	}

	boost::scoped_ptr<TFile> file(TFile::Open(pileUpEstimationFile.c_str()));
	boost::shared_ptr<TH1D> pileUp((TH1D*) file->Get("pileup")->Clone());
	file->Close();

	return pileUp;
}

const vector<double> EventWeights::getPUDistributionFromHistogram(const TH1DPtr puhist) {
	vector<double> distribution;

	for (unsigned int nVertex = 0; nVertex < NVERTEX; ++nVertex) {
		if (nVertex >= (unsigned int) puhist->GetNbinsX())
			break;

		distribution.push_back(puhist->GetBinContent(puhist->GetXaxis()->FindBin(nVertex)));
	}
	return distribution;
}

EventWeights::~EventWeights() {
}

void EventWeights::generate3DWeights(){

}

} /* namespace BAT */
