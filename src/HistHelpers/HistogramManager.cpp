/*
 * HistogramManager.cpp
 *
 *  Created on: 4 Jul 2010
 *      Author: kreczko
 */

#include "../../interface/HistHelpers/HistogramManager.h"
#include "../../interface/Readers/NTupleEventReader.h"
#include "../../interface/GlobalVariables.h"
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>

using namespace boost;
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
		collection3D(), //
		debugMode(false) {
}

HistogramManager::~HistogramManager() {
}

void HistogramManager::addH1D(std::string name, std::string title, unsigned int numberOfBins, float xmin, float xmax) {
	if (numberOfBins > 200000)
		cout << "WARNING: Histogram '" << name << "' has more than 200000 bins. Potential memory monster!" << endl;
	if (collection1D.find(currentHistogramFolder) == collection1D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			collection1D[currentHistogramFolder][type]->add(name, title, numberOfBins, xmin, xmax);
		}
	}
}

void HistogramManager::addH1D(std::string name, std::string title, unsigned int numberOfBins, float *xbins) {
	if (numberOfBins > 200000)
		cout << "WARNING: Histogram '" << name << "' has more than 200000 bins. Potential memory monster!" << endl;
	if (collection1D.find(currentHistogramFolder) == collection1D.end())
		addHistogramFolder(currentHistogramFolder);

	for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			collection1D[currentHistogramFolder][type]->add(name, title, numberOfBins, xbins);
		}
	}
}

void HistogramManager::addH1D_JetBinned(std::string name, std::string title, unsigned int numberOfBins, float xmin,
		float xmax) {

	for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << JetBin::names[jetbin];
				tmp_title << title << " (" << JetBin::names[jetbin] << ")";
				addH1D(tmp_name.str(), tmp_title.str(), numberOfBins, xmin, xmax);
			}
		}
	}
}

void HistogramManager::addH1D_JetBinned(std::string name, std::string title, unsigned int numberOfBins, float *xbins) {
	for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << JetBin::names[jetbin];
				tmp_title << title << " (" << JetBin::names[jetbin] << ")";
				addH1D(tmp_name.str(), tmp_title.str(), numberOfBins, xbins);
			}
		}
	}
}

void HistogramManager::addH1D_BJetBinned(std::string name, std::string title, unsigned int numberOfBins, float xmin,
		float xmax) {
	for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << BJetBin::names[jetbin];
				tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
				addH1D(tmp_name.str(), tmp_title.str(), numberOfBins, xmin, xmax);
			}
		}
	}
}

void HistogramManager::addH1D_BJetBinned(std::string name, std::string title, unsigned int numberOfBins, float *xbins) {
	for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << BJetBin::names[jetbin];
				tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
				addH1D(tmp_name.str(), tmp_title.str(), numberOfBins, xbins);
			}
		}
	}
}

void HistogramManager::addH2D_BJetBinned(std::string name, std::string title, unsigned int nXBins, float xmin,
		float xmax, unsigned int nYBins, float ymin, float ymax) {
	for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << BJetBin::names[jetbin];
				tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
				addH2D(tmp_name.str(), tmp_title.str(), nXBins, xmin, xmax, nYBins, ymin, ymax);
			}
		}
	}
}

void HistogramManager::addH2D_JetBinned(std::string name, std::string title, unsigned int nXBins, float xmin,
		float xmax, unsigned int nYBins, float ymin, float ymax) {
	for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << JetBin::names[jetbin];
				tmp_title << title << " (" << JetBin::names[jetbin] << ")";
				addH2D(tmp_name.str(), tmp_title.str(), nXBins, xmin, xmax, nYBins, ymin, ymax);
			}
		}
	}
}

void HistogramManager::addH2D(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
		unsigned int nYBins, float ymin, float ymax) {
	if (nXBins * nYBins > 260000)
		cout << "WARNING: Histogram '" << name << "' has more than 5000000 bins. Potential memory monster!" << endl;

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

	for (unsigned short jetbin = 0; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << BJetBin::names[jetbin];
				tmp_title << title << " (" << BJetBin::names[jetbin] << ")";
				addH3D(tmp_name.str(), tmp_title.str(), nXBins, xmin, xmax, nYBins, ymin, ymax, nZBins, zmin, zmax);
			}
		}
	}
}

void HistogramManager::addH3D_JetBinned(std::string name, std::string title, unsigned int nXBins, float xmin,
		float xmax, unsigned int nYBins, float ymin, float ymax, unsigned int nZBins, float zmin, float zmax) {
	for (unsigned short jetbin = 0; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
		for (unsigned short type = DataType::ElectronHad; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
			if (seenDataTypes.at(type)) {
				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << JetBin::names[jetbin];
				tmp_title << title << " (" << JetBin::names[jetbin] << ")";
				addH3D(tmp_name.str(), tmp_title.str(), nXBins, xmin, xmax, nYBins, ymin, ymax, nZBins, zmin, zmax);
			}
		}
	}
}

void HistogramManager::addH3D(std::string name, std::string title, unsigned int nXBins, float xmin, float xmax,
		unsigned int nYBins, float ymin, float ymax, unsigned int nZBins, float zmin, float zmax) {
	if (nXBins * nYBins * nZBins > 200000)
		cout << "WARNING: Histogram '" << name << "' has more than 200000 bins. Potential memory monster!" << endl;

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
	if (jetbin >= JetBin::NUMBER_OF_JET_BINS) {
		currentJetbin = JetBin::NUMBER_OF_JET_BINS - 1;
	} else
		currentJetbin = jetbin;
}

void HistogramManager::setCurrentBJetBin(unsigned int jetbin) {
	if (jetbin >= BJetBin::NUMBER_OF_BJET_BINS) {
		currentBJetbin = BJetBin::NUMBER_OF_BJET_BINS - 1;
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
	return H1D(histname + "_" + JetBin::names[currentJetbin]);
}

TH1Ptr HistogramManager::H1D_BJetBinned(std::string histname) {
	return H1D(histname + "_" + BJetBin::names[currentBJetbin]);
}

TH2Ptr HistogramManager::H2D_JetBinned(std::string histname) {
	return H2D(histname + "_" + JetBin::names[currentJetbin]);
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
	return H2D(histname + "_" + BJetBin::names[currentBJetbin]);
}

TH3Ptr HistogramManager::H3D_JetBinned(std::string histname) {
	return H3D(histname + "_" + JetBin::names[currentJetbin]);
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
	return H3D(histname + "_" + BJetBin::names[currentBJetbin]);
}

TH2Ptr HistogramManager::operator ()(std::string histname) {
	return H2D(histname);
}

void HistogramManager::prepareForSeenDataTypes(
		const boost::array<bool, DataType::NUMBER_OF_DATA_TYPES>& seenDataTypes) {
	this->seenDataTypes = seenDataTypes;

	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			const std::string filename = assembleFilename((DataType::value) type);
			boost::shared_ptr<TFile> file(new TFile(filename.c_str(), "RECREATE"));
			file->SetCompressionLevel(7);

			histFiles.at(type) = file;
			collection1D[""].at(type) = TH1CollectionRef(new TH1Collection());
			collection2D[""].at(type) = TH2CollectionRef(new TH2Collection());
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
	std::string jesSyst = (Globals::JESsystematic > 0) ? "_plusJES" : "_minusJES";
	std::string bjetSyst = (Globals::BJetSystematic > 0) ? "_plusBjet" : "_minusBJet";
	std::string lightJetSyst = (Globals::LightJetSystematic > 0) ? "_plusLightJet" : "_minusLightJet";
	std::string suffix = Globals::custom_file_suffix;
	std::string pdfWeight =
			(Globals::pdfWeightNumber != 0) ? "_PDF_" + boost::lexical_cast<std::string>(Globals::pdfWeightNumber) : "";
	if (Globals::JESsystematic == 0)
		jesSyst = "";
	if (Globals::BJetSystematic == 0)
		bjetSyst = "";
	if (Globals::LightJetSystematic == 0)
		lightJetSyst = "";
	if (suffix != "")
		suffix = "_" + suffix;

	str << name << "_" << Globals::luminosity << "pb";
	str << "_" << electronAlgo << "_" << muonAlgo << "_" << jetAlgo << "_" << metAlgo;
	str << jesSyst << pdfWeight << bjetSyst << lightJetSyst << suffix << ".root";
	return str.str();

}

void HistogramManager::writeToDisk() {
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type)) {
			//disable them for now
//			createJetSummedHistograms((DataType::value) type);
//			createBJetSummedHistograms((DataType::value) type);

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

			histFiles.at(type)->Write();
			histFiles.at(type)->Close();
		}

	}
}

void HistogramManager::createJetSummedHistograms(DataType::value type) {
	createJetSummedHistograms1D(type);
	createJetSummedHistograms2D(type);
	createJetSummedHistograms3D(type);
}

void HistogramManager::createJetSummedHistograms1D(DataType::value type) {
	for (unsigned short jetbinSum = 0; jetbinSum < JetBinSummed::NUMBER_OF_SUMMED_JET_BINS; ++jetbinSum) {
		for (unordered_map<std::string, TH1Array>::iterator iter = collection1D.begin(); iter != collection1D.end();
				++iter) {
			string collectionName = iter->first;
			TH1CollectionRef coll = iter->second[type];
			unordered_map<std::string, TH1Ptr> histMap = coll->getAllHistograms();

			for (unordered_map<std::string, TH1Ptr>::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				//check if it is a jet-binned histogram
				if (!boost::algorithm::ends_with(name, JetBin::names[jetbinSum]))
					continue;

				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << JetBinSummed::names[jetbinSum];
				TH1Ptr hist((TH1*) histIter->second->Clone(tmp_name.str().c_str()));
				tmp_title << hist->GetTitle() << " (" << JetBinSummed::names[jetbinSum] << ")";
				hist->SetTitle(tmp_title.str().c_str());
				for (unsigned short jetbin = jetbinSum + 1; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
					string histName = name;
					//replace the suffix of the current <base> histogram with the value in the loop.
					boost::algorithm::replace_last(histName, JetBin::names[jetbinSum], JetBin::names[jetbin]);

					hist->Add(collection1D[collectionName][type]->get(histName).get());
				}
				collection1D[collectionName][type]->add(tmp_name.str(), hist);
			}
		}
	}
}

void HistogramManager::createJetSummedHistograms2D(DataType::value type) {
	for (unsigned short jetbinSum = 0; jetbinSum < JetBinSummed::NUMBER_OF_SUMMED_JET_BINS; ++jetbinSum) {
		for (unordered_map<std::string, TH2Array>::iterator iter = collection2D.begin(); iter != collection2D.end();
				++iter) {
			string collectionName = iter->first;
			TH2CollectionRef coll = iter->second[type];
			unordered_map<std::string, TH2Ptr> histMap = coll->getAllHistograms();

			for (unordered_map<std::string, TH2Ptr>::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				//check if it is a jet-binned histogram
				if (!boost::algorithm::ends_with(name, JetBin::names[jetbinSum]))
					continue;

				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << JetBinSummed::names[jetbinSum];
				TH2Ptr hist((TH2*) histIter->second->Clone(tmp_name.str().c_str()));
				tmp_title << hist->GetTitle() << " (" << JetBinSummed::names[jetbinSum] << ")";
				hist->SetTitle(tmp_title.str().c_str());
				for (unsigned short jetbin = jetbinSum + 1; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
					string histName = name;
					//replace the suffix of the current <base> histogram with the value in the loop.
					boost::algorithm::replace_last(histName, JetBin::names[jetbinSum], JetBin::names[jetbin]);

					hist->Add(collection2D[collectionName][type]->get(histName).get());
				}
				collection2D[collectionName][type]->add(tmp_name.str(), hist);
			}
		}
	}
}

void HistogramManager::createJetSummedHistograms3D(DataType::value type) {
	for (unsigned short jetbinSum = 0; jetbinSum < JetBinSummed::NUMBER_OF_SUMMED_JET_BINS; ++jetbinSum) {
		for (unordered_map<std::string, TH3Array>::iterator iter = collection3D.begin(); iter != collection3D.end();
				++iter) {
			string collectionName = iter->first;
			TH3CollectionRef coll = iter->second[type];
			unordered_map<std::string, TH3Ptr> histMap = coll->getAllHistograms();

			for (unordered_map<std::string, TH3Ptr>::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				//check if it is a jet-binned histogram
				if (!boost::algorithm::ends_with(name, JetBin::names[jetbinSum]))
					continue;

				std::stringstream tmp_name, tmp_title;
				tmp_name << name << "_" << JetBinSummed::names[jetbinSum];
				TH3Ptr hist((TH3*) histIter->second->Clone(tmp_name.str().c_str()));
				tmp_title << hist->GetTitle() << " (" << JetBinSummed::names[jetbinSum] << ")";
				hist->SetTitle(tmp_title.str().c_str());
				for (unsigned short jetbin = jetbinSum + 1; jetbin < JetBin::NUMBER_OF_JET_BINS; ++jetbin) {
					string histName = name;
					//replace the suffix of the current <base> histogram with the value in the loop.
					boost::algorithm::replace_last(histName, JetBin::names[jetbinSum], JetBin::names[jetbin]);

					hist->Add(collection3D[collectionName][type]->get(histName).get());
				}
				collection3D[collectionName][type]->add(tmp_name.str(), hist);
			}
		}
	}
}

void HistogramManager::createBJetSummedHistograms(DataType::value type) {
	createBJetSummedHistograms1D(type);
	createBJetSummedHistograms2D(type);
	createBJetSummedHistograms3D(type);
}

/**
 * 1) loop over all sums
 * 2) loop over all histograms for the given data-type
 * 3) get histogram of same order (sum: >=0 -> hist: 0)
 * 4) add all higher multiplicity bins
 * 5) add summed histogram to collection
 */
void HistogramManager::createBJetSummedHistograms1D(DataType::value type) {
	for (unsigned short jetbinSum = 0; jetbinSum < BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS; ++jetbinSum) {
		for (unordered_map<std::string, TH1Array>::iterator iter = collection1D.begin(); iter != collection1D.end();
				++iter) {
			string collectionName = iter->first;
			TH1CollectionRef coll = iter->second[type];
			unordered_map<std::string, TH1Ptr> histMap = coll->getAllHistograms();

			for (unordered_map<std::string, TH1Ptr>::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				//check if it is a Bjet-binned histogram
				if (!boost::algorithm::ends_with(name, BJetBin::names[jetbinSum]))
					continue;

				string tmp_name(name);
				boost::algorithm::replace_last(tmp_name, BJetBin::names[jetbinSum], BJetBinSummed::names[jetbinSum]);
				TH1Ptr hist((TH1*) histIter->second->Clone(tmp_name.c_str()));
				string title(hist->GetTitle());
				boost::algorithm::replace_last(title, BJetBin::names[jetbinSum], BJetBinSummed::names[jetbinSum]);
				hist->SetTitle(title.c_str());

				for (unsigned short jetbin = jetbinSum + 1; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
					string histName = name;
					//replace the suffix of the current <base> histogram with the value in the loop.
					boost::algorithm::replace_last(histName, BJetBin::names[jetbinSum], BJetBin::names[jetbin]);

					hist->Add(collection1D[collectionName][type]->get(histName).get());
				}
				collection1D[collectionName][type]->add(tmp_name, hist);
			}
		}
	}
}

void HistogramManager::createBJetSummedHistograms2D(DataType::value type) {
	for (unsigned short jetbinSum = 0; jetbinSum < BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS; ++jetbinSum) {
		for (unordered_map<std::string, TH2Array>::iterator iter = collection2D.begin(); iter != collection2D.end();
				++iter) {
			string collectionName = iter->first;
			TH2CollectionRef coll = iter->second[type];
			unordered_map<std::string, TH2Ptr> histMap = coll->getAllHistograms();

			for (unordered_map<std::string, TH2Ptr>::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				//check if it is a Bjet-binned histogram
				if (!boost::algorithm::ends_with(name, BJetBin::names[jetbinSum]))
					continue;

				string tmp_name(name);
				boost::algorithm::replace_last(tmp_name, BJetBin::names[jetbinSum], BJetBinSummed::names[jetbinSum]);
				TH2Ptr hist((TH2*) histIter->second->Clone(tmp_name.c_str()));
				string title(hist->GetTitle());
				boost::algorithm::replace_last(title, BJetBin::names[jetbinSum], BJetBinSummed::names[jetbinSum]);
				hist->SetTitle(title.c_str());

				for (unsigned short jetbin = jetbinSum + 1; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
					string histName = name;
					//replace the suffix of the current <base> histogram with the value in the loop.
					boost::algorithm::replace_last(histName, BJetBin::names[jetbinSum], BJetBin::names[jetbin]);

					hist->Add(collection2D[collectionName][type]->get(histName).get());
				}
				collection2D[collectionName][type]->add(tmp_name, hist);
			}
		}
	}
}

void HistogramManager::createBJetSummedHistograms3D(DataType::value type) {
	for (unsigned short jetbinSum = 0; jetbinSum < BJetBinSummed::NUMBER_OF_SUMMED_BJET_BINS; ++jetbinSum) {
		for (unordered_map<std::string, TH3Array>::iterator iter = collection3D.begin(); iter != collection3D.end();
				++iter) {
			string collectionName = iter->first;
			TH3CollectionRef coll = iter->second[type];
			unordered_map<std::string, TH3Ptr> histMap = coll->getAllHistograms();

			for (unordered_map<std::string, TH3Ptr>::const_iterator histIter = histMap.begin();
					histIter != histMap.end(); ++histIter) {
				string name = histIter->first;
				//check if it is a Bjet-binned histogram
				if (!boost::algorithm::ends_with(name, BJetBin::names[jetbinSum]))
					continue;

				string tmp_name(name);
				boost::algorithm::replace_last(tmp_name, BJetBin::names[jetbinSum], BJetBinSummed::names[jetbinSum]);
				TH3Ptr hist((TH3*) histIter->second->Clone(tmp_name.c_str()));
				string title(hist->GetTitle());
				boost::algorithm::replace_last(title, BJetBin::names[jetbinSum], BJetBinSummed::names[jetbinSum]);
				hist->SetTitle(title.c_str());

				for (unsigned short jetbin = jetbinSum + 1; jetbin < BJetBin::NUMBER_OF_BJET_BINS; ++jetbin) {
					string histName = name;
					//replace the suffix of the current <base> histogram with the value in the loop.
					boost::algorithm::replace_last(histName, BJetBin::names[jetbinSum], BJetBin::names[jetbin]);

					hist->Add(collection3D[collectionName][type]->get(histName).get());
				}
				collection3D[collectionName][type]->add(tmp_name, hist);
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

void HistogramManager::add2DHistogramFolder(string folder) {
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type))
			collection2D[folder].at(type) = TH2CollectionRef(new TH2Collection(folder));
	}
}

void HistogramManager::add3DHistogramFolder(string folder) {
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (seenDataTypes.at(type))
			collection3D[folder].at(type) = TH3CollectionRef(new TH3Collection(folder));
	}
}

void HistogramManager::addHistogramFolder(string collection) {
	add1DHistogramFolder(collection);
	add2DHistogramFolder(collection);
	add3DHistogramFolder(collection);
}

void HistogramManager::setCurrentHistogramFolder(string collection) {
	currentHistogramFolder = collection;
}

void HistogramManager::enableDebugMode(bool enable) {
	debugMode = enable;
}

unsigned int HistogramManager::size() const {
	unsigned int size(0);
	for (unsigned type = 0; type < DataType::NUMBER_OF_DATA_TYPES; ++type) {
		if (!seenDataTypes.at(type))
			continue;
		size += size1D((DataType::value) type);
		size += size2D((DataType::value) type);
		size += size3D((DataType::value) type);
	}

	return size;
}

unsigned int HistogramManager::size1D(DataType::value type) const {
	unsigned int size(0);
	for (unordered_map<std::string, TH1Array>::const_iterator iter = collection1D.begin(); iter != collection1D.end();
			++iter) {
		size += iter->second[type]->size();
	}
	return size;
}

unsigned int HistogramManager::size2D(DataType::value type) const {
	unsigned int size(0);
	for (unordered_map<std::string, TH2Array>::const_iterator iter = collection2D.begin(); iter != collection2D.end();
			++iter) {
		size += iter->second[type]->size();
	}
	return size;
}

unsigned int HistogramManager::size3D(DataType::value type) const {
	unsigned int size(0);
	for (unordered_map<std::string, TH3Array>::const_iterator iter = collection3D.begin(); iter != collection3D.end();
			++iter) {
		size += iter->second[type]->size();
	}
	return size;
}
} //end namespace BAT

