#include "../interface/BTagWeight.h"
#include "../interface/GlobalVariables.h"

#include <functional>
#include <numeric>
#include <boost/scoped_ptr.hpp>
#include <boost/array.hpp>

using namespace std;
namespace BAT {

double BjetWeight(const JetCollection jets, const int systematic, const std::vector<std::string> bJetWPs) {
	boost::scoped_ptr<BTagWeight> btagWeight(new BTagWeight());

	double weight = btagWeight->weight( jets, systematic, bJetWPs );
	return weight;
}

BTagWeight::BTagWeight() :
				minNumberOfTags_(0), //
				maxNumberOfTags_(0) {
}

double BTagWeight::weight(const JetCollection jets, const int systematic, const std::vector<std::string> bJetWPs) const {
	float bTaggedMCJet = 1.0;
	float nonBTaggedMCJet = 1.0;
	float bTaggedDataJet = 1.0;
	float nonBTaggedDataJet = 1.0;

	for (unsigned int index = 0; index < jets.size(); ++index) {
		// Info on this jet
		const JetPointer jet(jets.at(index));

		double jetPt = jet->pt();
		if ( jetPt < 30 ) continue;

		const unsigned int hadronFlavour = abs( jet->hadronFlavour() );
		const bool isBTagged = jet->isBJet();
		bool isLight = false;
		(hadronFlavour==0) ? isLight = true : isLight = false;

		double sf = 0;
		double sf_up = 0;
		double sf_down = 0;

		// std::cout << "BJets WP : " << bJetWPs.at(0) << ", " << bJetWPs.at(0) << "\n";

		if (bJetWPs.at(0) == "M" && bJetWPs.at(1) == "M"){
			// Get scale factor for this jet given 2 medium btags
			sf = jet->getBTagSF( 0 );
			sf_up = jet->getBTagSF( 1 );
			sf_down = jet->getBTagSF( -1 );
		}
		else if (bJetWPs.at(0) == "T" && bJetWPs.at(1) == "T"){
			// Get scale factor for this jet given 2 tight btags
			sf = jet->getTightBTagSF( 0 );
			sf_up = jet->getTightBTagSF( 1 );
			sf_down = jet->getTightBTagSF( -1 );
		}

		// Get efficiency for this jet
		const double eff = getEfficiency( hadronFlavour, jet );

		// Systematic Option
		// 2 = b/c jet up
		// 1 = light jet up
		// 0 = central
		// -1 = light jet down
		// -2 = b/c jet down

		double sfToUse_l = sf;
		double sfToUse_bc = sf;
		double sfToUse = 0;
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

		(isLight==true) ? sfToUse = sfToUse_l : sfToUse = sfToUse_bc;

		if (isBTagged) {
			// std::cout << "BTagged eff*sf : " << eff*sfToUse << std::endl;
			bTaggedMCJet *= eff;
			bTaggedDataJet *= eff*sfToUse;
		}else{
			// std::cout << "Not BTagged eff*sf : " << ( 1 - eff*sfToUse ) << std::endl;
			nonBTaggedMCJet *= ( 1 - eff );
			nonBTaggedDataJet *= ( 1 - eff*sfToUse );
		}
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


double BTagWeight::getEfficiency( const unsigned int hadronFlavour, const JetPointer jet ) const {
	if ( hadronFlavour == 5) { //b-quark
		return getBEfficiency( jet );
	}
	else if ( hadronFlavour == 4) { //c-quark
		return getCEfficiency( jet );
	}
	else if ( hadronFlavour == 0) { // u/d/s/g/unclassified-quark
		return getUDSGEfficiency ( jet );
	}
	else return 0.;
}

float BTagWeight::getBEfficiency(const JetPointer jet) const {
	double jetPt = jet->pt();
	const double jetEta = jet->eta();
	const double maxPt = Globals::bQuarkJet->GetXaxis()->GetXmax();

	// If pt out of range set to last bin. eta not a consideration as analysis cut is last bin
	if ( jetPt >= maxPt ) {
		jetPt = Globals::bQuarkJet->GetXaxis()->GetBinCenter( Globals::bQuarkJet->GetNbinsX() );
	}

	int binNumber = Globals::bQuarkJet->FindBin( jetPt , jetEta );
	float BTagEff = Globals::bQuarkJet->GetBinContent( binNumber );

	// if ( jetPt >= maxPt ) {
	// 	std::cout << "Jet Pt : " << jetPt << ", Jet Eta : " << jetEta << std::endl;
	// 	std::cout << "B-quark Jet, B Tag Efficiency : " << BTagEff << std::endl;
	// }
	return BTagEff;
}

double BTagWeight::getCEfficiency(const JetPointer jet) const {
	double jetPt = jet->pt();
	const double jetEta = jet->eta();
	const double maxPt = Globals::cQuarkJet->GetXaxis()->GetXmax();

	// If pt out of range set to last bin. eta not a consideration as analysis cut is last bin
	if ( jetPt >= maxPt ) {
		jetPt = Globals::cQuarkJet->GetXaxis()->GetBinCenter( Globals::cQuarkJet->GetNbinsX() );
	}

	int binNumber = Globals::cQuarkJet->FindBin( jetPt , jetEta );
	float BTagEff = Globals::cQuarkJet->GetBinContent( binNumber );	
	// std::cout << "C-quark Jet, B Tag Efficiency : " << BTagEff << std::endl;
	return BTagEff;
}

double BTagWeight::getUDSGEfficiency(const JetPointer jet) const {
	double jetPt = jet->pt();
	const double jetEta = jet->eta();
	const double maxPt = Globals::udsgQuarkJet->GetXaxis()->GetXmax();

	// If pt out of range set to last bin. eta not a consideration as analysis cut is last bin
	if ( jetPt >= maxPt ) {
		jetPt = Globals::udsgQuarkJet->GetXaxis()->GetBinCenter( Globals::udsgQuarkJet->GetNbinsX() );
	}

	int binNumber = Globals::udsgQuarkJet->FindBin( jetPt , jetEta );
	float BTagEff = Globals::udsgQuarkJet->GetBinContent( binNumber );
	// std::cout << "UDS-quark Jet, B Tag Efficiency : " << BTagEff << std::endl;
	return BTagEff;
}

void BTagWeight::setNumberOfBtags(unsigned int min, unsigned int max) {
	minNumberOfTags_ = min;
	maxNumberOfTags_ = max;
}

}
