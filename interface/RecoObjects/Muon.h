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

	//getters
	bool isPFLepton() const;
	void setUsedAlgorithm(MuonAlgorithm::value algorithm);
	bool isGlobal() const;
	bool isTracker() const;
	bool isPFMuon() const;//PF ID
	double normChi2() const;
	int numberOfValidMuonHits() const;
	int numberOfValidPixelHits() const;
	int numberOfValidHits() const;
	int pixelLayersWithMeasurement() const;
	int trackerLayersWithMeasurement() const;
	int numberOfMatches() const;
	int numberOfMatchedStations() const;

	//setters
	void makeGlobal(bool global);
	void setTrackerMuon(bool isTrackerMuon);
	void setPFMuon(bool pf);
	void setNormalisedChi2(double normChi2);
	void setNumberOfValidHits(int nValidHits);
	void setNumberOfValidMuonHits(int nValidHits);
	void setNumberOfValidPixelHits(int nValidHits);
	void setPixelLayersWithMeasurement(int layers);
	void setTrackerLayersWithMeasurement(int layers);
	void setNumberOfMatchedStations(int nMatchedStations);
	void setNumberOfMatches(int nMatches);

	//other
	double getEfficiencyCorrection(bool qcd) const;

private:
	MuonAlgorithm::value usedAlgorithm_;
	bool isGlobalMuon_, isTrackerMuon_, isPFMuon_;
	double normalisedChi2_;
	int numberOfValidMuonHits_, numberOfValidPixelHits_, numberOfValidHits_, pixelLayersWithMeasurement_, trackerLayersWithMeasurement_;
	int numberOfMatches_, numberOfMatchedStations_;
};

typedef boost::shared_ptr<Muon> MuonPointer;
typedef std::vector<MuonPointer> MuonCollection;
}

#endif /* MUON_H_ */
