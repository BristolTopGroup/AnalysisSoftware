/*
 * Muon.cpp
 *
 *  Created on: Jun 25, 2010
 *      Author: lkreczko
 */

#include "../../interface/RecoObjects/Muon.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {
const double initialBigValue = 123456789;

Muon::Muon() :
		Lepton(), //
		usedAlgorithm_(MuonAlgorithm::Default), //
		isGlobalMuon_(false), //
		isTrackerMuon_(false), //
		isPFMuon_(false), //
		normalisedChi2_(initialBigValue), //
		numberOfValidMuonHits_(-1), //
		numberOfValidPixelHits_(-1), //
		numberOfValidHits_(-1), //
		pixelLayersWithMeasurement_(-1), //
		trackerLayersWithMeasurement_(-1), //
		numberOfMatches_(-1), //
		numberOfMatchedStations_(-1) {

}

Muon::Muon(double energy, double px, double py, double pz) :
		Lepton(energy, px, py, pz), //
		usedAlgorithm_(MuonAlgorithm::Default), //
		isGlobalMuon_(false), //
		isTrackerMuon_(false), //
		isPFMuon_(false), //
		normalisedChi2_(initialBigValue), //
		numberOfValidMuonHits_(-1), //
		numberOfValidPixelHits_(-1), //
		numberOfValidHits_(-1), //
		pixelLayersWithMeasurement_(-1), //
		trackerLayersWithMeasurement_(-1), //
		numberOfMatches_(-1), //
		numberOfMatchedStations_(-1) {

}

Muon::~Muon() {
}

bool Muon::isGlobal() const {
	return isGlobalMuon_;
}

bool Muon::isTracker() const {
	return isTrackerMuon_;
}

bool Muon::isPFMuon() const {
	return isPFMuon_;
}

void Muon::makeGlobal(bool global) {
	isGlobalMuon_ = global;
}

void Muon::setTrackerMuon(bool isTrackerMuon) {
	isTrackerMuon_ = isTrackerMuon;
}

void Muon::setPFMuon(bool pf) {
	isPFMuon_ = pf;
}

void Muon::setNormalisedChi2(double normChi2) {
	normalisedChi2_ = normChi2;
}

void Muon::setNumberOfValidHits(int nValidHits) {
	numberOfValidHits_ = nValidHits;
}

void Muon::setNumberOfValidMuonHits(int nValidHits) {
	numberOfValidMuonHits_ = nValidHits;
}

void Muon::setNumberOfValidPixelHits(int nValidHits) {
	numberOfValidPixelHits_ = nValidHits;
}

void Muon::setPixelLayersWithMeasurement(int pixelLayers) {
	pixelLayersWithMeasurement_ = pixelLayers;
}

void Muon::setNumberOfMatchedStations(int nMatchedStations) {
	numberOfMatchedStations_ = nMatchedStations;
}

void Muon::setNumberOfMatches(int nMatches) {
	numberOfMatches_ = nMatches;
}

bool Muon::isPFLepton() const {
	return usedAlgorithm_ == MuonAlgorithm::ParticleFlow;
}

void Muon::setUsedAlgorithm(MuonAlgorithm::value algorithm) {
	usedAlgorithm_ = algorithm;
}

void Muon::setTrackerLayersWithMeasurement(int layers) {
	trackerLayersWithMeasurement_ = layers;
}

int Muon::trackerLayersWithMeasurement() const {
	return trackerLayersWithMeasurement_;
}

int Muon::numberOfMatchedStations() const {
	return numberOfMatchedStations_;
}

int Muon::numberOfMatches() const {
	return numberOfMatches_;
}

int Muon::numberOfValidHits() const {
	return numberOfValidHits_;
}

int Muon::numberOfValidMuonHits() const {
	return numberOfValidMuonHits_;
}

int Muon::numberOfValidPixelHits() const {
	return numberOfValidPixelHits_;
}

int Muon::pixelLayersWithMeasurement() const {
	return pixelLayersWithMeasurement_;
}

double Muon::normChi2() const {
	return normalisedChi2_;
}

double Muon::getEfficiencyCorrection(bool qcd, int muon_scale_factor_systematic, int run_number) const {
	double correction(1.);
	double muEta(eta());
	double id_correction(0), iso_correction(0), trigger_correction(0), correction_A(0), correction_B(0), lumi_2011A(0), lumi_2011B(0), lumi_2011(0);

	// 7TeV scale factors from https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffs#2011_data (from 44X pickle file)
	if (Globals::energyInTeV == 7) { //Luminosity weighted average of 'combRelPFISO12_2011A' and 'combRelPFISO12_2011B' from pickle file
		//get scale factors based on muon eta bin
		if (fabs(muEta) <= 1.2) { // 'pt_abseta<1.2' in pickle file
			//get scale factors based on muon pt
			if ((10 <= pt()) && (pt() < 20)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = (0.97187329075 - 0.00895689491314);
					correction_B = (0.958160663139 - 0.0111131742061);
					break;
				case 1:
					correction_A = (0.97187329075 + 0.00895689491314);
					correction_B = (0.958160663139 + 0.0111131742061);
					break;
				default:
					correction_A = 0.97187329075;
					correction_B = 0.958160663139;
				}
			}
			else if ((20 <= pt()) && (pt() < 30)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 0.984568503228 - 0.00240687426577;
					correction_B = 0.975536117201 - 0.0029572697;
					break;
				case 1:
					correction_A = 0.984568503228 + 0.00240687426577;
					correction_B = 0.975536117201 + 0.0029572697;
					break;
				default:
					correction_A = 0.984568503228;
					correction_B = 0.975536117201;
				}
			}
			else if ((30 <= pt()) && (pt() < 40)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 0.992423030756 - 0.000954303610304;
					correction_B = 0.984073788145 - 0.00115894072787;
					break;
				case 1:
					correction_A = 0.992423030756 + 0.000954303610304;
					correction_B = 0.984073788145 + 0.00115894072787;
					break;
				default:
					correction_A = 0.992423030756;
					correction_B = 0.984073788145;
				}
			}
			else if ((40 <= pt()) && (pt() < 50)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 0.99535554365 - 0.000585467761915;
					correction_B = 0.990656279301 - 0.000349929656376;
					break;
				case 1:
					correction_A = 0.99535554365 + 0.000585467761915;
					correction_B = 0.990656279301 + 0.000349929656376;
					break;
				default:
					correction_A = 0.99535554365;
					correction_B = 0.990656279301;
				}
			}
			else if ((50 <= pt()) && (pt() < 60)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 0.992170058662 - 0.00107765420273;
					correction_B = 0.991084452935 - 0.00101971189879;
					break;
				case 1:
					correction_A = 0.992170058662 + 0.00107765420273;
					correction_B = 0.991084452935 + 0.00101971189879;
					break;
				default:
					correction_A = 0.992170058662;
					correction_B = 0.991084452935;
				}
			}
			else if ((60 <= pt()) && (pt() < 80)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 0.993872313396 - 0.00894417946891;
					correction_B = 0.98892037777 - 0.00174550565213;
					break;
				case 1:
					correction_A = 0.993872313396 + 0.00894417946891;
					correction_B = 0.98892037777 + 0.00174550565213;
					break;
				default:
					correction_A = 0.993872313396;
					correction_B = 0.98892037777;
				}
			}
			else if ((80 <= pt()) && (pt() < 250)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 0.993271247496 + 0.00348975304908;
					correction_B = 0.999774630941 - 0.00194422004257;
					break;
				case 1:
					correction_A = 0.993271247496 + 0.00348975304908;
					correction_B = 0.999774630941 + 0.00194422004257;
					break;
				default:
					correction_A = 0.993271247496;
					correction_B = 0.999774630941;
				}
			}
		}
		else if (fabs(muEta) > 1.2) { // 'pt_abseta>1.2' in pickle file
			//get scale factors based on muon pt
			if ((10 <= pt()) && (pt() < 20)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 0.989190191868 + 0.00717327834943;
					correction_B = 0.999006216758 - 0.00943036154567;
					break;
				case 1:
					correction_A = 0.989190191868 + 0.00717327834943;
					correction_B = 0.999006216758 + 0.00943036154567;
					break;
				default:
					correction_A = 0.989190191868;
					correction_B = 0.999006216758;
				}
			}
			else if ((20 <= pt()) && (pt() < 30)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 1.00271480782 - 0.00243884195789;
					correction_B = 1.00043597215 - 0.0030187008106;
					break;
				case 1:
					correction_A = 1.00271480782 + 0.00243884195789;
					correction_B = 1.00043597215 + 0.0030187008106;
					break;
				default:
					correction_A = 1.00271480782;
					correction_B = 1.00043597215;
				}
			}
			else if ((30 <= pt()) && (pt() < 40)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 1.00274598636 - 0.00114614223879;
					correction_B = 1.0019607868 - 0.00144883691237;
					break;
				case 1:
					correction_A = 1.00274598636 + 0.00114614223879;
					correction_B = 1.0019607868 + 0.00144883691237;
					break;
				default:
					correction_A = 1.00274598636;
					correction_B = 1.0019607868;
				}
			}
			else if ((40 <= pt()) && (pt() < 50)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 1.00158789661 - 0.000533242647022;
					correction_B = 1.00242767691 - 0.000305384682504;
					break;
				case 1:
					correction_A = 1.00158789661 + 0.000533242647022;
					correction_B = 1.00242767691 + 0.000305384682504;
					break;
				default:
					correction_A = 1.00158789661;
					correction_B = 1.00242767691;
				}
			}
			else if ((50 <= pt()) && (pt() < 60)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 0.99539699871 - 3.16725652446e-06;
					correction_B = 1.00113878923 - 0.00134269877895;
					break;
				case 1:
					correction_A = 0.99539699871 + 3.16725652446e-06;
					correction_B = 1.00113878923 + 0.00134269877895;
					break;
				default:
					correction_A = 0.99539699871;
					correction_B = 1.00113878923;
				}
			}
			else if ((60 <= pt()) && (pt() < 80)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 0.995568584925 - 0.00484818770698;
					correction_B = 0.993575837027 - 0.0108856854386;
					break;
				case 1:
					correction_A = 0.995568584925 + 0.00484818770698;
					correction_B = 0.993575837027 + 0.0108856854386;
					break;
				default:
					correction_A = 0.995568584925;
					correction_B = 0.993575837027;
				}
			}
			else if ((80 <= pt()) && (pt() < 250)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					correction_A = 0.988039073331 - 0.0029717666005;
					correction_B= 0.993270235255 - 0.00286742004516;
					break;
				case 1:
					correction_A = 0.988039073331 + 0.0029717666005;
					correction_B = 0.993270235255 + 0.00286742004516;
					break;
				default:
					correction_A = 0.988039073331;
					correction_B = 0.993270235255;
				}
			}
		}
		//luminosity weighted average of correction_A and correction_B:
		lumi_2011A = 2.311; //fb^-1
		lumi_2011B = 2.739; //fb^-1
		lumi_2011 = 5.050; //fb^-1
		correction = ((lumi_2011A/lumi_2011) * correction_A) + ((lumi_2011B/lumi_2011)*correction_B);

		//8TeV scale factors from https://twiki.cern.ch/twiki/bin/viewauth/CMS/MuonReferenceEffs#22Jan2013_ReReco_of_2012_data_re (from pickle files)
	} else if (Globals::energyInTeV == 8){ //corrections for ID ('Tight'), Iso ('combRelIsoPF04dBeta<012_Tight') and Trigger ('IsoMu24', 'TightID_IsodB') respectively (keys used in pickle file)
		if (fabs(muEta) < 0.9) { // 'ptabseta<0.9' in pickle file
			if ((10 <= pt()) && (pt() < 20)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9702748917248193 - 0.004982317322867869);
					iso_correction = (0.9403381500487888 - 0.004324350053532247);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				case 1:
					id_correction = (0.9702748917248193 + 0.00499703242820167);
					iso_correction = (0.9403381500487888 + 0.004318581625402814);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				default:
					id_correction = 0.9702748917248193;
					iso_correction = 0.9403381500487888;
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
				}
			}
			else if ((20 <= pt()) && (pt() < 25)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9888647105027486 - 0.001643440492591923);
					iso_correction = (0.9767384928595166 - 0.0021767900001671775);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				case 1:
					id_correction = (0.9888647105027486 + 0.0016383654526922649);
					iso_correction = (0.9767384928595166 + 0.0021821051076251698);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				default:
					id_correction = 0.9888647105027486;
					iso_correction = 0.9767384928595166;
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
				}
			}
			else if ((25 <= pt()) && (pt() < 30)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9923382289443887 - 0.0007738755871121118);
					iso_correction = (0.9961727553245637 - 0.0012289007276559836);
					trigger_correction = (0.9837252438433461 - 0.0010075240449770098);
					break;
				case 1:
					id_correction = (0.9923382289443887 + 0.0007699641249559135);
					iso_correction = (0.9961727553245637 + 0.0012260921898145376);
					trigger_correction = (0.9837252438433461 + 0.0010031080858819209);
					break;
				default:
					id_correction = 0.9923382289443887;
					iso_correction = 0.9961727553245637;
					trigger_correction = 0.9837252438433461;
				}
			}
			else if ((30 <= pt()) && (pt() < 35)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9932832435710769 - 0.0005371101221978478);
					iso_correction = (0.9932276697519965 - 0.0008000024510296704);
					trigger_correction = (0.9840634431547701 - 0.0006375087089531662);
					break;
				case 1:
					id_correction = (0.9932832435710769 + 0.0005350106861297489);
					iso_correction = (0.9932276697519965 + 0.000797997767987445);
					trigger_correction = (0.9840634431547701 + 0.000635352051297595);
					break;
				default:
					id_correction = 0.9932832435710769;
					iso_correction = 0.9932276697519965;
					trigger_correction = 0.9840634431547701;
				}
			}
			else if ((35 <= pt()) && (pt() < 40)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9936619045236875 - 0.000414816752718396);
					iso_correction = (0.993713125125077 - 0.0005441266874037132);
					trigger_correction = (0.9839165818168554 - 0.0004455899076950242);
					break;
				case 1:
					id_correction = (0.9936619045236875 + 0.000412461019925756);
					iso_correction = (0.993713125125077 + 0.0005427617384492858);
					trigger_correction = (0.9839165818168554 + 0.0004442222064760867);
					break;
				default:
					id_correction = 0.9936619045236875;
					iso_correction = 0.993713125125077;
					trigger_correction = 0.9839165818168554;
				}
			}
			else if ((40 <= pt()) && (pt() < 50)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9923918872503162 - 0.0002660261219219064);
					iso_correction = (0.9940841647892507 - 0.0002738361221515379);
					trigger_correction = (0.9834525270057036 - 0.00027066643447339175);
					break;
				case 1:
					id_correction = (0.9923918872503162 + 0.0002657187570658239);
					iso_correction = (0.9940841647892507 + 0.000273165921743292);
					trigger_correction = (0.9834525270057036 + 0.0002973682013735615);
					break;
				default:
					id_correction = 0.9923918872503162;
					iso_correction = 0.9940841647892507;
					trigger_correction = 0.9834525270057036;
				}
			}
			else if ((50 <= pt()) && (pt() < 60)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9911899786643166 - 0.0006554588808384644);
					iso_correction = (0.9964353940407351 - 0.0005008512469386801);
					trigger_correction = (0.9842917703915748 - 0.0006025738955897044);
					break;
				case 1:
					id_correction = (0.9911899786643166 + 0.0006523229773447217);
					iso_correction = (0.9964353940407351 + 0.0004974482080672698);
					trigger_correction = (0.9842917703915748 + 0.0006028357319538372);
					break;
				default:
					id_correction = 0.9911899786643166;
					iso_correction = 0.9964353940407351;
					trigger_correction = 0.9842917703915748;
				}
			}
			else if ((60 <= pt()) && (pt() < 90)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9894167956611032 - 0.0010379053444045262);
					iso_correction = (0.9986584316076454 - 0.0005847233489211289);
					trigger_correction = (0.9846720184248945 - 0.0009337102816812775);
					break;
				case 1:
					id_correction = (0.9894167956611032 + 0.001032321205147137);
					iso_correction = (0.9986584316076454 + 0.000577833172750727);
					trigger_correction = (0.9846720184248945 + 0.0009275608433477268);
					break;
				default:
					id_correction = 0.9894167956611032;
					iso_correction = 0.9986584316076454;
					trigger_correction = 0.9846720184248945;
				}
			}
			else if ((90 <= pt()) && (pt() < 140)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.0037489877154855 - 0.003163558210568693);
					iso_correction = (1.0003297152126076 - 0.001132956483897305);
					trigger_correction = (0.9809171165806959 - 0.003207045599196006);
					break;
				case 1:
					id_correction = (1.0037489877154855 + 0.0031360561818843846);
					iso_correction = (1.0003297152126076 + 0.0010892719712468886);
					trigger_correction = (0.9809171165806959 + 0.0031287565107191648);
					break;
				default:
					id_correction = 1.0037489877154855;
					iso_correction = 1.0003297152126076;
					trigger_correction = 0.9809171165806959;
				}
			}
			else if ((140 <= pt()) && (pt() < 300)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.0185025540365602 - 0.017259062669990236);
					iso_correction = (0.998812853360961 - 0.0028067206733671736);
					trigger_correction = (0.9804174981053351 - 0.008746682959269938); // given for pt '140_500' in pickle file
					break;
				case 1:
					id_correction = (1.0185025540365602 + 0.017381584793380422);
					iso_correction = (0.998812853360961 + 0.002470517643171293);
					trigger_correction = (0.9804174981053351 + 0.008133261348953102); // given for pt '140_500' in pickle file
					break;
				default:
					id_correction = 1.0185025540365602;
					iso_correction = 0.998812853360961;
					trigger_correction = 0.9804174981053351; // given for pt '140_500' in pickle file
				}
			}
		} else if ((fabs(muEta) >= 0.9 && fabs(muEta) < 1.2)) { // 'ptabseta0.9-1.2' in pickle file
			if ((10 <= pt()) && (pt() < 20)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.0017313093647269 - 0.006811572943202555);
					iso_correction = (0.9484342057793206 - 0.004820905035781334);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				case 1:
					id_correction = (1.0017313093647269 + 0.006843632524858654);
					iso_correction = (0.9484342057793206 + 0.0047987815547716394);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				default:
					id_correction = 1.0017313093647269;
					iso_correction = 0.9484342057793206;
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
				}
			}
			else if ((20 <= pt()) && (pt() < 25)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9939466451892177 - 0.0025039574199766336);
					iso_correction = (0.9863669311896185 - 0.0030800748311970065);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				case 1:
					id_correction = (0.9939466451892177 + 0.0024934393093577955);
					iso_correction = (0.9863669311896185 + 0.003062157005932066);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				default:
					id_correction = 0.9939466451892177;
					iso_correction = 0.9863669311896185;
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
				}
			}
			else if ((25 <= pt()) && (pt() < 30)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9947219657132994 - 0.001387585336030451);
					iso_correction = (1.0003560413214891 - 0.0020214031744360712);
					trigger_correction = (0.9683812755993191 - 0.002591557665753458);
					break;
				case 1:
					id_correction = (0.9947219657132994 + 0.0013775986784426517);
					iso_correction = (1.0003560413214891 + 0.00201228294958148);
					trigger_correction = (0.9683812755993191 + 0.002578757586626204);
					break;
				default:
					id_correction = 0.9947219657132994;
					iso_correction = 1.0003560413214891;
					trigger_correction = 0.9683812755993191;
				}
			}
			else if ((30 <= pt()) && (pt() < 35)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9933913481874554 - 0.0010439262890231983);
					iso_correction = (1.0000867333890535 - 0.001440363297378371);
					trigger_correction = (0.965380548896101 - 0.001901806697793774);
					break;
				case 1:
					id_correction = (0.9933913481874554 + 0.001035656088260019);
					iso_correction = (1.0000867333890535 + 0.0014337485842282113);
					trigger_correction = (0.965380548896101 + 0.0018925540568696933);
					break;
				default:
					id_correction = 0.9933913481874554;
					iso_correction = 1.0000867333890535;
					trigger_correction = 0.965380548896101;
				}
			}
			else if ((35 <= pt()) && (pt() < 40)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9922848270630622 - 0.0007691551185383584);
					iso_correction = (0.9990915568314974 - 0.0009298377105783976);
					trigger_correction = (0.9669651415167049 - 0.001442964213476463);
					break;
				case 1:
					id_correction = (0.9922848270630622 + 0.0007641440984878135);
					iso_correction = (0.9990915568314974 + 0.0009240671245245422);
					trigger_correction = (0.9669651415167049 + 0.001438586494549083);
					break;
				default:
					id_correction = 0.9922848270630622;
					iso_correction = 0.9990915568314974;
					trigger_correction = 0.9669651415167049;
				}
			}
			else if ((40 <= pt()) && (pt() < 50)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9918700391817427 - 0.000479106239467286);
					iso_correction = (0.9988684367869946 - 0.0004374630112646542);
					trigger_correction = (0.966679581608325 - 0.000530486707024449);
					break;
				case 1:
					id_correction = (0.9918700391817427 + 0.0004771945052710441);
					iso_correction = (0.9988684367869946 + 0.00043507001811199467);
					trigger_correction = (0.966679581608325 + 0.000529772216686672);
					break;
				default:
					id_correction = 0.9918700391817427;
					iso_correction = 0.9988684367869946;
					trigger_correction = 0.966679581608325;
				}
			}
			else if ((50 <= pt()) && (pt() < 60)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.995010062219747 - 0.0012348803272911781);
					iso_correction = (0.9978903999478148 - 0.0008054649238451882);
					trigger_correction = (0.9627395755250187 - 0.0019327403824020816);
					break;
				case 1:
					id_correction = (0.995010062219747 + 0.0012248477719783437);
					iso_correction = (0.9978903999478148 + 0.0007940293028615975);
					trigger_correction = (0.9627395755250187 + 0.0019297478815599588);
					break;
				default:
					id_correction = 0.995010062219747;
					iso_correction = 0.9978903999478148;
					trigger_correction = 0.9627395755250187;
				}
			}
			else if ((60 <= pt()) && (pt() < 90)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.99040605451701 - 0.0019600150019625376);
					iso_correction = (0.9992149992903525 - 0.000994456913054677);
					trigger_correction = (0.9595241683475331 - 0.003042186855462462);
					break;
				case 1:
					id_correction = (0.99040605451701 + 0.0019408960540877624);
					iso_correction = (0.9992149992903525 + 0.0009704078907222729);
					trigger_correction = (0.9595241683475331 + 0.003022110290623624);
					break;
				default:
					id_correction = 0.99040605451701;
					iso_correction = 0.9992149992903525;
					trigger_correction = 0.9595241683475331;
				}
			}
			else if ((90 <= pt()) && (pt() < 140)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.0090275981046186 - 0.00628190755085892);
					iso_correction = (1.0014212099113895 - 0.0018536242011445699);
					trigger_correction = (0.9644418246112644 - 0.010200424767055799);
					break;
				case 1:
					id_correction = (1.0090275981046186 + 0.006194544939853352);
					iso_correction = (1.0014212099113895 + 0.0017077193014169727);
					trigger_correction = (0.9644418246112644 + 0.010055105850685283);
					break;
				default:
					id_correction = 1.0090275981046186;
					iso_correction = 1.0014212099113895;
					trigger_correction = 0.9644418246112644;
				}
			}
			else if ((140 <= pt()) && (pt() < 300)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.010956056270124 - 0.034515129519407155);
					iso_correction = (1.0018540386614887 - 0.005585413192511395);
					trigger_correction = (0.9712789619617556 - 0.02743437935083038); // given for pt '140_500' in pickle file
					break;
				case 1:
					id_correction = (1.010956056270124 + 0.03207362331383597);
					iso_correction = (1.0018540386614887 + 0.004324712404120638);
					trigger_correction = (0.9712789619617556 + 0.025376053351416492); // given for pt '140_500' in pickle file
					break;
				default:
					id_correction = 1.010956056270124;
					iso_correction = 1.0018540386614887;
					trigger_correction = 0.9712789619617556; // given for pt '140_500' in pickle file
				}
			}
		} else if ((fabs(muEta) >= 1.2 && fabs(muEta) < 2.1)) { // 'ptabseta1.2-2.1' in pickle file
			if ((10 <= pt()) && (pt() < 20)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.0180184284620057 - 0.003787312420833548);
					iso_correction = (0.9724367240900078 - 0.0023234729482449396);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				case 1:
					id_correction = (1.0180184284620057 + 0.0037948316398176323);
					iso_correction = (0.9724367240900078 + 0.00231474447946669);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				default:
					id_correction = 1.0180184284620057;
					iso_correction = 0.9724367240900078;
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
				}
			}
			else if ((20 <= pt()) && (pt() < 25)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.0003513994342943 - 0.0014175455381989866);
					iso_correction = (0.9900544131155273 - 0.0015337801457362742);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				case 1:
					id_correction = (1.0003513994342943 + 0.0014140119111793995);
					iso_correction = (0.9900544131155273 + 0.001528926500845685);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				default:
					id_correction = 1.0003513994342943;
					iso_correction = 0.9900544131155273;
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
				}
			}
			else if ((25 <= pt()) && (pt() < 30)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9984860359215375 - 0.0008594891073122313);
					iso_correction = (1.0028199004178158 - 0.0010371165852728213);
					trigger_correction = (1.0051991254438037 - 0.0017823280400050144);
					break;
				case 1:
					id_correction = (0.9984860359215375 + 0.0008560586071446904);
					iso_correction = (1.0028199004178158 + 0.0010333761270942092);
					trigger_correction = (1.0051991254438037 + 0.0017782159910835391);
					break;
				default:
					id_correction = 0.9984860359215375;
					iso_correction = 1.0028199004178158;
					trigger_correction = 1.0051991254438037;
				}
			}
			else if ((30 <= pt()) && (pt() < 35)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9965584500063546 - 0.0007065827787686896);
					iso_correction = (1.0040456735929117 - 0.0007942077368265961);
					trigger_correction = (1.0013781590159485 - 0.0013715807241176374);
					break;
				case 1:
					id_correction = (0.9965584500063546 + 0.0007037175367824803);
					iso_correction = (1.0040456735929117 + 0.0007906927040862312);
					trigger_correction = (1.0013781590159485 + 0.0013693380303715827);
					break;
				default:
					id_correction = 0.9965584500063546;
					iso_correction = 1.0040456735929117;
					trigger_correction = 1.0013781590159485;
				}
			}
			else if ((35 <= pt()) && (pt() < 40)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.996026448928709 - 0.0005876149628036986);
					iso_correction = (1.0021465355614672 - 0.0005643853519922642);
					trigger_correction = (0.99616640424792 - 0.0011118453938292981);
					break;
				case 1:
					id_correction = (0.996026448928709 + 0.0005847865225019135);
					iso_correction = (1.0021465355614672 + 0.0005621070062466183);
					trigger_correction = (0.99616640424792 + 0.0011101094832906267);
					break;
				default:
					id_correction = 0.996026448928709;
					iso_correction = 1.0021465355614672;
					trigger_correction = 0.99616640424792;
				}
			}
			else if ((40 <= pt()) && (pt() < 50)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9960618126306268 - 0.00023914919466043162);
					iso_correction = (1.0009158373782485 - 0.0002604376167052789);
					trigger_correction = (0.9942541014104305 - 0.0007155191193513695);
					break;
				case 1:
					id_correction = (0.9960618126306268 + 0.00023889542772002653);
					iso_correction = (1.0009158373782485 + 0.00026012261483755164);
					trigger_correction = (0.9942541014104305 + 0.0007158145625096358);
					break;
				default:
					id_correction = 0.9960618126306268;
					iso_correction = 1.0009158373782485;
					trigger_correction = 0.9942541014104305;
				}
			}
			else if ((50 <= pt()) && (pt() < 60)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9951827274866013 - 0.0009262393069269017);
					iso_correction = (1.0004039805545037 - 0.0004849685117981604);
					trigger_correction = (0.990544673012178 - 0.0015867039726075091);
					break;
				case 1:
					id_correction = (0.9951827274866013 + 0.0009218578127467899);
					iso_correction = (1.0004039805545037 + 0.00047966845893161465);
					trigger_correction = (0.990544673012178 + 0.001581754019526234);
					break;
				default:
					id_correction = 0.9951827274866013;
					iso_correction = 1.0004039805545037;
					trigger_correction = 0.990544673012178;
				}
			}
			else if ((60 <= pt()) && (pt() < 90)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9924861810666658 - 0.0015526240554359254);
					iso_correction = (1.00050232016842 - 0.0006064036321286692);
					trigger_correction = (0.9882937419288585 - 0.0025430635002634187);
					break;
				case 1:
					id_correction = (0.9924861810666658 + 0.0015447865068057166);
					iso_correction = (1.00050232016842 + 0.0005957445733490045);
					trigger_correction = (0.9882937419288585 + 0.002533529446019015);
					break;
				default:
					id_correction = 0.9924861810666658;
					iso_correction = 1.00050232016842;
					trigger_correction = 0.9882937419288585;
				}
			}
			else if ((90 <= pt()) && (pt() < 140)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.023129380083607 - 0.005380834815775498);
					iso_correction = (0.9991578234416382 - 0.0012614746219956135);
					trigger_correction = (0.9818759899390823 - 0.008157385522907809);
					break;
				case 1:
					id_correction = (1.023129380083607 + 0.00535135313393348);
					iso_correction = (0.9991578234416382 + 0.0011892673238781191);
					trigger_correction = (0.9818759899390823 + 0.008060505027357067);
					break;
				default:
					id_correction = 1.023129380083607;
					iso_correction = 0.9991578234416382;
					trigger_correction = 0.9818759899390823;
				}
			}
			else if ((140 <= pt()) && (pt() < 300)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9747541719440429 - 0.029172667852370958);
					iso_correction = (0.9964395746067783 - 0.0032089887704663577);
					trigger_correction = (0.9941686682904833 - 0.027846431972317096); // given for pt '140_500' in pickle file
					break;
				case 1:
					id_correction = (0.9747541719440429 + 0.030098827448524764);
					iso_correction = (0.9964395746067783 + 0.0023591072371684303);
					trigger_correction = (0.9941686682904833 + 0.027302993818728562); // given for pt '140_500' in pickle file
					break;
				default:
					id_correction = 0.9747541719440429;
					iso_correction = 0.9964395746067783;
					trigger_correction = 0.9941686682904833; // given for pt '140_500' in pickle file
				}
			}
		} else if (fabs(muEta) >= 2.1 && fabs(muEta) <= 2.4) { // 'ptabseta2.1-2.4' in pickle file. Note: Trigger scale factors only provided up to absolute eta of 2.1 in the link above, so I have used the same as for the 1.2 to 2.1 eta range.
			if ((10 <= pt()) && (pt() < 20)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.0050443332472756 - 0.006984425170970493);
					iso_correction = (1.1167270384985806 - 0.005564792415329163);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				case 1:
					id_correction = (1.0050443332472756 + 0.007015146334748514);
					iso_correction = (1.1167270384985806 + 0.0055447969541930105);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				default:
					id_correction = 1.0050443332472756;
					iso_correction = 1.1167270384985806;
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
				}
			}
			else if ((20 <= pt()) && (pt() < 25)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9980890826544107 - 0.0029278411327994035);
					iso_correction = (1.1155400869984835 - 0.004077789818862577);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				case 1:
					id_correction = (0.9980890826544107 + 0.0029203985501578306);
					iso_correction = (1.1155400869984835 + 0.00406505554629319);
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
					break;
				default:
					id_correction = 0.9980890826544107;
					iso_correction = 1.1155400869984835;
					trigger_correction = 1.0; // corrections only provided for pt>=25 so 1.0 used
				}
			}
			else if ((25 <= pt()) && (pt() < 30)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9961828904505218 - 0.001798763326811934);
					iso_correction = (1.096718301830121 - 0.002847410975399852);
					trigger_correction = (1.0051991254438037 - 0.0017823280400050144);
					break;
				case 1:
					id_correction = (0.9961828904505218 + 0.00178971651288244);
					iso_correction = (1.096718301830121 + 0.0028355134012020564);
					trigger_correction = (1.0051991254438037 + 0.0017782159910835391);
					break;
				default:
					id_correction = 0.9961828904505218;
					iso_correction = 1.096718301830121;
					trigger_correction = 1.0051991254438037;
				}
			}
			else if ((30 <= pt()) && (pt() < 35)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.000551051988681 - 0.00151251709208394);
					iso_correction = (1.074812169170078 - 0.00216397977125747);
					trigger_correction = (1.0013781590159485 - 0.0013715807241176374);
					break;
				case 1:
					id_correction = (1.000551051988681 + 0.0015035662104497108);
					iso_correction = (1.074812169170078 + 0.0021554430112260894);
					trigger_correction = (1.0013781590159485 + 0.0013693380303715827);
					break;
				default:
					id_correction = 1.000551051988681;
					iso_correction = 1.074812169170078;
					trigger_correction = 1.0013781590159485;
				}
			}
			else if ((35 <= pt()) && (pt() < 40)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9925634188516349 - 0.0013364320973364);
					iso_correction = (1.0605853576108657 - 0.001668443166949562);
					trigger_correction =  (0.99616640424792 - 0.0011118453938292981);
					break;
				case 1:
					id_correction = (0.9925634188516349 + 0.0013291528785315703);
					iso_correction = (1.0605853576108657 + 0.0016608666126636076);
					trigger_correction = (0.99616640424792 + 0.0011101094832906267);
					break;
				default:
					id_correction = 0.9925634188516349;
					iso_correction = 1.0605853576108657;
					trigger_correction = 0.99616640424792;
				}
			}
			else if ((40 <= pt()) && (pt() < 50)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9951441282077462 - 0.000985529534415516);
					iso_correction = (1.0377065931130076 - 0.0009233708589486433);
					trigger_correction = (0.9942541014104305 - 0.0007155191193513695);
					break;
				case 1:
					id_correction = (0.9951441282077462 + 0.0009820065022356174);
					iso_correction = (1.0377065931130076 + 0.0009184892169446883);
					trigger_correction = (0.9942541014104305 + 0.0007158145625096358);
					break;
				default:
					id_correction = 0.9951441282077462;
					iso_correction = 1.0377065931130076;
					trigger_correction = 0.9942541014104305;
				}
			}
			else if ((50 <= pt()) && (pt() < 60)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.993590319966609 - 0.0026554371737963407);
					iso_correction = (1.0252924559186363 - 0.0016918474028294471);
					trigger_correction = (0.990544673012178 - 0.0015867039726075091);
					break;
				case 1:
					id_correction = (0.993590319966609 + 0.002640140803174166);
					iso_correction = (1.0252924559186363 + 0.0016693106217563717);
					trigger_correction = (0.990544673012178 + 0.001581754019526234);
					break;
				default:
					id_correction = 0.993590319966609;
					iso_correction = 1.0252924559186363;
					trigger_correction = 0.990544673012178;
				}
			}
			else if ((60 <= pt()) && (pt() < 90)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.9894841861002339 - 0.004989294206167835);
					iso_correction = (1.0149496896687298 - 0.0021464301366117796);
					trigger_correction = (0.9882937419288585 - 0.0025430635002634187);
					break;
				case 1:
					id_correction = (0.9894841861002339 + 0.004971901577796156);
					iso_correction = (1.0149496896687298 + 0.0020941202601336955);
					trigger_correction = (0.9882937419288585 + 0.002533529446019015);
					break;
				default:
					id_correction = 0.9894841861002339;
					iso_correction = 1.0149496896687298;
					trigger_correction = 0.9882937419288585;
				}
			}
			else if ((90 <= pt()) && (pt() < 140)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (1.0601733432927236 - 0.015680312313880205);
					iso_correction = (1.0081292468302308 - 0.004701272504751554);
					trigger_correction = (0.9818759899390823 - 0.008157385522907809);
					break;
				case 1:
					id_correction = (1.0601733432927236 + 0.010351620474848782);
					iso_correction = (1.0081292468302308 + 0.0043568597346185555);
					trigger_correction = (0.9818759899390823 + 0.008060505027357067);
					break;
				default:
					id_correction = 1.0601733432927236;
					iso_correction = 1.0081292468302308;
					trigger_correction = 0.9818759899390823;
				}
			}
			else if ((140 <= pt()) && (pt() < 300)) {
				switch (muon_scale_factor_systematic) {
				case -1:
					id_correction = (0.890546814737379 - 0.16003113987502632);
					iso_correction = (1.0106204522044593 - 0.020879578336130224);
					trigger_correction = (0.9941686682904833 - 0.027846431972317096); // given for pt '140_500' in pickle file
					break;
				case 1:
					id_correction = (0.890546814737379 + 0.12406238997906899);
					iso_correction = (1.0106204522044593 + 0.014974686230990275);
					trigger_correction = (0.9941686682904833 + 0.027302993818728562); // given for pt '140_500' in pickle file
					break;
				default:
					id_correction = 0.890546814737379;
					iso_correction = 1.0106204522044593;
					trigger_correction = 0.9941686682904833; // given for pt '140_500' in pickle file
				}
			}
		}
		//Combine the 3 values for the final correction; QCD has non-isolated muon hence remove isolation scale factor if QCD
		if (qcd == false) {
			correction = id_correction * iso_correction * trigger_correction;
		}
		else {
			correction = id_correction * trigger_correction;
		}
	}
	return correction;
}
}
