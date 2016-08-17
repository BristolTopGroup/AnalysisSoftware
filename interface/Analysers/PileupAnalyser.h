/*
 * PileupAnalyser.h
 *
 *  Created on: 16 October 2015
 *      Author: ejclemen
 */

#ifndef PILEUPANALYSER_H_
#define PILEUPANALYSER_H_
#include "BasicAnalyser.h"
#include <boost/scoped_ptr.hpp>

namespace BAT {

	class PileupAnalyser: public BasicAnalyser {

		public:
			PileupAnalyser(TreeManagerPtr treeMan, std::string histogramFolder = "PileupAnalysericiencies");
			virtual ~PileupAnalyser();

			void analyse(const EventPtr);
			void createTrees();

		};

	typedef boost::scoped_ptr<PileupAnalyser> PileupAnalyserLocalPtr;
}

#endif /* PILEUPANALYSER_H_ */