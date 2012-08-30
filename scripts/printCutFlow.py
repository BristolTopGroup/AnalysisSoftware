'''
Created on Dec 1, 2011

@author: Lukasz Kreczko

Email: Lukasz.Kreczko@cern.ch

The AnalysisTools produce a 2D histogram with the cut flow for each sample which is saved in the histogram file
First dimension is the cut-stage, second dimension the jet-bin (== 1, ==2, ==3, >=4)

printCutFlow reads 2D histogram
outputFormat = CSV|plain|twiki|latex
'''

from __future__ import division
from ROOT import *
import tools.ROOTFileReader as FileReader
import tools.PlottingUtilities as plotting
import FILES


cuts = None
cuts_electrons = [
        "All events after skim",
        "Event cleaning and High Level Trigger", 
                "exactly one isolated electron", 
                "loose muon veto", 
                "di-lepton veto", 
                "Conversion veto", 
                ">= 3 jets", 
                ">= 4 jets", 
                ">=1 CSV b-tag", 
                ">=2 CSV b-tag" 
        ]

cuts_muons = [
        "All events after skim",
        "Event cleaning and High Level Trigger", 
                "exactly one isolated muon", 
                "loose lepton veto", 
                "di-lepton veto", 
                ">= 3 jets", 
                ">= 4 jets", 
                ">=1 CSV b-tag", 
                ">=2 CSV b-tag"  
        ]

def printCutFlow(hist, analysis):
    used_data = 'ElectronHad'
    if 'Mu' in analysis:
        used_data = 'SingleMu'
    hist_1mBtag =  'TTbarPlusMetAnalysis/' + analysis + '/Ref selection/MET/patMETsPFlow/MET_1orMoreBtag'
    hist_2mBtag = 'TTbarPlusMetAnalysis/' + analysis + '/Ref selection/MET/patMETsPFlow/MET_2orMoreBtags'
    hists = [hist, #due to b-tag scale factors these are not as simple any more
             hist_1mBtag,
             hist_2mBtag
             ]
    hists = FileReader.getHistogramsFromFiles(hists, FILES.files)
    hists['QCD'] = plotting.sumSamples(hists, plotting.qcd_samples)
    hists['SingleTop'] = plotting.sumSamples(hists, plotting.singleTop_samples)
    hists['Di-Boson'] = plotting.sumSamples(hists, plotting.diboson_samples)
    hists['W+Jets'] = plotting.sumSamples(hists, plotting.wplusjets_samples)
    hists['SumMC'] = plotting.sumSamples(hists, plotting.allMC_samples)
    
    header = "| Step | TTJet | W+jets | DY + Jets | single top | QCD | Sum MC | Data |"
    row = " | %s  |  %d |  %d |  %d |  %d |  %d|  %d |  %d | "
    print header
    
    numbers = getEventNumbers(hists, hist, hist_1mBtag, hist_2mBtag)# + '_0orMoreBtag')
    for step in range(len(cuts)):
        nums = numbers[step]
        sumMC = nums['TTJet'] + nums['W+Jets'] + nums['DYJetsToLL'] + nums['SingleTop'] + nums['QCD'] + nums['Di-Boson']
        print row % (cuts[step], nums['TTJet'], nums['W+Jets'], nums['DYJetsToLL'], nums['SingleTop'], nums['QCD'], sumMC, nums[used_data])

def getEventNumbers(hists, histname, hist_1mBtag, hist_2mBtag):        
    eventNumbers = []
    for step in range(len(cuts)):
        events = {}
        for sample in hists.keys():
            events[sample] = hists[sample][histname].GetBinContent(step + 1)
#            if step == len(cuts) - 2:
#                events[sample] = hists[sample][hist_1mBtag].Integral()
#            if step == len(cuts) - 1:
#                events[sample] = hists[sample][hist_2mBtag].Integral()
        eventNumbers.append(events)
    return eventNumbers
    

if __name__ == "__main__":
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
    cuts = cuts_electrons
    print '='*120
    print 'TTbarEplusJetsRefSelection'
    printCutFlow('EventCount/TTbarEplusJetsRefSelection', 'EPlusJets')
    print '='*120
    cuts = cuts_muons
    print '='*120
    print 'TTbarMuPlusJetsRefSelection'
    printCutFlow('EventCount/TTbarMuPlusJetsRefSelection', 'MuPlusJets')
    print '='*120
