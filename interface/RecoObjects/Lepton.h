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
	virtual void setPFRelIso03( double isolation );
	virtual void setPFRelIso04( double isolation );
	virtual void setPFRelIso03DeltaBeta( double isolation );
	virtual void setPFRelIso04DeltaBeta( double isolation );
    virtual void setPFRelIsoWithEA( double isolation );
	virtual void setsumChargedHadronPt03(double isolation);
	virtual void setsumNeutralHadronPt03(double isolation);
	virtual void setsumPhotonPt03(double isolation);
	virtual void setsumPUPt03(double isolation);
	virtual void setsumChargedHadronPt04(double isolation);
	virtual void setsumNeutralHadronPt04(double isolation);
	virtual void setsumPhotonPt04(double isolation);
	virtual void setsumPUPt04(double isolation);

	//other
	virtual void setZDistanceToPrimaryVertex(double dist);

	//getters
	//detector isolation
	virtual double ecalIsolation(double coneSize = 0.3) const;
	virtual double hcalIsolation(double coneSize = 0.3) const;
	virtual double trackerIsolation(double coneSize = 0.3) const;
	virtual double relativeIsolation(double coneSize = 0.3) const;

	virtual double PFRelIso03() const;
	virtual double PFRelIso04() const;
	virtual double PFRelIso03DeltaBeta() const;
	virtual double PFRelIso04DeltaBeta() const;	
    virtual double PFRelIsoWithEA() const;

	virtual double sumChargedHadronPt03() const;
	virtual double sumNeutralHadronPt03() const;
	virtual double sumPhotonPt03() const;
	virtual double sumPUPt03() const;
	virtual double sumChargedHadronPt04() const;
	virtual double sumNeutralHadronPt04() const;
	virtual double sumPhotonPt04() const;
	virtual double sumPUPt04() const;

	//other
	virtual double ZDistanceToPrimaryVertex() const;

protected:
	double ecal_Isolation_DR03_, hcal_Isolation_DR03_, tracker_Isolation_DR03_;

	double PFRelIso03_, PFRelIso04_;
	double PFRelIso03DeltaBeta_, PFRelIso04DeltaBeta_, PFRelIsoWithEA_;
	double sumChargedHadronPt03_, sumNeutralHadronPt03_, sumPhotonPt03_, sumPUPt03_;
	double sumChargedHadronPt04_, sumNeutralHadronPt04_, sumPhotonPt04_, sumPUPt04_;

	double zDistanceToPrimaryVertex_;

};

typedef boost::shared_ptr<Lepton> LeptonPointer;
typedef boost::ptr_vector<Lepton> LeptonCollection;

} /* namespace BAT */
#endif /* LEPTON_H_ */
