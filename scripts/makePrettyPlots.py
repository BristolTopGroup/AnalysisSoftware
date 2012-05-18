'''
Created on Nov 22, 2011

@author: Lukasz Kreczko

Email: Lukasz.Kreczko@cern.ch

important features:

- read MC and data histograms and combine them
- set styles and colors
- allow switches for log-scale, cumulitative histograms, underflow/overflow bins, error sources

'''

import ROOT

from tools import PlottingUtilities

def plot(histpath, qcdShapeFrom, qcdRateEstimate, rebin = 1,bjetBinned=True, jetBinned=False, bjetBins = PlottingUtilities.allBJetBins, jetBins = []):
    pass

inputFiles = {}



print "hello world"