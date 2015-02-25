/*
 * TBranchCollection.cpp
 *
 *  Created on: 5 Feb 2015
 *      Author: ejclemen
 */

#include "../../interface/TreeHelpers/TBranchCollection.h"
#include <iostream>

using namespace std;

namespace BAT {

void TBranchCollection::addBranchToTree(std::string branchName, std::string branchLabel, TreePointer tree) {

	// Make variable associated with this branch
	varMap_[branchLabel] = VarPointer(new float() );
	// Make branch
	tree->Branch( branchName.c_str(), varMap_[branchLabel].get(), ( branchName+"/F").c_str() );
}

void TBranchCollection::addBranchToTree(std::string branchName, std::string branchLabel, TreePointer tree);

void TBranchCollection::setBranchVariable(std::string branchLabel, float value) {
    *varMap_[branchLabel] = value;
}

unsigned int TBranchCollection::size() const {
    return varMap_.size();
}

void TBranchCollection::writeToFile(boost::shared_ptr<TFile> treeFile) {
    treeFile_ = treeFile;
    treeFile_->cd();
    writeDirectories();
    treeFile_->Cd(path_.c_str());
    writeTrees();
}

bool TBranchCollection::contains(std::string name){
    return varMap_.find(name) != varMap_.end();
}

void TBranchCollection::writeDirectories() {
    std::string currentPath = "";

    for (unsigned short index = 0; index < directories_.size(); ++index) {
        const std::string dir = directories_.at(index);

        if (index == 0) {
            if (treeFile_->Get(dir.c_str()) == 0)
                treeFile_->mkdir(dir.c_str());
            currentPath = dir;
        } else {
            TDirectory* currentDir = (TDirectory*) treeFile_->Get(currentPath.c_str());
            assert(currentDir != 0);
            if (currentDir->Get(dir.c_str()) == 0)
                currentDir->mkdir(dir.c_str());
            currentPath += "/" + dir;
        }
    }
}

void TBranchCollection::writeTrees() {
    for ( boost::unordered_map<std::string, TreePointer>::const_iterator iter = treeMap_.begin(); iter != treeMap_.end(); ++iter) {
        iter->second->Write();
    }
}

}
