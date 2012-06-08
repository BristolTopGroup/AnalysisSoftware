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
        'FilterOutScraping',
    'HighLevelTrigger',
    'GoodPrimaryvertex',
    'OneIsolatedElectron',
    'LooseMuonVeto',
    'Zveto',
    'ConversionRejection',
    'ConversionFinder',
    'AtLeastOneGoodJets',
    'AtLeastTwoGoodJets',
    'AtLeastThreeGoodJets',
    'AtLeastFourGoodJets',
    'MissingTransverseEnergy',
    'AsymmetricJetCuts',]

cuts = [
        "Event cleaning and High Level Trigger", 
                "exactly one isolated electron", 
                "loose muon veto", 
                "di-lepton veto", 
                "Conversion veto (missing hits)", 
                "Conversion finder (partner track)", 
                ">= 1 jets", 
                ">= 2 jets", 
                ">= 3 jets", 
                ">= 4 jets", 
                "jet pT: 1st > 70 GeV, 2nd > 50 GeV",
                ">=1 CSV b-tag", 
                ">=2 CSV b-tag" ]

samples = [
           'ttbar', 'wjets', 'zjets',
           'data', 'qcd', 'singleTop']

def printCutFlow(hist):
#    hists = []
#    hists.append('EventCount/TTbarEplusJetsSelection')
#    hists.append('EventCount/TTbarEplusJetsSelection_3jets')
#    hists.append('EventCount/TTbarEplusJetsSelection_noMETCut_3jets')
#    hists.append('EventCount/TTbarEplusJetsSelection_noMETCut')
    files = inputFiles.files
    hists = HistGetter.getHistsFromFiles([hist], files, bJetBins=[])#HistPlotter.allBjetBins)
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
#    numbersAtleastOneBTag = getEventNumbers(hists, hist + '_1orMoreBtag')
#    numbersAtleastTwoBTags = getEventNumbers(hists, hist + '_2orMoreBtags')
#    
#    nums = numbersAtleastOneBTag[-1]
#    sumMC = nums['ttbar'] + nums['wjets'] + nums['zjets'] + nums['singleTop'] + nums['qcd']
#    print row % ('>= 1 btag', nums['ttbar'], nums['wjets'], nums['zjets'], nums['singleTop'], nums['qcd'], sumMC, nums['data'])
#    
#    nums = numbersAtleastTwoBTags[-1]
#    sumMC = nums['ttbar'] + nums['wjets'] + nums['zjets'] + nums['singleTop'] + nums['qcd']
#    print row % ('>= 2 btag', nums['ttbar'], nums['wjets'], nums['zjets'], nums['singleTop'], nums['qcd'], sumMC, nums['data'])

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
    print 'TTbarEplusJetsRefSelection'
    printCutFlow('EventCount/TTbarEplusJetsRefSelection')
    print '='*60
#    print 'TTbarEplusJetsSelection 3Jets'
#    printCutFlow('EventCount/TTbarEplusJetsSelection_3jets')
#    print '='*60
#    print 'TTbarEplusJetsSelection no MET cut 3 Jets'
#    printCutFlow('EventCount/TTbarEplusJetsSelection_noMETCut_3jets')
#    print '='*60
    print 'TTbarEplusJetsSelection no MET cut, asym jets'
    printCutFlow('EventCount/TTbarEplusJetsRefAsymJetsSelection')
