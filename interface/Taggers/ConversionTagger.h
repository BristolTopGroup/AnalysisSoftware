/*
 * ConversionTagger.h
 *
 *  Created on: Jun 30, 2010
 *      Author: lkreczko
 */

#ifndef CONVERSIONTAGGER_H_
#define CONVERSIONTAGGER_H_
#include "../RecoObjects/Electron.h"
#include "../RecoObjects/Track.h"
#include <boost/shared_ptr.hpp>

namespace BAT {

class ConversionTagger {
public:
    ElectronPointer electron;
    TrackCollection tracks;
    TrackPointer partnerTrack;
    TrackPointer electronTrack;
    double minFracSharedHits;
    double distance, deltaCot;
    int electronTrackID;
private:
    void findClosestTrack();
public:
    ConversionTagger();
    virtual ~ConversionTagger();
    void calculateConversionVariables(const ElectronPointer electron, const TrackCollection tracks,
            const double bfieldAtOrigin, double minFracSharedHits);
    const TrackPointer getElectronTrack();
    bool isFromConversion(double maxAbsDistance, double maxAbsDeltaCot);
};

typedef boost::shared_ptr<ConversionTagger> ConversionTaggerPointer;

}

#endif /* CONVERSIONTAGGER_H_ */
