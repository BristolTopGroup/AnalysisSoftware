/*
 * QCDAnalyser.h
 *
 *  Created on: Jun 28, 2011
 *      Author: lkreczko
 */

#ifndef QCDANALYSER_H_
#define QCDANALYSER_H_
#include <boost/shared_ptr.hpp>
#include "../../interface/TopPairEventCandidate.h"
#include "../../interface/HistHelpers/HistogramManager.h"

namespace BAT {

class QCDAnalyser {
public:
	QCDAnalyser(boost::shared_ptr<HistogramManager> histMan);
	~QCDAnalyser();

	void analyse(TopPairEventCandidate& Event);
	void createHistograms();

private:
	boost::shared_ptr<HistogramManager> histMan;
};

}

#endif /* QCDANALYSER_H_ */
