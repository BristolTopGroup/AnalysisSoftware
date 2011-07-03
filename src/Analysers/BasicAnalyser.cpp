/*
 * BasicAnalyser.cpp
 *
 *  Created on: 3 Jul 2011
 *      Author: kreczko
 */

#include "../../interface/Analysers/BasicAnalyser.h"

namespace BAT {
BasicAnalyser::BasicAnalyser(boost::shared_ptr<HistogramManager> histMan):
        histMan(histMan){

}

BasicAnalyser::~BasicAnalyser(){

}
}
