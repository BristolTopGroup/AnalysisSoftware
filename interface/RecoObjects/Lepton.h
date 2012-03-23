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

	//flags
	virtual bool isGood(short leptonID) const = 0;
	virtual bool isLoose() const = 0;
	virtual bool isPFLepton() const = 0;

	//setters
	//detector isolation
	virtual void setEcalIsolation(double isolation, double coneSize = 0.3);
	virtual void setHcalIsolation(double isolation, double coneSize = 0.3);
	virtual void setTrackerIsolation(double isolation, double coneSize = 0.3);
	//particle based isolation
	virtual void setPFGammaIsolation(double pfGammaIso, double coneSize = 0.3);
	virtual void setPFChargedHadronIsolation(double chargedHadronIso, double coneSize = 0.3);
	virtual void setPFNeutralHadronIsolation(double neutralHadronIso, double coneSize = 0.3);
	virtual void setPFIsolationWithGaussianFallOff(double iso, double coneSize = 0.3);
	//directional particle based isolation
	virtual void setDirectionalIsolation(double iso, double coneSize = 0.3);
	virtual void setDirectionalIsolationWithGaussianFallOff(double iso, double coneSize = 0.3);
	//other
	virtual void setZDistanceToPrimaryVertex(double dist);

	//getters
	//detector isolation
	virtual double ecalIsolation(double coneSize = 0.3) const;
	virtual double hcalIsolation(double coneSize = 0.3) const;
	virtual double trackerIsolation(double coneSize = 0.3) const;
	virtual double relativeIsolation(double coneSize = 0.3) const;
	//particle based isolation
	virtual double PFGammaIsolation(double coneSize = 0.3) const;
	virtual double PFChargedHadronIsolation(double coneSize = 0.3) const;
	virtual double PFNeutralHadronIsolation(double coneSize = 0.3) const;
	virtual double pfRelativeIsolation(double coneSize = 0.3) const;
	virtual double pfRelativeIsolationPUCorrected(double rho, double coneSize = 0.3) const;
	virtual double pfIsolationWithGaussianFallOff(double coneSize = 0.3) const;
	//directional particle based isolation
	virtual double directionalIsolation(double coneSize = 0.2) const;
	virtual double directionalIsolationWithGaussianFallOff(double coneSize = 0.2) const;
	//other
	virtual double ZDistanceToPrimaryVertex() const;

protected:
	double ecal_Isolation_DR03_, hcal_Isolation_DR03_, tracker_Isolation_DR03_;
	double ecal_Isolation_DR04_, hcal_Isolation_DR04_, tracker_Isolation_DR04_;
	double ecal_Isolation_DR05_, hcal_Isolation_DR05_, tracker_Isolation_DR05_;
	double PFGamma_Isolation_DR03_, PFChargedHadron_Isolation_DR03_, PFNeutralHadron_Isolation_DR03_;
	double PFGamma_Isolation_DR04_, PFChargedHadron_Isolation_DR04_, PFNeutralHadron_Isolation_DR04_;
	double PFGamma_Isolation_DR05_, PFChargedHadron_Isolation_DR05_, PFNeutralHadron_Isolation_DR05_;
	double zDistanceToPrimaryVertex_, directionalIsolation_DR02_, directionalIsolation_DR03_;
	double  directionalIsolation_GaussianFallOff_DR02_, directionalIsolation_GaussianFallOff_DR03_;
	double  pfIsolation_GaussianFallOff_DR02_, pfIsolation_GaussianFallOff_DR03_;

};

typedef boost::shared_ptr<Lepton> LeptonPointer;
typedef boost::ptr_vector<Lepton> LeptonCollection;

} /* namespace BAT */
#endif /* LEPTON_H_ */
