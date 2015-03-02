/*
 * TBranchCollection.h
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
    typedef boost::shared_ptr<TTree> TreePointer;
    typedef boost::shared_ptr<float> VarPointer;
    typedef boost::shared_ptr<std::vector<float> > VarVectorPointer;
    typedef boost::unordered_map<std::string, TreePointer> TreeMap;
    typedef boost::unordered_map<std::string, TBranchPointer> TBranchMap;
    typedef boost::unordered_map<std::string, VarPointer> VariableMap;
    typedef boost::unordered_map<std::string, VarVectorPointer > VariableVectorMap;

protected:
    boost::shared_ptr<TFile> treeFile_;
    std::string path_;
    std::vector<std::string> directories_;
    std::vector<TreePointer> trees_;
    std::string prefix_, suffix_;
public:

    // FIXME Put as protected and provide getters and setters
    TreeMap treeMap_;
    VariableMap varMap_;
    VariableVectorMap varVectorMap_;

    TBranchCollection(std::string virtualPath = "") :
        treeFile_(),
        path_(virtualPath),
        directories_(getDirectoriesFromPath(path_)),
        trees_(),
        prefix_(""),
        suffix_(""),
        treeMap_(),
        varMap_(),
        varVectorMap_() {

    }

    ~TBranchCollection() {

    }

    void addBranchToTree(std::string branchLabel, std::string varType, TreePointer tree, bool isSingleValuePerEvent=true);

    void setBranchVariable(std::string branchLabel, float value);
    void resetBranchVariables();

    unsigned int size() const;

    void writeToFile(boost::shared_ptr<TFile> treeFile);

    bool contains(std::string name);
    bool vectorContains(std::string name);

private:
    void writeDirectories();
    void writeTrees();

};

typedef boost::shared_ptr<TBranchCollection > TBranchCollectionRef;

}

#endif /* TBRANCHCOLLECTION_H_ */
