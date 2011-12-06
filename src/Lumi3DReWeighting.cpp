/*
 * Lumi3DReWeighting.cpp
 *
 *  Created on: 5 Dec 2011
 *      Author: kreczko
 */

#include "../interface/Lumi3DReWeighting.h"

#include "TRandom1.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TStopwatch.h"
#include "TH1.h"
#include "TH3.h"
#include "TFile.h"
#include <string>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <iostream>

namespace BAT {

Lumi3DReWeighting::Lumi3DReWeighting(std::string generatedFile, std::string dataFile,
		std::string GenHistName = "pileup", std::string DataHistName = "pileup") :
		generatedFileName_(generatedFile), //
		dataFileName_(dataFile), //
		GenHistName_(GenHistName), //
		DataHistName_(DataHistName) {
	generatedFile_ = boost::shared_ptr<TFile>(new TFile(generatedFileName_.c_str())); //MC distribution
	dataFile_ = boost::shared_ptr<TFile>(new TFile(dataFileName_.c_str())); //Data distribution

	boost::scoped_ptr<TH1> Data_temp((static_cast<TH1*>(dataFile_->Get(DataHistName_.c_str())->Clone())));

	boost::scoped_ptr<TH1> MC_temp((static_cast<TH1*>(generatedFile_->Get(GenHistName_.c_str())->Clone())));

	MC_distr_ = boost::shared_ptr<TH1>((static_cast<TH1*>(generatedFile_->Get(GenHistName_.c_str())->Clone())));
	Data_distr_ = boost::shared_ptr<TH1>((static_cast<TH1*>(dataFile_->Get(DataHistName_.c_str())->Clone())));

	// MC * data/MC = data, so the weights are data/MC:

	// normalize both histograms first

	Data_distr_->Scale(1.0 / Data_distr_->Integral());
	MC_distr_->Scale(1.0 / MC_distr_->Integral());

}

Lumi3DReWeighting::Lumi3DReWeighting(std::vector<float> MC_distr, std::vector<float> Lumi_distr) {
	// no histograms for input: use vectors

	// now, make histograms out of them:

	Int_t NMCBins = MC_distr.size();

	MC_distr_ = boost::shared_ptr<TH1>(new TH1F("MC_distr", "MC dist", NMCBins, 0., float(NMCBins)));

	Int_t NDBins = Lumi_distr.size();

	Data_distr_ = boost::shared_ptr<TH1>(new TH1F("Data_distr", "Data dist", NDBins, 0., float(NDBins)));

	for (int ibin = 1; ibin < NMCBins + 1; ++ibin) {
		MC_distr_->SetBinContent(ibin, MC_distr[ibin - 1]);
	}

	for (int ibin = 1; ibin < NDBins + 1; ++ibin) {
		Data_distr_->SetBinContent(ibin, Lumi_distr[ibin - 1]);
	}

	// check integrals, make sure things are normalized

	float deltaH = Data_distr_->Integral();
	if (fabs(1.0 - deltaH) > 0.001) { //*OOPS*...
		Data_distr_->Scale(1.0 / Data_distr_->Integral());
	}
	float deltaMC = MC_distr_->Integral();
	if (fabs(1.0 - deltaMC) > 0.001) {
		MC_distr_->Scale(1.0 / MC_distr_->Integral());
	}

}

double Lumi3DReWeighting::weight3D(int pv1, int pv2, int pv3) {

	using std::min;

	int npm1 = min(pv1, NVERTEX - 1);
	int np0 = min(pv2, NVERTEX - 1);
	int npp1 = min(pv3, NVERTEX - 1);

	return Weight3D_[npm1][np0][npp1];

}

/**
 * @PARAM ScaleFactor: Scale factor is used to shift target distribution (i.e. luminosity scale)  1. = no shift
 */
void Lumi3DReWeighting::weight3D_init(float ScaleFactor) {

	//create histogram to write output weights, save pain of generating them again...

	boost::scoped_ptr<TH3D> WHist(
			new TH3D("WHist", "3D weights", NVERTEX, -.5, 49.5, NVERTEX, -.5, 49.5, NVERTEX, -.5, 49.5));
	boost::scoped_ptr<TH3D> DHist(
			new TH3D("DHist", "3D weights", NVERTEX, -.5, 49.5, NVERTEX, -.5, 49.5, NVERTEX, -.5, 49.5));
	boost::scoped_ptr<TH3D> MHist(
			new TH3D("MHist", "3D weights", NVERTEX, -.5, 49.5, NVERTEX, -.5, 49.5, NVERTEX, -.5, 49.5));

	using std::min;

	if (MC_distr_->GetEntries() == 0) {
		std::cout << " MC and Data distributions are not initialised! You must call the Lumi3DReWeighting constructor. "
				<< std::endl;
	}

	// arrays for storing number of interactions

	double MC_ints[NVERTEX][NVERTEX][NVERTEX];
	double Data_ints[NVERTEX][NVERTEX][NVERTEX];

	for (int i = 0; i < NVERTEX; i++) {
		for (int j = 0; j < NVERTEX; j++) {
			for (int k = 0; k < NVERTEX; k++) {
				MC_ints[i][j][k] = 0.;
				Data_ints[i][j][k] = 0.;
			}
		}
	}

	double factorial[NVERTEX];
	double PowerSer[NVERTEX];
	double base = 1.;

	factorial[0] = 1.;
	PowerSer[0] = 1.;

	for (int i = 1; i < NVERTEX + 1; ++i) {
		base = base * float(i);
		factorial[i] = base;
	}

	double x;
	double xweight;
	double probi, probj, probk;
	double Expval, mean;
	int xi;

	// Get entries for Data, MC, fill arrays:

	int NMCbin = MC_distr_->GetNbinsX();

	for (int jbin = 1; jbin < NMCbin + 1; jbin++) {
		x = MC_distr_->GetBinCenter(jbin);
		xweight = MC_distr_->GetBinContent(jbin); //use as weight for matrix

		//for Summer 11, we have this int feature:
		xi = int(x);

		// Generate Poisson distribution for each value of the mean

		mean = double(xi);

		if (mean < 0.) {
//      throw cms::Exception("BadInputValue") << " Your histogram generates MC luminosity values less than zero!"
//						<< " Please Check.  Terminating." << std::endl;
		}

		if (mean == 0.) {
			Expval = 1.;
		} else {
			Expval = exp(-1. * mean);
		}

		base = 1.;

		for (int i = 1; i < NVERTEX; ++i) {
			base = base * mean;
			PowerSer[i] = base; // PowerSer is mean^i
		}

		// compute poisson probability for each Nvtx in weight matrix

		for (int i = 0; i < NVERTEX; i++) {
			probi = PowerSer[i] / factorial[i] * Expval;
			for (int j = 0; j < NVERTEX; j++) {
				probj = PowerSer[j] / factorial[j] * Expval;
				for (int k = 0; k < NVERTEX; k++) {
					probk = PowerSer[k] / factorial[k] * Expval;
					// joint probability is product of event weights multiplied by weight of input distribution bin
					MC_ints[i][j][k] = MC_ints[i][j][k] + probi * probj * probk * xweight;
				}
			}
		}

	}

	int NDatabin = Data_distr_->GetNbinsX();

	for (int jbin = 1; jbin < NDatabin + 1; jbin++) {
		mean = (Data_distr_->GetBinCenter(jbin)) * ScaleFactor;
		xweight = Data_distr_->GetBinContent(jbin);

		// Generate poisson distribution for each value of the mean

		if (mean < 0.) {
//      throw cms::Exception("BadInputValue") << " Your histogram generates Data luminosity values less than zero!"
//						<< " Please Check.  Terminating." << std::endl;
		}

		if (mean == 0.) {
			Expval = 1.;
		} else {
			Expval = exp(-1. * mean);
		}

		base = 1.;

		for (int i = 1; i < NVERTEX; ++i) {
			base = base * mean;
			PowerSer[i] = base;
		}

		// compute poisson probability for each Nvtx in weight matrix

		for (int i = 0; i < NVERTEX; i++) {
			probi = PowerSer[i] / factorial[i] * Expval;
			for (int j = 0; j < NVERTEX; j++) {
				probj = PowerSer[j] / factorial[j] * Expval;
				for (int k = 0; k < NVERTEX; k++) {
					probk = PowerSer[k] / factorial[k] * Expval;
					// joint probability is product of event weights multiplied by weight of input distribution bin
					Data_ints[i][j][k] = Data_ints[i][j][k] + probi * probj * probk * xweight;
				}
			}
		}

	}

	for (int i = 0; i < NVERTEX; i++) {
		//if(i<5) std::cout << "i = " << i << std::endl;
		for (int j = 0; j < NVERTEX; j++) {
			for (int k = 0; k < NVERTEX; k++) {
				if ((MC_ints[i][j][k]) > 0.) {
					Weight3D_[i][j][k] = Data_ints[i][j][k] / MC_ints[i][j][k];
				} else {
					Weight3D_[i][j][k] = 0.;
				}
				WHist->SetBinContent(i + 1, j + 1, k + 1, Weight3D_[i][j][k]);
				DHist->SetBinContent(i + 1, j + 1, k + 1, Data_ints[i][j][k]);
				MHist->SetBinContent(i + 1, j + 1, k + 1, MC_ints[i][j][k]);
				//	if(i<5 && j<5 && k<5) std::cout << Weight3D_[i][j][k] << " " ;
			}
			//      if(i<5 && j<5) std::cout << std::endl;
		}
	}

	std::cout << " 3D Weight Matrix initialized! " << std::endl;
	std::cout << " Writing weights to file Weight3D.root for re-use...  " << std::endl;

	boost::scoped_ptr<TFile> outfile(new TFile("Weight3D.root", "RECREATE"));
	outfile->cd();
	WHist->Write();
	MHist->Write();
	DHist->Write();
	outfile->Write();
	outfile->Close();
//	outfile->Delete();

	return;

}

void Lumi3DReWeighting::weight3D_init(std::string WeightFileName) {

	boost::scoped_ptr<TFile> infile(new TFile(WeightFileName.c_str()));
	boost::scoped_ptr<TH3D> WHist((TH3D*) infile->Get("WHist"));

	// Check if the histogram exists
	if (!WHist) {
//    throw cms::Exception("HistogramNotFound") << " Could not find the histogram WHist in the file "
//					      << "in the file " << WeightFileName << "." << std::endl;
	}

	for (int i = 0; i < NVERTEX; i++) {
		for (int j = 0; j < NVERTEX; j++) {
			for (int k = 0; k < NVERTEX; k++) {
				Weight3D_[i][j][k] = WHist->GetBinContent(i + 1, j + 1, k + 1);
			}
		}
	}

	std::cout << " 3D Weight Matrix initialized! " << std::endl;

	return;

}

void Lumi3DReWeighting::weight3D_init(std::string MCWeightFileName, std::string DataWeightFileName) {

	boost::scoped_ptr<TFile> infileMC(new TFile(MCWeightFileName.c_str()));
	boost::scoped_ptr<TH3D> MHist((TH3D*) infileMC->Get("MHist"));

	// Check if the histogram exists
	if (!MHist) {
//    throw cms::Exception("HistogramNotFound") << " Could not find the histogram MHist in the file "
//					      << "in the file " << MCWeightFileName << "." << std::endl;
	}

	boost::scoped_ptr<TFile> infileD(new TFile(DataWeightFileName.c_str()));
	boost::scoped_ptr<TH3D> DHist((TH3D*) infileD->Get("DHist"));

	// Check if the histogram exists
	if (!DHist) {
//    throw cms::Exception("HistogramNotFound") << " Could not find the histogram DHist in the file "
//					      << "in the file " << DataWeightFileName << "." << std::endl;
	}

	for (int i = 0; i < NVERTEX; i++) {
		for (int j = 0; j < NVERTEX; j++) {
			for (int k = 0; k < NVERTEX; k++) {
				Weight3D_[i][j][k] = DHist->GetBinContent(i + 1, j + 1, k + 1)
						/ MHist->GetBinContent(i + 1, j + 1, k + 1);
			}
		}
	}

	std::cout << " 3D Weight Matrix initialized! " << std::endl;

	return;

}

} /* namespace BAT */
