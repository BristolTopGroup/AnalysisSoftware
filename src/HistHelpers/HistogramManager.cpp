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

HistogramManager::HistogramManager() : //
		seenDataTypes(), //
		histFiles(), //
		currentDataType(DataType::ElectronHad), //
		currentJetbin(0), //
		currentBJetbin(0), //
		currentHistogramFolder(""), //
		collection1D(), //
		collection2D(), //
		jetCollection1D(), //
		jetCollection2D(), //
		bJetCollection1D(), //
		bJetCollection2D(), //
		debugMode(false) //
{
}

HistogramManager::~HistogramManager() {
}

void HistogramManager::addH1D(std::string name, std::string title, unsigned int numberOfBins, float xmin, float xmax) {
	if (collection1D.find(currentHistogramFolder) == collection1D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			collection1D[currentHistogramFolder][type]->add(name, title, numberOfBins, xmin, xmax);
		}
	}
}

void HistogramManager::addH1D_JetBinned(std::string name, std::string title, unsigned int numberOfBins, float xmin,
		float xmax) {
	if (jetCollection1D.find(currentHistogramFolder) == jetCollection1D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_name, tmp_title;
				tmp_title << title << " (" << JetBin::names[jetbin] << ")";
				jetCollection1D[currentHistogramFolder][type][jetbin]->add(name, tmp_title.str(), numberOfBins, xmin, xmax);
			}
		}
	}
}

void HistogramManager::addH1D_BJetBinned(std::string name, std::string title, unsigned int numberOfBins, float xmin,
		float xmax) {
	if (bJetCollection1D.find(currentHistogramFolder) == bJetCollection1D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_title;
				tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
				bJetCollection1D[currentHistogramFolder][type][jetbin]->add(name, tmp_title.str(), numberOfBins, xmin, xmax);
			}
		}
	}

}

void HistogramManager::addH2D_BJetBinned(std::string name, std::string title, unsigned int nXBins, float xmin,
		float xmax, unsigned int nYBins, float ymin, float ymax) {
	if (bJetCollection2D.find(currentHistogramFolder) == bJetCollection2D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_title;
				tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
				bJetCollection2D[currentHistogramFolder][type][jetbin]->add(name, tmp_title.str(), nXBins, xmin, xmax,
						nYBins, ymin, ymax);
			}
		}
	}
}

void HistogramManager::addH2D_JetBinned(std::string name, std::string title, unsigned int nXBins, float xmin,
		float xmax, unsigned int nYBins, float ymin, float ymax) {

	if (jetCollection2D.find(currentHistogramFolder) == jetCollection2D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_title;
				tmp_title << title << " (" << JetBin::names[jetbin] << ")";
				jetCollection2D[currentHistogramFolder][type][jetbin]->add(name, tmp_title.str(), nXBins, xmin, xmax, nYBins,
						ymin, ymax);
			}
		}
	}

}

void HistogramManager::addH2D(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
		unsigned int nYBins, float ymin, float ymax) {
	if (collection2D.find(currentHistogramFolder) == collection2D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			collection2D[currentHistogramFolder][type]->add(name, title, nXBins, xmin, xmax, nYBins, ymin, ymax);
		}
	}

}

void HistogramManager::addH3D_BJetBinned(std::string name, std::string title, unsigned int nXBins, float xmin,
		float xmax, unsigned int nYBins, float ymin, float ymax, unsigned int nZBins, float zmin, float zmax) {
	if (bJetCollection3D.find(currentHistogramFolder) == bJetCollection3D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_title;
				tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
				bJetCollection3D[currentHistogramFolder][type][jetbin]->add(name, tmp_title.str(), nXBins, xmin, xmax,
						nYBins, ymin, ymax, nZBins, zmin, zmax);
			}
		}
	}
}

void HistogramManager::addH3D_JetBinned(std::string name, std::string title, unsigned int nXBins, float xmin,
		float xmax, unsigned int nYBins, float ymin, float ymax, unsigned int nZBins, float zmin, float zmax) {

	if (jetCollection3D.find(currentHistogramFolder) == jetCollection3D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_title;
				tmp_title << title << " (" << JetBin::names[jetbin] << ")";
				jetCollection3D[currentHistogramFolder][type][jetbin]->add(name, tmp_title.str(), nXBins, xmin, xmax, nYBins,
						ymin, ymax, nZBins, zmin, zmax);
			}
		}
	}

}

void HistogramManager::addH3D(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
		unsigned int nYBins, float ymin, float ymax, unsigned int nZBins, float zmin, float zmax) {
	if (collection3D.find(currentHistogramFolder) == collection3D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			collection3D[currentHistogramFolder][type]->add(name, title, nXBins, xmin, xmax, nYBins, ymin, ymax, nZBins,
					zmin, zmax);
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

TH1Ptr HistogramManager::operator [](std::string histname) {
	return H1D(histname);
}

TH1Ptr HistogramManager::H1D(std::string histname) {
	if (debugMode)
		if (collection1D.find(currentHistogramFolder) == collection1D.end()
				|| !collection1D[currentHistogramFolder][currentDataType]->contains(histname)) {
			throw HistogramAccessException(
					"Histogram '" + histname + "' in collection '" + currentHistogramFolder + "' not found");
		}

	return collection1D[currentHistogramFolder][currentDataType]->get(histname);
}

TH1Ptr HistogramManager::H1D_JetBinned(std::string histname) {
	if (debugMode)
		if (jetCollection1D.find(currentHistogramFolder) == jetCollection1D.end()
				|| !jetCollection1D[currentHistogramFolder][currentDataType][currentJetbin]->contains(histname)) {
			throw HistogramAccessException(
					"Histogram '" + histname + "' in jet collection '" + currentHistogramFolder + "' not found");
		}

	return jetCollection1D[currentHistogramFolder][currentDataType][currentJetbin]->get(histname);
}

TH1Ptr HistogramManager::H1D_BJetBinned(std::string histname) {
	if (debugMode)
		if (bJetCollection1D.find(currentHistogramFolder) == bJetCollection1D.end()
				|| !bJetCollection1D[currentHistogramFolder][currentDataType][currentBJetbin]->contains(histname)) {
			throw HistogramAccessException(
					"Histogram '" + histname + "' in bjet collection '" + currentHistogramFolder + "' not found");
		}

	return bJetCollection1D[currentHistogramFolder][currentDataType][currentBJetbin]->get(histname);
}

TH2Ptr HistogramManager::H2D_JetBinned(std::string histname) {
	if (debugMode)
		if (jetCollection2D.find(currentHistogramFolder) == jetCollection2D.end()
				|| !jetCollection2D[currentHistogramFolder][currentDataType][currentJetbin]->contains(histname)) {
			throw HistogramAccessException(
					"Histogram '" + histname + "' in jet collection '" + currentHistogramFolder + "' not found");
		}

	return jetCollection2D[currentHistogramFolder][currentDataType][currentJetbin]->get(histname);
}

TH2Ptr HistogramManager::H2D(std::string histname) {
	if (debugMode)
		if (collection2D.find(currentHistogramFolder) == collection2D.end()
				|| !collection2D[currentHistogramFolder][currentDataType]->contains(histname)) {
			throw HistogramAccessException(
					"Histogram '" + histname + "' in collection '" + currentHistogramFolder + "' not found");
		}

	return collection2D[currentHistogramFolder][currentDataType]->get(histname);
}

TH2Ptr HistogramManager::H2D_BJetBinned(std::string histname) {
	if (debugMode)
		if (bJetCollection2D.find(currentHistogramFolder) == bJetCollection2D.end()
				|| !bJetCollection2D[currentHistogramFolder][currentDataType][currentBJetbin]->contains(histname)) {
			throw HistogramAccessException(
					"Histogram '" + histname + "' in bjet collection '" + currentHistogramFolder + "' not found");
		}

	return bJetCollection2D[currentHistogramFolder][currentDataType][currentBJetbin]->get(histname);
}

TH3Ptr HistogramManager::H3D_JetBinned(std::string histname) {
	if (debugMode)
		if (jetCollection3D.find(currentHistogramFolder) == jetCollection3D.end()
				|| !jetCollection3D[currentHistogramFolder][currentDataType][currentJetbin]->contains(histname)) {
			throw HistogramAccessException(
					"Histogram '" + histname + "' in jet collection '" + currentHistogramFolder + "' not found");
		}

	return jetCollection3D[currentHistogramFolder][currentDataType][currentJetbin]->get(histname);
}

TH3Ptr HistogramManager::H3D(std::string histname) {
	if (debugMode)
		if (collection3D.find(currentHistogramFolder) == collection3D.end()
				|| !collection3D[currentHistogramFolder][currentDataType]->contains(histname)) {
			throw HistogramAccessException(
					"Histogram '" + histname + "' in collection '" + currentHistogramFolder + "' not found");
		}

	return collection3D[currentHistogramFolder][currentDataType]->get(histname);
}

TH3Ptr HistogramManager::H3D_BJetBinned(std::string histname) {
	if (debugMode)
		if (bJetCollection3D.find(currentHistogramFolder) == bJetCollection3D.end()
				|| !bJetCollection3D[currentHistogramFolder][currentDataType][currentBJetbin]->contains(histname)) {
			throw HistogramAccessException(
					"Histogram '" + histname + "' in bjet collection '" + currentHistogramFolder + "' not found");
		}

	return bJetCollection3D[currentHistogramFolder][currentDataType][currentBJetbin]->get(histname);
}

TH2Ptr HistogramManager::operator ()(std::string histname) {
	return H2D(histname);
}

void HistogramManager::prepareForSeenDataTypes(
		const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes) {
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

			for (unordered_map<std::string, TH1Array>::iterator iter = collection1D.begin(); iter != collection1D.end();
					++iter) {
				iter->second[type]->writeToFile(histFiles.at(type));
			}

			for (unordered_map<std::string, TH2Array>::iterator iter = collection2D.begin(); iter != collection2D.end();
					++iter) {
				iter->second[type]->writeToFile(histFiles.at(type));
			}

			for (unordered_map<std::string, TH3Array>::iterator iter = collection3D.begin(); iter != collection3D.end();
					++iter) {
				iter->second[type]->writeToFile(histFiles.at(type));
			}

			for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
				for (unordered_map<std::string, TH1MultiArray>::iterator iter = jetCollection1D.begin();
						iter != jetCollection1D.end(); ++iter) {
					iter->second[type][jetbin]->writeToFile(histFiles.at(type));
				}

				for (unordered_map<std::string, TH2MultiArray>::iterator iter = jetCollection2D.begin();
						iter != jetCollection2D.end(); ++iter) {
					iter->second[type][jetbin]->writeToFile(histFiles.at(type));
				}

				for (unordered_map<std::string, TH3MultiArray>::iterator iter = jetCollection3D.begin();
						iter != jetCollection3D.end(); ++iter) {
					iter->second[type][jetbin]->writeToFile(histFiles.at(type));
				}
			}

			for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {

				for (unordered_map<std::string, TH1MultiArray>::iterator iter = bJetCollection1D.begin();
						iter != bJetCollection1D.end(); ++iter) {
					iter->second[type][jetbin]->writeToFile(histFiles.at(type));
				}

				for (unordered_map<std::string, TH2MultiArray>::iterator iter = bJetCollection2D.begin();
						iter != bJetCollection2D.end(); ++iter) {
					iter->second[type][jetbin]->writeToFile(histFiles.at(type));
				}

				for (unordered_map<std::string, TH3MultiArray>::iterator iter = bJetCollection3D.begin();
						iter != bJetCollection3D.end(); ++iter) {
					iter->second[type][jetbin]->writeToFile(histFiles.at(type));
				}
			}
			histFiles.at(type)->Write();
			histFiles.at(type)->Close();
		}

	}
}

void HistogramManager::createJetSummedHistograms(DataType::value type) {
	for (unsigned short jetbinSum = 0; jetbinSum < JetBinSummed::NUMBER_OF_SUMMED_JET_BINS; ++jetbinSum) {
		for (unordered_map<std::string, TH1MultiArray>::iterator iter = jetCollection1D.begin();
				iter != jetCollection1D.end(); ++iter) {
			string collectionName = iter->first;
			TH1CollectionRef coll = iter->second[type][jetbinSum];
			if (collection1D.find(collectionName) == collection1D.end())
				addHistogramFolder(collectionName);

			unordered_map<std::string, TH1Ptr > histMap = coll->getAllHistograms();
			for (unordered_map<std::string, TH1Ptr >::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << JetBinSummed::names[jetbinSum];

				TH1Ptr hist((TH1*) histIter->second->Clone(tmp_name.str().c_str()));
				tmp_title << hist->GetTitle() << " (" << JetBinSummed::names[jetbinSum] << ")";
				hist->SetTitle(tmp_title.str().c_str());

				for (unsigned short jetbin = jetbinSum + 1; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
					hist->Add(jetCollection1D[collectionName][type][jetbin]->get(name).get());
				}
				collection1D[collectionName][type]->add(tmp_name.str(), hist);
			}
		}

		for (unordered_map<std::string, TH2MultiArray>::iterator iter = jetCollection2D.begin();
				iter != jetCollection2D.end(); ++iter) {
			string collectionName = iter->first;
			TH2CollectionRef coll = iter->second[type][jetbinSum];
			if (collection2D.find(collectionName) == collection2D.end())
				addHistogramFolder(collectionName);

			unordered_map<std::string, TH2Ptr > histMap = coll->getAllHistograms();
			for (unordered_map<std::string, TH2Ptr >::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << JetBinSummed::names[jetbinSum];

				TH2Ptr hist((TH2*) histIter->second->Clone(tmp_name.str().c_str()));
				tmp_title << hist->GetTitle() << " (" << JetBinSummed::names[jetbinSum] << ")";
				hist->SetTitle(tmp_title.str().c_str());

				for (unsigned short jetbin = jetbinSum + 1; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
					hist->Add(jetCollection2D[collectionName][type][jetbin]->get(name).get());
				}
				collection2D[collectionName][type]->add(tmp_name.str(), hist);
			}
		}

		for (unordered_map<std::string, TH3MultiArray>::iterator iter = jetCollection3D.begin();
				iter != jetCollection3D.end(); ++iter) {
			string collectionName = iter->first;
			TH3CollectionRef coll = iter->second[type][jetbinSum];
			if (collection3D.find(collectionName) == collection3D.end())
				addHistogramFolder(collectionName);

			unordered_map<std::string, TH3Ptr> histMap = coll->getAllHistograms();
			for (unordered_map<std::string, TH3Ptr>::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << JetBinSummed::names[jetbinSum];

				TH3Ptr hist((TH3*) histIter->second->Clone(tmp_name.str().c_str()));
				tmp_title << hist->GetTitle() << " (" << JetBinSummed::names[jetbinSum] << ")";
				hist->SetTitle(tmp_title.str().c_str());

				for (unsigned short jetbin = jetbinSum + 1; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
					hist->Add(jetCollection3D[collectionName][type][jetbin]->get(name).get());
				}
				collection3D[collectionName][type]->add(tmp_name.str(), hist);
			}
		}

	}
}

void HistogramManager::createBJetSummedHistograms(DataType::value type) {
	for (unsigned short jetbinSum = 0; jetbinSum < BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS; ++jetbinSum) {
		for (unordered_map<std::string, TH1MultiArray>::iterator iter = bJetCollection1D.begin();
				iter != bJetCollection1D.end(); ++iter) {
			string collectionName = iter->first;
			TH1CollectionRef coll = iter->second[type][jetbinSum];
			if (collection1D.find(collectionName) == collection1D.end())
				addHistogramFolder(collectionName);

			unordered_map<std::string, TH1Ptr > histMap = coll->getAllHistograms();
			for (unordered_map<std::string, TH1Ptr >::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << BJetBinSummed::names[jetbinSum];

				TH1Ptr hist((TH1*) histIter->second->Clone(tmp_name.str().c_str()));
				tmp_title << hist->GetTitle() << " (" << BJetBinSummed::names[jetbinSum] << ")";
				hist->SetTitle(tmp_title.str().c_str());

				for (unsigned short jetbin = jetbinSum + 1; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
					hist->Add(bJetCollection1D[collectionName][type][jetbin]->get(name).get());
				}
				collection1D[collectionName][type]->add(tmp_name.str(), hist);
			}
		}

		for (unordered_map<std::string, TH2MultiArray>::iterator iter = bJetCollection2D.begin();
				iter != bJetCollection2D.end(); ++iter) {
			string collectionName = iter->first;
			TH2CollectionRef coll = iter->second[type][jetbinSum];
			if (collection2D.find(collectionName) == collection2D.end())
				addHistogramFolder(collectionName);

			unordered_map<std::string, TH2Ptr > histMap = coll->getAllHistograms();
			for (unordered_map<std::string, TH2Ptr >::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << BJetBinSummed::names[jetbinSum];

				TH2Ptr hist((TH2*) histIter->second->Clone(tmp_name.str().c_str()));
				tmp_title << hist->GetTitle() << " (" << BJetBinSummed::names[jetbinSum] << ")";
				hist->SetTitle(tmp_title.str().c_str());

				for (unsigned short jetbin = jetbinSum + 1; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
					hist->Add(bJetCollection2D[collectionName][type][jetbin]->get(name).get());
				}
				collection2D[collectionName][type]->add(tmp_name.str(), hist);
			}
		}

		for (unordered_map<std::string, TH3MultiArray>::iterator iter = bJetCollection3D.begin();
				iter != bJetCollection3D.end(); ++iter) {
			string collectionName = iter->first;
			TH3CollectionRef coll = iter->second[type][jetbinSum];
			if (collection3D.find(collectionName) == collection3D.end())
				addHistogramFolder(collectionName);

			unordered_map<std::string, TH3Ptr> histMap = coll->getAllHistograms();
			for (unordered_map<std::string, TH3Ptr>::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << BJetBinSummed::names[jetbinSum];

				TH3Ptr hist((TH3*) histIter->second->Clone(tmp_name.str().c_str()));
				tmp_title << hist->GetTitle() << " (" << BJetBinSummed::names[jetbinSum] << ")";
				hist->SetTitle(tmp_title.str().c_str());

				for (unsigned short jetbin = jetbinSum + 1; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
					hist->Add(bJetCollection3D[collectionName][type][jetbin]->get(name).get());
				}
				collection3D[collectionName][type]->add(tmp_name.str(), hist);
			}
		}

	}
}

void HistogramManager::add1DHistogramFolder(string folder) {
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type))
			collection1D[folder].at(type) = TH1CollectionRef(new TH1Collection(folder));
	}
}

void HistogramManager::add1DJetBinnedHistogramFolder(string folder) {
	jetCollection1D[folder].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]);

	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
				TH1CollectionRef collection(new TH1Collection(folder));
				collection->setSuffix(JetBin::names.at(jetbin));
				jetCollection1D[folder][type][jetbin] = collection;
			}
		}
	}
}

void HistogramManager::add1DBJetBinnedHistogramFolder(string folder) {
	bJetCollection1D[folder].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]);

	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
				TH1CollectionRef collection(new TH1Collection(folder));
				collection->setSuffix(BJetBin::names.at(jetbin));
				bJetCollection1D[folder][type][jetbin] = collection;
			}
		}
	}
}

void HistogramManager::add2DHistogramFolder(string folder) {
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type))
			collection2D[folder].at(type) = TH2CollectionRef(new TH2Collection(folder));
	}
}

void HistogramManager::add2DJetBinnedHistogramFolder(string folder) {
	jetCollection2D[folder].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]);

	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
				TH2CollectionRef collection(new TH2Collection(folder));
				collection->setSuffix(JetBin::names.at(jetbin));
				jetCollection2D[folder][type][jetbin] = collection;
			}
		}
	}
}

void HistogramManager::add2DBJetBinnedHistogramFolder(string folder) {
	bJetCollection2D[folder].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][BJetBin::NUMBER_OF_BJET_BINS]);

	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
				TH2CollectionRef collection(new TH2Collection(folder));
				collection->setSuffix(BJetBin::names.at(jetbin));
				bJetCollection2D[folder][type][jetbin] = collection;
			}
		}
	}
}

void HistogramManager::add3DHistogramFolder(string folder) {
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type))
			collection3D[folder].at(type) = TH3CollectionRef(new TH3Collection(folder));
	}
}

void HistogramManager::add3DJetBinnedHistogramFolder(string folder) {
	jetCollection3D[folder].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][JetBin::NUMBER_OF_JET_BINS]);

	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
				TH3CollectionRef collection(new TH3Collection(folder));
				collection->setSuffix(JetBin::names.at(jetbin));
				jetCollection3D[folder][type][jetbin] = collection;
			}
		}
	}
}

void HistogramManager::add3DBJetBinnedHistogramFolder(string folder) {
	bJetCollection3D[folder].resize(boost::extents[DataType::NUMBER_OF_DATA_TYPES][BJetBin::NUMBER_OF_BJET_BINS]);

	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
				TH3CollectionRef collection(new TH3Collection(folder));
				collection->setSuffix(BJetBin::names.at(jetbin));
				bJetCollection3D[folder][type][jetbin] = collection;
			}
		}
	}
}

void HistogramManager::addHistogramFolder(string collection) {
	add1DHistogramFolder(collection);
	add1DJetBinnedHistogramFolder(collection);
	add1DBJetBinnedHistogramFolder(collection);
	add2DHistogramFolder(collection);
	add2DJetBinnedHistogramFolder(collection);
	add2DBJetBinnedHistogramFolder(collection);
	add3DHistogramFolder(collection);
	add3DJetBinnedHistogramFolder(collection);
	add3DBJetBinnedHistogramFolder(collection);
}

void HistogramManager::setCurrentHistogramFolder(string collection) {
	currentHistogramFolder = collection;
}

void HistogramManager::enableDebugMode(bool enable) {
	debugMode = enable;
}
}
