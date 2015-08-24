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

		// For now, no scale factors are provided for jets with pt between 25 and 30, so return 1
		// Fix with new ntuples
		if ( jet->pt() < 25 ) continue;

		const unsigned int partonFlavour = abs( jet->partonFlavour() );
		const bool isBTagged = jet->isBJet();

		// Get scale factor for this jet
		const double sf = jet->getBTagSF( 0 );
		const double sf_up = jet->getBTagSF( 1 );
		const double sf_down = jet->getBTagSF( -1 );

		// Get efficiency for this jet
		const double eff = getEfficiency( partonFlavour, jet );

		double sfToUse = sf;
		if ( systematic == 1 ) {
			sfToUse = sf_up;
		}
		else if ( systematic == -1 ) {
			sfToUse = sf_down;
		}

		// Jets with large pt (>670) don't have scale factors, set to 1
		// Fix with new ntuples
		if ( sfToUse == 0 ) sfToUse = 1;

		if ( isBTagged ) {
			bTaggedMCJet *= eff;
			bTaggedDataJet *= eff*sfToUse;
		}
		else {
			nonBTaggedMCJet *= ( 1 - eff );
			nonBTaggedDataJet *= ( 1 - eff*sfToUse );
		}
	}

	double bTagWeight = (nonBTaggedDataJet * bTaggedDataJet) / (nonBTaggedMCJet * bTaggedMCJet);

	return bTagWeight;
}


double BTagWeight::getEfficiency( const unsigned int partonFlavour, const JetPointer jet ) const {
	if ( partonFlavour == 5) { //b-quark
		return getBEfficiency( jet );
	}
	else if ( partonFlavour == 4) { //c-quark
		return getCEfficiency( jet );
	}
	else if ( partonFlavour == 3 || partonFlavour == 2 || partonFlavour == 1 || partonFlavour == 0) { // u/d/s/unclassified-quark
		return getUDSEfficiency ( jet );
	}
	else if ( partonFlavour == 21) { //gluon
		return getGEfficiency( jet );
	}
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
