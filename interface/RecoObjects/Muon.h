/*
 * Muon.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef MUON_H_
#define MUON_H_
#include "Particle.h"
#include <vector>
#include <string>
#include <boost/array.hpp>

namespace BAT {

namespace MuonAlgorithm {
enum value {
    Default,
    ParticleFlow,
    NUMBER_OF_MUONALGORITHMS
};

const boost::array<std::string, MuonAlgorithm::NUMBER_OF_MUONALGORITHMS> prefixes = { {
        "selectedPatMuons",
        "selectedPatMuonsLoosePFlow"  } };

const boost::array<std::string, MuonAlgorithm::NUMBER_OF_MUONALGORITHMS> names = { {
        "Muon",
        "PFMuon"} };
}


class Muon: public Particle {
public:
	Muon();
	Muon(float energy, float px, float py, float pz);
	virtual ~Muon();
	bool isGood() const;
	bool isIsolated() const;
	bool isGlobal() const;
	float ecalIsolation() const;
	float hcalIsolation() const;
	float trackerIsolation() const;
	float relativeIsolation() const;

	void makeGlobal(bool global);
	void setEcalIsolation(float isolation);
	void setHcalIsolation(float isolation);
	void setTrackerIsolation(float isolation);
private:
	bool is_Global;
	float ecal_Isolation, hcal_Isolation, tracker_Isolation;
};

typedef boost::shared_ptr<Muon> MuonPointer;
typedef std::vector<MuonPointer> MuonCollection;
}

#endif /* MUON_H_ */
