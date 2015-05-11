/*
 * TreeManager.h
 *
 *  Created on: 5 Feb 2015
 *      Author: ejclemen
 */

#ifndef TREEMANAGER_H_
#define TREEMANAGER_H_
#include "TBranchCollection.h"
#include "TTree.h"
#include "TBranch.h"
#include "../../interface/DataTypes.h"
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#include "TFile.h"
#include <string>
#include <vector>
 
namespace BAT {
typedef boost::array<TBranchCollectionRef, DataType::NUMBER_OF_DATA_TYPES> TBranchArray;
typedef boost::shared_ptr<TBranch> TBranchPtr;

struct BranchAccessException: public std::exception {
	TString msg;
	BranchAccessException(TString message) :
			msg(message) {
	}
	~BranchAccessException() throw () {
	}

	const char* what() const throw () {
		return msg;
	}
};

typedef unsigned short ushort;
class TreeManager {
public:
	TreeManager();
	virtual ~TreeManager();
	void addBranch(std::string branchLabel, std::string varType, std::string treeName, bool isSingleValuePerEvent=true);

	void Fill(std::string branchLabel, float fillValue);
	void FillTrees();

	void setCurrentDataType(DataType::value type);
	void prepareForSeenDataTypes(const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes);

	void addFolder(std::string folder);
	void addTreeToFolder(std::string treeName, std::string folder, unsigned int dataType );

	void setCurrentFolder(std::string folder);

	void writeToDisk();

private:

	boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenDataTypes_;
	boost::array<boost::shared_ptr<TFile>, DataType::NUMBER_OF_DATA_TYPES> treeFiles_;

	DataType::value currentDataType_;
	std::string currentFolder_;

	boost::unordered_map<std::string, TBranchArray> collection_;

	std::vector<double> bjetbin_weights_;

	const std::string assembleFilename(DataType::value) const;

};

typedef boost::shared_ptr<TreeManager> TreeManagerPtr;

}

#endif /* TREEMANAGER_H_ */