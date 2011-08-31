/*
 * TrackReader.cpp
 *
 *  Created on: 20 Sep 2010
 *      Author: kreczko
 */

#include "../../interface/Readers/TrackReader.h"
namespace BAT {

TrackReader::TrackReader() :
    phiReader(),
    etaReader(),
    ptReader(),
    thetaReader(),
    chargeReader(),
    d0Reader(),
    highPurityReader() {

}

TrackReader::TrackReader(TChainPointer input) :
    phiReader(input, "Track.Phi"),
    etaReader(input, "Track.Eta"),
    ptReader(input, "Track.Pt"),
    thetaReader(input, "Track.Theta"),
    chargeReader(input, "Track.Charge"),
    d0Reader(input, "Track.D0"),
    highPurityReader(input, "Track.isHighPurity") {

}

TrackReader::~TrackReader() {
}

const TrackCollection& TrackReader::getTracks() {
    if (tracks.empty() == false)
        tracks.clear();
    readTracks();
    return tracks;
}

void TrackReader::readTracks() {
    for (unsigned int index = 0; index < phiReader.size(); index++) {
    	double phi = phiReader.getVariableAt(index);
    	double eta = etaReader.getVariableAt(index);
    	double pt = ptReader.getVariableAt(index);
    	double theta = thetaReader.getVariableAt(index);
        TrackPointer track(new Track(phi, eta, pt, theta));
        track->setCharge(chargeReader.getIntVariableAt(index));
        track->setD0(d0Reader.getVariableAt(index));
        track ->setHighPurity(highPurityReader.getBoolVariableAt(index) > 0);

        tracks.push_back(track);
    }
}

void TrackReader::initialise() {
    phiReader.initialise();
    etaReader.initialise();
    ptReader.initialise();
    thetaReader.initialise();
    chargeReader.initialise();
    d0Reader.initialise();
    highPurityReader.initialise();
}

}
