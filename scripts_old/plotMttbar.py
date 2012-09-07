from __future__ import division
from numpy import arange
from tdrStyle import *
from ROOT import *
from ROOT import kAzure, kYellow, kViolet, kRed, gROOT, kMagenta, kGreen, TLegend, THStack, TCanvas, gStyle, kGray, TH1F, gPad
#from QCDEstimation import getQCDEstimate#, estimateQCDFor
import QCDEstimation

import HistPlotter
import HistGetter
import inputFiles

from optparse import OptionParser
canvases = []
scanvases = []
setLogY = False
normalise = False
drawQCDError = False

def plotMttbar():
    saveAs = HistPlotter.saveAs
    
    performRescale = False
    HistPlotter.setStyle()
    lumi = 5100#5050.0#1959.75#1611.95
    oldLumi = 5100#5050#/(0.95**2)
    scale = lumi / oldLumi;
    
    outputFormats = [
                     'pdf',
                     'png']
    reverseMCOrder = False
    qcdFromData = 'topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets_0orMoreBtag'
    wjetScale = 1.#0.75#*81268812/81345381
    topScale = 1.#*3701947/59038455
    savePath = "/storage/results/plots/AN-12-241/"
    print 'Top scale', topScale
    print 'W+jets scale', wjetScale
    hists = [];
    

    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/Electron/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCDConversions/Electron/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCD non iso e+jets/Electron/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCD non iso e+jets, non iso trigger/Electron/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCDAntiID/Electron/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCDNoIsoNoID/Electron/electron_AbsEta')

#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCD non iso e+jets/Electron/electron_pfIsolation_03')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_PFMET_bin_0-25/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_PFMET_bin_25-45/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_PFMET_bin_45-70/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_PFMET_bin_70-100/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_PFMET_bin_100-inf/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_patType1CorrectedPFMet_bin_0-25/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_patType1CorrectedPFMet_bin_25-45/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_patType1CorrectedPFMet_bin_45-70/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_patType1CorrectedPFMet_bin_70-100/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_patType1CorrectedPFMet_bin_100-inf/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_patType1p2CorrectedPFMet_bin_0-25/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_patType1p2CorrectedPFMet_bin_25-45/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_patType1p2CorrectedPFMet_bin_45-70/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_patType1p2CorrectedPFMet_bin_70-100/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/Electron_patType1p2CorrectedPFMet_bin_100-inf/electron_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/bjet_invariant_mass')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/MET/patMETsPFlow/MET')
    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/MET/patType1CorrectedPFMet/MET')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/MET/patType1p2CorrectedPFMet/MET')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/MET/patMETsPFlow/MET_phi')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/MET/patType1CorrectedPFMet/MET_phi')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/MET/patType1p2CorrectedPFMet/MET_phi')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCDConversions/MET/patMETsPFlow/MET')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCD non iso e+jets/MET/patMETsPFlow/MET')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCDConversions/MET/patType1CorrectedPFMet/MET')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCD non iso e+jets/MET/patType1CorrectedPFMet/MET')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCDConversions/MET/patType1p2CorrectedPFMet/MET')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCD non iso e+jets/MET/patType1p2CorrectedPFMet/MET')
#    
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/QCD e+jets PFRelIso/Electron/electron_pfIsolation_03')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/MET/patType1CorrectedPFMet/Transverse_Mass')
#    hists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/MET/patType1CorrectedPFMet/Angle_lepton_MET')
    
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/Muon/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/QCD non iso mu+jets/Muon/muon_AbsEta')
#
##    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_PFMET_bin_0-25/muon_AbsEta')
##    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_PFMET_bin_25-45/muon_AbsEta')
##    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_PFMET_bin_45-70/muon_AbsEta')
##    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_PFMET_bin_70-100/muon_AbsEta')
##    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_PFMET_bin_100-inf/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_patType1CorrectedPFMet_bin_0-25/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_patType1CorrectedPFMet_bin_25-45/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_patType1CorrectedPFMet_bin_45-70/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_patType1CorrectedPFMet_bin_70-100/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_patType1CorrectedPFMet_bin_100-inf/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_patType1p2CorrectedPFMet_bin_0-25/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_patType1p2CorrectedPFMet_bin_25-45/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_patType1p2CorrectedPFMet_bin_45-70/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_patType1p2CorrectedPFMet_bin_70-100/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/BinnedMETAnalysis/Muon_patType1p2CorrectedPFMet_bin_100-inf/muon_AbsEta')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/bjet_invariant_mass')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/MET/patMETsPFlow/MET')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/MET/patType1CorrectedPFMet/MET')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/MET/patType1p2CorrectedPFMet/MET')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/MET/patMETsPFlow/MET_phi')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/MET/patType1CorrectedPFMet/MET_phi')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/MET/patType1p2CorrectedPFMet/MET_phi')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/QCD non iso mu+jets/MET/patMETsPFlow/MET')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/QCD non iso mu+jets/MET/patType1CorrectedPFMet/MET')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/QCD non iso mu+jets/MET/patType1p2CorrectedPFMet/MET')
##    
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/QCD non iso mu+jets/Muon/muon_pfIsolation_04')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/QCD mu+jets PFRelIso/Muon/muon_pfIsolation_04')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/MET/patType1CorrectedPFMet/Transverse_Mass')
#    hists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/MET/patType1CorrectedPFMet/Angle_lepton_MET')
    
    
    
    
    files = inputFiles.files
    hists = HistGetter.getHistsFromFiles(hists, files, bJetBins=[
#                                                                 '0orMoreBtag',
#                                                                 '0btag', 
#                                                                 '1btag', 
                                                                 '2orMoreBtags',
#                                                                 '4orMoreBtags'
                                                                 ])
    suffixes = HistPlotter.allBjetBins
#    suffixes = ["0orMoreBtag", '0btag',
#        "1orMoreBtag",
#        "2orMoreBtags",
#        "3orMoreBtags", '4orMoreBtags' ]
#    hists = [hist + '_' + suffix for hist in hists for suffix in suffixes]
#    
    otherHists = []
#    otherHists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/Vertices/nVertex')
#    otherHists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/Vertices/nVertex_reweighted')
#    
#    otherHists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/N_BJets')
#    otherHists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/N_BJets_reweighted')
    
##    otherHists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/Vertices/nVertex')
#    otherHists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/Vertices/nVertex_reweighted')
    
#    otherHists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/N_BJets')
#    otherHists.append('TTbarPlusMetAnalysis/MuPlusJets/Ref selection/N_BJets_reweighted')

#    otherHists.append('TTbarPlusMetAnalysis/EPlusJets/Ref selection/Electron/electron_pT')
#    otherHists.append('ElectronAnalysis/All_Electron_mvaTrigV0')
#    otherHists.append('VertexAnalyser/nVertex')
#    otherHists.append('VertexAnalyser/nVertex_reweighted')
#    otherHists.append('VertexAnalyser/nVertex_reweighted_PUWeightInTimeOnly')
#    otherHists.append('VertexAnalyser/nVertex_reweighted_PUWeight3BX')
#    otherHists.append('VertexAnalyser/nVertex_reweighted_PUWeight3D')
    
#    otherHists.append('QCDStudy/QCDest_PFIsolation_1btag_WithMETCutAndAsymJetCuts_3jets')
    if len(otherHists) > 0:
        otherHists = HistGetter.getHistsFromFiles(otherHists, files)
        hists = HistGetter.joinHistogramDictionaries([hists,
                                                  otherHists])
    gcd = gROOT.cd

    reverseMCOrdertmp = reverseMCOrder
    for histname in hists[hists.keys()[0]]:
        reverseMCOrder = reverseMCOrdertmp
        print '=' * 70
        print 'Plotting:', histname
        gcd()
        
        hist_data = hists['data'][histname]

        hist_ttbar = hists['ttbar'][histname]
#        totalWPlusJets = 47896878 + 71828418 + 25400440 + 7685939 + 10814233 # = 163625908
#        hists['wjets'][histname].Scale(0)
##        hists['wjets'][histname].Scale(totalWPlusJets/47896878.)
#        hists['W1Jet'][histname].Scale(totalWPlusJets/76048786.*4480.0/31314.)
#        hists['W2Jets'][histname].Scale(totalWPlusJets/25400440.*1674./31314.)
#        hists['W3Jets'][histname].Scale(totalWPlusJets/7685939.*484.7/31314.)
#        hists['W4Jets'][histname].Scale(totalWPlusJets/12998049.*211.7/31314.)
        hist_wjets = hists['W1Jet'][histname]
        hist_wjets += hists['W2Jets'][histname] 
        hist_wjets += hists['W3Jets'][histname] 
        hist_wjets += hists['W4Jets'][histname]
        hist_zjets = hists['zjets'][histname]

        hist_bce1 = hists['bce1'][histname]
        hist_bce2 = hists['bce2'][histname]
        hist_bce3 = hists['bce3'][histname]
        hist_muQCD = hists['muQCD'][histname]
        hist_enri1 = hists['enri1'][histname]
        hist_enri2 = hists['enri2'][histname]
        hist_enri3 = hists['enri3'][histname]
        hist_pj1 = hists['pj1'][histname]
        hist_pj2 = hists['pj2'][histname]
        hist_pj3 = hists['pj3'][histname]
        hist_singleTop = hists['T_TuneZ2_tW-channel'][histname] + hists['T_TuneZ2_t-channel'][histname] + hists['T_TuneZ2_s-channel'][histname]
        hist_singleTop += hists['Tbar_TuneZ2_tW-channel'][histname] + hists['Tbar_TuneZ2_t-channel'][histname] + hists['Tbar_TuneZ2_s-channel'][histname]
        
        hist_diboson = hists['ww'][histname] + hists['wz'][histname] + hists['zz'][histname]
        
#        hist_ttbarZ = hists['TTbarZIncl'][histname]
#        hist_ttbarW = hists['TTbarInclWIncl'][histname]

        hist_ttbar.Scale(scale);
        hist_ttbar.Scale(topScale);
        hist_wjets.Scale(scale);
        hist_wjets.Scale(wjetScale);
        hist_zjets.Scale(scale);
        hist_singleTop.Scale(scale)
        hist_diboson.Scale(scale)
        hist_bce1.Scale(scale);
        hist_bce2.Scale(scale);
        hist_bce3.Scale(scale);
#        hist_muQCD.Scale(scale);
#        hist_ttbarZ.Scale(scale * 100)
#        hist_ttbarW.Scale(scale * 100)
        hist_enri1.Scale(scale);
        hist_enri2.Scale(scale);
        hist_enri3.Scale(scale);

        hist_pj1.Scale(scale);
        hist_pj2.Scale(scale);
        hist_pj3.Scale(scale);


        hist_qcd = hist_bce1 + hist_bce2 + hist_bce3
        hist_qcd += hist_enri1 + hist_enri2 + hist_enri3
        hist_qcd += hist_pj1 + hist_pj2 + hist_pj3
        
        nqcd = hist_qcd.Integral();
        mttbars = ['topReconstruction/mttbar_' + suffix for suffix in suffixes]
        mttbars2 = ['topReconstruction/mttbar_withMETAndAsymJets_' + suffix for suffix in suffixes]
        mttbars3 = ['topReconstruction/mttbar_3jets_withMETAndAsymJets_' + suffix for suffix in suffixes]
        mttbars3_new = ['MttbarAnalysis/ElectronPlusJets/ThreeJetChi2/mttbar_withMETAndAsymJets_' + suffix for suffix in suffixes]
        shapeErrorHist = None 
        errorHist = None
        relativeQCDEstimationError = 0
        #TODO: fix this for muon+jets
        if 'EPlusJets' in histname or 'Electron' in histname:
            if histname in mttbars or histname in mttbars2:
    ##            print "Taking QCD shape from DATA (%s)" % qcdFromData
                qcdHists = HistGetter.getHistsFromFiles([qcdFromData], {'data':files['data']})
                hist_qcd = qcdHists['data'][qcdFromData]
                shapeErrorHist = QCDEstimation.getShapeErrorHistogram(qcdFromData, files)
                
            if histname in mttbars3:
                qcd3JetsFromData = 'topReconstruction/backgroundShape/mttbar_3jets_conversions_withMETAndAsymJets_0orMoreBtag'
                qcdHists = HistGetter.getHistsFromFiles([qcd3JetsFromData], {'data':files['data']})
                hist_qcd = qcdHists['data'][qcd3JetsFromData]
            
            
            qcdFromMCPlots = 'QCD' in histname or 'Vertices' in histname or 'jets' in histname.split('/')[-1].lower()         
            if 'TTbarPlusMetAnalysis' in histname and not qcdFromMCPlots and not 'bjet_invariant_mass' in histname:
                qcdRateEstimate = 'TTbarPlusMetAnalysis/EPlusJets/QCD e+jets PFRelIso/Electron/electron_pfIsolation_03'
    #            qcdRateEstimate = 'TTbarPlusMetAnalysis/EPlusJets/Ref + AsymJets selection/QCD e+jets PFRelIso, non iso trigger/Electron/electron_pfIsolation_03'
                currentBin = HistPlotter.getBjetBin(histname)
                estimate, err = QCDEstimation.getQCDEstimate(files['data'],
                                                             bjetBin=currentBin,
                                                             histogramForEstimation=qcdRateEstimate,
                                                             function='expo')
                if not estimate == 0:
                    relativeQCDEstimationError = err / estimate
                print 'Estimated QCD background: %.1f +- %.1f' % (estimate, err)
                if(hist_qcd.Integral() > 0 and estimate >= 0):
                    hist_qcd.Scale(estimate / hist_qcd.Integral())
                    
                variable = histname.split('/')[-1]
                analyser = histname.split('/')[-2]
                if 'MET' in variable or 'Transverse_Mass' in variable or 'Angle_lepton_MET' in variable:
                    variable = histname.split('/')[-2] + '/' + histname.split('/')[-1]
                    analyser = histname.split('/')[-3]
                
                print 'Using QCD shape from data for %s/%s' % (analyser, variable)
                if 'btag' in histname.lower():#always use 0 b-tag bin if available
                    currentBin = HistPlotter.getBjetBin(histname)
                    variable = variable.replace(currentBin, '0btag')
                    
                qcdShapeFromData = 'TTbarPlusMetAnalysis/EPlusJets/QCDConversions/' + analyser + '/' + variable
                qcdShapeComparison = 'TTbarPlusMetAnalysis/EPlusJets/QCD non iso e+jets/' + analyser + '/' + variable
                
                if 'BinnedMETAnalysis' in histname:
                    qcdShapeFromData = 'TTbarPlusMetAnalysis/EPlusJets/QCDConversions/BinnedMETAnalysis/' + analyser + '/' + variable
                    qcdShapeComparison = 'TTbarPlusMetAnalysis/EPlusJets/QCD non iso e+jets/BinnedMETAnalysis/' + analyser + '/' + variable
                
    #            qcdShapeFromData = 'TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/QCD non iso e+jets/' + analyser + '/' + variable
    #            qcdShapeComparison = 'TTbarPlusMetAnalysis/EPlusJets/Ref selection/BinnedMETAnalysis/QCDConversions/' + analyser + '/' + variable
                print 'QCD shape from Data:', qcdShapeFromData
                qcdHists = HistGetter.getHistsFromFiles([qcdShapeFromData], {'data':files['data']})
                
                nQCD = hist_qcd.Integral()
                hist_qcd = qcdHists['data'][qcdShapeFromData]
                nShape = hist_qcd.Integral()
                if nShape > 0:
                    hist_qcd.Scale(nQCD / nShape)
                shapeErrorHist = QCDEstimation.getShapeErrorHistogram(files,
                                                                      histogramForShape=qcdShapeFromData,
                                                                      histogramForComparison=qcdShapeComparison)
    #            errorHist = QCDEstimation.createErrorHistogram([hist_ttbar, hist_wjets, hist_zjets, hist_singleTop, hist_diboson, hist_qcd],
    #                                                       hist_qcd, relativeQCDEstimationError, shapeErrorHist)
                
        if 'MuPlusJets' in histname or 'Muon' in histname:
            nMuQCD = hist_muQCD.Integral()
            #scale by factor
            nMuQCD = nMuQCD*1.21
            #get template from anti-isolated region
            qcdFromMCPlots = 'QCD' in histname or 'Vertices' in histname or 'jets' in histname.split('/')[-1].lower()   
            if 'TTbarPlusMetAnalysis' in histname and not qcdFromMCPlots and not 'bjet_invariant_mass' in histname:
                variable = histname.split('/')[-1]
                analyser = histname.split('/')[-2]
                if 'MET' in variable or 'Transverse_Mass' in variable or 'Angle_lepton_MET' in variable:
                    variable = histname.split('/')[-2] + '/' + histname.split('/')[-1]
                    analyser = histname.split('/')[-3]
                
                print 'Using QCD shape from data for %s/%s' % (analyser, variable)
                if 'btag' in histname.lower():#always use 0 b-tag bin if available
                    currentBin = HistPlotter.getBjetBin(histname)
                    variable = variable.replace(currentBin, '0btag')
                    
                qcdShapeFromData = 'TTbarPlusMetAnalysis/MuPlusJets/QCD non iso mu+jets/' + analyser + '/' + variable
                
                if 'BinnedMETAnalysis' in histname:
                    qcdShapeFromData = 'TTbarPlusMetAnalysis/MuPlusJets/QCD non iso mu+jets/BinnedMETAnalysis/' + analyser + '/' + variable
                
                print 'QCD shape from Data:', qcdShapeFromData
                qcdHists = HistGetter.getHistsFromFiles([qcdShapeFromData], {'data':files['data']})
                hist_muQCD = qcdHists['data'][qcdShapeFromData]
                nShape = hist_muQCD.Integral()
                if nShape > 0:
                    hist_muQCD.Scale(nMuQCD / nShape)
            
            
                
        if performRescale:        
            print '=' * 100
            print '"best" rescaling for', histname
            rescales = rescaleMC(hist_data, hist_ttbar, hist_wjets, hist_qcd)
            print rescales
            print '=' * 100
            topScale = rescales[1]['ttbar']
            wjetScale = rescales[1]['wjets']
            qcdScale2 = rescales[1]['qcd']

            hist_ttbar.Scale(topScale)
            hist_wjets.Scale(wjetScale)
            hist_qcd.Scale(qcdScale2) 
        
        

        rebin = 1;
        Urange = (0, 5000)
        if ("mttbar" in histname):
            hist_data.SetXTitle("m(t#bar{t}) [GeV]");
            rebin = 50;
            hist_data.SetYTitle("Events/(%f GeV)" % hist_data.GetBinWidth(1) * rebin);
            if setLogY:
                Urange = (300, 3000)
            else:
                Urange = (300, 1500)
        if "All_Electron_mvaTrigV0" in histname:
            hist_data.SetXTitle("mva disc");
            hist_data.SetYTitle("Events/(0.01)");
            rebin = 1;
            Urange = (-1.1, 1.1)
        elif ("m3" in histname):
            hist_data.SetXTitle("M3 [GeV]");
            hist_data.SetYTitle("Events/(50 GeV)");
            rebin = 50;
            Urange = (0, 2000)
        elif (histname == "electron_et"):
            hist_data.SetXTitle("electron p_{T} [GeV]");
            hist_data.SetYTitle("Events/(5 GeV)");
            rebin = 5;
        elif ("ttbar_pt" in histname):
            hist_data.SetXTitle("p_{T} of t#bar{t} system [GeV]");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10;
            if setLogY:
                Urange = (0, 700)
            else:
                Urange = (0, 300)
        elif ("ttbar_px" in histname):
            hist_data.SetXTitle("p_{x} of t#bar{t} system [GeV]");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10;
            Urange = (0, 1000)
        elif ("ttbar_py" in histname):
            hist_data.SetXTitle("p_{y} of t#bar{t} system [GeV]");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10;
            Urange = (0, 1000)
        elif ("ttbar_pz" in histname):
            hist_data.SetXTitle("p_{z} of t#bar{t} system [GeV]");
            hist_data.SetYTitle("Events/(50 GeV)");
            rebin = 50;
            Urange = (0, 2500)
        elif ("HT" in histname):
            hist_data.SetXTitle("#Sigma p_{T} [GeV]");
            hist_data.SetYTitle("Events/(50 GeV)");
            rebin = 50;
            Urange = (0, 2500)
        elif (histname == "numberOfJets"):
            hist_data.SetXTitle("number of jets");
            hist_data.SetYTitle("Events");
        elif (histname == "numberOfBJets"):
            hist_data.SetXTitle("number of b-tagged jets (SSVHE medium)");
            hist_data.SetYTitle("Events");
        elif ('MET_' in histname and not 'phi' in histname and not 'Angle_lepton_MET' in histname):
            hist_data.SetXTitle("E_{T}^{miss} [GeV]");
            hist_data.SetYTitle("Events/(5 GeV)");
            rebin = 5
            if setLogY:
                Urange = (200, 590)
                rebin = 10
                hist_data.SetYTitle("Events/(10 GeV)")
            else:
                Urange = (0, 195)
        elif ('MET_phi' in histname):
            hist_data.SetXTitle("#phi(E_{T}^{miss})");
            hist_data.SetYTitle("Events/(0.1)");
            rebin = 1;
            Urange = (-4, 4)
        elif 'Angle_lepton_MET' in histname:
            hist_data.SetXTitle("angle(l,E_{T}^{miss})");
            hist_data.SetYTitle("Events/0.05");
            rebin = 5;
            Urange = (0, 3.2)
        elif 'METsignificance' in histname:
            hist_data.SetXTitle("METsignificance [GeV]");
            hist_data.SetYTitle("Events/(5 GeV)");
            rebin = 5;
            Urange = (0, 200)
        elif ("mtW" in histname or 'Transverse_Mass' in histname):
            hist_data.SetXTitle("transverse W-boson mass [GeV]");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10;
            Urange = (0, 500)
            if setLogY:
                Urange = (0, 500)
            else:
                Urange = (0, 200)
        elif ("electronD0" in histname):
            hist_data.SetXTitle("electron d_{0} / cm");
            hist_data.SetYTitle("Events/(0.001 cm)");
            rebin = 10;
        elif ("angleTops" in histname):
            hist_data.SetXTitle("angle between top quarks");
            hist_data.SetYTitle("Events/(0.1 rad)");
            rebin = 20;
        elif ("neutrino_pz" in histname):
            hist_data.SetXTitle("neutrino p_{Z} [GeV]");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10;
            Urange = (-500, 500)
        elif ('mHadronicTop' in histname or 'mLeptonicTop' in histname or 'mAllTop' in histname):
            hist_data.SetXTitle("top mass [GeV]");
            hist_data.SetYTitle("Events/(20 GeV)");
            rebin = 20;
            Urange = (0, 1500)
        elif ('pt_leadingTop' in histname or 'pt_NextToLeadingTop' in histname):
            hist_data.SetXTitle("top p_{T} [GeV]");
            hist_data.SetYTitle("Events/(20 GeV)");
            rebin = 20;
        elif('QCDest_CombRelIso' in histname):
            hist_data.SetXTitle("relative isolation");
            hist_data.SetYTitle("Events/(0.1)");
            rebin = 10;
            Urange = (0, 2)
        elif('pfisolation' in histname.lower()):
            hist_data.SetXTitle("Relative isolation");
            hist_data.SetYTitle("Events/(0.1)");
            rebin = 10
            Urange = (0, 2)
            reverseMCOrder = False
        elif 'DirectionalIsolation' in histname:
            hist_data.SetXTitle("directional isolation");
            hist_data.SetYTitle("Events/(0.01)");
            rebin = 10
            Urange = (0, 2)
        elif 'DirectionalIsolationWithGaussianFallOff' in histname:
            hist_data.SetXTitle("directional isolation");
            hist_data.SetYTitle("Events/(0.01)");
            rebin = 10
            Urange = (0, 2)
        elif('diElectron' in histname):
            hist_data.SetXTitle("m(ee)");
            hist_data.SetYTitle("Events/(10 GeV)");
            rebin = 10
            Urange = (50, 2000)
        elif 'JetMass' in histname:
            hist_data.SetXTitle("m(jet)");
            hist_data.SetYTitle("Events/(5 GeV)");
            rebin = 5
            Urange = (0, 200)
        elif 'nVertex' in histname:
            Urange = (0, 21)
        elif 'electron_pT' in histname:
            hist_data.SetXTitle("p_{T}(e)");
            hist_data.SetYTitle("Events/(5 GeV)");
            rebin = 5
            Urange = (20, 1000)
        elif 'electron_eta' in histname:
            hist_data.SetYTitle("Events/(0.2)");
            rebin = 10
            Urange = (-3, 3)
        elif 'N_BJets' in histname:
            Urange = (0, 5)
        elif 'bjet_invariant_mass' in histname:
            Urange = (0, 600)
            rebin = 20
            hist_data.SetYTitle("Events/(20 GeV)");
            hist_data.SetXTitle("m(b,b)");
        if 'electron_AbsEta' in histname:
            hist_data.SetYTitle("Events/(0.1)");
            hist_data.SetXTitle("|#eta(e)|");
            rebin = 10
            Urange = (0, 2.5)
        if 'muon_AbsEta' in histname:
            hist_data.SetYTitle("Events/(0.1)");
            hist_data.SetXTitle("|#eta(#mu)|");
            rebin = 10
            Urange = (0, 2.5)
        if normalise:
            title = hist_data.GetYaxis().GetTitle()
            title = title.replace('Events', 'normalised to unit area')
            title = title.replace('events', 'normalised to unit area')
            hist_data.SetYTitle(title);
        hist_data.Rebin(rebin);
        hist_ttbar.Rebin(rebin);
        hist_wjets.Rebin(rebin);
        hist_zjets.Rebin(rebin);
        hist_qcd.Rebin(rebin);
#        hist_ttbarW.Rebin(rebin)
#        hist_ttbarZ.Rebin(rebin)
        hist_muQCD.Rebin(rebin);
        hist_singleTop.Rebin(rebin)
        hist_diboson.Rebin(rebin)
        hist_data.SetAxisRange(Urange[0], Urange[1]);
        hist_ttbar.SetAxisRange(Urange[0], Urange[1]);
        hist_wjets.SetAxisRange(Urange[0], Urange[1]);
        hist_zjets.SetAxisRange(Urange[0], Urange[1]);
        hist_qcd.SetAxisRange(Urange[0], Urange[1]);
        hist_muQCD.SetAxisRange(Urange[0], Urange[1]);
        hist_singleTop.SetAxisRange(Urange[0], Urange[1]);
        hist_diboson.SetAxisRange(Urange[0], Urange[1]);

        if shapeErrorHist:
            shapeErrorHist.Rebin(rebin)
            shapeErrorHist.SetAxisRange(Urange[0], Urange[1])
        hist_data.SetMarkerStyle(8);
        
        hist_ttbar.SetFillStyle(1001);
        hist_ttbar.SetFillColor(kRed + 1);
#        hist_ttbar.SetLineColor(kRed + 1);
        hist_wjets.SetFillStyle(1001);
        hist_wjets.SetFillColor(kGreen - 3);
#        hist_wjets.SetLineColor(kGreen - 3);
        hist_zjets.SetFillStyle(1001);
        hist_zjets.SetFillColor(kAzure - 2);
#        hist_zjets.SetLineColor(kAzure - 2);
        hist_qcd.SetFillStyle(1001);
        hist_qcd.SetFillColor(kYellow);
#        hist_qcd.SetLineColor(kYellow);
        hist_muQCD.SetFillStyle(1001);
        hist_muQCD.SetFillColor(kYellow);
        hist_singleTop.SetFillStyle(1001);
        hist_singleTop.SetFillColor(kMagenta)
#        hist_singleTop.SetLineColor(kMagenta)
        
        hist_diboson.SetFillStyle(1001);
        hist_diboson.SetFillColor(kWhite)
#        hist_diboson.SetLineColor(kWhite)
        
#        hist_ttbarZ.SetLineColor(kCyan-4)
#        hist_ttbarZ.SetFillStyle(4000)
#        hist_ttbarZ.SetLineWidth(3)
#        hist_ttbarZ.SetLineStyle(4);
#        hist_ttbarW.SetLineColor(kBlue +2)
#        hist_ttbarW.SetFillStyle(4000)
#        hist_ttbarW.SetLineWidth(3)
#        hist_ttbarW.SetLineStyle(4);

        leg = TLegend(0.696, 0.95, 0.94, 0.55);
        leg.SetBorderSize(0);
        leg.SetLineStyle(0);
        leg.SetTextFont(42);
        leg.SetFillStyle(0);

        leg.AddEntry(hist_data, "data", "P");
        #        leg.AddEntry(hist_data2, "data(no HLT)", "P");
        leg.AddEntry(hist_ttbar, "t#bar{t}", "f")
        leg.AddEntry(hist_singleTop, "Single-Top", "f")
        leg.AddEntry(hist_wjets, "W#rightarrowl#nu", "f");
        leg.AddEntry(hist_zjets, "Z/#gamma*#rightarrowl^{+}l^{-}", "f");
        if 'MuPlusJets' in histname or 'Muon' in histname:
            leg.AddEntry(hist_muQCD, "QCD #mu Enriched", "f");
        else:
            leg.AddEntry(hist_qcd, "QCD/#gamma + jets", "f");
        leg.AddEntry(hist_diboson, "VV + X", "f")
#        leg.AddEntry(hist_ttbarW, "t#bar{t} + W x 100", "f")
#        leg.AddEntry(hist_ttbarZ, "t#bar{t} + Z x 100", "f")
        
#        leg.AddEntry(hist_Zprime500, "Z' 0.5TeV (50pb)");
#        leg.AddEntry(hist_Zprime750, "Z' 0.75TeV (50pb)");
#        leg.AddEntry(hist_Zprime1000, "Z' 1TeV (50pb)");
#        leg.AddEntry(hist_Zprime1250, "Z' 1.25TeV (50pb)");
#        leg.AddEntry(hist_Zprime4000, "Z' 4TeV (50pb)");
        if normalise:
            if 'EPlusJets' in histname:
                normalisePlotsToUnitArea(hist_data, hist_ttbar, hist_wjets, hist_zjets, hist_singleTop, hist_qcd, hist_diboson)
            else:
                normalisePlotsToUnitArea(hist_data, hist_ttbar, hist_wjets, hist_zjets, hist_singleTop, hist_muQCD, hist_diboson)
#            nTTW = hist_ttbarW.Integral()
#            nTTZ = hist_ttbarZ.Integral()
#            if nTTW > 0:
#                hist_ttbarW.Scale(1 / nTTW)
#            if nTTZ > 0:
#                hist_ttbarZ.Scale(1 / nTTZ)
            

        
        canvases.append(TCanvas("cname" + histname, histname, 1600, 1200))
        canvases[-1].cd().SetRightMargin(0.04);
        if 'TTbarPlusMetAnalysis' in histname and not 'QCD' in histname and 'EPlusJets' in histname:
#            relativeQCDEstimationError = 0
            errorHist = QCDEstimation.createErrorHistogram([hist_ttbar, hist_wjets, hist_zjets, hist_singleTop, hist_diboson, hist_qcd],
                                                       hist_qcd, relativeQCDEstimationError, shapeErrorHist)
        
        hs = THStack("MC", "MC");
        if reverseMCOrder:
            hs.Add(hist_ttbar);
            hs.Add(hist_wjets);
            hs.Add(hist_zjets);
            hs.Add(hist_singleTop);
            if 'MuPlusJets' in histname:
                hs.Add(hist_muQCD);
            else:
                hs.Add(hist_qcd);
            hs.Add(hist_diboson);
        
        else:
            hs.Add(hist_diboson);
            if 'MuPlusJets' in histname:
                hs.Add(hist_muQCD);
            else:
                hs.Add(hist_qcd);
            hs.Add(hist_zjets);
            hs.Add(hist_wjets);
            hs.Add(hist_singleTop);
            hs.Add(hist_ttbar);
            
            
            
            
        
        
        
        #hs.SetMinimum(1)
        #hist_data.SetMinimum(1)
        max = 0
        if hs.GetMaximum() > hist_data.GetMaximum():
            max = hs.GetMaximum() * 1.4
        else:
            max = hist_data.GetMaximum() * 1.4
        hist_data.GetYaxis().SetRangeUser(0, max);
        if setLogY:
            hist_data.GetYaxis().SetRangeUser(0.1, max);
        hist_data.Draw('error');
        hs.Draw("hist same");
#        hist_ttbarW.Draw("same")
#        hist_ttbarZ.Draw("same")
#        hist_Zprime500.Draw("same");
#        hist_Zprime750.Draw("same");
#        hist_Zprime1000.Draw("same");
#        hist_Zprime1250.Draw("same");
#        hist_Zprime4000.Draw("same");
        if errorHist and drawQCDError:
            gStyle.SetErrorX(0.5);
            errorHist.SetFillColor(kGray + 3)
            errorHist.SetMarkerStyle(0)
            errorHist.SetFillStyle(3001);
            leg.AddEntry(errorHist, "QCD uncertainty")
            errorHist.Draw('E2 same')
        else:
            gStyle.SetErrorX(0);
#        if not 'background' in histname and not 'QCDStudy' in histname and 'withMETAndAsymJets' in histname and errorHist:
#            gStyle.SetErrorX(0.5);
#            errorHist.SetFillColor(kGray + 3)
#            errorHist.SetMarkerStyle(0)
#            errorHist.SetFillStyle(3001);
#            leg.AddEntry(errorHist, "QCD uncertainty")
#            errorHist.Draw('E2 same')
            
        hist_data.Draw("error same");
        leg.Draw();
        text1, text2 = HistPlotter.get_cms_label(lumiInInvPb=lumi, njet=HistPlotter.getJetBin(histname),
                                          nbjet=HistPlotter.getBjetBin(histname), channel=HistPlotter.getChannel(histname))
        text1.Draw();
        text2.Draw();
#
        postfix = ''
        if setLogY:
            canvases[-1].SetLogy(1)
            postfix = '_log'
        if normalise:
            postfix = postfix + '_norm'
            
        prefix = 'EPlusJets_'
        if 'MuPlusJets' in histname or 'Muon' in histname:
            prefix = 'MuPlusJets_'
        selectionLabel = ''
        if 'Ref' in histname:
            selectionLabel = 'Ref_'
        if 'QCDConversions' in histname:
            selectionLabel = 'QCDConversions_'
        if 'QCD non iso e+jets' in histname:
            selectionLabel = 'AntiIsolated_'
            
        name = ''.join(histname[:histname.rfind('/') + 1]) + prefix + selectionLabel + ''.join(histname[histname.rfind('/') + 1:]) + postfix
        #specific selections
        
        
#        fullName = name.replace('EPlusJets', 'EPlusJets' + selectionLabel) + postfix
        saveAs(canvas=canvases[-1], name=name, outputFormats=outputFormats, outputFolder=savePath)
        del canvases[-1]

        cu_hist_data = getCumulativePlot(hist_data, "data_" + histname);
        cu_hist_ttbar = getCumulativePlot(hist_ttbar, "ttbar_" + histname);
        cu_hist_wjets = getCumulativePlot(hist_wjets, "wjets_" + histname);
        cu_hist_zjets = getCumulativePlot(hist_zjets, "zjets_" + histname);
        cu_hist_qcd = getCumulativePlot(hist_qcd, "qcd_" + histname);
        cu_hist_muQCD = getCumulativePlot(hist_qcd, "MUqcd_" + histname);
        cu_hist_singleTop = getCumulativePlot(hist_singleTop, "singleTop_" + histname);
        cu_hist_diboson = getCumulativePlot(hist_diboson, "di-boson_" + histname);
#        cu_hist_ttbarW = getCumulativePlot(hist_ttbarW, "ttbarW_" + histname);
#        cu_hist_ttbarZ = getCumulativePlot(hist_ttbarZ, "ttbarZ_" + histname);
##        cu_hist_Zprime500 = getCumulativePlot(hist_Zprime500, "Zprime500");
##        cu_hist_Zprime750 = getCumulativePlot(hist_Zprime750, "Zprime750");
##        cu_hist_Zprime1000 = getCumulativePlot(hist_Zprime1000, "Zprime1000");
##        cu_hist_Zprime1250 = getCumulativePlot(hist_Zprime1250, "Zprime1250");
#        cu_hist_Zprime4000 = getCumulativePlot(hist_Zprime4000, "Zprime4000");
        cu_hist_data.SetYTitle("Integrated" + cu_hist_data.GetYaxis().GetTitle())
##        
#
        cu_hist_data.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_ttbar.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_wjets.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_zjets.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_qcd.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_muQCD.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_singleTop.SetAxisRange(Urange[0], Urange[1]);
        cu_hist_diboson.SetAxisRange(Urange[0], Urange[1]);
#        cu_hist_ttbarW.SetAxisRange(Urange[0], Urange[1]);
#        cu_hist_ttbarZ.SetAxisRange(Urange[0], Urange[1]);
        
        cu_hs = THStack("cu_MC", "cu_MC");
        cu_hs.Add(cu_hist_diboson);
        cu_hs.Add(cu_hist_ttbar);
        cu_hs.Add(cu_hist_wjets);
        cu_hs.Add(cu_hist_zjets);
        cu_hs.Add(cu_hist_singleTop);
        if 'Muon' in histname:
                hs.Add(cu_hist_muQCD);
        else:
                cu_hs.Add(cu_hist_qcd);
        
        scanvases.append(TCanvas("cu_cname" + histname, histname + "(cu)", 1600, 1200))
        scanvases[-1].cd().SetRightMargin(0.04);
        
        cu_hist_data.Draw("error");
        cu_hs.Draw("hist same");
#        cu_hist_ttbarW.Draw("same");
#        cu_hist_ttbarZ.Draw("same");
###        cu_hist_Zprime500.Draw("same");
###        cu_hist_Zprime750.Draw("same");
###        cu_hist_Zprime1000.Draw("same");
###        cu_hist_Zprime1250.Draw("same");
#        cu_hist_Zprime4000.Draw("same");
##        #        cu_hist_data2.Draw("error same");
        cu_hist_data.Draw("error same");
        leg.Draw();
##
        text1.Draw()
        postfix = ''
        if setLogY:
            scanvases[-1].SetLogy(1)
            postfix = '_log'
#        scanvases[-1].SetGridy(1)       
        name = ''.join(histname[:histname.rfind('/') + 1]) + prefix + ''.join(histname[histname.rfind('/') + 1:])
        saveAs(canvas=scanvases[-1], name=name + '_integrated' + postfix, outputFormats=outputFormats, outputFolder=savePath)
        del scanvases[-1]
        del cu_hs
        del cu_hist_data
        del cu_hist_ttbar
        del cu_hist_wjets
        del cu_hist_zjets
        del cu_hist_qcd
        del cu_hist_singleTop
        del cu_hist_diboson


# fixOverlay: Redraws the axis

def fixOverlay():
    gPad.RedrawAxis();


def getCumulativePlot(initial, type):
    name = initial.GetName()
    name = "cu_" + name + "_" + type;
    title = initial.GetTitle()
    title = "cu_" + title + "_" + type;
    xaxis = initial.GetXaxis().GetTitle();
    yaxis = initial.GetYaxis().GetTitle();
    nBins = initial.GetNbinsX();
    cu = TH1F(name, title, nBins, initial.GetXaxis().GetXmin(), initial.GetXaxis().GetXmax());
#    cu.Sumw2()
    for bin in range(1, nBins + 1):
        cu.SetBinContent(bin, initial.Integral(bin, nBins));
    
    cu.SetFillStyle(initial.GetFillStyle());
    cu.SetFillColor(initial.GetFillColor());
    cu.SetLineColor(initial.GetLineColor());
    cu.SetMarkerSize(initial.GetMarkerSize());
    cu.SetMarkerStyle(initial.GetMarkerStyle());
    cu.SetMarkerColor(initial.GetMarkerColor());
    cu.SetLineWidth(initial.GetLineWidth());
    cu.GetXaxis().SetTitle(xaxis);
    cu.GetYaxis().SetTitle(yaxis);
    return cu;


def normalisePlotsToUnitArea(data, hist_ttbar, hist_wjets, hist_zjets, hist_singleTop, hist_qcd, hist_diboson):
    nData = data.Integral()
    nTop = hist_ttbar.Integral()
    nWJ = hist_wjets.Integral()
    nZJ = hist_zjets.Integral()
    nST = hist_singleTop.Integral()
    nQCD = hist_qcd.Integral()
    nDB = hist_diboson.Integral()
    data.Sumw2()   
    nMC = sum([nTop, nWJ, nZJ, nST, nQCD, nDB])
    print 'nMC', nMC
    if not nData == 0:
        data.Scale(1 / nData)
    
    
    if not nMC == 0:
        hist_ttbar.Scale(1 / nMC)
        hist_wjets.Scale(1 / nMC)
        hist_zjets.Scale(1 / nMC)
        hist_singleTop.Scale(1 / nMC)
        hist_qcd.Scale(1 / nMC)
        hist_diboson.Scale(1 / nMC)
    
    chi2 = 0
    for bin in range(1, data.GetNbinsX() + 1):
        nData = data.GetBinContent(bin)
        nTop = hist_ttbar.GetBinContent(bin)
        nWJ = hist_wjets.GetBinContent(bin)
        nZJ = hist_zjets.GetBinContent(bin)
        nST = hist_singleTop.GetBinContent(bin)
        nQCD = hist_qcd.GetBinContent(bin)
        nDB = hist_diboson.GetBinContent(bin)
        nMC = sum([nTop, nWJ, nZJ, nST, nQCD, nDB])
        chi2 += (nData - nMC) ** 2
    print 'Chi2:', chi2
        
    
def rescaleMC(data, ttbar, wjets, qcd):
    #rescaling
    
    
    chi2 = {}
    
    eventsData = []
    appendData = eventsData.append
    eventsTTbar = []
    appendTTbar = eventsTTbar.append
    eventsWJets = []
    appendWJets = eventsWJets.append
    eventsQCD = []
    appendQCD = eventsQCD.append
    nBins = data.GetNbinsX()
    
    getData = data.GetBinContent
    getttbar = ttbar.GetBinContent
    getwjets = wjets.GetBinContent
    getqcd = qcd.GetBinContent
    for bin in range(nBins):        
        appendData(getData(bin))
        appendTTbar(getttbar(bin))
        appendWJets(getwjets(bin))
        appendQCD(getqcd(bin))
    
    events = {}
    events['data'] = eventsData
    events['ttbar'] = eventsTTbar
    events['wjets'] = eventsWJets
    events['qcd'] = eventsQCD
    
    for tscale in arange(1, 2, 0.2):
        for wscale in arange(1, 4, 0.4):
            for qscale in arange(1, 4, 0.4):
                res = getChi2(events, {'ttbar': tscale, 'wjets':wscale, 'qcd':qscale})
                chi2[res] = {'ttbar': tscale, 'wjets':wscale, 'qcd':qscale}
    keys = chi2.keys()
    result = {'ttbar': 1, 'wjets':1, 'qcd':1}
    if len(keys) > 0:
        result = sorted(keys)[0]
    return result, chi2[result]
        
            
def getChi2(events={}, scales={}):
    diff = 0
    for eventsData, eventsTTbar, eventsWJets, eventsQCD  in zip(events['data'], events['ttbar'], events['wjets'], events['qcd']):        
        eventsMC = eventsTTbar * scales['ttbar'] + eventsWJets * scales['wjets'] + eventsQCD * scales['qcd']
        diff += (eventsData - eventsMC) ** 2
    return diff / len(events['data'])

if __name__ == "__main__":
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
    parser = OptionParser()
    parser.add_option("-l", "--logY",
                  action="store_true", dest="logY", default=False,
                  help="enable logarithmic y-axis")
    parser.add_option("-n", "--normalise",
                  action="store_true", dest="norm", default=False,
                  help="normalise to unit area")

    (options, args) = parser.parse_args()
    setLogY = options.logY
    normalise = options.norm
    plotMttbar()
