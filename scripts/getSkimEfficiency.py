from __future__ import division

from optparse import OptionParser
import sys
from fileInfo import *
from ROOT import *
from mergeROOTFilesWithCompression import getProcess

pathToSkimHist = "LJFilter/EventCount/EventCounter"
pathToEventTree = "rootTupleTree/tree"

def getSkimmedEvents(files):
    skimInfo = getSkimInfo(files)
    skimHist = skimInfo[0]
    
    numberOfTotalEvents = skimHist.GetBinContent(1)
    numberOfSkimmedEvents = skimHist.GetBinContent(2)
    numberOfEventsAfterNoiseFilter = skimInfo[1]
    electronSkimEfficiency = numberOfSkimmedEvents/numberOfTotalEvents
    TotalSkimEfficiency = numberOfEventsAfterNoiseFilter/numberOfTotalEvents
    return {'total': numberOfTotalEvents, 'skimmed':numberOfSkimmedEvents, 
            'electronSkimEfficiency':electronSkimEfficiency,
            'afterSkimAndNoiseFilter':numberOfEventsAfterNoiseFilter,
            'totalSkimEfficiency':TotalSkimEfficiency}
            
def getSkimInfo(files):
    skimHist = None
    eventBranch = None
    eventTree = None
    n_skim = 0
    TfOpen = TFile.Open
    gcd = gROOT.cd
    
    for file in files:
        f = TfOpen(file)
        gcd()
        eventTree = f.Get(pathToEventTree)
        if eventTree is None:
            print 'Could not find tree', pathToEventTree, 'in file', file
        else:
            eventBranch = eventTree.GetBranch('event')
            
        if eventBranch is None:
            print 'Could not find branch', pathToEventTree + '/event', 'in file', file
        else:
            n_skim += eventBranch.GetEntries()
        
        if skimHist is None:
            skimHist = f.Get(pathToSkimHist)
            if skimHist is None:
                print 'Could not find histogram', pathToSkimHist, 'in file', file
                continue
        else:
            hist = f.Get(pathToSkimHist)
            if hist is None:
                print 'Could not find histogram', pathToSkimHist, 'in file', file
                continue
            skimHist.Add(hist)
    return (skimHist, n_skim)
            

if __name__ == "__main__":
    args = sys.argv
    if not len(args) == 2:
        print "Please specify a folder to merge files in."
        sys.exit()
        
    path = sys.argv[1]
    files = getROOTFiles(path)
    events = getSkimmedEvents(files)
    print '='*100
    print '{0:70} : {1:15d}'.format('Total number of files', len(files))
    print '{0:70} : {1:15s}'.format('Process recognised', getProcess(files[0]))
    print '{0:70} : {1:15d}'.format('Total number of events', int(events['total']))
    print '{0:70} : {1:15d}'.format('Number of events after electron skim', int(events['skimmed']))
    print '{0:70} : {1:15d}'.format('Number of events after electron skim and HBHE noise filter', events['afterSkimAndNoiseFilter'])
    print
    print '{0:70} : {1:15f}'.format('Electron skim efficiency', events['electronSkimEfficiency'])
    print '{0:70} : {1:15f}'.format('Total skim efficiency', events['totalSkimEfficiency'])
    print '='*100
#    print 'Total number of files:', len(files)
#    print 'Process recognised:', getProcess(files[0])
#    print 'Total number of events:', int(events['total'])
#    print 'Number of events after electron skim:', int(events['skimmed'])
#    print 'Electron skim efficiency:', events['electronSkimEfficiency']
#    print 'Number of events after electron skim and HBHE noise filter:', events['afterSkimAndNoiseFilter']
#    print 'Total skim efficiency:', events['totalSkimEfficiency']
