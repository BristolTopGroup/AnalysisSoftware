'''
Created on Mar 5, 2011

@author: lkreczko
'''
from __future__ import division
from ROOT import *

def getBranchInfo(listOfBranches):
    branches = []
    bapp = branches.append
    for branch in listOfBranches:
        info = {}
        info['name'] = branch.GetName()
        info['totalSize'] = branch.GetTotalSize()
        info['totalBytes'] = branch.GetTotBytes()
        info['zippedBytes'] = branch.GetZipBytes()
        bapp(info)
    return branches


def printTwikiTable(branches, filesize):
    
    prevObj = ' '
    info = {}
    info['totalSize'] = 0
    info['zippedBytes'] = 0
    info['totalBytes'] = 0
    for branch in sorted(branches):
        name = branch['name']
        size = branch['totalSize'] / 1024 / 1024 #MB
        zipSize = branch['zippedBytes'] / 1024 / 1024#MB
        compression = size / zipSize
        totalBytes = branch['totalBytes'] / 1024 / 1024#MB
        buffer = (size - totalBytes) * 1024#KB
        fraction = zipSize / filesize * 100#%
        
        obj = ' '
        if '.' in name:
            obj = name.split('.')[0] + '.'
        else:
            obj = name.capitalize()
        
        if not name.startswith(prevObj):
            if '.' in prevObj:
                Osize = info['totalSize']
                OzipSize = info['zippedBytes']
                Ocompression = Osize / OzipSize
                Obuffer = (size - info['totalBytes'] / 1024 / 1024) * 1024#KB
                Ofraction = OzipSize / filesize * 100
                print '| *Total*  |  %.3f |  %.3f | %.2f |  %.3f |  %.2f%% |' % (Osize, OzipSize, Ocompression, Obuffer, Ofraction)
                print
            #print summary
            print '---+++ %s' % obj.replace('.', '')
            print '|  *%s*  ||||||' % obj.replace('.', '')
            print '| *Name* | *Total Size (MB)* | *Compressed size (MB)* | *compression factor* | *Buffer (KB)* | *Fraction of file size* |'
            info['totalSize'] = 0
            info['zippedBytes'] = 0
            info['totalBytes'] = 0
        else:
            info['totalSize'] += size
            info['zippedBytes'] += zipSize
            info['totalBytes'] += totalBytes
        print '| !%s  |  %.3f |  %.3f | %.2f |  %.3f |  %.2f%% |' % (name, size, zipSize, compression, buffer, fraction)
        prevObj = obj

if __name__ == '__main__':
    gROOT.SetBatch(1);
    chain = TChain("rootTupleTree/tree");

    #chain.Add("/storage/TopQuarkGroup/mc/Spring11/TTJets_TuneD6T_7TeV-madgraph-tauola_Spring11-PU_S1_START311_V1G1-v1/LQNTuple_TTJets_merged_1.root");
    chain.Add("/storage/TopQuarkGroup/mc/fall10_7TeV_v1_e25skim/TTJets_TuneD6T_7TeV-madgraph-tauola_Fall10-START38_V12-v2/nTuple_ttjet_merged_1.root");
    filesize = chain.GetFile().GetSize() / 1024 / 1024#MB
    
    branches = getBranchInfo(chain.GetListOfBranches())
    numberOfEvents = chain.GetEntries()
    print '---++ MC content'
    print 'Size of event: %.3f KB' % (filesize/numberOfEvents*1024)
    printTwikiTable(branches, filesize)
    
    chain = TChain("rootTupleTree/tree");
    #chain.Add("/storage/TopQuarkGroup/data/Run2011A-PromptReco-v2/*_1.root");
    chain.Add("/storage/TopQuarkGroup/data/Nov4ReReco_JEC_Spring_V8_36.145pb_e25skim/Run2010B/*_1.root");
    filesize = chain.GetFile().GetSize() / 1024 / 1024#MB
    
    branches = getBranchInfo(chain.GetListOfBranches())
    
    print '---++ DATA content'
    print 'Size of event: %.3f KB' % (filesize/numberOfEvents*1024)
    printTwikiTable(branches, filesize)
