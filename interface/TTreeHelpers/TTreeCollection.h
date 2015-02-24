/*
 * TTreeCollection.h
 *
 *  Created on: 5 Feb 2015
 *      Author: ejclemen
 */

#ifndef TBRANCHCOLLECTION_H_
#define TBRANCHCOLLECTION_H_
#include <string>
#include <assert.h>
#include "TFile.h"
#include "TDirectory.h"
#include "TTree.h"
#include "TBranch.h"

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include "../HistHelpers/StringOperations.h"

namespace BAT {

class TBranchCollection {
    typedef boost::shared_ptr<TBranch> TBranchPointer;
    typedef boost::shared_ptr<TTree> TTreePointer;
    typedef boost::shared_ptr<float> VarPointer;
    typedef boost::unordered_map<std::string, TTreePointer> TTreeMap;
    typedef boost::unordered_map<std::string, TBranchPointer> TBranchMap;
    typedef boost::unordered_map<std::string, VarPointer> VariableMap;

protected:
    boost::shared_ptr<TFile> tTreeFile;
    std::string path;
    std::vector<std::string> directories;
    std::vector<TTreePointer> trees;
    std::string prefix, suffix;
public:

    // FIXME Put as protected and provide getters and setters
    TTreeMap treeMap;
    VariableMap varMap;

    TBranchCollection(std::string virtualPath = "") :
        tTreeFile(),
        path(virtualPath),
        directories(getDirectoriesFromPath(path)),
        trees(),
        prefix(""),
        suffix(""),
        treeMap(),
        varMap() {

    }

    ~TBranchCollection() {

    }

    void addBranchToTree(std::string branchName, std::string branchLabel, TTreePointer tree);

    void setBranchVariable(std::string branchLabel, float value);

    unsigned int size() const;

    void writeToFile(boost::shared_ptr<TFile> treeFile);

    bool contains(std::string name);

private:
    void writeDirectories();
    void writeTrees();

};

typedef boost::shared_ptr<TBranchCollection > TBranchCollectionRef;

}

#endif /* TBRANCHCOLLECTION_H_ */
