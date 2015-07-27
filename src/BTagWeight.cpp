#include "../interface/BTagWeight.h"
#include "../interface/GlobalVariables.h"

#include <functional>
#include <numeric>
#include <boost/scoped_ptr.hpp>
#include <boost/array.hpp>

namespace BAT {

std::vector<double> BjetWeights(const JetCollection jets, unsigned int numberOfBtags) {
	boost::scoped_ptr<BTagWeight> btagWeight(new BTagWeight());


	double event_weight = btagWeight->weight( jets );
	std::vector<double> event_weights;
	for (unsigned int nTag = 0; nTag <= numberOfBtags; ++nTag) { // >= 4 is our last b-tag bin!
		btagWeight->setNumberOfBtags(nTag, 20);
		if ( nTag == numberOfBtags )
			event_weights.push_back(event_weight);
		else
			event_weights.push_back(0.);
	}
	return event_weights;
}

BTagWeight::BTagWeight() :
				minNumberOfTags_(0), //
				maxNumberOfTags_(0) {
}

double BTagWeight::weight(const JetCollection jets) const {
	float bTaggedMCJet = 1.0;
	float nonBTaggedMCJet = 1.0;
	float bTaggedDataJet = 1.0;
	float nonBTaggedDataJet = 1.0;

	float err1 = 0.0;
	float err2 = 0.0;
	float err3 = 0.0;
	float err4 = 0.0;


	for (unsigned int index = 0; index < jets.size(); ++index) {
		// Info on this jet
		const JetPointer jet(jets.at(index));
		const unsigned int partonFlavour = abs( jet->partonFlavour() );
		const bool isBTagged = jet->isBJet();

		// Get scale factor for this jet
		const double sf = jet->getBTagSF( 0 );
		const double sf_up = jet->getBTagSF( 1 );
		const double sf_down = jet->getBTagSF( -1 );

		// For now...improve FIXME
		double sfError = sf_up;
		if (Globals::BJetSystematic == -1) {
			sfError = sf_down;
		}


		// Get efficiency for this jet
		const double eff = getEfficiency( partonFlavour, jet );

		if ( isBTagged ) {

			bTaggedMCJet *= eff;
			bTaggedDataJet *= eff*sf;

			if(partonFlavour==5 || partonFlavour ==4)  err1 += sfError/sf; ///correlated for b/c
			else err3 += sfError/sf; //correlated for light
		}
		else {
			nonBTaggedMCJet *= ( 1 - eff );
			nonBTaggedDataJet *= ( 1 - eff*sf );

			if(partonFlavour==5 || partonFlavour ==4 ) err2 += (-eff*sfError)/(1-eff*sf); /// /correlated for b/c
			else err4 +=  (-eff*sfError)/(1-eff*sf);  ////correlated for light
		}
	}

	double bTagWeight = (nonBTaggedDataJet * bTaggedDataJet) / (nonBTaggedMCJet * bTaggedMCJet);

	double error_BTagWeight = sqrt(pow(err1 + err2, 2) + pow(err3 + err4, 2)) * bTagWeight; ///un-correlated for b/c and light

	if (Globals::BJetSystematic == +1) {
		return bTagWeight + error_BTagWeight;
	} else if (Globals::BJetSystematic == -1) {
		return bTagWeight - error_BTagWeight;
	} else if (Globals::BJetSystematic == 0) {
		return bTagWeight;
	}

	return 0.;
}


double BTagWeight::getEfficiency( const unsigned int partonFlavour, const JetPointer jet ) const {
	if ( partonFlavour == 5) { //b-quark
		return getBEfficiency( jet );
	}
	else if ( partonFlavour == 4) { //c-quark
		return getCEfficiency( jet );
	}
	else if ( partonFlavour != 4 && partonFlavour != 5) { //not a c- or b-quark
		return getUDSGEfficiency( jet );
	}
	else return 0.;
}

// Methods below for 2011 only: https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2011_Data_and_MC
double BTagWeight::getBEfficiency(const JetPointer jet) const {

	std::vector<double> binEdges{ 20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600, 800 };
	const double jetPt = jet->pt();

	if (Globals::energyInTeV == 8) { // 2012

		std::vector<double> eff{
			0.494606375694,
			0.597042381763,
			0.62597990036,
			0.670304358006,
			0.688453614712,
			0.708679795265,
			0.720353424549,
			0.72651040554,
			0.727195739746,
			0.702006459236,
			0.679377436638,
			0.618240535259,
			0.567078948021,
			0.459390848875,
			0.480620145798,
			0.376811593771
		};

		// Which pt bin to use
		for ( unsigned int binIndex=0; binIndex < binEdges.size()-1; binIndex++ ) {
			if ( jetPt >= binEdges[binIndex] && jetPt < binEdges[binIndex+1] )
				return eff[binIndex];
		}
		return eff[eff.size()-1];
	} else if (Globals::energyInTeV == 7) { // 2011
		std::vector<double> eff{
			0.510614693165,
			0.625705659389,
			0.650622546673,
			0.689294397831,
			0.705310404301,
			0.723900854588,
			0.738776385784,
			0.744492590427,
			0.7463555336,
			0.721650719643,
			0.691826879978,
			0.641910970211,
			0.583380103111,
			0.539074957371,
			0.438271611929,
			0.423076927662
		};

		// Which pt bin to use
		for ( unsigned int binIndex=0; binIndex < binEdges.size()-1; binIndex++ ) {
			if ( jetPt >= binEdges[binIndex] && jetPt < binEdges[binIndex+1] )
				return eff[binIndex];
		}
		return eff[eff.size()-1];
	}
	return 0.;
}

double BTagWeight::getCEfficiency(const JetPointer jet) const {
	std::vector<double> binEdges{ 20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600, 800 };
	const double jetPt = jet->pt();

	if (Globals::energyInTeV == 8) { // 2012

		std::vector<double> eff{
			0.137571901083,
			0.17440225184,
			0.168547004461,
			0.190776929259,
			0.186730355024,
			0.201694756746,
			0.211470022798,
			0.208002716303,
			0.19966648519,
			0.172328293324,
			0.174669444561,
			0.148199439049,
			0.115044251084,
			0.140540540218,
			0.0810810774565,
			0.0909090936184
		};

		// Which pt bin to use
		for ( unsigned int binIndex=0; binIndex < binEdges.size()-1; binIndex++ ) {
			if ( jetPt >= binEdges[binIndex] && jetPt < binEdges[binIndex+1] )
				return eff[binIndex];
		}
		return eff[eff.size()-1];
	} else if (Globals::energyInTeV == 7) { // 2011
		std::vector<double> eff{
			0.147453084588,
			0.191264390945,
			0.188042387366,
			0.202001750469,
			0.206677630544,
			0.216319575906,
			0.224087715149,
			0.222374990582,
			0.220220595598,
			0.206811457872,
			0.187931656837,
			0.170212760568,
			0.140024781227,
			0.132492110133,
			0.0921052619815,
			0.147058829665
		};

		// Which pt bin to use
		for ( unsigned int binIndex=0; binIndex < binEdges.size()-1; binIndex++ ) {
			if ( jetPt >= binEdges[binIndex] && jetPt < binEdges[binIndex+1] )
				return eff[binIndex];
		}
		return eff[eff.size()-1];
	}
	return 0.;
}

double BTagWeight::getUDSGEfficiency(const JetPointer jet) const {
	std::vector<double> binEdges{ 20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600, 800 };
	const double jetPt = jet->pt();

	if (Globals::energyInTeV == 8) { // 2012

		std::vector<double> eff{
			0.0141531676054,
			0.0166055671871,
			0.0122715132311,
			0.0121522741392,
			0.0121911447495,
			0.0128813134506,
			0.0122034205124,
			0.0123074641451,
			0.0138469943777,
			0.0146079286933,
			0.0160924512893,
			0.0136655950919,
			0.0142469471321,
			0.019417475909,
			0.0195121951401,
			0.0188679248095
		};

		// Which pt bin to use
		for ( unsigned int binIndex=0; binIndex < binEdges.size()-1; binIndex++ ) {
			if ( jetPt >= binEdges[binIndex] && jetPt < binEdges[binIndex+1] )
				return eff[binIndex];
		}
		return eff[eff.size()-1];
	} else if (Globals::energyInTeV == 7) { // 2011
		std::vector<double> eff{
			0.0144789591432,
			0.0180907342583,
			0.0137853939086,
			0.0135071650147,
			0.0124906441197,
			0.0128347137943,
			0.0132970232517,
			0.0132412724197,
			0.0141806136817,
			0.0142573462799,
			0.0167632680386,
			0.01748948358,
			0.0179928019643,
			0.0181461498141,
			0.0192926041782,
			0.00740740727633
		};

		// Which pt bin to use
		for ( unsigned int binIndex=0; binIndex < binEdges.size()-1; binIndex++ ) {
			if ( jetPt >= binEdges[binIndex] && jetPt < binEdges[binIndex+1] )
				return eff[binIndex];
		}
		return eff[eff.size()-1];
	}
	return 0.;
}

void BTagWeight::setNumberOfBtags(unsigned int min, unsigned int max) {
	minNumberOfTags_ = min;
	maxNumberOfTags_ = max;

}

}
