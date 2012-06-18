/*
 * ABCDAnalyser.h
 *
 *  Created on: 13th May 2012
 *      Author: J. Jacob
 */

#ifndef ABCDMethodANALYSER_H_
#define ABCDMethodANALYSER_H_
#include "BasicAnalyser.h"
#include "../Selections/QCDNoIsoNoIDSelection.h"
#include <boost/scoped_ptr.hpp>

namespace BAT {

//create class ABCDMethodAnalyser which inherits from BasicAnalyser
class ABCDMethodAnalyser: public BasicAnalyser {

//constructor; takes as arguments: the histogram manager and histogram folder previously created.
public:
	ABCDMethodAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "ABCDMethodAnalysis");
	virtual ~ABCDMethodAnalyser();
	//analyse function: takes as arguments: constant event pointer to a particular event and reads its content. Carried out once for each event.
	void analyse(const EventPtr);
	//createHistograms function: function to create histograms folder and the histograms themselves.
	void createHistograms();

private:
	//signal selections
	QCDNoIsoNoIDSelectionPointer qcdNoIsoNoIDSelection_;
};

typedef boost::scoped_ptr<ABCDMethodAnalyser> ABCDMethodAnalyserLocalPtr;
}

#endif /* ABCDMethodANALYSER_H_ */
