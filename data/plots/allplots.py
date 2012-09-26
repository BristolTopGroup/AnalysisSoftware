## This file contains all the necessary calls to the rootplot API to produce
## the same set of plots that were created from the command-line.

## You can use this file to intercept the objects and manipulate them before
## the figure is saved, making any custom changes that are not possible from
## the command-line.

## 'objects' is a python dictionary containing all the elements used in the
## plot, including 'hists', 'legend', etc.
##   ex: objects['hists'] returns a list of histograms

try:
  ## the normal way to import rootplot
  from rootplot import plot, plotmpl
except ImportError:
  ## special import for CMSSW installations of rootplot
  from PhysicsTools.PythonAnalysis.rootplot import plot, plotmpl

import os
os.chdir('..')  # return to the directory with the ROOT files

canvas, objects = plot('PileUp_2011_truth_finebin_64600microbarn.root', 'PileUp_2011_truth_finebin_68000microbarn.root', 'PileUp_2011_truth_finebin_71400microbarn.root', 'pileup', ext='root', xlabel='Number of interactions per crossing', title='CMS Preliminary 5.1 fb^{-1} at #sqrt{s} = 7 TeV', legend_entries='inelastic cross-section = 64600 #mub,inelastic cross-section = 68000 #mub,inelastic cross-section = 71400 #mub')
canvas.SaveAs('plots/pileup.root')
