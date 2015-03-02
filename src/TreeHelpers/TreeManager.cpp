/*
 * TreeManager.cpp
 *
 *  Created on: 5 Feb 2015
 *      Author: ejclemen
 */

#include "../../interface/TreeHelpers/TreeManager.h"
#include "../../interface/Readers/NTupleEventReader.h"
#include "../../interface/GlobalVariables.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>


using namespace boost;
using namespace std;
namespace BAT {

TreeManager::TreeManager() : //
		seenDataTypes_(), //
		treeFiles_(), //
		currentDataType_(DataType::ElectronHad), //
		currentFolder_(""), //
		collection_() {
}

TreeManager::~TreeManager() {
}

void TreeManager::addBranch(std::string branchLabel, std::string varType, std::string treeName, bool isSingleValuePerEvent) {

	// Check if folder exists
	// And check if tree of the same name in this folder exists
	if (collection_.find(currentFolder_) == collection_.end() )
		addFolder(currentFolder_);

	for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes_.at(type)) {
			if (  collection_[currentFolder_][type]->treeMap_.find(currentFolder_) == collection_[currentFolder_][type]->treeMap_.end() ) {
				addTreeToFolder(treeName, currentFolder_, type);				
			}

			// Add branch to tree
			collection_[currentFolder_][type]->addBranchToTree(branchLabel, varType, collection_[currentFolder_].at(type)->treeMap_[currentFolder_], isSingleValuePerEvent);
		}
	}

}

void TreeManager::Fill(std::string branchLabel, float fillValue) {
	collection_[currentFolder_][currentDataType_]->setBranchVariable( branchLabel, fillValue );
}

void TreeManager::FillTrees() {
	// Loop over all known trees, and call "Fill"
	for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes_.at(type)) {

			for ( boost::unordered_map<std::string, boost::shared_ptr<TTree>>::const_iterator iter = collection_[currentFolder_][type]->treeMap_.begin(); iter != collection_[currentFolder_][type]->treeMap_.end(); ++iter) {
				iter->second->Fill();
			}

		    // Reset all variables
			collection_[currentFolder_][type]->resetBranchVariables();

		}
    }
}

void TreeManager::setCurrentDataType(DataType::value type) {
	currentDataType_ = type;
}

void TreeManager::prepareForSeenDataTypes( const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes_ ) {
	this->seenDataTypes_ = seenDataTypes_;

	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes_.at(type)) {
			const std::string filename = "tree_" + assembleFilename((DataType::value) type);
			boost::shared_ptr<TFile> file(new TFile(filename.c_str(), "RECREATE"));
			file->SetCompressionLevel(7);

			treeFiles_.at(type) = file;
			collection_[""].at(type) = TBranchCollectionRef(new TBranchCollection());
		}
	}
}

const std::string TreeManager::assembleFilename(DataType::value type) const {
	const std::string name = DataType::names[type];
	std::stringstream str;
	std::string electronAlgo = ElectronAlgorithm::names[Globals::electronAlgorithm];
	std::string jetAlgo = JetAlgorithm::names[Globals::jetAlgorithm];
	std::string metAlgo = METAlgorithm::names[Globals::metAlgorithm];
	std::string muonAlgo = MuonAlgorithm::names[Globals::muonAlgorithm];
	std::string electronSyst = (Globals::ElectronScaleFactorSystematic > 0) ? "_plusElectron" : "_minusElectron";
	std::string muonSyst = (Globals::MuonScaleFactorSystematic > 0) ? "_plusMuon" : "_minusMuon";
	std::string jesSyst = (Globals::JESsystematic > 0) ? "_plusJES" : "_minusJES";
	std::string jetSmearingSyst = (Globals::JetSmearingSystematic > 0) ? "_plusJER" : "_minusJER";
	std::string bjetSyst = (Globals::BJetSystematic > 0) ? "_plusBjet" : "_minusBJet";
	std::string lightJetSyst = (Globals::LightJetSystematic > 0) ? "_plusLightJet" : "_minusLightJet";
	std::string suffix = Globals::custom_file_suffix;
	std::string pdfWeight =
			(Globals::pdfWeightNumber != 0) ? "_PDF_" + boost::lexical_cast<std::string>(Globals::pdfWeightNumber) : "";
	if (Globals::ElectronScaleFactorSystematic == 0)
		electronSyst = "";
	if (Globals::MuonScaleFactorSystematic == 0)
		muonSyst = "";
	if (Globals::JESsystematic == 0)
		jesSyst = "";
	if (Globals::JetSmearingSystematic == 0)
		jetSmearingSyst = "";
	if (Globals::BJetSystematic == 0)
		bjetSyst = "";
	if (Globals::LightJetSystematic == 0)
		lightJetSyst = "";
	if (suffix != "")
		suffix = "_" + suffix;

	str << name << "_" << Globals::luminosity << "pb";
	str << "_" << electronAlgo << "_" << muonAlgo << "_" << jetAlgo << "_" << metAlgo;
	str << electronSyst << muonSyst << jesSyst << jetSmearingSyst << pdfWeight << bjetSyst << lightJetSyst << suffix << ".root";
	return str.str();

}

void TreeManager::writeToDisk() {
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes_.at(type)) {
			for (unordered_map<std::string, TBranchArray>::iterator iter = collection_.begin(); iter != collection_.end();
					++iter) {
				iter->second[type]->writeToFile(treeFiles_.at(type));
			}

			treeFiles_.at(type)->Write();
			treeFiles_.at(type)->Close();
		}

	}
}
void TreeManager::addFolder(string folder)
 {
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes_.at(type))
			collection_[folder].at(type) = TBranchCollectionRef(new TBranchCollection(folder));
	}
}

void TreeManager::addTreeToFolder(string treeName, string folder, unsigned int dataType) {
			collection_[folder].at(dataType)->treeMap_[folder] = boost::shared_ptr<TTree>(new TTree(treeName.c_str(),treeName.c_str()));
			collection_[folder].at(dataType)->treeMap_[folder]->SetDirectory(0);
			collection_[folder].at(dataType)->addBranchToTree("EventWeight","F",collection_[folder].at(dataType)->treeMap_[folder]);
}

void TreeManager::setCurrentFolder(std::string collection) {
	currentFolder_ = collection;
}

} //end namespace BAT
