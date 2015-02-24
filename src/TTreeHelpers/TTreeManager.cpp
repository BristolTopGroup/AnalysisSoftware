/*
 * TTreeManager.cpp
 *
 *  Created on: 5 Feb 2015
 *      Author: ejclemen
 */

#include "../../interface/TTreeHelpers/TTreeManager.h"
#include "../../interface/Readers/NTupleEventReader.h"
#include "../../interface/GlobalVariables.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>


using namespace boost;
using namespace std;
namespace BAT {

TTreeManager::TTreeManager() : //
		seenDataTypes(), //
		treeFiles(), //
		currentDataType(DataType::ElectronHad), //
		currentTreeFolder(""), //
		collection() {
}

TTreeManager::~TTreeManager() {
}

void TTreeManager::addBranch(std::string branchName, std::string branchLabel, std::string treeName) {

	// Check if folder exists
	// And check if tree of the same name in this folder exists
	if (collection.find(currentTreeFolder) == collection.end() )
		addFolder(currentTreeFolder);

	for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			if (  collection[currentTreeFolder][type]->treeMap.find(currentTreeFolder) == collection[currentTreeFolder][type]->treeMap.end() ) {
				addTreeToFolder(currentTreeFolder, type, treeName);				
			}

			// Add branch to tree
			collection[currentTreeFolder][type]->addBranchToTree(branchName, branchLabel, collection[currentTreeFolder].at(type)->treeMap[currentTreeFolder]);
		}
	}

}

void TTreeManager::Fill(std::string branchLabel, float fillValue) {
	collection[currentTreeFolder][currentDataType]->setBranchVariable( branchLabel, fillValue );
}

void TTreeManager::FillTrees() {
	// Loop over all known trees, and call "Fill"
	for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {

			for ( boost::unordered_map<std::string, boost::shared_ptr<TTree>>::const_iterator iter = collection[currentTreeFolder][type]->treeMap.begin(); iter != collection[currentTreeFolder][type]->treeMap.end(); ++iter) {
				iter->second->Fill();
			}
		}
    }
}

void TTreeManager::setCurrentDataType(DataType::value type) {
	currentDataType = type;
}

void TTreeManager::prepareForSeenDataTypes( const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes ) {
	this->seenDataTypes = seenDataTypes;

	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			const std::string filename = "tree_" + assembleFilename((DataType::value) type);
			boost::shared_ptr<TFile> file(new TFile(filename.c_str(), "RECREATE"));
			file->SetCompressionLevel(7);

			treeFiles.at(type) = file;
			collection[""].at(type) = TBranchCollectionRef(new TBranchCollection());
		}
	}
}

const std::string TTreeManager::assembleFilename(DataType::value type) const {
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

void TTreeManager::writeToDisk() {
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			for (unordered_map<std::string, TBranchArray>::iterator iter = collection.begin(); iter != collection.end();
					++iter) {
				iter->second[type]->writeToFile(treeFiles.at(type));
			}

			treeFiles.at(type)->Write();
			treeFiles.at(type)->Close();
		}

	}
}
void TTreeManager::addFolder(string folder)
 {
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type))
			collection[folder].at(type) = TBranchCollectionRef(new TBranchCollection(folder));
	}
}

void TTreeManager::addTreeToFolder(string folder, unsigned int dataType, string treeName) {
			collection[folder].at(dataType)->treeMap[folder] = boost::shared_ptr<TTree>(new TTree(treeName.c_str(),treeName.c_str()));
			collection[folder].at(dataType)->treeMap[folder]->SetDirectory(0);
			collection[folder].at(dataType)->addBranchToTree("EventWeight","EventWeight",collection[folder].at(dataType)->treeMap[folder]);
}

void TTreeManager::setCurrentTreeFolder(std::string collection) {
	currentTreeFolder = collection;
}

} //end namespace BAT
