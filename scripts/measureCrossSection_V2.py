#Step 1: get histograms of the distribution you are interested in
#Step 2: Pass the distributions with starting values to TMinuit
#Step 3: ???
#Step 4: Profit

from ROOT import *
import FILES
import tools.ROOTFileReader as FileReader
from array import array
from tools import Styles
import tools.PlottingUtilities as plotting
import QCDRateEstimation

availableSamples = FILES.samplesToLoad
files= FILES.files
lumi= FILES.luminosity

def getTtbarCrossSection(templates = {}, normalisation = {}, theoryXsection = 157.5):
    fit = None
    value, fitError = 0
    
    return fit, value, fitError

def prepareHistograms(histograms):
    return histograms

def getNormalisation(histograms):
    #plug in QCD estimation here
    return {}

if "__main__" in name:
    btagBins = [
                '0btag',
                '1btag',
                '2orMoreBtags']
    mets = ['patMETsPFlow',
            
            ]
    histograms = [
                  'TTbarEplusJetsPlusMetAnalysis/Ref + AsymJets selection/MET/patMETsPFlow/MET',
                  'TTbarEplusJetsPlusMetAnalysis/Ref + AsymJets selection/QCDConversions/MET/patMETsPFlow/MET']
    histograms = [hist + '_' + btagBin for hist in histograms for btagBin in btagBins]
    #initial binning = 1GeV for MET
    hists = plotting.rebin(hists, 5, '*MET_')
    hists = plotting.setYTitle(hists, title="events/5 GeV", histname='*MET_')
    print '='*60
    print 'Starting Top-pair cross section measurement'
    print '='*60
    #rebin histograms
    
    