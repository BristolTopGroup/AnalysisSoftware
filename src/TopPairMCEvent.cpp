/*
 * TopPairMCEvent.cpp
 *
 *  Created on: 18 Mar 2014
 *      Author: phxlk
 */

#include "../interface/TopPairMCEvent.h"

namespace BAT {

TopPairMCEvent::TopPairMCEvent() :
		top_quark_(), //
		anti_top_quark_(), //
		W_boson_1_(), //
		W_boson_2_(), //
		quark_1_from_tW_vertex_(), //
		quark_2_from_tW_vertex_(), //
		particles_from_W_1_(), //
		particles_from_W_2_(), //
		number_of_electrons_(0), //
		number_of_muons_(0), //
		number_of_taus_(0) {

}

TopPairMCEvent::TopPairMCEvent(const MCParticleCollection particles) :
		top_quark_(), //
		anti_top_quark_(), //
		W_boson_1_(), //
		W_boson_2_(), //
		quark_1_from_tW_vertex_(), //
		quark_2_from_tW_vertex_(), //
		number_of_electrons_(0), //
		number_of_muons_(0), //
		number_of_taus_(0) {

	for (unsigned int i = 0; i < particles.size(); ++i) {
		MCParticlePointer particle = particles.at(i);
		MCParticlePointer mother(new MCParticle());
		if (particle->motherIndex() >= 0 && particle->motherIndex() < particles.size())
			mother = particles.at(particle->motherIndex());

		//first, lets find the two top quarks
		if (abs(particle->pdgId()) == top_PDG_ID && particle->status() == 3) {
			if (particle->pdgId() > 0)
				top_quark_ = particle;
			else
				anti_top_quark_ = particle;
		}

		//next the quarks from the top decay
		if (abs(particle->pdgId()) < top_PDG_ID && abs(mother->pdgId()) == top_PDG_ID) {
			// quark from top quark
			if (particle->pdgId() < 0)
				quark_1_from_tW_vertex_ = particle;
			else
				quark_2_from_tW_vertex_ = particle;
		}

		// now the W bosons
		// for MC@NLO we are looking for status 2 W bosons, otherwise for status 3
		if (abs(particle->pdgId()) == W_PDG_ID && particle->status() == 3) {
			// W from top quark
			if (particle->pdgId() > 0)
				W_boson_1_ = particle;
			else
				W_boson_2_ = particle;
		}

		//finally the decay products of the W
		// W1
		if (W_boson_1_) {
			if (abs(particle->pdgId()) != W_PDG_ID && particle->status() == 3
					&& mother->pdgId() == W_boson_1_->pdgId()) {
				particles_from_W_1_.push_back(particle);
			}
		}
		if (W_boson_2_) {
			if (abs(particle->pdgId()) != W_PDG_ID && particle->status() == 3
					&& mother->pdgId() == W_boson_2_->pdgId()) {
				particles_from_W_2_.push_back(particle);
			}
		}
	}
	// now count the number of leptons
	for (unsigned int i = 0; i < particles_from_W_1_.size(); ++i) {
		MCParticlePointer particle = particles_from_W_1_.at(i);
		if (abs(particle->pdgId()) == e_PDG_ID)
			number_of_electrons_++;
		else if (abs(particle->pdgId()) == mu_PDG_ID)
			number_of_muons_++;
		else if (abs(particle->pdgId()) == tau_PDG_ID)
			number_of_taus_++;
	}
	for (unsigned int i = 0; i < particles_from_W_2_.size(); ++i) {
		MCParticlePointer particle = particles_from_W_2_.at(i);
		if (abs(particle->pdgId()) == e_PDG_ID)
			number_of_electrons_++;
		else if (abs(particle->pdgId()) == mu_PDG_ID)
			number_of_muons_++;
		else if (abs(particle->pdgId()) == tau_PDG_ID)
			number_of_taus_++;
	}
}

TopPairDecays TopPairMCEvent::get_top_pair_decay() {
	unsigned int sum_leptons = number_of_electrons_ + number_of_muons_ + number_of_taus_;
	if (sum_leptons == 2) { // fully leptonic
		if (number_of_electrons_ == 2)
			return full_leptonic_electron_electron;
		else if (number_of_muons_ == 2)
			return full_leptonic_muon_muon;
		else if (number_of_taus_ == 2)
			return full_leptonic_tau_tau;
		else if (number_of_electrons_ == 1 && number_of_muons_ == 1)
			return full_leptonic_electron_muon;
		else if (number_of_electrons_ == 1 && number_of_taus_ == 1)
			return full_leptonic_electron_tau;
		else
			return full_leptonic_muon_tau;
	} else if (sum_leptons == 1) {
		//semi leptonic
		if (number_of_electrons_ == 1)
			return semi_leptonic_electron;
		else if (number_of_muons_ == 1)
			return semi_leptonic_muon;
		else
			return semi_leptonic_tau;
	} else
		return full_hadronic;
}

bool TopPairMCEvent::is_semi_leptonic() {
	TopPairDecays decay = get_top_pair_decay();
	return decay == semi_leptonic_electron || decay == semi_leptonic_muon || decay == semi_leptonic_tau;
}

const MCParticlePointer TopPairMCEvent::get_single_lepton() {
	if (is_semi_leptonic()) {
		MCParticleCollection all_particles_from_W;
		all_particles_from_W.reserve(particles_from_W_1_.size() + particles_from_W_2_.size()); // preallocate memory
		all_particles_from_W.insert(all_particles_from_W.end(), particles_from_W_1_.begin(), particles_from_W_1_.end());
		all_particles_from_W.insert(all_particles_from_W.end(), particles_from_W_2_.begin(), particles_from_W_2_.end());

		int particle_index(-1);
		for (unsigned int i = 0; i < all_particles_from_W.size(); ++i) {
			MCParticlePointer particle = all_particles_from_W.at(i);
			unsigned int id = abs(particle->pdgId());
			if (id == e_PDG_ID || id == mu_PDG_ID || id == tau_PDG_ID) {
				particle_index = i;
				break;
			}
		}
		if (particle_index >= 0)
			return all_particles_from_W.at(particle_index);
	}
	return MCParticlePointer();
}

const MCParticlePointer TopPairMCEvent::get_single_neutrino() {
	if (is_semi_leptonic()) {
		MCParticleCollection all_particles_from_W;
		all_particles_from_W.reserve(particles_from_W_1_.size() + particles_from_W_2_.size()); // preallocate memory
		all_particles_from_W.insert(all_particles_from_W.end(), particles_from_W_1_.begin(), particles_from_W_1_.end());
		all_particles_from_W.insert(all_particles_from_W.end(), particles_from_W_2_.begin(), particles_from_W_2_.end());

		int particle_index(-1);
		for (unsigned int i = 0; i < all_particles_from_W.size(); ++i) {
			MCParticlePointer particle = all_particles_from_W.at(i);
			unsigned int id = abs(particle->pdgId());
			if (id == nu_e_PDG_ID || id == nu_mu_PDG_ID || id == nu_tau_PDG_ID) {
				particle_index = i;
				break;
			}
		}
		if (particle_index >= 0)
			return all_particles_from_W.at(particle_index);
	}

	return MCParticlePointer();
}

TopPairMCEvent::~TopPairMCEvent() {
}

} /* namespace BAT */
