/*
 * BJetTagger.cpp
 *
 *  Created on: Jun 30, 2010
 *      Author: lkreczko
 */

#include "../../interface/Taggers/BJetTagger.h"

namespace BAT {
bool BJetTagger::doesDiscriminatorPassBtagOfType(float discriminator, BJetTagger::Algorithm type) {
	switch (type) {
	case BJetTagger::NONE:
		return true;
	case BJetTagger::FAKE:
		return false;
	case BJetTagger::SimpleSecondaryVertex:
		return discriminator > simpleSecondaryVertexHighEfficiencyMediumCut;
	default:
		return false;
	}
}

}
