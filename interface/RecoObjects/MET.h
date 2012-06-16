/*
 * MET.h
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#ifndef MET_H_
#define MET_H_
#include "Particle.h"
#include <boost/shared_ptr.hpp>
#include <string>
#include <boost/array.hpp>

namespace BAT {

namespace METAlgorithm {
enum value {
	patMETsPFlow,
	patType1CorrectedPFMet,
	patType1p2CorrectedPFMet,
	patType1p2CorrectedPFMetElectronEnUp,
	patType1p2CorrectedPFMetElectronEnDown,
	patType1p2CorrectedPFMetMuonEnUp,
	patType1p2CorrectedPFMetMuonEnDown,
	patType1p2CorrectedPFMetTauEnUp,
	patType1p2CorrectedPFMetTauEnDown,
	patType1p2CorrectedPFMetJetResUp,
	patType1p2CorrectedPFMetJetResDown,
	patType1p2CorrectedPFMetJetEnUp,
	patType1p2CorrectedPFMetJetEnDown,
	patType1p2CorrectedPFMetUnclusteredEnUp,
	patType1p2CorrectedPFMetUnclusteredEnDown,
	NUMBER_OF_METALGORITHMS
};

const boost::array<std::string, METAlgorithm::NUMBER_OF_METALGORITHMS> prefixes = { {
//MET names as stored in the nTuples
		"patMETsPFlow",//
		"patType1CorrectedPFMet", //
		"patType1p2CorrectedPFMet", //
		"patType1p2CorrectedPFMetElectronEnUp", //
		"patType1p2CorrectedPFMetElectronEnDown", //
		"patType1p2CorrectedPFMetMuonEnUp", //
		"patType1p2CorrectedPFMetMuonEnDown", //
		"patType1p2CorrectedPFMetTauEnUp", //
		"patType1p2CorrectedPFMetTauEnDown", //
		"patType1p2CorrectedPFMetJetResUp", //
		"patType1p2CorrectedPFMetJetResDown", //
		"patType1p2CorrectedPFMetJetEnUp", //
		"patType1p2CorrectedPFMetJetEnDown", //
		"patType1p2CorrectedPFMetUnclusteredEnUp", //
		"patType1p2CorrectedPFMetUnclusteredEnDown"

} };

const boost::array<std::string, METAlgorithm::NUMBER_OF_METALGORITHMS> names = { {
//MET names as used in output file for the used (central) MET
		"PFMET",//
		"patType1CorrectedPFMet", //
		"patType1p2CorrectedPFMet", //
		"patType1p2CorrectedPFMetElectronEnUp", //
		"patType1p2CorrectedPFMetElectronEnDown", //
		"patType1p2CorrectedPFMetMuonEnUp", //
		"patType1p2CorrectedPFMetMuonEnDown", //
		"patType1p2CorrectedPFMetTauEnUp", //
		"patType1p2CorrectedPFMetTauEnDown", //
		"patType1p2CorrectedPFMetJetResUp", //
		"patType1p2CorrectedPFMetJetResDown", //
		"patType1p2CorrectedPFMetJetEnUp", //
		"patType1p2CorrectedPFMetJetEnDown", //
		"patType1p2CorrectedPFMetUnclusteredEnUp", //
		"patType1p2CorrectedPFMetUnclusteredEnDown"
} };
}

class MET: public Particle {
public:
	MET();
	MET(double ex, double ey);
	virtual ~MET();
	bool isGood() const;

	METAlgorithm::value getUsedAlgorithm() const;
	double significance() const;
	double sumET() const;
	double resolution() const;

	void setUsedAlgorithm(METAlgorithm::value algo);
	void setSignificance(double significance);
	void setSumET(double sumET);
private:
	METAlgorithm::value usedAlgorithm;
	double significance_, sumET_;
};

typedef boost::shared_ptr<MET> METPointer;
typedef std::vector<METPointer> METCollection;

}

#endif /* MET_H_ */
