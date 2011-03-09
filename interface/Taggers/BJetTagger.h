/*
 * BJetTagger.h
 *
 *  Created on: Jun 30, 2010
 *      Author: lkreczko
 */

#ifndef BJETTAGGER_H_
#define BJETTAGGER_H_

#include "../Constants.h"

namespace BAT {

namespace BJetTagger {
enum Algorithm {
	NONE, FAKE, TrackCountingHighEfficiency, TrackCountingHighPurity,
	//		JetBProbability,
	//		JetProbability,
	SimpleSecondaryVertex,
	//		SoftElectron,
	//		SoftMuon,
	//		SoftMuonNoIP,
	NUMBER_OF_BTAGALGORITHMS
};
const float simpleSecondaryVertexHighEfficiencyMediumCut = 1.7;

//	BJetTagger();
//	virtual ~BJetTagger();
bool doesDiscriminatorPassBtagOfType(float discriminator, Algorithm type);

}
}

#endif /* BJETTAGGER_H_ */
