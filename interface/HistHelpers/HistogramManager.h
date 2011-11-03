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
#include "../../interface/DataTypes.h"
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/multi_array.hpp>
#include "TFile.h"
#include <string>

namespace BAT {
typedef boost::array<TH1CollectionRef, DataType::NUMBER_OF_DATA_TYPES> TH1Array;
typedef boost::array<TH2CollectionRef, DataType::NUMBER_OF_DATA_TYPES> TH2Array;
typedef boost::multi_array<TH1CollectionRef, 2> TH1MultiArray;
typedef boost::multi_array<TH2CollectionRef, 2> TH2MultiArray;

namespace JetBin {
enum value {
    NoJet,
    OneJet,
    TwoJets,
    ThreeJets,
    FourOrMoreJets,
    NUMBER_OF_JET_BINS
};

const boost::array<std::string, JetBin::NUMBER_OF_JET_BINS> names = {
        { "0jet", "1jet", "2jets", "3jets", "4orMoreJets"
        } };
}

namespace JetBinSummed {
enum value {
    allJet,
    OneOrMoreJets,
    TwoOrMoreJets,
    ThreeOrMoreJets,
    NUMBER_OF_SUMMED_JET_BINS
};

const boost::array<std::string, JetBinSummed::NUMBER_OF_SUMMED_JET_BINS> names = { {
        "0orMoreJets",
        "1orMoreJets",
        "2orMoreJets",
        "3orMoreJets" } };
}

namespace BJetBin {
enum value {
    NoBtag,
    OneBTag,
    TwoBtags,
    ThreeBtags,
    FourOrMoreBTags,
    NUMBER_OF_BJET_BINS
};

const boost::array<std::string, BJetBin::NUMBER_OF_BJET_BINS> names = { {
        "0btag",
        "1btag",
        "2btags",
        "3btags",
        "4orMoreBtags" } };
}

namespace BJetBinSummed {
enum value {
    allBtags,
    OneOrMoreBTags,
    TwoOrMoreBTags,
    ThreeOrMoreBTags,
    NUMBER_OF_SUMMED_BJET_BINS
};

const boost::array<std::string, BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS> names = { {
        "0orMoreBtag",
        "1orMoreBtag",
        "2orMoreBtags",
        "3orMoreBtags" } };
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
    void addH1D_JetBinned(std::string name, std::string title, unsigned int nBins, float xmin, float xmax);
    void addH1D_BJetBinned(std::string name, std::string title, unsigned int nBins, float xmin, float xmax);
    void addH2D_BJetBinned(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
            unsigned int nYBins, float ymin, float ymax);
    void addH2D_JetBinned(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
                unsigned int nYBins, float ymin, float ymax);

    void addH2D(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax, unsigned int nYBins,
            float ymin, float ymax);

    void setCurrentDataType(DataType::value type);
    void setCurrentJetBin(unsigned int jetbin);
    void setCurrentBJetBin(unsigned int jetbin);
//    void setCurrentLumi(float lumi);
    void prepareForSeenDataTypes(const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes);

    void add1DCollection(std::string collection);
    void add1DJetCollection(std::string collection);
    void add1DBJetCollection(std::string collection);

    void add2DCollection(std::string collection);
    void add2DJetCollection(std::string collection);
    void add2DBJetCollection(std::string collection);

    void addCollection(std::string collection);



    void setCurrent1DCollection(std::string collection);
    void setCurrent1DJetCollection(std::string collection);
    void setCurrent1DBJetCollection(std::string collection);
    void setCurrent2DCollection(std::string collection);
    void setCurrent2DJetCollection(std::string collection);
    void setCurrent2DBJetCollection(std::string collection);

    void setCurrentCollection(std::string collection);

    boost::shared_ptr<TH1> operator[](std::string);
    boost::shared_ptr<TH1> H1D(std::string);
    boost::shared_ptr<TH1> H1D_JetBinned(std::string);
    boost::shared_ptr<TH1> H1D_BJetBinned(std::string);
    boost::shared_ptr<TH2> operator()(std::string);
    boost::shared_ptr<TH2> H2D(std::string);
    boost::shared_ptr<TH2> H2D_JetBinned(std::string);
    boost::shared_ptr<TH2> H2D_BJetBinned(std::string);

    void writeToDisk();

    void enableDebugMode(bool enable);
private:

    boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenDataTypes;
    boost::array<boost::shared_ptr<TFile>, DataType::NUMBER_OF_DATA_TYPES> histFiles;

    DataType::value currentDataType;
    unsigned int currentJetbin;
    unsigned int currentBJetbin;
//    float currentIntegratedLumi;
    std::string current1DCollection, current1DJetCollection, current1DBJetCollection;
    std::string current2DCollection, current2DJetCollection, current2DBJetCollection;

    unordered_map<std::string, TH1Array> collection1D;
    unordered_map<std::string, TH2Array> collection2D;
    unordered_map<std::string, TH1MultiArray> jetCollection1D;
    unordered_map<std::string, TH2MultiArray> jetCollection2D;
    unordered_map<std::string, TH1MultiArray> bJetCollection1D;
    unordered_map<std::string, TH2MultiArray> bJetCollection2D;

    bool debugMode;

    const std::string assembleFilename(DataType::value) const;
    void createSummedHistograms(DataType::value);
    void createJetSummedHistograms(DataType::value);
    void createBJetSummedHistograms(DataType::value);
};

}

#endif /* HISTOGRAMMANAGER_H_ */
