/*
 * THCollection.cpp
 *
 *  Created on: 9 Aug 2010
 *      Author: kreczko
 */

#include "../../interface/HistHelpers/THCollection.h"

namespace BAT {
template<>
void THCollection<TH1>::add(std::string name, std::string title, unsigned int numberOfBins, float xmin, float xmax) {
	histMap[name] = boost::shared_ptr<TH1>(new TH1D(name.c_str(), title.c_str(), numberOfBins, xmin, xmax));
	histMap[name]->Sumw2();
}

template<>
void THCollection<TH1>::add(std::string name, std::string title, unsigned int numberOfBins, float *xbins) {
	histMap[name] = boost::shared_ptr<TH1>(new TH1D(name.c_str(), title.c_str(), numberOfBins, xbins));
	histMap[name]->Sumw2();
}

template<>
void THCollection<TH2>::add(std::string name, std::string title, unsigned int numberOfXBins, float xmin, float xmax,
		unsigned int numberOfYBins, float ymin, float ymax) {
	histMap[name] = boost::shared_ptr<TH2>(
			new TH2D(name.c_str(), title.c_str(), numberOfXBins, xmin, xmax, numberOfYBins, ymin, ymax));
	histMap[name]->Sumw2();
}

template<>
void THCollection<TH3>::add(std::string name, std::string title, unsigned int numberOfXBins, float xmin, float xmax,
		unsigned int numberOfYBins, float ymin, float ymax, unsigned int numberOfZBins, float zmin, float zmax) {
	histMap[name] = boost::shared_ptr<TH3>(
			new TH3D(name.c_str(), title.c_str(), numberOfXBins, xmin, xmax, numberOfYBins, ymin, ymax, numberOfZBins,
					zmin, zmax));
	histMap[name]->Sumw2();
}
}
