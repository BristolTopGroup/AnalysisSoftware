from ROOT import TFile, gROOT
import tools.Log as Log

#Reads a single histogram from a single file
def getHistogramFromFile(histname, filename):
    file = TFile.Open(filename)
    
    if not file:
        Log.logErrorMessage('Could not find file: ' + filename)
        return
    hist = file.Get(histname)
    
    if not hist:
        Log.logErrorMessage('Histogram \n"%s" \ncould not be found in file:\n%s' % (histname, filename))
        return
    
    gROOT.cd()
    histogram = hist.Clone()
    file.Close()
    
    return histogram 
    
#Reads a single histogram from each given file
#and returns a dictionary with the same naming as 'files'
def getHistogramDictionary(histname, files={}):
    hists = {}
    for sample, filename in files.iteritems():
        hists[sample] = getHistogramFromFile(histname, filename)
    return hists

#Reads a list of histograms from each given file
def getHistogramsFromFiles(histnames=[], files={}):
    histograms = {}
    for sample, filename in files.iteritems():
        histograms[sample] = {}
        for histname in histnames:
            histograms[sample][histname] = getHistogramFromFile(histname, filename)
    return histograms


