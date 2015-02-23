/*
 * SelectionOutputInfo.h
 *
 *  Created on: Dec 10, 2014
 *      Author: Emyr Clement
 */

#ifndef SELECTIONOUTPUTINFO_H_
#define SELECTIONOUTPUTINFO_H_
#include <vector>
#include <string>
#include <boost/array.hpp>
#include "Jet.h"

namespace BAT {


class SelectionOutputInfo {
public:
	SelectionOutputInfo();
	virtual ~SelectionOutputInfo();

	//getters
	unsigned int getNumberOfJets() const;
	unsigned int getNumberOfBJets() const;
	unsigned int getSignalLeptonIndex() const;
	std::vector<unsigned int> getCleanedJetIndex() const;
	std::vector<unsigned int> getCleanedBJetIndex() const;

	//setters
	void setNumberOfJets( unsigned int nJets );
	void setNumberOfBJets( unsigned int nBJets );
	void setSignalLeptonIndex( unsigned int signalLeptonIndices );
	void setCleanedJetIndex( std::vector<unsigned int> cleanedJetsIndices );
	void setCleanedBJetIndex( std::vector<unsigned int> cleanedBJetsIndices );

private:
	unsigned int nJets_, nBJets_;
	unsigned int signalLeptonIndex_;
	std::vector<unsigned int> cleanedJetsIndices_;
	std::vector<unsigned int> cleanedBJetsIndices_;
};

}

#endif /* SELECTIONOUTPUTINFO_H_ */
