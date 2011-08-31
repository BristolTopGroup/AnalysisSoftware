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
	Muon(double energy, double px, double py, double pz);
	virtual ~Muon();
	bool isGood() const;
	bool isIsolated() const;
	bool isGlobal() const;
	double ecalIsolation() const;
	double hcalIsolation() const;
	double trackerIsolation() const;
	double relativeIsolation() const;

	void makeGlobal(bool global);
	void setEcalIsolation(double isolation);
	void setHcalIsolation(double isolation);
	void setTrackerIsolation(double isolation);
private:
	bool is_Global;
	double ecal_Isolation, hcal_Isolation, tracker_Isolation;
};

typedef boost::shared_ptr<Muon> MuonPointer;
typedef std::vector<MuonPointer> MuonCollection;
}

#endif /* MUON_H_ */
