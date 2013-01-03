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
	virtual void setPFPUChargedHadronIsolation(double isolation, double coneSize = 0.3);
	virtual void setPFIsolationWithGaussianFallOff(double iso, double coneSize = 0.3);
	virtual void setPFRelativeIsolationRho(double isolation);
	virtual void setPFDeltaBeta_Isolation_DR04(double isolation);
	virtual void setsumChargedHadronPt04(double isolation);
	virtual void setsumNeutralHadronPt04(double isolation);
	virtual void setsumPhotonPt04(double isolation);
	virtual void setsumPUPt04(double isolation);
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
	virtual double PfPUChargedHadronIso(double coneSize = 0.3) const;
	virtual double pfRelativeIsolation(double coneSize = 0.3, bool deltaBetaCorrection = true) const;
	virtual double pfIsolation(double coneSize = 0.3, bool deltaBetaCorrection = true) const;
	virtual double pfRelativeIsolationPUCorrected(double rho, double coneSize = 0.3) const;
	virtual double pfIsolationWithGaussianFallOff(double coneSize = 0.3) const;
	virtual double PFDeltaBeta_Isolation_DR04() const;
	virtual double sumChargedHadronPt04() const;
	virtual double sumNeutralHadronPt04() const;
	virtual double sumPhotonPt04() const;
	virtual double sumPUPt04() const;
	//directional particle based isolation
	virtual double directionalIsolation(double coneSize = 0.2) const;
	virtual double directionalIsolationWithGaussianFallOff(double coneSize = 0.2) const;
	//other
	virtual double ZDistanceToPrimaryVertex() const;
	virtual std::string toString() const;

protected:
	double ecal_Isolation_DR03_, hcal_Isolation_DR03_, tracker_Isolation_DR03_;
	double ecal_Isolation_DR04_, hcal_Isolation_DR04_, tracker_Isolation_DR04_;
	double ecal_Isolation_DR05_, hcal_Isolation_DR05_, tracker_Isolation_DR05_;
	double PFGamma_Isolation_DR03_, PFChargedHadron_Isolation_DR03_, PFNeutralHadron_Isolation_DR03_,PFPUChargedHadron_Isolation_DR03_;
	double PFGamma_Isolation_DR04_, PFChargedHadron_Isolation_DR04_, PFNeutralHadron_Isolation_DR04_, PFPUChargedHadron_Isolation_DR04_;
	double PFGamma_Isolation_DR05_, PFChargedHadron_Isolation_DR05_, PFNeutralHadron_Isolation_DR05_, PFPUChargedHadron_Isolation_DR05_;
	double PFDeltaBeta_Isolation_DR04_;
	double sumChargedHadronPt04_, sumNeutralHadronPt04_, sumPhotonPt04_, sumPUPt04_;
	double PFRelativeIsolationRho_DR03_;
	double zDistanceToPrimaryVertex_, directionalIsolation_DR02_, directionalIsolation_DR03_;
	double  directionalIsolation_GaussianFallOff_DR02_, directionalIsolation_GaussianFallOff_DR03_;
	double  pfIsolation_GaussianFallOff_DR02_, pfIsolation_GaussianFallOff_DR03_;

};

typedef boost::shared_ptr<Lepton> LeptonPointer;
typedef boost::ptr_vector<Lepton> LeptonCollection;

} /* namespace BAT */
#endif /* LEPTON_H_ */
