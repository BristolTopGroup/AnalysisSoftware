/*
 * BasicAnalyser.h
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */

#ifndef BASICANALYSER_H_
#define BASICANALYSER_H_
#include <string>
#include <boost/scoped_ptr.hpp>
#include "../HistHelpers/HistogramManager.h"
#include "../Event.h"

namespace BAT {
/**
 * @brief Interface/abstract class for Analysers.
 *
 * Interface class for Analysers providing basic functionality for other analysers.
 * Stores a pointer to a HistogramManager for Histogram creation and manipulation.
 *
 */
class BasicAnalyser {
protected:
	HistogramManagerPtr histMan_;
	std::string histogramFolder_;
	double weight_;
	unsigned int prescale_;
	double scale_;

public:
	/**
	 * Default constructor for an analyser. Has to take a pointer to a HistogramManager to have access to the histogram pool.
	 * @param histMan Pointer to the histogram manager which organises the histogram-creation and -access.
	 * @param histogramFolder Optional parameter to set the folder in the ROOT file. By default this should be the analyser name.
	 *
	 * Analysers are created at the beginning of the analysis.
	 */
	BasicAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "BasicAnalyser");
	virtual ~BasicAnalyser();
	/**
	 * The main functionality of an analyser is available in the analyse function.
	 * The function takes a constant pointer to an Event and reads the eventcontent.
	 * All changes to the event must be done on a private copy of this event in order to not distort the results of other analysers.
	 * The function is executed once for each event in the analysis.
	 */
	virtual void analyse(const EventPtr) = 0;
	/**
	 * The histogram folder as well as all histograms used by this analyser should be defined by this function
	 * The function is executed on the creation of the analysis.
	 */
	virtual void createHistograms() = 0;

	virtual void setPrescale(unsigned int prescale);
	virtual void setScale(double scale);
};

typedef boost::scoped_ptr<BasicAnalyser> BasicAnalyserLocalPtr;
}

#endif /* BASICANALYSER_H_ */
