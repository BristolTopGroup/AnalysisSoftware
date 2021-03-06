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

class MET;

namespace METCorrections {
	enum value {
		// pfMetSysShiftCorrections,
		// pfMetType0Corrections,
		// pfMetType1Corrections,
		NUMBER_OF_METCORRECTIONS
	};

	const boost::array<std::string, METCorrections::NUMBER_OF_METCORRECTIONS> prefixes = {
			//MET correction names as stored in the nTuples
					// "pfMetSysShiftCorrections", //
					// "pfMetType0Corrections", //
					// "pfMetType1Corrections"  
	};
}

namespace METAlgorithm {
	enum value {

		// 2015 miniAOD
		MET,
		// METNoHF,
		NUMBER_OF_METALGORITHMS
	};

	const boost::array<std::string, METAlgorithm::NUMBER_OF_METALGORITHMS> prefixes = { {

		// 2015 miniAOD
		"MET",
		// "METNoHF"

	} };

	const boost::array<std::string, METAlgorithm::NUMBER_OF_METALGORITHMS> names = { {
		// 2015 miniAOD
		"MET",
		// "METNoHF"
	} };
}

	typedef boost::shared_ptr<MET> METPointer;
	typedef std::vector<METPointer> METCollection;

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
		static bool isMCOnlyMETType(unsigned int type);
		static bool isAvailableInNTupleVersion(unsigned int ntupleVersion, unsigned int type);

		void setMETUncertinaties( std::vector<double> newUncertainties );
		void setMET_Px_Uncertinaties( std::vector<double> newUncertainties );
		void setMET_Py_Uncertinaties( std::vector<double> newUncertainties );
		std::vector< double > getAllMETUncertainties( );
		BAT::METPointer getMETForUncertainty( unsigned int unc_i );
	private:
		METAlgorithm::value usedAlgorithm;
		double significance_, sumET_;

		std::vector< double > metUncertaintyPt_, metUncertaintyPx_, metUncertaintyPy_;
	};

	}

#endif /* MET_H_ */
