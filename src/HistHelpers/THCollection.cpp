/*
 * THCollection.cpp
 *
 *  Created on: 9 Aug 2010
 *      Author: kreczko
 */

#include "../../interface/HistHelpers/THCollection.h"

namespace BAT {
template<>
void THCollection<TH1>::add(std::string name, std::string title, unsigned int numberOfBins, float xmin, float xmax) {
    histMap[name] = boost::shared_ptr<TH1>(new TH1F(name.c_str(), title.c_str(), numberOfBins, xmin, xmax));
}

template<>
void THCollection<TH2>::add(std::string name, std::string title, unsigned int numberOfXBins, float xmin, float xmax,
        unsigned int numberOfYBins, float ymin, float ymax) {
    histMap[name] = boost::shared_ptr<TH2>(new TH2F(name.c_str(), title.c_str(), numberOfXBins, xmin, xmax,
            numberOfYBins, ymin, ymax));
}
}
