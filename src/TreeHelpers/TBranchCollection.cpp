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

void TBranchCollection::addBranchToTree(std::string branchLabel, std::string varType, TreePointer tree, bool isSingleValuePerEvent) {

    if ( isSingleValuePerEvent ) {
        // Make variable associated with this branch
        varMap_[branchLabel] = VarPointer(new float() );
        // Make branch
        tree->Branch( branchLabel.c_str(), varMap_[branchLabel].get(), ( branchLabel+"/"+varType).c_str() );
    }
    else {
        // Make variable associated with this branch
        varVectorMap_[branchLabel] = VarVectorPointer(new std::vector<float>() );
        // Make branch
        tree->Branch( branchLabel.c_str(), "std::vector<float>", varVectorMap_[branchLabel].get() );    
    }
}

void TBranchCollection::setBranchVariable(std::string branchLabel, float value) {
    if ( contains(branchLabel) ) {
        *varMap_[branchLabel] = value;
    }
    else if ( vectorContains(branchLabel) ) {
        varVectorMap_[branchLabel]->push_back(value);
    }
}

void TBranchCollection::resetBranchVariables() {
    for ( VariableMap::const_iterator iter = varMap_.begin(); iter != varMap_.end(); ++iter) {
        *iter->second = -99;        
    }
    for ( VariableVectorMap::const_iterator iter = varVectorMap_.begin(); iter != varVectorMap_.end(); ++iter) {
        iter->second->clear();        
    }

}

unsigned int TBranchCollection::size() const {
    return varMap_.size();
}

bool TBranchCollection::contains(std::string name){
    return varMap_.find(name) != varMap_.end();
}

bool TBranchCollection::vectorContains(std::string name){
    return varVectorMap_.find(name) != varVectorMap_.end();
}

void TBranchCollection::writeDirectory( std::string folder, boost::shared_ptr<TFile> treeFile ) {
    std::string currentPath = "";

        // if (index == 0) {
        if (treeFile->Get(folder.c_str()) == 0)
            treeFile->mkdir(folder.c_str());
        // } else {
        //     TDirectory* currentDir = (TDirectory*) treeFile->Get(currentPath.c_str());
        //     assert(currentDir != 0);
        //     if (currentDir->Get(dir.c_str()) == 0)
        //         currentDir->mkdir(dir.c_str());
        //     currentPath += "/" + dir;
        // }
    }

}
