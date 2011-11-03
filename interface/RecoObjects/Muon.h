/*
 * Muon.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef MUON_H_
#define MUON_H_
#include "Lepton.h"
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


class Muon: public Lepton {
public:
	Muon();
	Muon(double energy, double px, double py, double pz);
	virtual ~Muon();
	bool isGood(short leptonID = 0) const;
	bool isLoose() const;
	bool isGlobal() const;
	void setUsedAlgorithm(MuonAlgorithm::value algorithm);
	bool isPFLepton() const;

	void makeGlobal(bool global);
	void setTrackerMuon(bool isTrackerMuon);
	void setNormalisedChi2(double normChi2);
	void setNumberOfValidHits(int nValidHits);
	void setNumberOfValidMuonHits(int nValidHits);
	void setPixelLayersWithMeasurement(int pixelLayers);
	void setNumberOfMatchedStations(int nMatchedStations);
	void setNumberOfMatches(int nMatches);

private:
	MuonAlgorithm::value usedAlgorithm;
	bool is_GlobalMuon, is_TrackerMuon;
	double normalisedChi2;
	int numberOfValidMuonHits, numberOfValidHits, pixelLayersWithMeasurement;
	int numberOfMatches, numberOfMatchedStations;
};

typedef boost::shared_ptr<Muon> MuonPointer;
typedef std::vector<MuonPointer> MuonCollection;
}

#endif /* MUON_H_ */
