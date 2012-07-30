/*
 * METBinnedElectronAnalyser.h
 *
 *  Created on: 3 Jul 2012
 *      Author: kreczko
 */

#ifndef METBINNEDELECTRONANALYSER_H_
#define METBINNEDELECTRONANALYSER_H_

#include "BasicAnalyser.h"

namespace BAT {

class METBinnedElectronAnalyser: public BAT::BasicAnalyser {
public:
	METBinnedElectronAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "METBinnedElectronAnalyser");
	virtual ~METBinnedElectronAnalyser();
};

} /* namespace BAT */
#endif /* METBINNEDELECTRONANALYSER_H_ */
