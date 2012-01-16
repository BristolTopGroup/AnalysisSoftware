/*
 * HistogramManager.cpp
 *
 *  Created on: 4 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/HistHelpers/HistogramManager.h"
#include "../../interface/Readers/NTupleEventReader.h"
#include "../../interface/GlobalVariables.h"

namespace BAT {

HistogramManager::HistogramManager() :
    seenDataTypes(),
    histFiles(),
    currentDataType(DataType::ElectronHad),
    currentJetbin(0),
    currentBJetbin(0),
//    currentIntegratedLumi(0),
    current1DCollection(""),
    current1DJetCollection(""),
    current1DBJetCollection(""),
    current2DCollection(""),
    current2DJetCollection(""),
    current2DBJetCollection(""),
    collection1D(),
    collection2D(),
    jetCollection1D(),
    jetCollection2D(),
    bJetCollection1D(),
    bJetCollection2D(),
    debugMode(false){
}

HistogramManager::~HistogramManager() {
}

void HistogramManager::addH1D(std::string name, std::string title, unsigned int numberOfBins, float xmin, float xmax) {
    if(collection1D.find(current1DCollection) == collection1D.end())
        add1DCollection(current1DCollection);

    for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            collection1D[current1DCollection][type]->add(name, title, numberOfBins, xmin, xmax);
        }
    }
}


void HistogramManager::addH1D_JetBinned(std::string name, std::string title, unsigned int numberOfBins, float xmin,
        float xmax) {
    if (jetCollection1D.find(current1DJetCollection) == jetCollection1D.end())
        add1DJetCollection(current1DJetCollection);

    for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
        for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
            if (seenDataTypes.at(type)) {
                std::stringstream tmp_name, tmp_title;
                tmp_title << title << " (" << JetBin::names[jetbin] << ")";
                jetCollection1D[current1DJetCollection][type][jetbin]->add(name, tmp_title.str(), numberOfBins, xmin,
                        xmax);
            }
        }
    }
}

void HistogramManager::addH1D_BJetBinned(std::string name, std::string title, unsigned int numberOfBins, float xmin,
        float xmax) {
    if (bJetCollection1D.find(current1DBJetCollection) == bJetCollection1D.end())
        add1DBJetCollection(current1DBJetCollection);

    for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
        for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
            if (seenDataTypes.at(type)) {
                std::stringstream tmp_title;
                tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
                bJetCollection1D[current1DBJetCollection][type][jetbin]->add(name, tmp_title.str(), numberOfBins, xmin,
                        xmax);
            }
        }
    }

}

void HistogramManager::addH2D_BJetBinned(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
        unsigned int nYBins, float ymin, float ymax) {
    if (bJetCollection2D.find(current2DBJetCollection) == bJetCollection2D.end())
                add2DBJetCollection(current2DBJetCollection);

    for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
            for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
                if (seenDataTypes.at(type)) {
                    std::stringstream tmp_title;
                    tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
                    bJetCollection2D[current2DBJetCollection][type][jetbin]->add(name, tmp_title.str(), nXBins, xmin, xmax, nYBins, ymin, ymax);
                }
            }
        }
}

void HistogramManager::addH2D_JetBinned(std::string name, std::string title, unsigned int nXBins, float xmin,
        float xmax, unsigned int nYBins, float ymin, float ymax) {

    if (jetCollection2D.find(current2DJetCollection) == jetCollection2D.end())
            add2DJetCollection(current2DJetCollection);

    for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
        for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
            if (seenDataTypes.at(type)) {
                std::stringstream tmp_title;
                tmp_title << title << " (" << JetBin::names[jetbin] << ")";
                jetCollection2D[current2DJetCollection][type][jetbin]->add(name, tmp_title.str(), nXBins, xmin, xmax,
                        nYBins, ymin, ymax);
            }
        }
    }

}

void HistogramManager::addH2D(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
        unsigned int nYBins, float ymin, float ymax) {
    if (collection2D.find(current2DCollection) == collection2D.end())
        add2DCollection(current2DCollection);

    for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            collection2D[current2DCollection][type]->add(name, title, nXBins, xmin, xmax, nYBins, ymin, ymax);
        }
    }

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

unsigned int HistogramManager::getCurrentJetBin() const {
	return currentJetbin;
}

unsigned int HistogramManager::getCurrentBJetBin() const {
	return currentBJetbin;
}

boost::shared_ptr<TH1> HistogramManager::operator [](std::string histname) {
    return H1D(histname);
}

boost::shared_ptr<TH1> HistogramManager::H1D(std::string histname) {
    if (debugMode)
        if (collection1D.find(current1DCollection) == collection1D.end()
                || !collection1D[current1DCollection][currentDataType]->contains(histname)) {
            throw HistogramAccessException(
                    "Histogram '" + histname + "' in collection '" + current1DCollection + "' not found");
        }

    return collection1D[current1DCollection][currentDataType]->get(histname);
}

boost::shared_ptr<TH2> HistogramManager::H2D(std::string histname) {
    if (debugMode)
        if (collection2D.find(current2DCollection) == collection2D.end()
                || !collection2D[current2DCollection][currentDataType]->contains(histname)) {
            throw HistogramAccessException(
                    "Histogram '" + histname + "' in collection '" + current2DCollection + "' not found");
        }

    return collection2D[current2DCollection][currentDataType]->get(histname);
}

boost::shared_ptr<TH1> HistogramManager::H1D_JetBinned(std::string histname) {
    if (debugMode)
        if (jetCollection1D.find(current1DJetCollection) == jetCollection1D.end()
                || !jetCollection1D[current1DJetCollection][currentDataType][currentJetbin]->contains(histname)) {
            throw HistogramAccessException(
                    "Histogram '" + histname + "' in jet collection '" + current1DJetCollection + "' not found");
        }

    return jetCollection1D[current1DJetCollection][currentDataType][currentJetbin]->get(histname);
}

boost::shared_ptr<TH2> HistogramManager::H2D_JetBinned(std::string histname) {
    if (debugMode)
        if (jetCollection2D.find(current2DJetCollection) == jetCollection2D.end()
                || !jetCollection2D[current2DJetCollection][currentDataType][currentJetbin]->contains(histname)) {
            throw HistogramAccessException(
                    "Histogram '" + histname + "' in jet collection '" + current2DJetCollection + "' not found");
        }

    return jetCollection2D[current2DJetCollection][currentDataType][currentJetbin]->get(histname);
}

boost::shared_ptr<TH1> HistogramManager::H1D_BJetBinned(std::string histname) {
    if (debugMode)
        if (bJetCollection1D.find(current1DBJetCollection) == bJetCollection1D.end()
                || !bJetCollection1D[current1DBJetCollection][currentDataType][currentBJetbin]->contains(histname)) {
            throw HistogramAccessException(
                    "Histogram '" + histname + "' in bjet collection '" + current1DBJetCollection + "' not found");
        }

    return bJetCollection1D[current1DBJetCollection][currentDataType][currentBJetbin]->get(histname);
}

boost::shared_ptr<TH2> HistogramManager::H2D_BJetBinned(std::string histname) {
    if (debugMode)
        if (bJetCollection2D.find(current2DBJetCollection) == bJetCollection2D.end()
                || !bJetCollection2D[current2DBJetCollection][currentDataType][currentBJetbin]->contains(histname)) {
            throw HistogramAccessException(
                    "Histogram '" + histname + "' in bjet collection '" + current2DBJetCollection + "' not found");
        }

    return bJetCollection2D[current2DBJetCollection][currentDataType][currentBJetbin]->get(histname);
}

boost::shared_ptr<TH2> HistogramManager::operator ()(std::string histname) {
    return H2D(histname);
}

void HistogramManager::prepareForSeenDataTypes(const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes) {
    this->seenDataTypes = seenDataTypes;
    jetCollection1D[""].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]);
    jetCollection2D[""].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]);

    bJetCollection1D[""].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][BJetBin::NUMBER_OF_BJET_BINS]);
    bJetCollection2D[""].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][BJetBin::NUMBER_OF_BJET_BINS]);

    for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            const std::string filename = assembleFilename((DataType::value) type);
            boost::shared_ptr<TFile> file(new TFile(filename.c_str(), "RECREATE"));
            file->SetCompressionLevel(7);

            histFiles.at(type) = file;
            collection1D[""].at(type) = TH1CollectionRef(new TH1Collection());
            collection2D[""].at(type) = TH2CollectionRef(new TH2Collection());

            for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
                TH1CollectionRef coll(new TH1Collection());
                TH2CollectionRef coll2D(new TH2Collection());
                coll->setSuffix(JetBin::names.at(jetbin));
                coll2D->setSuffix(JetBin::names.at(jetbin));
                jetCollection1D[""][type][jetbin] = coll;
                jetCollection2D[""][type][jetbin] = coll2D;
            }

            for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
                TH1CollectionRef coll(new TH1Collection());
                TH2CollectionRef coll2D(new TH2Collection());
                coll->setSuffix(BJetBin::names.at(jetbin));
                coll2D->setSuffix(BJetBin::names.at(jetbin));

                bJetCollection1D[""][type][jetbin] = coll;
                bJetCollection2D[""][type][jetbin] = coll2D;
            }
        }
    }
}

const std::string HistogramManager::assembleFilename(DataType::value type) const {
    const std::string name = DataType::names[type];
    std::stringstream str;
    std::string electronAlgo = ElectronAlgorithm::names[Globals::electronAlgorithm];
    std::string jetAlgo = JetAlgorithm::names[Globals::jetAlgorithm];
    std::string metAlgo = METAlgorithm::names[Globals::metAlgorithm];
    std::string muonAlgo = MuonAlgorithm::names[Globals::muonAlgorithm];
    str << name << "_" << Globals::luminosity << "pb";
    str << "_" << electronAlgo << "_" << muonAlgo << "_" << jetAlgo << "_" << metAlgo << ".root";
    return str.str();

}

void HistogramManager::writeToDisk() {
    for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            createJetSummedHistograms((DataType::value) type);
            createBJetSummedHistograms((DataType::value) type);


            for (unordered_map<std::string, TH1Array>::iterator iter = collection1D.begin(); iter != collection1D.end(); ++iter) {
                iter->second[type]->writeToFile(histFiles.at(type));
            }

            for (unordered_map<std::string, TH2Array>::iterator iter = collection2D.begin(); iter != collection2D.end(); ++iter) {
                iter->second[type]->writeToFile(histFiles.at(type));
            }


            for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
                for (unordered_map<std::string, TH1MultiArray>::iterator iter = jetCollection1D.begin(); iter
                        != jetCollection1D.end(); ++iter) {
                    iter->second[type][jetbin]->writeToFile(histFiles.at(type));
                }

                for (unordered_map<std::string, TH2MultiArray>::iterator iter = jetCollection2D.begin(); iter
                        != jetCollection2D.end(); ++iter) {
                    iter->second[type][jetbin]->writeToFile(histFiles.at(type));
                }
            }

            for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {

                for (unordered_map<std::string, TH1MultiArray>::iterator iter = bJetCollection1D.begin(); iter
                        != bJetCollection1D.end(); ++iter) {
                    iter->second[type][jetbin]->writeToFile(histFiles.at(type));
                }

                for (unordered_map<std::string, TH2MultiArray>::iterator iter = bJetCollection2D.begin(); iter
                        != bJetCollection2D.end(); ++iter) {
                    iter->second[type][jetbin]->writeToFile(histFiles.at(type));
                }
            }
            histFiles.at(type)->Write();
            histFiles.at(type)->Close();
        }

    }
}

void HistogramManager::createSummedHistograms(DataType::value type){

}

void HistogramManager::createJetSummedHistograms(DataType::value type) {
    for (unsigned short jetbinSum = 0; jetbinSum < JetBinSummed::NUMBER_OF_SUMMED_JET_BINS; ++jetbinSum) {
        for (unordered_map<std::string, TH1MultiArray>::iterator iter = jetCollection1D.begin(); iter
                != jetCollection1D.end(); ++iter) {
            string collectionName = iter->first;
            TH1CollectionRef coll = iter->second[type][jetbinSum];
            if (collection1D.find(collectionName) == collection1D.end())
                add1DCollection(collectionName);

            unordered_map<std::string, boost::shared_ptr<TH1> > histMap = coll->getAllHistograms();
            for (unordered_map<std::string, boost::shared_ptr<TH1> >::const_iterator histIter = histMap.begin(); histIter
                    != histMap.end(); ++histIter) {
                string name = histIter->first;
                std::stringstream tmp_name, tmp_title;
                tmp_name << name << "_" << JetBinSummed::names[jetbinSum];

                boost::shared_ptr<TH1> hist((TH1*) histIter->second->Clone(tmp_name.str().c_str()));
                tmp_title << hist->GetTitle() << " (" << JetBinSummed::names[jetbinSum] << ")";
                hist->SetTitle(tmp_title.str().c_str());

                for (unsigned short jetbin = jetbinSum + 1; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
                    hist->Add(jetCollection1D[collectionName][type][jetbin]->get(name).get());
                }
                collection1D[collectionName][type]->add(tmp_name.str(), hist);
            }
        }

        for (unordered_map<std::string, TH2MultiArray>::iterator iter = jetCollection2D.begin(); iter
                        != jetCollection2D.end(); ++iter) {
                    string collectionName = iter->first;
                    TH2CollectionRef coll = iter->second[type][jetbinSum];
                    if (collection2D.find(collectionName) == collection2D.end())
                        add2DCollection(collectionName);

                    unordered_map<std::string, boost::shared_ptr<TH2> > histMap = coll->getAllHistograms();
                    for (unordered_map<std::string, boost::shared_ptr<TH2> >::const_iterator histIter = histMap.begin(); histIter
                            != histMap.end(); ++histIter) {
                        string name = histIter->first;
                        std::stringstream tmp_name, tmp_title;
                        tmp_name << name << "_" << JetBinSummed::names[jetbinSum];

                        boost::shared_ptr<TH2> hist((TH2*) histIter->second->Clone(tmp_name.str().c_str()));
                        tmp_title << hist->GetTitle() << " (" << JetBinSummed::names[jetbinSum] << ")";
                        hist->SetTitle(tmp_title.str().c_str());

                        for (unsigned short jetbin = jetbinSum + 1; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
                            hist->Add(jetCollection2D[collectionName][type][jetbin]->get(name).get());
                        }
                        collection2D[collectionName][type]->add(tmp_name.str(), hist);
                    }
                }

    }
}

void HistogramManager::createBJetSummedHistograms(DataType::value type) {
    for (unsigned short jetbinSum = 0; jetbinSum < BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS; ++jetbinSum) {
        for (unordered_map<std::string, TH1MultiArray>::iterator iter = bJetCollection1D.begin(); iter
                != bJetCollection1D.end(); ++iter) {
            string collectionName = iter->first;
            TH1CollectionRef coll = iter->second[type][jetbinSum];
            if (collection1D.find(collectionName) == collection1D.end())
                add1DCollection(collectionName);

            unordered_map<std::string, boost::shared_ptr<TH1> > histMap = coll->getAllHistograms();
            for (unordered_map<std::string, boost::shared_ptr<TH1> >::const_iterator histIter = histMap.begin(); histIter
                    != histMap.end(); ++histIter) {
                string name = histIter->first;
                std::stringstream tmp_name, tmp_title;
                tmp_name << name << "_" << BJetBinSummed::names[jetbinSum];

                boost::shared_ptr<TH1> hist((TH1*) histIter->second->Clone(tmp_name.str().c_str()));
                tmp_title << hist->GetTitle() << " (" << BJetBinSummed::names[jetbinSum] << ")";
                hist->SetTitle(tmp_title.str().c_str());

                for (unsigned short jetbin = jetbinSum + 1; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
                    hist->Add(bJetCollection1D[collectionName][type][jetbin]->get(name).get());
                }
                collection1D[collectionName][type]->add(tmp_name.str(), hist);
            }
        }

        for (unordered_map<std::string, TH2MultiArray>::iterator iter = bJetCollection2D.begin(); iter
                != bJetCollection2D.end(); ++iter) {
            string collectionName = iter->first;
            TH2CollectionRef coll = iter->second[type][jetbinSum];
            if (collection2D.find(collectionName) == collection2D.end())
                add2DCollection(collectionName);

            unordered_map<std::string, boost::shared_ptr<TH2> > histMap = coll->getAllHistograms();
            for (unordered_map<std::string, boost::shared_ptr<TH2> >::const_iterator histIter = histMap.begin(); histIter
                    != histMap.end(); ++histIter) {
                string name = histIter->first;
                std::stringstream tmp_name, tmp_title;
                tmp_name << name << "_" << BJetBinSummed::names[jetbinSum];

                boost::shared_ptr<TH2> hist((TH2*) histIter->second->Clone(tmp_name.str().c_str()));
                tmp_title << hist->GetTitle() << " (" << BJetBinSummed::names[jetbinSum] << ")";
                hist->SetTitle(tmp_title.str().c_str());

                for (unsigned short jetbin = jetbinSum + 1; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
                    hist->Add(bJetCollection2D[collectionName][type][jetbin]->get(name).get());
                }
                collection2D[collectionName][type]->add(tmp_name.str(), hist);
            }
        }

    }
}

void HistogramManager::add1DCollection(string collection) {
    for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type))
            collection1D[collection].at(type) = TH1CollectionRef(new TH1Collection(collection));
    }
}

void HistogramManager::add1DJetCollection(string collection) {
    jetCollection1D[collection].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]);

    for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
                TH1CollectionRef coll(new TH1Collection(collection));
                coll->setSuffix(JetBin::names.at(jetbin));
                jetCollection1D[collection][type][jetbin] = coll;
            }
        }
    }
}

void HistogramManager::add1DBJetCollection(string collection) {
    bJetCollection1D[collection].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]);

    for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
                TH1CollectionRef coll(new TH1Collection(collection));
                coll->setSuffix(BJetBin::names.at(jetbin));
                bJetCollection1D[collection][type][jetbin] = coll;
            }
        }
    }
}

void HistogramManager::add2DCollection(string collection) {
    for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type))
            collection2D[collection].at(type) = TH2CollectionRef(new TH2Collection(collection));
    }
}

void HistogramManager::add2DJetCollection(string collection) {
    jetCollection2D[collection].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]);

    for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
                TH2CollectionRef coll(new TH2Collection(collection));
                coll->setSuffix(JetBin::names.at(jetbin));
                jetCollection2D[collection][type][jetbin] = coll;
            }
        }
    }
}

void HistogramManager::add2DBJetCollection(string collection) {
    bJetCollection2D[collection].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][BJetBin::NUMBER_OF_BJET_BINS]);

    for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
        if (seenDataTypes.at(type)) {
            for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
                TH2CollectionRef coll(new TH2Collection(collection));
                coll->setSuffix(BJetBin::names.at(jetbin));
                bJetCollection2D[collection][type][jetbin] = coll;
            }
        }
    }
}

void HistogramManager::addCollection(string collection) {
    add1DCollection(collection);
    add1DJetCollection(collection);
    add1DBJetCollection(collection);
    add2DCollection(collection);
    add2DJetCollection(collection);
    add2DBJetCollection(collection);
}

void HistogramManager::setCurrent1DCollection(string collection){
    current1DCollection = collection;
}

void HistogramManager::setCurrent1DJetCollection(string collection){
    current1DJetCollection = collection;
}

void HistogramManager::setCurrent1DBJetCollection(string collection){
    current1DBJetCollection = collection;
}

void HistogramManager::setCurrent2DCollection(string collection){
    current2DCollection = collection;
}

void HistogramManager::setCurrent2DJetCollection(string collection){
    current2DJetCollection = collection;
}

void HistogramManager::setCurrent2DBJetCollection(string collection){
    current2DBJetCollection = collection;
}

void HistogramManager::setCurrentCollection(string collection) {
    setCurrent1DCollection(collection);
    setCurrent1DJetCollection(collection);
    setCurrent1DBJetCollection(collection);

    setCurrent2DCollection(collection);
    setCurrent2DJetCollection(collection);
    setCurrent2DBJetCollection(collection);
}

void HistogramManager::enableDebugMode(bool enable){
    debugMode = enable;
}
}
