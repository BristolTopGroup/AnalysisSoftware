/*
 * HistogramManager.h
 *
 *  Created on: 4 Jul 2010
 *      Author: kreczko
 */

#ifndef HISTOGRAMMANAGER_H_
#define HISTOGRAMMANAGER_H_
#include "THCollection.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "../../interface/DataTypes.h"
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#include "TFile.h"
#include <string>
#include <vector>

namespace BAT {
typedef boost::array<TH1CollectionRef, DataType::NUMBER_OF_DATA_TYPES> TH1Array;
typedef boost::array<TH2CollectionRef, DataType::NUMBER_OF_DATA_TYPES> TH2Array;
typedef boost::array<TH3CollectionRef, DataType::NUMBER_OF_DATA_TYPES> TH3Array;

typedef boost::shared_ptr<TH1> TH1Ptr;
typedef boost::shared_ptr<TH2> TH2Ptr;
typedef boost::shared_ptr<TH3> TH3Ptr;

namespace JetBin {
enum value {
    NoJet,
    OneJet,
    TwoJets,
	ThreeJets,
	FourOrMoreJets,
	NUMBER_OF_JET_BINS
};

const boost::array<std::string, JetBin::NUMBER_OF_JET_BINS> names = { { //
		"0jet", //
		"1jet", //
		"2jets", //
		"3jets", //
				"4orMoreJets" } };
}

namespace JetBinSummed {
enum value {
    allJet,
    OneOrMoreJets,
    TwoOrMoreJets,
	ThreeOrMoreJets,
	NUMBER_OF_SUMMED_JET_BINS
};

const boost::array<std::string, JetBinSummed::NUMBER_OF_SUMMED_JET_BINS> names = { { //
		"0orMoreJets", //
		"1orMoreJets", //
		"2orMoreJets", //
		"3orMoreJets" //
		} };
}

namespace BJetBin {
enum value {
	NoBtag, //
	OneBTag, //
	TwoBtags, //
	ThreeBtags, //
	FourOrMoreBTags, //
	NUMBER_OF_BJET_BINS
};

const boost::array<std::string, BJetBin::NUMBER_OF_BJET_BINS> names = { { //
		"0btag", //
				"1btag", //
				"2btags", //
				"3btags", //
				"4orMoreBtags" //
		} };
}

namespace BJetBinSummed {
enum value {
	allBtags, //
	OneOrMoreBTags, //
	TwoOrMoreBTags, //
	ThreeOrMoreBTags, //
	NUMBER_OF_SUMMED_BJET_BINS
};

const boost::array<std::string, BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS> names = { { //
		"0orMoreBtag", //
				"1orMoreBtag", //
				"2orMoreBtags", //
				"3orMoreBtags" //
		} };
}

struct HistogramAccessException: public std::exception {
	TString msg;
	HistogramAccessException(TString message) :
			msg(message) {
	}
	~HistogramAccessException() throw () {
	}

	const char* what() const throw () {
		return msg;
	}
};

typedef unsigned short ushort;
class HistogramManager {
public:
	HistogramManager();
	virtual ~HistogramManager();
	void addH1D(std::string name, std::string title, unsigned int nBins, float xmin, float xmax);
	void addH1D(std::string name, std::string title, unsigned int nBins, float *xbins);
	void addH1D_JetBinned(std::string name, std::string title, unsigned int nBins, float xmin, float xmax);
	void addH1D_JetBinned(std::string name, std::string title, unsigned int nBins, float *xbins);
	void addH1D_BJetBinned(std::string name, std::string title, unsigned int nBins, float xmin, float xmax);
	void addH1D_BJetBinned(std::string name, std::string title, unsigned int nBins, float *xbins);

	void addH2D(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax, unsigned int nYBins,
			float ymin, float ymax);
	void addH2D_BJetBinned(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
			unsigned int nYBins, float ymin, float ymax);
	void addH2D_JetBinned(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
			unsigned int nYBins, float ymin, float ymax);

	void addH3D(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax, unsigned int nYBins,
			float ymin, float ymax, unsigned int nZBins, float zmin, float zmax);
	void addH3D_BJetBinned(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
			unsigned int nYBins, float ymin, float ymax, unsigned int nZBins, float zmin, float zmax);
	void addH3D_JetBinned(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
			unsigned int nYBins, float ymin, float ymax, unsigned int nZBins, float zmin, float zmax);

	void setCurrentDataType(DataType::value type);
	void setCurrentJetBin(unsigned int jetbin);
	void setCurrentBJetBin(unsigned int jetbin);
	unsigned int getCurrentJetBin() const;
	unsigned int getCurrentBJetBin() const;
//    void setCurrentLumi(float lumi);
	void prepareForSeenDataTypes(const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes);

	void addHistogramFolder(std::string folder);

	void setCurrentHistogramFolder(std::string folder);
	unsigned int size() const;
	unsigned int size1D(DataType::value) const;
	unsigned int size2D(DataType::value) const;
	unsigned int size3D(DataType::value) const;

	TH1Ptr operator[](std::string);
	TH1Ptr H1D(std::string);
	TH1Ptr H1D_JetBinned(std::string);
	TH1Ptr H1D_BJetBinned(std::string);
	TH2Ptr operator()(std::string);
	TH2Ptr H2D(std::string);
	TH2Ptr H2D_JetBinned(std::string);
	TH2Ptr H2D_BJetBinned(std::string);

	TH3Ptr H3D(std::string);
	TH3Ptr H3D_JetBinned(std::string);
	TH3Ptr H3D_BJetBinned(std::string);

	void writeToDisk();

	void enableDebugMode(bool enable);

	void beginDataType(DataType::value type);
	void endDataType();

private:

	void add1DHistogramFolder(std::string folder);
	void add2DHistogramFolder(std::string folder);
	void add3DHistogramFolder(std::string folder);

	boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenDataTypes;
	boost::array<boost::shared_ptr<TFile>, DataType::NUMBER_OF_DATA_TYPES> histFiles;

	DataType::value currentDataType;
	unsigned int currentJetbin;
	unsigned int currentBJetbin;
	std::string currentHistogramFolder;

	boost::unordered_map<std::string, TH1Array> collection1D;
	boost::unordered_map<std::string, TH2Array> collection2D;
	boost::unordered_map<std::string, TH3Array> collection3D;

	bool debugMode;
	std::vector<double> bjetbin_weights_;

	const std::string assembleFilename(DataType::value) const;
	void createJetSummedHistograms(DataType::value);
	//TODO: make them templated and take boost::unordered_map<std::string, template> as parameter
	void createJetSummedHistograms1D(DataType::value);
	void createJetSummedHistograms2D(DataType::value);
	void createJetSummedHistograms3D(DataType::value);
	void createBJetSummedHistograms(DataType::value);
	void createBJetSummedHistograms1D(DataType::value);
	void createBJetSummedHistograms2D(DataType::value);
	void createBJetSummedHistograms3D(DataType::value);

};

typedef boost::shared_ptr<HistogramManager> HistogramManagerPtr;

}

#endif /* HISTOGRAMMANAGER_H_ */
