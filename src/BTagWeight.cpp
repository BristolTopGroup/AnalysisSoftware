#include "../interface/BTagWeight.h"
#include "../interface/GlobalVariables.h"

#include <functional>
#include <numeric>

namespace BAT {

unsigned int fact(unsigned int n) {
	if (n < 1)
		return 1;
	unsigned int r = 1;
	for (unsigned int i = n; i > 1; i--)
		r *= i;

	return r;
}

unsigned int comb(unsigned int n, unsigned int k) {
	return fact(n) / fact(k) / fact(n - k);
}

BTagWeight::BTagWeight(unsigned int minNumberOfTags, unsigned int maxNumberOfTags) :
		minNumberOfTags_(minNumberOfTags), //
		maxNumberOfTags_(maxNumberOfTags) {

}

bool BTagWeight::filter(unsigned int numberOfTags) const {
	return (numberOfTags >= minNumberOfTags_ && numberOfTags <= maxNumberOfTags_);
}

double BTagWeight::weight(unsigned int numberOf_b_Jets, unsigned int numberOf_c_Jets, unsigned int numberOf_udsg_Jets,
		double mean_bJetEfficiency, double mean_cJetEfficiency, double mean_udsgJetEfficiency, double scaleFactor_b,
		double scaleFactor_c, double scaleFactor_udsg, unsigned int numberOfTags) const {
	if (!filter(numberOfTags)) {
		return 0;
	}

	double probabilityMC = 0;
	double probabilityData = 0;
	for (unsigned int b_index = 0; b_index <= numberOf_b_Jets; ++b_index)
		for (unsigned int c_index = 0; c_index <= numberOf_c_Jets; ++c_index)
			for (unsigned int udsg_index = 0; udsg_index <= numberOf_udsg_Jets; ++udsg_index) {
				unsigned int t = b_index + c_index + udsg_index;
				if (!filter(t) && t != numberOfTags)
					continue;

				// how many equivalent ways
				unsigned int totalCombinations = comb(numberOf_b_Jets, b_index) * comb(numberOf_c_Jets, c_index)
						* comb(numberOf_udsg_Jets, udsg_index);

				probabilityMC += 1. * totalCombinations * pow(mean_bJetEfficiency, b_index)
						* pow(1. - mean_bJetEfficiency, numberOf_b_Jets - b_index) * pow(mean_cJetEfficiency, c_index)
						* pow(1. - mean_cJetEfficiency, numberOf_c_Jets - c_index)
						* pow(mean_udsgJetEfficiency, udsg_index)
						* pow(1. - mean_udsgJetEfficiency, numberOf_udsg_Jets - udsg_index);

				probabilityData += 1. * totalCombinations * pow(mean_bJetEfficiency * scaleFactor_b, b_index)
						* pow(1. - mean_bJetEfficiency * scaleFactor_b, numberOf_b_Jets - b_index)
						* pow(mean_cJetEfficiency * scaleFactor_c, c_index)
						* pow(1. - mean_cJetEfficiency * scaleFactor_c, numberOf_c_Jets - c_index)
						* pow(mean_udsgJetEfficiency * scaleFactor_udsg, udsg_index)
						* pow(1. - mean_udsgJetEfficiency * scaleFactor_udsg, numberOf_udsg_Jets - udsg_index);
			}
	if (probabilityMC == 0)
		return 0;
	return probabilityData / probabilityMC;
}

std::vector<double> BTagWeight::BjetWeights(const JetCollection jets, unsigned int numberOfBtags) const {
	//get b-jets
	const JetCollection bjets(getBJets(jets));
	//get c-jets
	const JetCollection cjets(getCJets(jets));
	//get udsg jets
	const JetCollection udsgjets(getUDSGJets(jets));

	//get mean scale factors
	double SF_b = getAverageBScaleFactor(bjets);
	double SF_c = getAverageCScaleFactor(cjets);
	double SF_udsg = getAverageUDSGScaleFactor(udsgjets);
	//get mean efficiencies
	double mean_bJetEfficiency = getAverageBEfficiency();
	double mean_cJetEfficiency = getAverageCEfficiency();
	double mean_udsgJetEfficiency = getAverageUDSGEfficiency(udsgjets);

	std::vector<double> event_weights;
	for (unsigned int nTag = 0; nTag < numberOfBtags; ++nTag) {
		double event_weight = weight(bjets.size(), cjets.size(), udsgjets.size(), mean_bJetEfficiency,
				mean_cJetEfficiency, mean_udsgJetEfficiency, SF_b, SF_c, SF_udsg, nTag);
		event_weights.push_back(event_weight);
	}
	return event_weights;
}

JetCollection BTagWeight::getBJets(const JetCollection jets) const {
	JetCollection bjets;
	for (unsigned int index = 0; index < jets.size(); ++index) {
		if (abs(jets.at(index)->partonFlavour()) == 5) //b-quark
			bjets.push_back(jets.at(index));
	}
	return bjets;
}

JetCollection BTagWeight::getCJets(const JetCollection jets) const {
	JetCollection cjets;
	for (unsigned int index = 0; index < jets.size(); ++index) {
		if (abs(jets.at(index)->partonFlavour()) == 4) //c-quark
			cjets.push_back(jets.at(index));
	}
	return cjets;
}

JetCollection BTagWeight::getUDSGJets(const JetCollection jets) const {
	JetCollection udsgjets;
	for (unsigned int index = 0; index < jets.size(); ++index) {
		if (abs(jets.at(index)->partonFlavour()) != 4 && abs(jets.at(index)->partonFlavour()) != 5) //not a c- or b-quark
			udsgjets.push_back(jets.at(index));
	}
	return udsgjets;
}

double BTagWeight::getAverageBScaleFactor(const JetCollection jets, double uncertaintyFactor) const {
	std::vector<double> scaleFactors;
	const boost::array<double, 14> SFb_error = { { 0.0295675, 0.0295095, 0.0210867, 0.0219349, 0.0227033, 0.0204062,
			0.0185857, 0.0256242, 0.0383341, 0.0409675, 0.0420284, 0.0541299, 0.0578761, 0.0655432 } };

	const boost::array<double, 15> ptbins = { { 30, 40, 50, 60, 70, 80, 100, 120, 160, 210, 260, 320, 400, 500 } };

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));
		double SFb(0);
		double sf_error(0);
		//these numbers are for CSVM only
		double pt = jet->pt();
		if (pt < 30) {
			SFb = 0.6981 * (1. + 0.414063 * 30) / (1. + 0.300155 * 30);
			sf_error = 0.12;
		} else if (pt > 670) {
			SFb = 0.6981 * (1. + 0.414063 * 670) / (1. + 0.300155 * 670);
			//use twice the uncertainty
			sf_error = 2 * SFb_error[SFb_error.size() - 1];
		} else {
			SFb = 0.6981 * (1. + 0.414063 * pt) / (1. + 0.300155 * pt);
			unsigned int ptbin(0);
			for (unsigned int bin = 0; bin < ptbins.size() + 1; ++bin) {
				double upperCut = index < ptbins.size() ? ptbins.at(index) : 670.;
				double lowerCut = index == 0 ? 0. : ptbins.at(index - 1);

				if (pt > lowerCut && pt < upperCut) {
					ptbin = bin;
					break;
				}
			}
			sf_error = SFb_error.at(ptbin);
		}
		SFb += sf_error * Globals::BJetSystematic * uncertaintyFactor;
		scaleFactors.push_back(SFb);
	}
	double sumOfScaleFactors = std::accumulate(scaleFactors.begin(), scaleFactors.end(), 0.0);
	if (scaleFactors.size() == 0)
		return 1.;
	else
		return sumOfScaleFactors / scaleFactors.size();
}

double BTagWeight::getAverageCScaleFactor(const JetCollection jets) const {
	return getAverageBScaleFactor(jets, 2.0); //SF_c = SF_b with twice the uncertainty
}

double BTagWeight::getAverageUDSGScaleFactor(const JetCollection jets) const {
	std::vector<double> scaleFactors;

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));
		double SF_udsg_mean(0), SF_udsg_min(0), SF_udsg_max(0);
		//these numbers are for CSVM only
		double pt = jet->pt();
		if (pt < 20) {
			continue;
		} else if (pt > 670) {
			SF_udsg_mean = getMeanUDSGScaleFactor(670.);
			SF_udsg_min = getMinUDSGScaleFactor(670);
			SF_udsg_max = getMaxUDSGScaleFactor(670);
			//use twice the uncertainty
			SF_udsg_min -= (SF_udsg_mean - SF_udsg_min);
			SF_udsg_max += (SF_udsg_max - SF_udsg_mean);
		} else {
			SF_udsg_mean = getMeanUDSGScaleFactor(pt);
			SF_udsg_min = getMinUDSGScaleFactor(pt);
			SF_udsg_max = getMaxUDSGScaleFactor(pt);
		}
		if (Globals::LightJetSystematic == -1)
			scaleFactors.push_back(SF_udsg_min);
		else if (Globals::LightJetSystematic == 0)
			scaleFactors.push_back(SF_udsg_mean);
		else if (Globals::LightJetSystematic == 1)
			scaleFactors.push_back(SF_udsg_max);
	}
	double sumOfScaleFactors = std::accumulate(scaleFactors.begin(), scaleFactors.end(), 0.0);
	if (scaleFactors.size() == 0)
		return 1.;
	else
		return sumOfScaleFactors / scaleFactors.size();
}

double BTagWeight::getMeanUDSGScaleFactor(double jetPT) const {
	return 1.04318 + 0.000848162 * jetPT - 2.5795e-06 * pow(jetPT, 2) + 1.64156e-09 * pow(jetPT, 3);
}

double BTagWeight::getMinUDSGScaleFactor(double jetPT) const {
	return 0.962627 + 0.000448344 * jetPT - 1.25579e-06 * pow(jetPT, 2) + 4.82283e-10 * pow(jetPT, 3);
}

double BTagWeight::getMaxUDSGScaleFactor(double jetPT) const {
	return 1.12368 + 0.00124806 * jetPT - 3.9032e-06 * pow(jetPT, 2) + 2.80083e-09 * pow(jetPT, 3);
}

double BTagWeight::getAverageBEfficiency() const {
	double discriminator_cut = 0.679; //== CSVM
	return -1.73338329789 * pow(discriminator_cut, 4) + 1.26161794785 * pow(discriminator_cut, 3)
			+ 0.784721653518 * pow(discriminator_cut, 2) + -1.03328577451 * discriminator_cut + 1.04305075822;

}

double BTagWeight::getAverageCEfficiency() const {
	double discriminator_cut = 0.679; //== CSVM
	return -1.5734604211 * pow(discriminator_cut, 4) + 1.52798999269 * pow(discriminator_cut, 3)
			+ 0.866697059943 * pow(discriminator_cut, 2) + -1.66657942274 * discriminator_cut + 0.780639301724;

}

double BTagWeight::getAverageUDSGEfficiency(const JetCollection jets) const {
	std::vector<double> efficiencies;

	for (unsigned int index = 0; index < jets.size(); ++index) {
		const JetPointer jet(jets.at(index));
		double efficiency(0);
		//these numbers are for CSVM only
		double pt = jet->pt();
		if (pt < 20) {
			continue;
		} else if (pt > 670) {
			efficiency = getMeanUDSGEfficiency(670.);
		} else {
			efficiency = getMeanUDSGEfficiency(pt);
		}
		efficiencies.push_back(efficiency);
	}
	double sumOfEfficiencies = std::accumulate(efficiencies.begin(), efficiencies.end(), 0.0);
	if (efficiencies.size() == 0)
		return 1.;
	else
		return sumOfEfficiencies / efficiencies.size();
}

double BTagWeight::getMeanUDSGEfficiency(double jetPT) const {
	return 0.0113428 + 5.18983e-05 * jetPT - 2.59881e-08 * pow(jetPT, 2);
}

}
