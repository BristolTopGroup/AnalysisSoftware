/*
 * QCDAntiIDEPlusJetsSelection.cpp
 *
 *  Created on: 28 May 2012
 *      Author: kreczko
 */

#include "../../interface/Selections/QCDAntiIDEPlusJetsSelection.h"

namespace BAT {

QCDAntiIDEPlusJetsSelection::QCDAntiIDEPlusJetsSelection(unsigned int numberOfSelectionSteps) :
		TopPairEPlusJetsReferenceSelection(numberOfSelectionSteps) {

}

bool QCDAntiIDEPlusJetsSelection::isGoodElectron(const ElectronPointer electron) const {
	bool passesEtAndEta = electron->et() > 30. && fabs(electron->eta()) < 2.5 && !electron->isInCrack();
	bool passesD0 = fabs(electron->d0()) < 0.02; //cm
	bool passesDistanceToPV = fabs(electron->ZDistanceToPrimaryVertex()) < 1;

	bool passSigmaIetaIeta(false);
	bool passDphiIn(false);
	bool passDetaIn(false);
	bool passHoverE(false);

	if(electron->isInBarrelRegion()){
		passSigmaIetaIeta = electron->sigmaIEtaIEta() < 0.01;
		passDphiIn= fabs(electron->dPhiIn()) < 0.06;
		passDetaIn= fabs(electron->dEtaIn()) < 0.004;
		passHoverE = electron->HadOverEm() < 0.12;
	}
	if(electron->isInEndCapRegion()){
		passSigmaIetaIeta = electron->sigmaIEtaIEta() < 0.03;
		passDphiIn= fabs(electron->dPhiIn()) < 0.03;
		passDetaIn= fabs(electron->dEtaIn()) < 0.007;
		passHoverE = electron->HadOverEm() < 0.1;
	}

	bool failsTwoOfFourWP80Conditions((passSigmaIetaIeta + passDphiIn + passDetaIn + passHoverE) < 2);

	return passesEtAndEta && passesD0 && passesDistanceToPV && failsTwoOfFourWP80Conditions;
}

bool QCDAntiIDEPlusJetsSelection::isIsolated(const LeptonPointer lepton) const{
	return lepton->pfRelativeIsolation(Globals::electronIsolationCone) < 0.2;
}

QCDAntiIDEPlusJetsSelection::~QCDAntiIDEPlusJetsSelection() {
}

} /* namespace BAT */
