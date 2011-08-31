/*
 * Track.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Track.h"

namespace BAT {
const double Track::BFIELD = 3.8;

Track::Track() :
    PseudoParticle(), highPurity(false) {

}

Track::Track(const Track& track) :
    PseudoParticle((PseudoParticle) track), highPurity(track.highPurity) {

}

Track::Track(double phi, double eta, double pt, double theta) :
    PseudoParticle(phi, eta, pt, theta), highPurity(false) {

}

Track::~Track() {
}

double Track::curvature(double BField) const {
    return -0.3 * BField * charge() / pt() / 100.;
}

double Track::radius(double Bfield) const {
    return fabs(1 / curvature(Bfield));
}

double Track::x(double Bfield) const {
    return (1/curvature(Bfield) - d0()) * cos(phi());
}

double Track::y(double Bfield) const {
    return (1/curvature(Bfield) - d0()) * sin(phi());
}

double Track::deltaCotTheta(const TrackPointer otherTrack) const {
    return fabs(1 / tan(theta()) - 1 / tan(otherTrack->theta()));
}

double Track::distance(const TrackPointer otherTrack, double BField) const {
    double dx = x(BField) - otherTrack->x(BField);
    double dy = y(BField) - otherTrack->y(BField);
    double dist = sqrt(pow(dx, 2) + pow(dy, 2));
    return fabs(dist - (radius(BField) + otherTrack->radius(BField)));
}

void Track::setHighPurity(bool isPure) {
    highPurity = isPure;
}

bool Track::isHighPurity() const {
    return highPurity;
}

}
