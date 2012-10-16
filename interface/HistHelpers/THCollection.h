/*
 * THCollection.h
 *
 *  Created on: 9 Aug 2010
 *      Author: kreczko
 */

#ifndef THCOLLECTION_H_
#define THCOLLECTION_H_
#include <string>

#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"

#include <boost/unordered_map.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>
#include <iostream>
#include "StringOperations.h"

namespace BAT {

template<class histType>
class THCollection {
    typedef boost::shared_ptr<histType> histPointer;
    typedef boost::unordered_map<std::string, histPointer> map;

protected:
    boost::shared_ptr<TFile> histogramFile;
    std::string path;
    std::vector<std::string> directories;
    map histMap;
    std::string prefix, suffix;
public:

    THCollection(std::string virtualPath = "") :
        histogramFile(),
        path(virtualPath),
        directories(getDirectoriesFromPath(path)),
        histMap(),
        prefix(""),
        suffix("") {

    }

    ~THCollection() {

    }

    void add(std::string name, std::string title, unsigned int numberOfBins, float xmin, float xmax);
    void add(std::string name, std::string title, unsigned int numberOfBins, float *xbins);
    void add(std::string name, std::string title, unsigned int numberOfXBins, float xmin, float xmax,
            unsigned int numberOfYBins, float ymin, float ymax);
    void add(std::string name, std::string title, unsigned int numberOfXBins, float xmin, float xmax,
			unsigned int numberOfYBins, float ymin, float ymax, unsigned int numberOfZBins, float zmin, float zmax);

    void add(std::string name, boost::shared_ptr<histType> hist){
        histMap[name] = hist;
    }

    boost::shared_ptr<histType> get(std::string name) {
        return histMap[name];
    }

    unsigned int size() const {
        return histMap.size();
    }

    void writeToFile(boost::shared_ptr<TFile> histFile) {
        histogramFile = histFile;
        histogramFile->cd();
        writeDirectories();
        histogramFile->Cd(path.c_str());
        writeHistograms();
    }

    void setPrefix(std::string pre){
        prefix = pre;
    }

    void setSuffix(std::string suf){
        suffix = suf;
    }

    const map& getAllHistograms(){
        return histMap;
    }

    bool contains(std::string name){
        return histMap.find(name) != histMap.end();
    }

private:

    void writeDirectories() {
        std::string currentPath = "";

        for (unsigned short index = 0; index < directories.size(); ++index) {
            const std::string dir = directories.at(index);

            if (index == 0) {
                if (histogramFile->Get(dir.c_str()) == 0)
                    histogramFile->mkdir(dir.c_str());
                currentPath = dir;
            } else {
                TDirectory* currentDir = (TDirectory*) histogramFile->Get(currentPath.c_str());
                assert(currentDir != 0);
                if (currentDir->Get(dir.c_str()) == 0)
                    currentDir->mkdir(dir.c_str());
                currentPath += "/" + dir;
            }
        }
    }

    void writeHistograms() {
        for (typename map::const_iterator iter = histMap.begin(); iter != histMap.end(); ++iter) {
            std::string newName(iter->second->GetName());
            if (prefix != "") {
                newName = prefix + "_" + newName;
            }
            if (suffix != "")
                newName = newName + "_" + suffix;
            iter->second->Write(newName.c_str());
        }
    }

};

typedef THCollection<TH1> TH1Collection;
typedef THCollection<TH2> TH2Collection;
typedef THCollection<TH3> TH3Collection;
typedef boost::shared_ptr<THCollection<TH1> > TH1CollectionRef;
typedef boost::shared_ptr<THCollection<TH2> > TH2CollectionRef;
typedef boost::shared_ptr<THCollection<TH3> > TH3CollectionRef;

}

#endif /* THCOLLECTION_H_ */
