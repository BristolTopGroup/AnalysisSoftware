/*
 * TopPairMCEvent.h
 *
 *  Created on: 18 Mar 2014
 *      Author: phxlk
 */

#ifndef TOPPAIRMCEVENT_H_
#define TOPPAIRMCEVENT_H_
#include "../interface/RecoObjects/MCParticle.h"

// PDG constants from
// http://pdg.lbl.gov/2002/montecarlorpp.pdf
#define top_PDG_ID 6
#define e_PDG_ID 11
#define nu_e_PDG_ID 12
#define mu_PDG_ID 13
#define nu_mu_PDG_ID 14
#define tau_PDG_ID 15
#define nu_tau_PDG_ID 16
#define W_PDG_ID 24

namespace BAT {

enum TopPairDecays {
	full_hadronic, //
	semi_leptonic_electron, //
	semi_leptonic_muon, //
	semi_leptonic_tau, //
	full_leptonic_electron_electron, //
	full_leptonic_electron_muon, //
	full_leptonic_electron_tau, //
	full_leptonic_muon_muon, //
	full_leptonic_muon_tau, //
	full_leptonic_tau_tau, //
};

class TopPairMCEvent {
public:
	TopPairMCEvent();
	TopPairMCEvent(const MCParticleCollection particles);
	virtual ~TopPairMCEvent();

	/**
	 * Returns true if the event is a semi-leptonic event
	 */
	bool is_semi_leptonic();
	const MCParticlePointer get_single_lepton();
	const MCParticlePointer get_single_neutrino();
	TopPairDecays get_top_pair_decay();
private:
	// top quark is positive, anti-top quark is negative
	MCParticlePointer top_quark_, anti_top_quark_;
	// let's not make the same distinction for W-bosons
	MCParticlePointer W_boson_1_, W_boson_2_;
	// This is *almost* (99.99% or so) always a b-quark.
	MCParticlePointer quark_1_from_tW_vertex_, quark_2_from_tW_vertex_;
	MCParticleCollection particles_from_W_1_,particles_from_W_2_;
	unsigned int number_of_electrons_,number_of_muons_,number_of_taus_;

};

} /* namespace BAT */

#endif /* TOPPAIRMCEVENT_H_ */
