/*
 * TTreeManager.h
 *
 *  Created on: 5 Feb 2015
 *      Author: ejclemen
 */

#ifndef TTreeManager_H_
#define TTreeManager_H_
#include "TTreeCollection.h"
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
class TTreeManager {
public:
	TTreeManager();
	virtual ~TTreeManager();
	void addBranch(std::string branchName, std::string branchLabel, std::string treeName);

	void Fill(std::string branchLabel, float fillValue);
	void FillTrees();

	void setCurrentDataType(DataType::value type);
	void prepareForSeenDataTypes(const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes);

	void addFolder(std::string folder);
	void addTreeToFolder(std::string folder, unsigned int dataType, std::string treeName);

	void setCurrentTreeFolder(std::string folder);

	void writeToDisk();

private:

	boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenDataTypes;
	boost::array<boost::shared_ptr<TFile>, DataType::NUMBER_OF_DATA_TYPES> treeFiles;

	DataType::value currentDataType;
	std::string currentTreeFolder;

	boost::unordered_map<std::string, TBranchArray> collection;

	std::vector<double> bjetbin_weights_;

	const std::string assembleFilename(DataType::value) const;

};

typedef boost::shared_ptr<TTreeManager> TTreeManagerPtr;

}

#endif /* TTreeManager_H_ */