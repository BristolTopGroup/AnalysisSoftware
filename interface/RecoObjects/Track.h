/*
 * Track.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef TRACK_H_
#define TRACK_H_
#include "PseudoParticle.h"
#include <boost/shared_ptr.hpp>
#include <vector>

namespace BAT {
class Track;
typedef boost::shared_ptr<Track> TrackPointer;
typedef std::vector<TrackPointer> TrackCollection;

class Track : public PseudoParticle{
private:
    static const double BFIELD;
    bool highPurity;
public:
    Track();
    Track(const Track& other);
    Track(double phi, double eta, double pt, double theta);
    virtual ~Track();
    double curvature(double Bfield = BFIELD) const;
    double distance(const TrackPointer otherTrack, double Bfield = BFIELD) const;
    double deltaCotTheta(const TrackPointer otherTrack) const;
    double x(double Bfield = BFIELD) const;
    double y(double Bfield = BFIELD) const;
    double radius(double Bfield = BFIELD) const;
    void setHighPurity(bool isPure);
    bool isHighPurity() const;
};

}

#endif /* TRACK_H_ */
