/*
 * JetAnalyser.cpp
 *
 *  Created on: 1 Nov 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/JetAnalyser.h"

namespace BAT {

JetAnalyser::JetAnalyser(boost::shared_ptr<HistogramManager> histMan):
	BasicAnalyser(histMan) {

}

JetAnalyser::~JetAnalyser() {
}

} /* namespace BAT */
