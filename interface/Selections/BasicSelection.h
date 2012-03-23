/*
 * BasicSelection.h
 *
 *  Created on: 25 Feb 2012
 *      Author: kreczko
 */

#ifndef BASICSELECTION_H_
#define BASICSELECTION_H_

#include "../Event.h"
#include "../RecoObjects/Jet.h"
#include "../RecoObjects/Electron.h"
#include "../RecoObjects/Muon.h"

#include <vector>
#include <string>

#include <boost/shared_ptr.hpp>

namespace BAT {

class BasicSelection;
typedef boost::shared_ptr<BasicSelection> SelectionPointer;

class BasicSelection {
public:
	BasicSelection(unsigned int numberOfSelectionSteps);
	virtual ~BasicSelection();
	//definitions of good objects
	virtual bool isGoodJet(const JetPointer jet) const = 0;
	virtual bool isGoodElectron(const ElectronPointer electron) const = 0;
	virtual bool isGoodMuon(const MuonPointer electron) const = 0;
	//definitions of loose objects
	virtual bool isLooseElectron(const ElectronPointer electron) const = 0;
	virtual bool isLooseMuon(const MuonPointer electron) const = 0;

	virtual bool passesFullSelection(const EventPtr event) const;
	virtual bool passesSelectionStep(const EventPtr event, unsigned int selectionStep) const = 0;
	virtual bool passesFullSelectionExceptStep(const EventPtr event, unsigned int selectionStep) const; //N-1 cuts
	virtual bool passesSelectionUpToStep(const EventPtr event, unsigned int selectionStep) const;

private:
	unsigned int numberOfSelectionSteps_;
};

} /* namespace BAT */
#endif /* BASICSELECTION_H_ */
