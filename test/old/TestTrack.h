/*
 * TestTrack.h
 *
 *  Created on: 19 Sep 2010
 *      Author: kreczko
 */

#ifndef TESTTRACK_H_
#define TESTTRACK_H_
#include "cute/cute.h"
#include "cute/cute_suite.h"
#include "../interface/RecoObjects/Track.h"

using namespace BAT;

struct TestTrack {
private:
    ParticlePointer particle1, particle2;
    TrackPointer track1, track2;
public:
    TestTrack();
    void testCurvatureWithSTDBField();
    void testCurvatureWithCustomBField();
    void testRadiusWithSTDBField();
    void testRadiusWithCustomBField();
    void testXWithSTDBField();
    void testXWithCustomBField();
    void testYWithSTDBField();
    void testYWithCustomBField();
    void testDeltaCotThetaWithSTDBField();
    void testDistanceWithSTDBfield();
    void testDistanceWithCustomBfield();
    void testEta();
    void testPhi();
    void testPt();
    void testTheta();
    void testCustomEta();
    void testCustomPhi();
    void testCustomPt();
    void testCustomTheta();
    void testXNCharge();
    void testYNCharge();
    void testXPCharge();
    void testYPCharge();
};

extern cute::suite make_suite_TestTrack();
#endif /* TESTTRACK_H_ */
