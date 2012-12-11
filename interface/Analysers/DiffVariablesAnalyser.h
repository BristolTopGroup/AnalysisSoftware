/*
 * DiffVariablesAnalyser.h
 *
 *  Created on: Oct 10, 2012
 *      Author: phzss
 */

#ifndef DIFFVARIABLESANALYSER_H_
#define DIFFVARIABLESANALYSER_H_
#include "BasicAnalyser.h"
#include <boost/scoped_ptr.hpp>
#include "../Selections/BasicSelection.h"

namespace BAT {

class DiffVariablesAnalyser: public BAT::BasicAnalyser {
public:
	DiffVariablesAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "DiffVariablesAnalyser");
	virtual ~DiffVariablesAnalyser();
	void ePlusJetsSignalAnalysis(const EventPtr);
	void muPlusJetsSignalAnalysis(const EventPtr);

    void analyse(const EventPtr);
    void createHistograms();

private:
	//signal selections
	SelectionPointer topEplusJetsRefSelection_, topMuPlusJetsRefSelection_;
};

}



#endif /* DIFFVARIABLESANALYSER_H_ */
