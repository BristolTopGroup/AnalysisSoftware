from __future__ import division
from numpy import arange
from tdrStyle import *
from ROOT import *
#from QCDEstimation import getQCDEstimate#, estimateQCDFor
import QCDEstimation

import HistPlotter
import HistGetter
import inputFiles

cuts = [
        "Event cleaning and High Level Trigger", 
                "exactly one isolated electron", 
                "loose muon veto", 
                "di-lepton veto", 
                "Conversion veto", 
                ">= 3 jets", 
                ">= 4 jets", 
                ">=1 CSV b-tag", 
                ">=2 CSV b-tag" ]

samples = [
           'ttbar', 'wjets', 'zjets',
           'data', 'qcd', 'singleTop']

def printCutFlow(hist):
    files = inputFiles.files
    hists = HistGetter.getHistsFromFiles([hist], files, bJetBins=[])
    hists = HistGetter.addSampleSum(hists)
    
    histname3 = 'EventCount/TTbarEplusJetsSelection_2orMoreBtags'
    
    header = "| Step | TTJet | W+jets | DY + Jets | single top | QCD | Sum MC | Data |"
    row = " | %s  |  %.1f |  %.1f |  %.1f |  %.1f |  %.1f |  %.1f |  %d | "
    print header
    
    numbers = getEventNumbers(hists, hist)# + '_0orMoreBtag')
    for step in range(len(cuts)):
        nums = numbers[step]
        sumMC = nums['ttbar'] + nums['wjets'] + nums['zjets'] + nums['singleTop'] + nums['qcd']
        print row % (cuts[step], nums['ttbar'], nums['wjets'], nums['zjets'], nums['singleTop'], nums['qcd'], sumMC, nums['data'])

def getEventNumbers(hists, histname):        
    eventNumbers = []
    for step in range(len(cuts)):
        events = {}
        for sample in samples:
            events[sample] = hists[sample][histname].GetBinContent(step + 1)
        eventNumbers.append(events)
    return eventNumbers
    

if __name__ == "__main__":
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
    print '='*120
    print 'TTbarEplusJetsRefSelection'
    printCutFlow('EventCount/TTbarEplusJetsRefSelection')
    print '='*120
