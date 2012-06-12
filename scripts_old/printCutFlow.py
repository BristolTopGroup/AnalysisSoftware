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
    totalWPlusJets = 47896878 + 71828418 + 25400440 + 7685939 + 10814233
    hists['wjets'][hist].Scale(0)
    hists['W1Jet'][hist].Scale(totalWPlusJets/71828418.*4480.0/31314.)
    hists['W2Jets'][hist].Scale(totalWPlusJets/25400440.*1674./31314.)
    hists['W3Jets'][hist].Scale(totalWPlusJets/7685939.*484.7/31314.)
    hists['W4Jets'][hist].Scale(totalWPlusJets/10814233.*211.7/31314.)
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
            if sample == "wjets":
                tmpHist = hists['W1Jet'][histname] + hists['W2Jets'][histname] + hists['W3Jets'][histname] + hists['W4Jets'][histname]
                events[sample] = tmpHist.GetBinContent(step + 1)
        eventNumbers.append(events)
    return eventNumbers
    

if __name__ == "__main__":
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
    print '='*120
    print 'TTbarEplusJetsRefSelection'
    printCutFlow('EventCount/TTbarEplusJetsRefSelection')
    print '='*120
