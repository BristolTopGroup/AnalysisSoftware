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
	bool getAllEvents() const;
	bool getAtLeastFourGoodJets() const;
	bool getAtLeastThreeGoodJets() const;
	bool getAtLeastTwoGoodJets() const;
	bool getAtLeastOneGoodJet() const;
	bool getAtLeastOneBTag() const;
	bool getAtLeastTwoBTags() const;
	bool getConversionVeto() const;
	bool getLooseElectronVeto() const;
	bool getLooseMuonVeto() const;
	//setters
	void setNumberOfJets( unsigned int nJets );
	void setNumberOfBJets( unsigned int nBJets );
	void setSignalLeptonIndex( unsigned int signalLeptonIndices );
	void setCleanedJetIndex( std::vector<unsigned int> cleanedJetsIndices );
	void setCleanedBJetIndex( std::vector<unsigned int> cleanedBJetsIndices );
	void setAllEvents( bool AllEvents);
	void setAtLeastFourGoodJets( bool AtLeastFourGoodJets);
	void setAtLeastThreeGoodJets( bool AtLeastThreeGoodJets);
	void setAtLeastTwoGoodJets( bool AtLeastTwoGoodJets);
	void setAtLeastOneGoodJet( bool AtLeastOneGoodJet);
	void setAtLeastOneBTag( bool AtLeastOneBTag);
	void setAtLeastTwoBTags( bool AtLeastTwoBTags);
	void setConversionVeto( bool ConversionVeto);
	void setLooseElectronVeto( bool LooseElectronVeto);
	void setLooseMuonVeto( bool LooseMuonVeto);
private:
	unsigned int nJets_, nBJets_;
	unsigned int signalLeptonIndex_;
	std::vector<unsigned int> cleanedJetsIndices_;
	std::vector<unsigned int> cleanedBJetsIndices_;
	bool AllEvents_;
	bool AtLeastFourGoodJets_;
	bool AtLeastThreeGoodJets_;
	bool AtLeastTwoGoodJets_;
	bool AtLeastOneGoodJet_;
	bool AtLeastOneBTag_;
	bool AtLeastTwoBTags_;
	bool ConversionVeto_;
	bool LooseElectronVeto_;
	bool LooseMuonVeto_;
};

}

#endif /* SELECTIONOUTPUTINFO_H_ */
