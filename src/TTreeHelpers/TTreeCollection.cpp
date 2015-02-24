/*
 * TTreeCollection.cpp
 *
 *  Created on: 5 Feb 2015
 *      Author: ejclemen
 */

#include "../../interface/TTreeHelpers/TTreeCollection.h"
#include <iostream>

using namespace std;

namespace BAT {

void TBranchCollection::addBranchToTree(std::string branchName, std::string branchLabel, TTreePointer tree) {

	// Make variable associated with this branch
	varMap[branchLabel] = VarPointer(new float() );
	// Make branch
	tree->Branch( branchName.c_str(), varMap[branchLabel].get(), ( branchName+"/F").c_str() );
}

void TBranchCollection::addBranchToTree(std::string branchName, std::string branchLabel, TTreePointer tree);

TBranchCollection::VarPointer TBranchCollection::getBranchVariable(std::string branchLabel) {
    return varMap[branchLabel];
}

void TBranchCollection::setBranchVariable(std::string branchLabel, float value) {
    *varMap[branchLabel] = value;
}

TBranchCollection::TTreePointer TBranchCollection::getBranchTree(std::string branchLabel) {
    return treeMap[branchLabel];
}

unsigned int TBranchCollection::size() const {
    return varMap.size();
}

void TBranchCollection::writeToFile(boost::shared_ptr<TFile> treeFile) {
    tTreeFile = treeFile;
    tTreeFile->cd();
    writeDirectories();
    tTreeFile->Cd(path.c_str());
    writeTrees();
}

void TBranchCollection::setPrefix(std::string pre){
    prefix = pre;
}

void TBranchCollection::setSuffix(std::string suf){
    suffix = suf;
}

const TBranchCollection::VariableMap TBranchCollection::getAllVariables(){
    return varMap;
}

bool TBranchCollection::contains(std::string name){
    return varMap.find(name) != varMap.end();
}

void TBranchCollection::writeDirectories() {
    std::string currentPath = "";

    for (unsigned short index = 0; index < directories.size(); ++index) {
        const std::string dir = directories.at(index);

        if (index == 0) {
            if (tTreeFile->Get(dir.c_str()) == 0)
                tTreeFile->mkdir(dir.c_str());
            currentPath = dir;
        } else {
            TDirectory* currentDir = (TDirectory*) tTreeFile->Get(currentPath.c_str());
            assert(currentDir != 0);
            if (currentDir->Get(dir.c_str()) == 0)
                currentDir->mkdir(dir.c_str());
            currentPath += "/" + dir;
        }
    }
}

void TBranchCollection::writeTrees() {
    for ( boost::unordered_map<std::string, TTreePointer>::const_iterator iter = treeMap.begin(); iter != treeMap.end(); ++iter) {
        iter->second->Write();
    }
}

}
