/*
 * HistogramManager.cpp
 *
 *  Created on: 4 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/HistHelpers/HistogramManager.h"
#include "../../interface/Readers/NTupleEventReader.h"

namespace BAT {

HistogramManager::HistogramManager() :
    jetBinned1DHists(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]),
    jetBinned1DHistNames(),
    jetBinned2DHists(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]),
    jetBinned2DHistNames(),
    bJetBinned1DHists(boost::extents[DataType::NUMBER_OF_DATA_TYPES][BJetBin::NUMBER_OF_BJET_BINS]),
    bJetBinned1DHistNames(),
    bJetBinned2DHists(boost::extents[DataType::NUMBER_OF_DATA_TYPES][BJetBin::NUMBER_OF_BJET_BINS]),
    bJetBinned2DHistNames(),
    seenDataTypes(),
    histFiles(),
    collection(),
    collection1DHistNames(),
    collection2D(),
    collection2DHistNames(),
    currentDataType(DataType::DATA),
    currentJetbin(0),
    currentBJetbin(0),
    currentIntegratedLumi(0) {
}

HistogramManager::~HistogramManager() {
}

void HistogramManager::addH1D(std::string name, std::string title, unsigned int numberOfBins, float xmin, float xmax) {
    for (unsigned short type = DataType::DATA; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            collection[type]->add(name, title, numberOfBins, xmin, xmax);
            collection1DHistNames.push_back(name);
        }
    }
    collection1DHistNames.push_back(name);
}

void HistogramManager::addH1D_JetBinned(std::string name, std::string title, unsigned int numberOfBins, float xmin,
        float xmax) {
    for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
        for (unsigned short type = DataType::DATA; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
            if (seenDataTypes.at(type)) {
                std::stringstream tmp_name, tmp_title;
//                tmp_name << name << "_" << JetBin::names[jetbin];
                tmp_title << title << " (" << JetBin::names[jetbin] << ")";
                jetBinned1DHists[type][jetbin]->add(name, tmp_title.str(), numberOfBins, xmin, xmax);
            }
        }
    }
    jetBinned1DHistNames.push_back(name);
}

void HistogramManager::addH1D_BJetBinned(std::string name, std::string title, unsigned int numberOfBins, float xmin,
        float xmax) {
    for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
        for (unsigned short type = DataType::DATA; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
            if (seenDataTypes.at(type)) {
                std::stringstream tmp_title;
                tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
                bJetBinned1DHists[type][jetbin]->add(name, tmp_title.str(), numberOfBins, xmin, xmax);
            }
        }
    }
    bJetBinned1DHistNames.push_back(name);
}

void HistogramManager::addH2D_BJetBinned(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
        unsigned int nYBins, float ymin, float ymax) {
    for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
        for (unsigned short type = DataType::DATA; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
            if (seenDataTypes.at(type)) {
                std::stringstream tmp_title;
                tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
                bJetBinned2DHists[type][jetbin]->add(name, tmp_title.str(), nXBins, xmin, xmax, nYBins, ymin, ymax);
            }
        }
    }
    bJetBinned2DHistNames.push_back(name);
}

void HistogramManager::addH2D(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
        unsigned int nYBins, float ymin, float ymax) {
    for (unsigned short type = DataType::DATA; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            collection2D[type]->add(name, title, nXBins, xmin, xmax, nYBins, ymin, ymax);
        }
    }
    collection2DHistNames.push_back(name);
}

void HistogramManager::setCurrentDataType(DataType::value type) {
    currentDataType = type;
}

void HistogramManager::setCurrentJetBin(unsigned int jetbin) {
    if (jetbin > 3) {
        currentJetbin = 4;
    } else
        currentJetbin = jetbin;
}

void HistogramManager::setCurrentBJetBin(unsigned int jetbin) {
    if (jetbin > 3) {
        currentBJetbin = 4;
    } else
        currentBJetbin = jetbin;
}

void HistogramManager::setCurrentLumi(float lumi) {
    currentIntegratedLumi = lumi;
}

boost::shared_ptr<TH1> HistogramManager::operator [](std::string histname) {
    return collection[currentDataType]->get(histname);
}

boost::shared_ptr<TH1> HistogramManager::H1D(std::string histname) {
    return collection[currentDataType]->get(histname);
}

boost::shared_ptr<TH1> HistogramManager::H1D_JetBinned(std::string histname) {
    return jetBinned1DHists[currentDataType][currentJetbin]->get(histname);
}

boost::shared_ptr<TH1> HistogramManager::H1D_BJetBinned(std::string histname) {
    return bJetBinned1DHists[currentDataType][currentBJetbin]->get(histname);
}

boost::shared_ptr<TH2> HistogramManager::H2D_BJetBinned(std::string histname) {
    return bJetBinned2DHists[currentDataType][currentBJetbin]->get(histname);
}

boost::shared_ptr<TH2> HistogramManager::operator ()(std::string histname) {
    return collection2D[currentDataType]->get(histname);
}

boost::shared_ptr<TH2> HistogramManager::H2D(std::string histname) {
    return collection2D[currentDataType]->get(histname);
}

void HistogramManager::createAllHistograms() {
    collection[DataType::DATA]->add("mttbar", "mttbar", 5000, 0, 5000);
}

void HistogramManager::prepareForSeenDataTypes(const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes) {
    this->seenDataTypes = seenDataTypes;
    for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            const std::string filename = assembleFilename((DataType::value) type);
            boost::shared_ptr<TFile> file(new TFile(filename.c_str(), "RECREATE"));
            file->SetCompressionLevel(7);
            histFiles.at(type) = file;
            collection.at(type) = TH1CollectionRef(new TH1Collection());
            collection2D.at(type) = TH2CollectionRef(new TH2Collection());
            for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
                TH1CollectionRef coll(new TH1Collection());
                coll->setSuffix(JetBin::names.at(jetbin));
                jetBinned1DHists[type][jetbin] = coll;
            }

            for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
                TH1CollectionRef coll(new TH1Collection());
                TH2CollectionRef coll2D(new TH2Collection());
                coll->setSuffix(BJetBin::names.at(jetbin));
                coll2D->setSuffix(BJetBin::names.at(jetbin));
                bJetBinned1DHists[type][jetbin] = coll;
                bJetBinned2DHists[type][jetbin] = coll2D;
            }
        }

    }
}

const std::string HistogramManager::assembleFilename(DataType::value type) const {
    const std::string name = DataType::names[type];
    std::stringstream str;
    std::string electronAlgo = ElectronAlgorithm::names[NTupleEventReader::electronAlgorithm];
    std::string jetAlgo = JetAlgorithm::names[NTupleEventReader::jetAlgorithm];
    std::string metAlgo = METAlgorithm::names[NTupleEventReader::metAlgorithm];
    str << name << "_" << currentIntegratedLumi << "pb";
    str << "_" << electronAlgo << "_" << jetAlgo << "_" << metAlgo << ".root";
    return str.str();

}

void HistogramManager::writeToDisk() {
    for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            createSummedHistograms((DataType::value) type);
            createJetSummedHistograms((DataType::value) type);
            createBJetSummedHistograms((DataType::value) type);

            collection.at(type)->writeToFile(histFiles.at(type));
            collection2D.at(type)->writeToFile(histFiles.at(type));
            for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
                jetBinned1DHists[type][jetbin]->writeToFile(histFiles.at(type));
            }

            for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
                bJetBinned1DHists[type][jetbin]->writeToFile(histFiles.at(type));
                bJetBinned2DHists[type][jetbin]->writeToFile(histFiles.at(type));
            }
            histFiles.at(type)->Write();
            histFiles.at(type)->Close();
        }

    }
}

void HistogramManager::createSummedHistograms(DataType::value type){

}

void HistogramManager::createJetSummedHistograms(DataType::value type){
    for (unsigned int nameIndex = 0; nameIndex < jetBinned1DHistNames.size(); ++nameIndex) {
        std::string name = jetBinned1DHistNames.at(nameIndex);
        for (unsigned short jetbinSum = 0; jetbinSum < JetBinSummed::NUMBER_OF_SUMMED_JET_BINS; ++jetbinSum) {
            std::stringstream tmp_name, tmp_title;
            tmp_name << name << "_" << JetBinSummed::names[jetbinSum];
            boost::shared_ptr<TH1> hist((TH1*)jetBinned1DHists[type][jetbinSum]->get(name)->Clone(tmp_name.str().c_str()));
            tmp_title << hist->GetTitle() << " (" << JetBinSummed::names[jetbinSum] << ")";
            hist->SetTitle(tmp_title.str().c_str());

            for (unsigned short jetbin = jetbinSum + 1; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
                hist->Add(jetBinned1DHists[type][jetbin]->get(name).get());
            }
            collection[type]->add(tmp_name.str(), hist);
        }
    }
    for (unsigned int nameIndex = 0; nameIndex < jetBinned2DHistNames.size(); ++nameIndex) {
        std::string name = jetBinned2DHistNames.at(nameIndex);
        for (unsigned short jetbinSum = 0; jetbinSum < JetBinSummed::NUMBER_OF_SUMMED_JET_BINS; ++jetbinSum) {
            std::stringstream tmp_name, tmp_title;
            tmp_name << name << "_" << JetBinSummed::names[jetbinSum];
            boost::shared_ptr<TH2> hist((TH2*) jetBinned2DHists[type][jetbinSum]->get(name)->Clone(
                    tmp_name.str().c_str()));
            tmp_title << hist->GetTitle() << " (" << JetBinSummed::names[jetbinSum] << ")";
            hist->SetTitle(tmp_title.str().c_str());

            for (unsigned short jetbin = jetbinSum + 1; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
                hist->Add(jetBinned2DHists[type][jetbin]->get(name).get());
            }
            collection2D[type]->add(tmp_name.str(), hist);
        }
    }
}

void HistogramManager::createBJetSummedHistograms(DataType::value type){
    for (unsigned int nameIndex = 0; nameIndex < bJetBinned1DHistNames.size(); ++nameIndex) {
        std::string name = bJetBinned1DHistNames.at(nameIndex);
        for (unsigned short jetbinSum = 0; jetbinSum < BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS; ++jetbinSum) {
            std::stringstream tmp_name, tmp_title;
            tmp_name << name << "_" << BJetBinSummed::names[jetbinSum];
            boost::shared_ptr<TH1> hist((TH1*) bJetBinned1DHists[type][jetbinSum]->get(name)->Clone(
                    tmp_name.str().c_str()));
            tmp_title << hist->GetTitle() << " (" << BJetBinSummed::names[jetbinSum] << ")";
            hist->SetTitle(tmp_title.str().c_str());

            for (unsigned short jetbin = jetbinSum + 1; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
                hist->Add(bJetBinned1DHists[type][jetbin]->get(name).get());
            }
            collection[type]->add(tmp_name.str(), hist);
        }
    }
    for (unsigned int nameIndex = 0; nameIndex < bJetBinned2DHistNames.size(); ++nameIndex) {
        std::string name = bJetBinned2DHistNames.at(nameIndex);
        for (unsigned short jetbinSum = 0; jetbinSum < BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS; ++jetbinSum) {
            std::stringstream tmp_name, tmp_title;
            tmp_name << name << "_" << BJetBinSummed::names[jetbinSum];
            boost::shared_ptr<TH2> hist((TH2*) bJetBinned2DHists[type][jetbinSum]->get(name)->Clone(
                    tmp_name.str().c_str()));
            tmp_title << hist->GetTitle() << " (" << BJetBinSummed::names[jetbinSum] << ")";
            hist->SetTitle(tmp_title.str().c_str());

            for (unsigned short jetbin = jetbinSum + 1; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
                hist->Add(bJetBinned2DHists[type][jetbin]->get(name).get());
            }
            collection2D[type]->add(tmp_name.str(), hist);
        }
    }
}

}
