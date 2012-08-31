/*
 * METBinnedElectronAnalyser.h
 *
 *  Created on: 3 Jul 2012
 *      Author: kreczko
 */

#ifndef METBINNEDELECTRONANALYSER_H_
#define METBINNEDELECTRONANALYSER_H_

#include "BasicAnalyser.h"
#include <vector>

namespace BAT {

class METBinnedElectronAnalyser: public BAT::BasicAnalyser {
public:
	METBinnedElectronAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "METBinnedElectronAnalyser");
	virtual ~METBinnedElectronAnalyser();
	void setMETbins(std::vector<double> metbins);

private:
	std::vector<double> metbins_;
};

} /* namespace BAT */
#endif /* METBINNEDELECTRONANALYSER_H_ */
