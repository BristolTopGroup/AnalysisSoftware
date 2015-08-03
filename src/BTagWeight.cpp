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

	// std::cout << "bTaggedMCJet : " << bTaggedMCJet << std::endl;
	// std::cout << "bTaggedDataJet : " << bTaggedDataJet << std::endl;
	// std::cout << "nonbTaggedMCJet : " << nonBTaggedMCJet << std::endl;
	// std::cout << "nonbTaggedDataJet : " << nonBTaggedDataJet << std::endl;
	// std::cout << "bTagWeight : " << bTagWeight << " +/- " << error_bTagWeight << std::endl;
	// std::cout << "******************************" << std::endl;

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
