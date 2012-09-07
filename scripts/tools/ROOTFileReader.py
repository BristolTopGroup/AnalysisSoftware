from ROOT import TFile, gROOT
import tools.Log as Log

openRootFile = TFile.Open
gcd = gROOT.cd
#Reads a single histogram from a single file
def getHistogramFromFile(histname, filename):
    rootFile = TFile.Open(filename)
    
    testIfFalidFile(rootFile, filename)

    hist = rootFile.Get(histname)
    
    if not isValidHist(hist, histname, filename):
        return
    
    gcd()
    histogram = hist.Clone()
    rootFile.Close()
    
    return histogram 
def testIfFalidFile(rootFile, filename):
    if not rootFile:
        Log.logErrorMessage('Could not find rootFile: ' + filename)
        
def isValidHist(rootHist, histname, filename):
    if not rootHist:
        Log.logErrorMessage('Histogram \n"%s" \ncould not be found in rootFile:\n%s' % (histname, filename))
        return False
    return True
        
#Reads a single histogram from each given rootFile
#and returns a dictionary with the same naming as 'files'
def getHistogramDictionary(histname, files={}):
    hists = {}
    for sample, filename in files.iteritems():
        hists[sample] = getHistogramFromFile(histname, filename)
    return hists

#Reads a list of histograms from each given file
def getHistogramsFromFiles(histnames=[], files={}, verbose = False):
    histograms = {}
    nHistograms = 0
    for sample, filename in files.iteritems():
        rootFile = openRootFile(filename)
        getHist = rootFile.Get
        histograms[sample] = {}
        for histname in histnames:
            rootHist = getHist(histname)
            if not isValidHist(rootHist, histname, filename):
                continue
            nHistograms += 1
            gcd()
            histograms[sample][histname] = rootHist.Clone()
            if verbose and nHistograms % 1000 == 0:
                print 'Read', nHistograms, 'histograms'
        rootFile.Close()
    return histograms


