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

typedef unsigned short ushort;
class HistogramManager {
public:
    HistogramManager();
    virtual ~HistogramManager();
    void createAllHistograms();
    void addH1D(std::string name, std::string title, unsigned int nBins, float xmin, float xmax);
    void addH1D_JetBinned(std::string name, std::string title, unsigned int nBins, float xmin, float xmax);
    void addH1D_BJetBinned(std::string name, std::string title, unsigned int nBins, float xmin, float xmax);
    void addH2D_BJetBinned(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
            unsigned int nYBins, float ymin, float ymax);

    void addH2D(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax, unsigned int nYBins,
            float ymin, float ymax);

    void setCurrentDataType(DataType::value type);
    void setCurrentJetBin(unsigned int jetbin);
    void setCurrentBJetBin(unsigned int jetbin);
    void setCurrentLumi(float lumi);
    void prepareForSeenDataTypes(const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes);

    boost::shared_ptr<TH1> operator[](std::string);
    boost::shared_ptr<TH1> H1D(std::string);
    boost::shared_ptr<TH1> H1D_JetBinned(std::string);
    boost::shared_ptr<TH1> H1D_BJetBinned(std::string);
    boost::shared_ptr<TH2> operator()(std::string);
    boost::shared_ptr<TH2> H2D(std::string);
    boost::shared_ptr<TH2> H2D_JetBinned(std::string);
    boost::shared_ptr<TH2> H2D_BJetBinned(std::string);

    void writeToDisk();
private:
    boost::multi_array<TH1CollectionRef, 2> jetBinned1DHists;
    std::vector<std::string> jetBinned1DHistNames;
    boost::multi_array<TH2CollectionRef, 2> jetBinned2DHists;
    std::vector<std::string> jetBinned2DHistNames;
    boost::multi_array<TH1CollectionRef, 2> bJetBinned1DHists;
    std::vector<std::string> bJetBinned1DHistNames;
    boost::multi_array<TH2CollectionRef, 2> bJetBinned2DHists;
    std::vector<std::string> bJetBinned2DHistNames;
    boost::array<bool, DataType::NUMBER_OF_DATA_TYPES> seenDataTypes;
    boost::array<boost::shared_ptr<TFile>, DataType::NUMBER_OF_DATA_TYPES> histFiles;
    boost::array<TH1CollectionRef, DataType::NUMBER_OF_DATA_TYPES> collection;// move to array of DataTypes
    std::vector<std::string> collection1DHistNames;
    boost::array<TH2CollectionRef, DataType::NUMBER_OF_DATA_TYPES> collection2D;
    std::vector<std::string> collection2DHistNames;
    DataType::value currentDataType;
    unsigned int currentJetbin;
    unsigned int currentBJetbin;
    float currentIntegratedLumi;

    const std::string assembleFilename(DataType::value) const;
    void createSummedHistograms(DataType::value);
    void createJetSummedHistograms(DataType::value);
    void createBJetSummedHistograms(DataType::value);
};

}

#endif /* HISTOGRAMMANAGER_H_ */
