/*
 * ConversionTagger.cpp
 *
 *  Created on: Jun 30, 2010
 *      Author: lkreczko
 *
 *      The nTuples in 2011 don't have the necessary information to calculate these things on the fly
 */

#include "../../interface/Taggers/ConversionTagger.h"
#include <iostream>

using namespace std;
namespace BAT {

ConversionTagger::ConversionTagger() :
    electron(), tracks(), partnerTrack(), electronTrack(), minFracSharedHits(99999), distance(-9999), deltaCot(-9999), electronTrackID(
            -999) {

}

ConversionTagger::~ConversionTagger() {
}

bool ConversionTagger::isFromConversion(double maxAbsDistance, double maxAbsDeltaCot) {
    return fabs(distance) < maxAbsDistance && fabs(deltaCot) < maxAbsDeltaCot;
}

void ConversionTagger::calculateConversionVariables(const ElectronPointer electron, const TrackCollection tracks,
        const double bfieldAtOrigin = 3.8, const double minFracSharedHits = 0.45) {
    this->electron = electron;
    this->tracks = tracks;
    this->minFracSharedHits = minFracSharedHits;
    electronTrackID = -999;
    distance = -9999;
    deltaCot = -9999;

    electronTrack = getElectronTrack();

    findClosestTrack();
    if (partnerTrack.get() != 0 && electronTrack != 0) {
        distance = partnerTrack->distance(electronTrack);
        deltaCot = partnerTrack->deltaCotTheta(electronTrack);
    }
}

const TrackPointer ConversionTagger::getElectronTrack() {
    if (electron->closestCTFTrackID() >= 0 && electron->shFracInnerLayer() > minFracSharedHits) {
        electronTrackID = electron->closestCTFTrackID();
        return tracks.at(electronTrackID);
    }

    else
        return electron->GSFTrack();
}

void ConversionTagger::findClosestTrack() {
    if(electronTrack.get() == 0)
        return;

    double mindDcot = 99999.;
    for (unsigned int trackIndex = 0; trackIndex < tracks.size(); ++trackIndex) {
        if (abs(electronTrackID) - trackIndex == 0)
            continue;

        const TrackPointer trackCandidate = tracks.at(trackIndex);
        if (electronTrack->isWithinDeltaR(0.5, trackCandidate) == false)
            continue;

        if (electronTrack->charge() + trackCandidate->charge() != 0)
            continue;

        double dcot = trackCandidate->deltaCotTheta(electronTrack);
        if (dcot < mindDcot) {
            mindDcot = dcot;
            partnerTrack = trackCandidate;
        }
    }
}
}
