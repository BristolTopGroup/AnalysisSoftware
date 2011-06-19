import HistGetter
import HistPlotter
from time import sleep
from ROOT import *

outputFormat = 'png'
outputFolder = '/storage/results/plots/ElectronHad/'

def compareDataControlRegions(dataHists, bJetBins):
    saveAs = HistPlotter.saveAs
    
    leg = TLegend(0.696, 0.35, 0.94, 0.92);
    leg.SetBorderSize(0);
    leg.SetLineStyle(0);
    leg.SetTextFont(42);
    leg.SetFillStyle(0);

    AddLegendEntry = leg.AddEntry 
    alreadyAdded = False
        
    for bin in bJetBins:
        normConv = dataHists['topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets_' + bin]
#        normContr = dataHists['topReconstruction/backgroundShape/mttbar_controlRegion_withMETAndAsymJets_' + bin]
        normAnti = dataHists['topReconstruction/backgroundShape/mttbar_antiIsolated_withMETAndAsymJets_' + bin]
        normConv.SetYTitle("a.u/50GeV");
    
        normConv.Sumw2()
#        normContr.Sumw2()
        normAnti.Sumw2()
        
        
        normConv = HistPlotter.normalise(normConv)
#        normContr = HistPlotter.normalise(normContr)
        normAnti = HistPlotter.normalise(normAnti)
    
        diff = normConv.Clone()
        diff.Divide(normAnti)
        line = TLine(300, 1, 1800, 1)
        line.SetLineColor(1)
        line.SetLineWidth(4)
    
    
        c = TCanvas("cname", 'cname', 1920, 1080)
        diff.SetYTitle("conversions/non-iso electrons /50GeV");
        diff.Draw('error')
        line.Draw('same')
        saveAs(c, 'shapeRatio_conversions_NonIsolatedElectrons' + '_' + bin , outputFormat)
        del c
    
#        diff = normConv.Clone()
#        diff.Divide(normContr)
    
#        c = TCanvas("cname2", 'cname2', 1920, 1080)
#        diff.SetYTitle("conversions/fake electrons /50GeV");
#        diff.Draw('error')
#        line.Draw('same')
#        saveAs(c, 'shapeRatio_conversions_fakeElectrons' + '_' + bin , outputFormat)
#        del c
        
        c = TCanvas("cname3", 'cname3', 1920, 1080)
    
        normConv.SetFillColor(kYellow)
        normConv.SetFillStyle(1001)
        
#        normContr.SetFillColor(kAzure - 2)
#        normContr.SetFillStyle(3005)
    
        normAnti.SetFillColor(kRed + 1)
        normAnti.SetFillStyle(3004)
        normConv.GetYaxis().SetRangeUser(0, 0.2);
        normConv.Draw('hist')
#        normContr.Draw('hist same')
        normAnti.Draw('hist same')
    
        if not alreadyAdded:
            AddLegendEntry(normConv, "conversions", "f");
#            AddLegendEntry(normContr, "fake electrons", "f");
            AddLegendEntry(normAnti, "non isolated electrons", "f");
            alreadyAdded = True
        
        leg.Draw()
        saveAs(c, 'shape_comparison' + '_' + bin , outputFormat)
        del c
        
    del leg


def plotQCDEstimationFits(allHists, bJetBins):
    saveAs = HistPlotter.saveAs
    
    leg = TLegend(0.696, 0.35, 0.94, 0.92);
    leg.SetBorderSize(0);
    leg.SetLineStyle(0);
    leg.SetTextFont(42);
    leg.SetFillStyle(0);

    AddLegendEntry = leg.AddEntry 
    alreadyAdded = False
    for bin in bJetBins:
        histname = 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_' + bin
        data = allHists['data'][histname]
        allMC = allHists['allMC'][histname]
        

if __name__ == '__main__':
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
    
    files = {
    'data':"/storage/results/histogramFiles/ElectronHad_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'ttbar' : "/storage/results/histogramFiles/TTJet_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'wjets' : "/storage/results/histogramFiles/WJetsToLNu_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'zjets' : "/storage/results/histogramFiles/DYJetsToLL_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'bce1' : "/storage/results/histogramFiles/QCD_Pt-20to30_BCtoE_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'bce2' : "/storage/results/histogramFiles/QCD_Pt-30to80_BCtoE_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'bce3' : "/storage/results/histogramFiles/QCD_Pt-80to170_BCtoE_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'enri1' : "/storage/results/histogramFiles/QCD_Pt-20to30_EMEnriched_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'enri2' : "/storage/results/histogramFiles/QCD_Pt-30to80_EMEnriched_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'enri3' : "/storage/results/histogramFiles/QCD_Pt-80to170_EMEnriched_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'pj1' : "/storage/results/histogramFiles/GJets_TuneD6T_HT-40To100_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'pj2' : "/storage/results/histogramFiles/GJets_TuneD6T_HT-100To200_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'pj3' : "/storage/results/histogramFiles/GJets_TuneD6T_HT-200_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'tW' : "/storage/results/histogramFiles/TToBLNu_TuneZ2_tW-channel_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'tchan' : "/storage/results/histogramFiles/TToBLNu_TuneZ2_t-channel_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'ww' : "/storage/results/histogramFiles/WWtoAnything_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'wz' : "/storage/results/histogramFiles/WZtoAnything_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    'zz' : "/storage/results/histogramFiles/ZZtoAnything_715.09pb_PFElectron_PF2PATJets_PFMET.root",
    }

    histsNames = [
                  'topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_antiIsolated_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_controlRegion_withMETAndAsymJets',
                  "QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts",
                  ]

    HistPlotter.setStyle()
    hists = HistGetter.getHistsFromFiles(histsNames, files, bJetBins=HistPlotter.allBjetBins)
    hists = HistGetter.addSampleSum( hists )
    hists = HistGetter.makeMCStack( hists )
#    hists = applyDefaultStylesAndColors( hists )
    
    hists = HistPlotter.rebin(hists, 50, 'mttbar*')
    hists = HistPlotter.setXRange(hists, (300, 1800), 'mttbar*')
    
    hists = HistPlotter.setXTitle(hists, 'm(t#bar{t})/GeV', 'mttbar*')
    hists = HistPlotter.setYTitle(hists, 'Events/(50 GeV)', 'mttbar*')
    
    compareDataControlRegions(dataHists=hists['data'], bJetBins=HistPlotter.inclusiveBjetBins)
    
#    hists = makeDetailedMCStack( hists )
    
#    
#    c = TCanvas("cname4", 'cname4', 1920, 1080)
#    histname = 'mttbar_antiIsolated_withMETAndAsymJets_allBtags'
#    hist = hists['allMCDetailed'][histname]
#    hists['enri1'][histname].GetYaxis().SetRangeUser(0, hist.GetMaximum()*1.4);
#    hists['enri1'][histname].Draw('hist')
#    hist.Draw('hist same')
#    
#    leg = TLegend(0.696, 0.35, 0.94, 0.92);
#    leg.SetBorderSize(0);
#    leg.SetLineStyle(0);
#    leg.SetTextFont(42);
#    leg.SetFillStyle(0);
#
#    leg.AddEntry(hists['enri1'][histname], "QCD EMEnr. 20to30", "f");
#    leg.AddEntry(hists['enri2'][histname], "QCD EMEnr. 30to80", "f");
#    leg.AddEntry(hists['enri3'][histname], "QCD EMEnr. 80to170", "f");
#    leg.AddEntry(hists['bce1'][histname], "QCD BCToE. 20to30", "f");
#    leg.AddEntry(hists['bce2'][histname], "QCD BCToE. 30to80", "f");
#    leg.AddEntry(hists['bce3'][histname], "QCD BCToE. 80to170", "f");
#    leg.AddEntry(hists['pj1'][histname], "QCD #gamma + jets 40to100", "f");
#    leg.AddEntry(hists['pj2'][histname], "QCD #gamma + jets  100to200", "f");
#    leg.AddEntry(hists['pj3'][histname], "QCD #gamma + jets  200toINF", "f");
#    leg.Draw()
#    
#    c.SaveAs('detailed_MC_NonIsoElectrons.png')
    
    
