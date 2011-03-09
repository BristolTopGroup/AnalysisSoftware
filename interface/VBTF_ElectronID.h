/*
 * VBTF_ElectronID.h
 *
 *  Created on: 19 Jul 2010
 *      Author: kreczko
 */

#ifndef VBTF_ELECTRONID_H_
#define VBTF_ELECTRONID_H_

namespace BAT {
namespace VBTF_W70 {
const float MaximalSigmaIEtaIEta_BarrelRegion = 0.01;
const float MaximalDPhiIn_BarrelRegion = 0.03;
const float MaximalDEtaIn_BarrelRegion = 0.004;
const float MaximalHadOverEm_BarrelRegion = 0.025;

const float MaximalSigmaIEtaIEta_EndcapRegion = 0.03;
const float MaximalDPhiIn_EndcapRegion = 0.02;
const float MaximalDEtaIn_EndcapRegion = 0.005;
const float MaximalHadOverEm_EndcapRegion = 0.025;
}

namespace VBTF_W95 {
const float MaximalSigmaIEtaIEta_BarrelRegion = 0.01;
const float MaximalDPhiIn_BarrelRegion = 0.8;
const float MaximalDEtaIn_BarrelRegion = 0.007;
const float MaximalHadOverEm_BarrelRegion = 0.15;

const float MaximalSigmaIEtaIEta_EndcapRegion = 0.03;
const float MaximalDPhiIn_EndcapRegion = 0.7;
const float MaximalDEtaIn_EndcapRegion = 0.01;
const float MaximalHadOverEm_EndcapRegion = 0.07;
}
}

#endif /* VBTF_ELECTRONID_H_ */
