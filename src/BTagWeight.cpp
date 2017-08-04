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

	double dummy1 = 0, dummy2 = 0;
	double weight = btagWeight->weight( jets, systematic, bJetWPs, dummy1, dummy2 );
	return weight;
}

double BjetWeights(const JetCollection jets, const int systematic, const std::vector<std::string> bJetWPs, double& efficiencyCorrection, double& alternativeWeight ) {
	boost::scoped_ptr<BTagWeight> btagWeight(new BTagWeight());

	double weight = btagWeight->weight( jets, systematic, bJetWPs, efficiencyCorrection, alternativeWeight );
	return weight;
}


BTagWeight::BTagWeight() :
				minNumberOfTags_(0), //
				maxNumberOfTags_(0) {
}

double BTagWeight::weight(const JetCollection jets, const int systematic, const std::vector<std::string> bJetWPs) const {
	double dummy1 = 0, dummy2 = 0;
	return weight( jets, systematic, bJetWPs, dummy1, dummy2 );
}

double BTagWeight::weight(const JetCollection jets, const int systematic, const std::vector<std::string> bJetWPs, double& efficiencyCorrection, double& alternativeWeight ) const {
	float bTaggedMCJet = 1.0;
	float nonBTaggedMCJet = 1.0;
	float bTaggedDataJet = 1.0;
	float nonBTaggedDataJet = 1.0;

	efficiencyCorrection = 1.;

	float alternative_bTaggedMCJet = 1.0;
	float alternative_nonBTaggedMCJet = 1.0;
	float alternative_bTaggedDataJet = 1.0;
	float alternative_nonBTaggedDataJet = 1.0;
	alternativeWeight = 1.;

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
		const double eff_PowhegPythia8 = getEfficiency( hadronFlavour, jet, true );

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

		// if (isBTagged) {
		// 	// std::cout << "BTagged eff*sf : " << eff*sfToUse << std::endl;
		// 	bTaggedMCJet *= eff;
		// 	bTaggedDataJet *= eff*sfToUse;
		// }else{
		// 	// std::cout << "Not BTagged eff*sf : " << ( 1 - eff*sfToUse ) << std::endl;
		// 	nonBTaggedMCJet *= ( 1 - eff );
		// 	nonBTaggedDataJet *= ( 1 - eff*sfToUse );
		// }


		// 						eff_data   	          		               		eff_PP8   
		// 			PROD ( ————————————————————  . eff_PP8 )    		PROD ( ————————— . eff_PH )
		// 	      			eff_MC(QCD (Pythia))			  		        	eff_PH
		// 	     (	—————————————————————————————————————————— ) .  ( ——————————————————————————————— )
		// 	       				PROD ( eff_PP8 )                             PROD ( eff_PH )

		
		// 						eff_data   	          		               			   
		// 			PROD ( ————————————————————  . eff_PP8 )    
		// 	      			eff_MC(QCD (Pythia))			  		        		
		// 	    (	—————————————————————————————————————————— ) 
		// 	       				PROD ( eff_PH ) 


		// NEW B tag scale factor
		// We apply two weights, both calculated by the same method
		// The first corrects the efficiencies in Powheg Pythia 8 MC (or the central MC, which should also be the same generator+PS used to derive the scale factors) to data
		// The second corrects the efficiencies in the sample you are considering (e.g. Powheg+Herwig++) to Powheg Pythia 8 (or whichever central MC you are using)
		// The denominator of the first weight cancels with the numerator of the second, so the total weight simplifies to this below
		// if (isBTagged) {
		// 	bTaggedMCJet *= eff;
		// 	bTaggedDataJet *= eff*sfToUse;
		// }else{
		// 	nonBTaggedMCJet *= ( 1 - eff );
		// 	nonBTaggedDataJet *= ( 1 - eff*sfToUse );
		// }

		if (isBTagged) {
			bTaggedMCJet *= eff_PowhegPythia8;
			bTaggedDataJet *= eff_PowhegPythia8*sfToUse;

			alternative_bTaggedMCJet *= eff;
			alternative_bTaggedDataJet *= eff*sfToUse;
		}else{
			nonBTaggedMCJet *= ( 1 - eff_PowhegPythia8 );
			nonBTaggedDataJet *= ( 1 - eff_PowhegPythia8*sfToUse );

			alternative_nonBTaggedMCJet *= ( 1 - eff );
			alternative_nonBTaggedDataJet *= ( 1 - eff*sfToUse );
		}

		// if ( std::isnan( eff_PowhegPythia8 ) || std::isnan( eff ) ) {
		// }
		// else {
		double correctionForThisJet = 1;
		if ( eff > 0 ) {
			correctionForThisJet = eff_PowhegPythia8 / eff;
		}
		if ( correctionForThisJet > 0 )  {
			efficiencyCorrection *= correctionForThisJet;
		}

		// if ( std::isnan( efficiencyCorrection ) ) {
		// 	std::cout << "Efficiency correction nan " << eff << " " << eff_PowhegPythia8 << " " << eff_PowhegPythia8 / eff  << std::endl;
		// }
	}

	double bTagWeight = (nonBTaggedDataJet * bTaggedDataJet) / (nonBTaggedMCJet * bTaggedMCJet);
	alternativeWeight = (alternative_nonBTaggedDataJet * alternative_bTaggedDataJet) / (alternative_nonBTaggedMCJet * alternative_bTaggedMCJet);

	if ( nonBTaggedMCJet == 0 || bTaggedMCJet == 0 ) {
		bTagWeight = 1;
	}

	if ( alternative_nonBTaggedMCJet == 0 || alternative_bTaggedMCJet == 0 ) {
		alternativeWeight = 1;
	}

	if ( std::isnan( bTagWeight )) {
		cout << "Btag weight is nan : " << bTagWeight << endl;
		cout << nonBTaggedDataJet << " " << bTaggedDataJet << " " << nonBTaggedMCJet << " " << bTaggedMCJet << " " << efficiencyCorrection << endl;
		bTagWeight = 1;
	}
	else if ( bTagWeight > 10 || bTagWeight < 0 ) {
		cout << "Odd b tag weight : " << bTagWeight << endl;
		cout << nonBTaggedDataJet << " " << bTaggedDataJet << " " << nonBTaggedMCJet << " " << bTaggedMCJet << endl;
	}

	if ( std::isnan( alternativeWeight )) {
		cout << "Alternative Btag weight is nan : " << alternativeWeight << endl;
		cout << alternative_nonBTaggedDataJet << " " << alternative_bTaggedDataJet << " " << alternative_nonBTaggedMCJet << " " << alternative_bTaggedMCJet << " " << efficiencyCorrection << endl;
		alternativeWeight = 1;
	}
	else if ( alternativeWeight > 10 || alternativeWeight < 0 ) {
		cout << "Odd alternative b tag weight : " << alternativeWeight << endl;
		cout << alternative_nonBTaggedDataJet << " " << alternative_bTaggedDataJet << " " << alternative_nonBTaggedMCJet << " " << alternative_bTaggedMCJet << endl;

	}

	return bTagWeight;
}


double BTagWeight::getEfficiency( const unsigned int hadronFlavour, const JetPointer jet, bool centralMCEfficiency ) const {
	if ( hadronFlavour == 5) { //b-quark
		return getBEfficiency( jet, centralMCEfficiency );
	}
	else if ( hadronFlavour == 4) { //c-quark
		return getCEfficiency( jet, centralMCEfficiency );
	}
	else if ( hadronFlavour == 0) { // u/d/s/g/unclassified-quark
		return getUDSGEfficiency ( jet, centralMCEfficiency );
	}
	else return 0.;
}

float BTagWeight::getBEfficiency(const JetPointer jet, bool centralMCEfficiency ) const {
	double jetPt = jet->pt();
	const double jetEta = jet->eta();
	const double maxPt = Globals::bQuarkJet->GetXaxis()->GetXmax();

	// If pt out of range set to last bin. eta not a consideration as analysis cut is last bin
	if ( jetPt >= maxPt ) {
		jetPt = Globals::bQuarkJet->GetXaxis()->GetBinCenter( Globals::bQuarkJet->GetNbinsX() );
	}

	int binNumber = Globals::bQuarkJet->FindBin( jetPt , jetEta );
	float BTagEff = 1;

	if ( centralMCEfficiency ) {
		BTagEff = Globals::bQuarkJet_PowhegPythia8->GetBinContent( binNumber );
	}
	else {
		BTagEff = Globals::bQuarkJet->GetBinContent( binNumber );
	}

	// if ( jetPt >= maxPt ) {
	// 	std::cout << "Jet Pt : " << jetPt << ", Jet Eta : " << jetEta << std::endl;
	// 	std::cout << "B-quark Jet, B Tag Efficiency : " << BTagEff << std::endl;
	// }
	return BTagEff;
}

double BTagWeight::getCEfficiency(const JetPointer jet, bool centralMCEfficiency ) const {
	double jetPt = jet->pt();
	const double jetEta = jet->eta();
	const double maxPt = Globals::cQuarkJet->GetXaxis()->GetXmax();

	// If pt out of range set to last bin. eta not a consideration as analysis cut is last bin
	if ( jetPt >= maxPt ) {
		jetPt = Globals::cQuarkJet->GetXaxis()->GetBinCenter( Globals::cQuarkJet->GetNbinsX() );
	}

	int binNumber = Globals::cQuarkJet->FindBin( jetPt , jetEta );
	float BTagEff = 1;

	if ( centralMCEfficiency ) {
		BTagEff = Globals::cQuarkJet_PowhegPythia8->GetBinContent( binNumber );
	}
	else {
		BTagEff = Globals::cQuarkJet->GetBinContent( binNumber );
	}
	// std::cout << "C-quark Jet, B Tag Efficiency : " << BTagEff << std::endl;
	return BTagEff;
}

double BTagWeight::getUDSGEfficiency(const JetPointer jet, bool centralMCEfficiency ) const {
	double jetPt = jet->pt();
	const double jetEta = jet->eta();
	const double maxPt = Globals::udsgQuarkJet->GetXaxis()->GetXmax();

	// If pt out of range set to last bin. eta not a consideration as analysis cut is last bin
	if ( jetPt >= maxPt ) {
		jetPt = Globals::udsgQuarkJet->GetXaxis()->GetBinCenter( Globals::udsgQuarkJet->GetNbinsX() );
	}

	int binNumber = Globals::udsgQuarkJet->FindBin( jetPt , jetEta );
	float BTagEff = 1;

	if ( centralMCEfficiency ) {
		BTagEff = Globals::udsgQuarkJet_PowhegPythia8->GetBinContent( binNumber );
	}
	else {
		BTagEff = Globals::udsgQuarkJet->GetBinContent( binNumber );
	}
	// std::cout << "UDS-quark Jet, B Tag Efficiency : " << BTagEff << std::endl;
	return BTagEff;
}

void BTagWeight::setNumberOfBtags(unsigned int min, unsigned int max) {
	minNumberOfTags_ = min;
	maxNumberOfTags_ = max;
}

}
