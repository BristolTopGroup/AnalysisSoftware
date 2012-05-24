from ROOT import TFile,gROOT
import tools.Log as Log

def getHistogramDictionary(histname, files):
    hists = {}
    TFileOpen = TFile.Open
    gcd = gROOT.cd
    for sample, filename in files.iteritems():
        file = TFileOpen(filename)
        if not file:
            Log.logErrorMessage('Could not find file: ' + filename)
            return
        
        hist = file.Get(histname)
        
        if not hist:
            Log.logErrorMessage('Histogram "%s" could not be found in file:\n%s' % (histname, filename))
            continue
        gcd()
        hists[sample] = hist.Clone()
        file.Close()
    return hists
