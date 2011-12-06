/*
 * Lumi3DReWeighting.h
 *
 *  Created on: 5 Dec 2011
 *      Author: kreczko
 */

#ifndef LUMI3DREWEIGHTING_H_
#define LUMI3DREWEIGHTING_H_
#include "TH1.h"
#include "TFile.h"
#include <cmath>
#include <string>
#include <boost/shared_ptr.hpp>
#include <vector>
#define NVERTEX 50

namespace BAT {

class Lumi3DReWeighting {
public:
	Lumi3DReWeighting(std::string generatedFile, std::string dataFile, std::string GenHistName,
			std::string DataHistName);

	Lumi3DReWeighting(std::vector<float> MC_distr, std::vector<float> Lumi_distr);

	double weight3D(int, int, int);

	void weight3D_set(std::string generatedFile, std::string dataFile, std::string GenHistName,
			std::string DataHistName);

	void weight3D_init(float Scale);

	void weight3D_init(std::string WeightFileName); // initialize from root file

	void weight3D_init(std::string MCFileName, std::string DataFileName); // initialize from root files

protected:

	std::string generatedFileName_;
	std::string dataFileName_;
	std::string GenHistName_;
	std::string DataHistName_;
	boost::shared_ptr<TFile> generatedFile_;
	boost::shared_ptr<TFile> dataFile_;
	boost::shared_ptr<TH1> weights_;

	//keep copies of normalized distributions:

	boost::shared_ptr<TH1> MC_distr_;
	boost::shared_ptr<TH1> Data_distr_;

	double Weight3D_[NVERTEX][NVERTEX][NVERTEX];

};
}

/* namespace BAT */
#endif /* LUMI3DREWEIGHTING_H_ */
