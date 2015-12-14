#include "../interface/BTagWeight.h"
#include "../interface/GlobalVariables.h"

#include <functional>
#include <numeric>
#include <boost/scoped_ptr.hpp>
#include <boost/array.hpp>

using namespace std;
namespace BAT {

double BjetWeight(const JetCollection jets, const int systematic) {
	boost::scoped_ptr<BTagWeight> btagWeight(new BTagWeight());

	double weight = btagWeight->weight( jets, systematic );
	return weight;
}

BTagWeight::BTagWeight() :
				minNumberOfTags_(0), //
				maxNumberOfTags_(0) {
}

double BTagWeight::weight(const JetCollection jets, const int systematic) const {
	float bTaggedMCJet = 1.0;
	float nonBTaggedMCJet = 1.0;
	float bTaggedDataJet = 1.0;
	float nonBTaggedDataJet = 1.0;

	for (unsigned int index = 0; index < jets.size(); ++index) {
		// Info on this jet
		const JetPointer jet(jets.at(index));

		double jetPt = jet->pt();
		if ( jetPt < 25 ) continue;

		// If the pt of the jet is outside the pt range of the SFs,
		// use the pt at the upper/lower edge and double the uncertainty.
		bool ptOutOfRange = false;
		if ( jetPt <= 30 || jet->pt() >= 670 ) {
			ptOutOfRange = true;
		}

		const unsigned int partonFlavour = abs( jet->partonFlavour() );
		const bool isBTagged = jet->isBJet();
		bool isLight = false;
		bool isBC = false;
		(partonFlavour==5 || partonFlavour==4) ? isBC = true : isBC = false;
		(partonFlavour==3 || partonFlavour==2 || partonFlavour==1 || partonFlavour==21) ? isLight = true : isLight = false;

		// Get scale factor for this jet
		const double sf = jet->getBTagSF( 0 );
		double sf_up = jet->getBTagSF( 1 );
		double sf_down = jet->getBTagSF( -1 );

		if ( ptOutOfRange ) {
			sf_up = sf + 2 * ( sf_up - sf );
			sf_down = sf - 2 * ( sf - sf_down );

			if ( sf_up < 0 ) sf_up = 0;
			if ( sf_down < 0 ) sf_down = 0;
		}

		// Get efficiency for this jet
		const double eff = getEfficiency( partonFlavour, jet );

		// Systematic Option
		// 2 = b/c jet up
		// 1 = light jet up
		// 0 = central
		// -1 = light jet down
		// -2 = b/c jet down

		double sfToUse_l = sf;
		double sfToUse_bc = sf;
		if ( systematic == 1 ) {
			sfToUse_l = sf_up;
			sfToUse_bc = sf;
		}
		else if ( systematic == -1 ) {
			sfToUse_l = sf_down;
			sfToUse_bc = sf;
		}
		else if ( systematic == 2 ) {
			sfToUse_l = sf;
			sfToUse_bc = sf_up;
		}
		else if ( systematic == -2 ) {
			sfToUse_l = sf;
			sfToUse_bc = sf_down;
		}


		if (isBTagged) {
			if (isLight) {
				bTaggedMCJet *= eff;
				if ( eff*sfToUse_l > 1 ) bTaggedDataJet *= 1;
				else if ( eff*sfToUse_l < 0 ) bTaggedDataJet *= 0;
				else bTaggedDataJet *= eff*sfToUse_l;
			}

			else {
			bTaggedMCJet *= eff;
				if ( eff*sfToUse_bc > 1 ) bTaggedDataJet *= 1;
				else if ( eff*sfToUse_bc < 0 ) bTaggedDataJet *= 0;
				else bTaggedDataJet *= eff*sfToUse_bc;
			}
		}

		else {
			if (isLight) {
				nonBTaggedMCJet *= ( 1 - eff );
				if ( eff*sfToUse_l > 1 ) nonBTaggedDataJet *= 0;
				else if ( eff*sfToUse_l < 0 ) nonBTaggedDataJet *= 1;
				else nonBTaggedDataJet *= ( 1 - eff*sfToUse_l );			
			}

			else {
				nonBTaggedMCJet *= ( 1 - eff );
				if ( eff*sfToUse_bc > 1 ) nonBTaggedDataJet *= 0;
				else if ( eff*sfToUse_bc < 0 ) nonBTaggedDataJet *= 1;
				else nonBTaggedDataJet *= ( 1 - eff*sfToUse_bc );
			}			
		}

		// if ( nonBTaggedMCJet < 0 || nonBTaggedDataJet < 0 ) {
		// 	cout << nonBTaggedMCJet << " " << nonBTaggedDataJet << endl;
		// 	cout << eff << " " << sfToUse << endl;
		// }
	}

	double bTagWeight = (nonBTaggedDataJet * bTaggedDataJet) / (nonBTaggedMCJet * bTaggedMCJet);

	if ( nonBTaggedMCJet == 0 || bTaggedMCJet == 0 ) {
		bTagWeight = 1;
	}

	if ( std::isnan( bTagWeight )) {
		cout << "Btag weight is nan : " << bTagWeight << endl;
		cout << nonBTaggedDataJet << " " << bTaggedDataJet << " " << nonBTaggedMCJet << " " << bTaggedMCJet << endl;
	}
	else if ( bTagWeight > 10 || bTagWeight < 0 ) {
		cout << "Odd b tag weight : " << bTagWeight << endl;
		cout << nonBTaggedDataJet << " " << bTaggedDataJet << " " << nonBTaggedMCJet << " " << bTaggedMCJet << endl;

	}
	return bTagWeight;
}


double BTagWeight::getEfficiency( const unsigned int partonFlavour, const JetPointer jet ) const {
	if ( partonFlavour == 5) { //b-quark
		return getBEfficiency( jet );
	}
	else if ( partonFlavour == 4) { //c-quark
		return getCEfficiency( jet );
	}
	else if ( partonFlavour == 3 || partonFlavour == 2 || partonFlavour == 1 || partonFlavour == 0 || partonFlavour == 21) { // u/d/s/unclassified-quark
		return getUDSEfficiency ( jet );
	}
	// else if ( partonFlavour == 21) { //gluon
	// 	return getGEfficiency( jet );
	// }
	else return 0.;
}

float BTagWeight::getBEfficiency(const JetPointer jet) const {

	const double jetPt = jet->pt();
	const double jetEta = jet->eta();
	// std::cout << "Jet Pt : " << jetPt << ", Jet Eta : " << jetEta << std::endl;
	int binNumber = Globals::bQuarkJet->FindBin( jetPt , jetEta );
	float BTagEff = Globals::bQuarkJet->GetBinContent( binNumber );
	// std::cout << "B-quark Jet, B Tag Efficiency : " << BTagEff << std::endl;
	return BTagEff;
}

double BTagWeight::getCEfficiency(const JetPointer jet) const {

	const double jetPt = jet->pt();
	const double jetEta = jet->eta();
	// std::cout << "Jet Pt : " << jetPt << ", Jet Eta : " << jetEta << std::endl;
	int binNumber = Globals::cQuarkJet->FindBin( jetPt , jetEta );
	float BTagEff = Globals::cQuarkJet->GetBinContent( binNumber );	
	// std::cout << "C-quark Jet, B Tag Efficiency : " << BTagEff << std::endl;
	return BTagEff;
}

double BTagWeight::getUDSEfficiency(const JetPointer jet) const {

	const double jetPt = jet->pt();
	const double jetEta = jet->eta();
	// std::cout << "Jet Pt : " << jetPt << ", Jet Eta : " << jetEta << std::endl;
	int binNumber = Globals::udsQuarkJet->FindBin( jetPt , jetEta );
	float BTagEff = Globals::udsQuarkJet->GetBinContent( binNumber );
	// std::cout << "UDS-quark Jet, B Tag Efficiency : " << BTagEff << std::endl;
	return BTagEff;
}

double BTagWeight::getGEfficiency(const JetPointer jet) const {

	const double jetPt = jet->pt();
	const double jetEta = jet->eta();
	// std::cout << "Jet Pt : " << jetPt << ", Jet Eta : " << jetEta << std::endl;
	int binNumber = Globals::gluonJet->FindBin( jetPt , jetEta );
	float BTagEff = Globals::gluonJet->GetBinContent( binNumber );	
	// std::cout << "Gluon Jet, B Tag Efficiency : " << BTagEff << std::endl;
	return BTagEff;
}

void BTagWeight::setNumberOfBtags(unsigned int min, unsigned int max) {
	minNumberOfTags_ = min;
	maxNumberOfTags_ = max;
}

}
