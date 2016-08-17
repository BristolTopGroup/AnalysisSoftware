/*
 * BTagEff.h
 *
 *  Created on: 23 Jul 2015
 *      Author: burns
 */

#ifndef BTAGEFFICIENCYANALYSER_H_
#define BTAGEFFICIENCYANALYSER_H_
#include "BasicAnalyser.h"
#include <boost/scoped_ptr.hpp>

namespace BAT {

	class BTagEff: public BasicAnalyser {

		public:
			BTagEff(TreeManagerPtr treeMan, std::string histogramFolder = "BTagEfficiencies");
			virtual ~BTagEff();

			void analyse(const EventPtr);
			void createTrees();

		};

	typedef boost::scoped_ptr<BTagEff> BTagEffAnalyserLocalPtr;
}

#endif /* BTAGEFFICIENCYANALYSER_H_ */