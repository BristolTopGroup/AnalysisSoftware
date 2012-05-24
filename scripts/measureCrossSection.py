#Step 1: get histograms of the distribution you are interested in
#Step 2: Pass the distributions with starting values to RooFit
#Step 3: ???
#Step 4: Profit

import FILES
import tools.ROOTFileReader as FileReader

availableSamples = FILES.samplesToLoad
files= FILES.files

histname ='TTbarEplusJetsPlusMetAnalysis/Ref + AsymJets selection/MET/patMETsPFlow/MET_0orMoreBtag'
hists = FileReader.getHistogramDictionary(histname, files)

for sample, hist in hists.iteritems():
    print sample, ' = ', hist 
    
