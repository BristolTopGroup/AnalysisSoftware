/*
 * Lepton.h
 *
 *  Created on: 30 Oct 2011
 *      Author: kreczko
 */

#ifndef LEPTON_H_
#define LEPTON_H_

#include "Particle.h"
#include <boost/ptr_container/ptr_vector.hpp>
namespace BAT {

class Lepton: public BAT::Particle {
public:
	Lepton();
	Lepton(double energy, double px, double py, double pz);
	virtual ~Lepton() = 0;

	virtual bool isGood(short leptonID) const = 0;
	virtual bool isLoose() const = 0;

	virtual bool isPFLepton() const = 0;

	virtual void setEcalIsolation(double isolation);
	virtual void setHcalIsolation(double isolation);
	virtual void setTrackerIsolation(double isolation);
	virtual double ecalIsolation() const;
	virtual double hcalIsolation() const;
	virtual double trackerIsolation() const;
	virtual double directionalIsolation() const;

	virtual void setPFGammaIsolation(double pfGammaIso);
	virtual void setPFChargedHadronIsolation(double chargedHadronIso);
	virtual void setPFNeutralHadronIsolation(double neutralHadronIso);
	virtual double PFGammaIsolation() const;
	virtual double PFChargedHadronIsolation() const;
	virtual double PFNeutralHadronIsolation() const;

	virtual double relativeIsolation() const;
	virtual double pfIsolation() const;

	virtual void setZDistanceToPrimaryVertex(double dist);
	virtual void setDirectionalIsolation(double iso);

protected:
	double ecal_Isolation, hcal_Isolation, tracker_Isolation;
	double PFGamma_Isolation, PFChargedHadron_Isolation, PFNeutralHadron_Isolation;
	double zDistanceToPrimaryVertex, directionalIsolation_;

};

typedef boost::shared_ptr<Lepton> LeptonPointer;
typedef boost::ptr_vector<Lepton> LeptonCollection;

} /* namespace BAT */
#endif /* LEPTON_H_ */
