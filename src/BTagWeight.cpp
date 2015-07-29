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
		const bool isBTagged = jet->isBJet(BtagAlgorithm::CombinedSecondaryVertexV2, BtagAlgorithm::MEDIUM);

		// Get scale factor for this jet
		std::vector<double> sfAndError = getScaleFactor( partonFlavour, jet );
		const double sf = sfAndError.at(0);
		const double sfError = sfAndError.at(1);

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

std::vector<double> BTagWeight::getScaleFactor( const double partonFlavour, const JetPointer jet ) const {

	if ( partonFlavour == 5) { //b-quark
		return getBScaleFactor(jet);
	}
	else if ( partonFlavour == 4) { //c-quark
		return getCScaleFactor(jet);
	}
	else if ( partonFlavour != 4 && partonFlavour != 5) { //not a c- or b-quark
		return getUDSGScaleFactor(jet);
	}
	else return std::vector<double>();
}

std::vector<double> BTagWeight::getBScaleFactor(const JetPointer jet, double uncertaintyFactor) const {
	double SFb(0);
	double sf_error(0);
	//these numbers are for CSVM only
	double pt = jet->pt();
	double eta = fabs(jet->eta());

	if (Globals::energyInTeV == 8) { // 2012 btag scale factors
		// From https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-pt_WITHttbar_payload_EPS13.txt,
		// which is linked from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2012_Data_and_MC_EPS13_prescript
		const boost::array<double, 16> SFb_error = { {0.0415707,
				0.0204209,
				0.0223227,
				0.0206655,
				0.0199325,
				0.0174121,
				0.0202332,
				0.0182446,
				0.0159777,
				0.0218531,
				0.0204688,
				0.0265191,
				0.0313175,
				0.0415417,
				0.0740446,
				0.0596716 } };

		//2012 pt bins low edges
		const boost::array<double, 16> ptbins = { {20, 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500, 600 } };

		if (pt < 20) {
			SFb = (0.938887 + (0.00017124*20))+(-2.76366e-07*(20*20));
			//use twice the uncertainty
			sf_error = 2 * SFb_error[0];

		} else if (pt > 800) {
			SFb = (0.938887 + (0.00017124*800))+(-2.76366e-07*(800*800));
			//use twice the uncertainty
			sf_error = 2 * SFb_error[SFb_error.size() - 1];

		} else {
			SFb = (0.938887 + (0.00017124*pt))+(-2.76366e-07*(pt*pt));
			unsigned int ptbin(0);
			for (unsigned int bin = 0; bin < ptbins.size() + 1; ++bin) {
				double upperCut = bin + 1 < ptbins.size() ? ptbins.at(bin + 1) : 800.;
				double lowerCut = ptbins.at(bin);

				if (pt > lowerCut && pt <= upperCut) {
					ptbin = bin;
					break;
				}
			}
			sf_error = SFb_error.at(ptbin);

			//use twice the uncertainty if outside the 0 to 2.4 eta range
			if (2.4 < eta && eta <=2.6) {
				sf_error = 2 * SFb_error.at(ptbin);
			}
		}
	}
	else if (Globals::energyInTeV == 7) { // 2011 btag scale factors
		// https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFb-mujet_payload.txt
		const boost::array<double, 14> SFb_error = { { 0.0295675,
				0.0295095,
				0.0210867,
				0.0219349,
				0.0227033,
				0.0204062,
				0.0185857,
				0.0256242,
				0.0383341,
				0.0409675,
				0.0420284,
				0.0541299,
				0.0578761,
				0.0655432 } };

		//2011 pt bins low edges
		const boost::array<double, 14> ptbins = { { 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500 } };

		if (pt < 30) {
			SFb = 0.6981*((1.+(0.414063*pt))/(1.+(0.300155*30)));
			//absolute uncertainty of +/-0.12
			sf_error = 0.12;

		} else if (pt > 670) {
			SFb = 0.6981*((1.+(0.414063*670))/(1.+(0.300155*670)));
			//use twice the uncertainty
			sf_error = 2 * SFb_error[SFb_error.size() - 1];

		} else {
			SFb = 0.6981*((1.+(0.414063*pt))/(1.+(0.300155*pt)));

			unsigned int ptbin(0);
			for (unsigned int bin = 0; bin < ptbins.size() + 1; ++bin) {
				double upperCut = bin + 1 < ptbins.size() ? ptbins.at(bin + 1) : 670.;
				double lowerCut = ptbins.at(bin);

				if (pt > lowerCut && pt <= upperCut) {
					ptbin = bin;
					break;
				}
			}
			sf_error = SFb_error.at(ptbin);

			//use twice the uncertainty if outside the 0 to 2.4 eta range
			if (2.4 < eta && eta <=2.6) {
				sf_error = 2 * SFb_error.at(ptbin);
			}
		}
	}

	std::vector<double> SF_b_and_error;
	SF_b_and_error.push_back(SFb);
	SF_b_and_error.push_back(sf_error * uncertaintyFactor);
	return SF_b_and_error;
}

std::vector<double> BTagWeight::getCScaleFactor(const JetPointer jet) const { // doesn't seem to be used anywhere??
	// Same in both 8TeV and 7TeV
	return getBScaleFactor(jet, 2.0);
}

std::vector<double> BTagWeight::getUDSGScaleFactor(const JetPointer jet) const {
	double pt = jet->pt();
	double eta = fabs(jet->eta());
	double SF_udsg_mean(0), SF_udsg_min(0), SF_udsg_max(0);
	std::vector<double> SF_udsg_and_error;

	if (Globals::energyInTeV == 8) { // 2012
		if (pt < 20) {
			SF_udsg_and_error.push_back(0.);
			SF_udsg_and_error.push_back(0.);
			return SF_udsg_and_error;
			//			return 0;
		} else if (pt > 850 && eta >= 1.6 && eta <= 2.5) {
			SF_udsg_mean = getMeanUDSGScaleFactor(850., eta);
			SF_udsg_min = getMinUDSGScaleFactor(850, eta);
			SF_udsg_max = getMaxUDSGScaleFactor(850, eta);
			//use twice the uncertainty
			SF_udsg_min -= (SF_udsg_mean - SF_udsg_min);
			SF_udsg_max += (SF_udsg_max - SF_udsg_mean);
		} else if (pt > 1000) {
			SF_udsg_mean = getMeanUDSGScaleFactor(1000., eta);
			SF_udsg_min = getMinUDSGScaleFactor(1000, eta);
			SF_udsg_max = getMaxUDSGScaleFactor(1000, eta);
			//use twice the uncertainty
			SF_udsg_min -= (SF_udsg_mean - SF_udsg_min);
			SF_udsg_max += (SF_udsg_max - SF_udsg_mean);
		} else {
			SF_udsg_mean = getMeanUDSGScaleFactor(pt, eta);
			SF_udsg_min = getMinUDSGScaleFactor(pt, eta);
			SF_udsg_max = getMaxUDSGScaleFactor(pt, eta);
		}
	} else if (Globals::energyInTeV == 7) { // 2011
		if (pt < 20) {
			SF_udsg_and_error.push_back(0.);
			SF_udsg_and_error.push_back(0.);
			return SF_udsg_and_error;
			//			return 0;
		} else if (pt > 670 && eta >= 0. && eta <= 2.5) {
			// Use integrated over all eta
			SF_udsg_mean = getMeanUDSGScaleFactor(pt, eta);
			SF_udsg_min = getMinUDSGScaleFactor(pt, eta);
			SF_udsg_max = getMaxUDSGScaleFactor(pt, eta);
			//use twice the uncertainty
			SF_udsg_min -= (SF_udsg_mean - SF_udsg_min);
			SF_udsg_max += (SF_udsg_max - SF_udsg_mean);
		} else {
			SF_udsg_mean = getMeanUDSGScaleFactor(pt, eta);
			SF_udsg_min = getMinUDSGScaleFactor(pt, eta);
			SF_udsg_max = getMaxUDSGScaleFactor(pt, eta);
		}
	}

	double SF_udsg_error = fabs(SF_udsg_max - SF_udsg_mean) > fabs(SF_udsg_min - SF_udsg_mean) ? fabs(SF_udsg_max - SF_udsg_mean) : fabs(SF_udsg_min - SF_udsg_mean);

	SF_udsg_and_error.push_back(SF_udsg_mean);
	SF_udsg_and_error.push_back(SF_udsg_error);
	return SF_udsg_and_error;
}

double BTagWeight::getMeanUDSGScaleFactor(double jetPT, double jetEta) const {
	// From https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_EPS2013.C
	// which is linked from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2012_Data_and_MC_EPS13_prescript

	if (Globals::energyInTeV == 8) { // 2012
		if(jetEta >=0 && jetEta < 0.8) {
			return ((1.07541+(0.00231827*jetPT))+(-4.74249e-06*(jetPT*jetPT)))+(2.70862e-09*(jetPT*(jetPT*jetPT)));
		} else if(jetEta >= 0.8 && jetEta < 1.6) {
			return ((1.05613+(0.00114031*jetPT))+(-2.56066e-06*(jetPT*jetPT)))+(1.67792e-09*(jetPT*(jetPT*jetPT)));
			// Technically only valid up to 2.4, but we go up to 2.5
		} else if(jetEta >= 1.6 && jetEta <= 2.5){
			return ((1.05625+(0.000487231*jetPT))+(-2.22792e-06*(jetPT*jetPT)))+(1.70262e-09*(jetPT*(jetPT*jetPT)));
		}
	}
	else if (Globals::energyInTeV == 7) { // 2011
		if(jetPT > 670 && jetEta >=0. && jetEta <=2.5){
			// Integrated over eta
			return ((1.04318+(0.000848162*jetPT))+(-2.5795e-06*(jetPT*jetPT)))+(1.64156e-09*(jetPT*(jetPT*jetPT)));
		} else if(jetEta >= 0 && jetEta < 0.8) {
			return ((1.06182+(0.000617034*jetPT))+(-1.5732e-06*(jetPT*jetPT)))+(3.02909e-10*(jetPT*(jetPT*jetPT)));
		} else if(jetEta >= 0.8 && jetEta < 1.6) {
			return ((1.111+(-9.64191e-06*jetPT))+(1.80811e-07*(jetPT*jetPT)))+(-5.44868e-10*(jetPT*(jetPT*jetPT)));
			// Technically only valid up to 2.4, but we go up to 2.5
		} else if(jetEta >= 1.6 && jetEta <= 2.5) {
			return ((1.08498+(-0.000701422*jetPT))+(3.43612e-06*(jetPT*jetPT)))+(-4.11794e-09*(jetPT*(jetPT*jetPT)));
		}
	}
	return 0.;
}

double BTagWeight::getMinUDSGScaleFactor(double jetPT, double jetEta) const {
	// From https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_EPS2013.C
	// which is linked from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2012_Data_and_MC_EPS13_prescript

	if (Globals::energyInTeV == 8) { // 2012
		if(jetEta >=0 && jetEta < 0.8) {
			return ((0.964527+(0.00149055*jetPT))+(-2.78338e-06*(jetPT*jetPT)))+(1.51771e-09*(jetPT*(jetPT*jetPT)));
		} else if(jetEta >= 0.8 && jetEta < 1.6) {
			return ((0.946051+(0.000759584*jetPT))+(-1.52491e-06*(jetPT*jetPT)))+(9.65822e-10*(jetPT*(jetPT*jetPT)));
			// Technically only valid up to 2.4, but we go up to 2.5
		} else if(jetEta >= 1.6 && jetEta <= 2.5) {
			return ((0.956736+(0.000280197*jetPT))+(-1.42739e-06*(jetPT*jetPT)))+(1.0085e-09*(jetPT*(jetPT*jetPT)));
		}
	}
	if (Globals::energyInTeV == 7) { //2011
		if (jetPT > 670 && jetEta >=0. && jetEta <=2.5) {
			// Integrated over eta
			return ((0.962627+(0.000448344*jetPT))+(-1.25579e-06*(jetPT*jetPT)))+(4.82283e-10*(jetPT*(jetPT*jetPT)));
		} else if(jetEta >= 0 && jetEta < 0.8) {
			return ((0.972455+(7.51396e-06*jetPT))+(4.91857e-07*(jetPT*jetPT)))+(-1.47661e-09*(jetPT*(jetPT*jetPT)));
		} else if(jetEta >= 0.8 && jetEta < 1.6) {
			return ((1.02055+(-0.000378856*jetPT))+(1.49029e-06*(jetPT*jetPT)))+(-1.74966e-09*(jetPT*(jetPT*jetPT)));
			// Technically only valid up to 2.4, but we go up to 2.5
		} else if(jetEta >= 1.6 && jetEta <= 2.5) {
			return ((0.983476+(-0.000607242*jetPT))+(3.17997e-06*(jetPT*jetPT)))+(-4.01242e-09*(jetPT*(jetPT*jetPT)));
		}
	}
	return 0.;
}

double BTagWeight::getMaxUDSGScaleFactor(double jetPT, double jetEta) const {
	// From https://twiki.cern.ch/twiki/pub/CMS/BtagPOG/SFlightFuncs_EPS2013.C
	// which is linked from https://twiki.cern.ch/twiki/bin/viewauth/CMS/BtagPOG#2012_Data_and_MC_EPS13_prescript

	if (Globals::energyInTeV == 8) { // 2012
		if (jetEta >-0. && jetEta < 0.8) {
			return ((1.18638+(0.00314148*jetPT))+(-6.68993e-06*(jetPT*jetPT)))+(3.89288e-09*(jetPT*(jetPT*jetPT)));
		} else if (jetEta >= 0.8 && jetEta < 1.6) {
			return ((1.16624+(0.00151884*jetPT))+(-3.59041e-06*(jetPT*jetPT)))+(2.38681e-09*(jetPT*(jetPT*jetPT)));
			// Technically only valid up to 2.4, but we go up to 2.5
		} else if(jetEta >= 1.6 && jetEta <= 2.5) {
			return ((1.15575+(0.000693344*jetPT))+(-3.02661e-06*(jetPT*jetPT)))+(2.39752e-09*(jetPT*(jetPT*jetPT)));
		}
	} else if (Globals::energyInTeV == 7) { // 2011
		if (jetEta >670 && jetEta >=0. && jetEta <=2.5){
			// Integrated over eta
			return ((1.12368+(0.00124806*jetPT))+(-3.9032e-06*(jetPT*jetPT)))+(2.80083e-09*(jetPT*(jetPT*jetPT)));
		}else if(jetEta >= 0 && jetEta < 0.8) {
			return ((1.15116+(0.00122657*jetPT))+(-3.63826e-06*(jetPT*jetPT)))+(2.08242e-09*(jetPT*(jetPT*jetPT)));
		} else if(jetEta >= 0.8 && jetEta < 1.6) {
			return ((1.20146+(0.000359543*jetPT))+(-1.12866e-06*(jetPT*jetPT)))+(6.59918e-10*(jetPT*(jetPT*jetPT)));
			// Technically only valid up to 2.4, but we go up to 2.5
		} else if(jetEta >= 1.6 && jetEta <= 2.5) {
			return ((1.18654+(-0.000795808*jetPT))+(3.69226e-06*(jetPT*jetPT)))+(-4.22347e-09*(jetPT*(jetPT*jetPT)));
		}
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
