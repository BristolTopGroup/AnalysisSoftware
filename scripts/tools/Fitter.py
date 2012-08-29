from ROOT import RooRealVar, RooDataHist, RooArgList, RooHistPdf, RooArgSet, RooAddPdf, RooMCStudy, RooFit

#TODO: move bkgN to list
# 1 data, 1 signal and N background histograms
def performFitInLeptonAbsEta(data_histogram, signal_histogram, bkg1_histogram, bkg2_histogram):
    N_Data = data_histogram.Integral()
    N_signal = signal_histogram.Integral()
    N_bkg1 = bkg1_histogram.Integral()
    N_bkg2 = bkg2_histogram.Integral()
    leptonAbsEta = RooRealVar("leptonAbsEta", "leptonAbsEta", 0., 3.)
    variables = RooArgList()
    variables.add(leptonAbsEta)
    variable_set = RooArgSet()
    variable_set.add(leptonAbsEta)
    
    lowerBound = 0
    upperBound = N_Data*2
    
    data_RooDataHist = RooDataHist("data", "dataset with leptonAbsEta", variables, data_histogram)
    signal_RooDataHist =  RooDataHist("rh_signal", "signal", variables, signal_histogram);
    bkg1_RooDataHist =  RooDataHist("rh_bkg1", "bkg1", variables, bkg1_histogram);
    bkg2_RooDataHist =  RooDataHist("rh_bkg2", "bkg2", variables, bkg2_histogram);

    signal_RooHistPdf = RooHistPdf("pdf_signal", "Signal PDF", variable_set, signal_RooDataHist, 0)
    signal_RooHistPdf = RooHistPdf("pdf_signal", "Signal PDF", variable_set, signal_RooDataHist, 0)

def performSimultaniousFitInLeptonAbsEta(data_histograms = [], signal_histograms = [], bkg_histograms = []):
    #create model A and B
    pass