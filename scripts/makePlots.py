from ROOT import *
import HistGetter
import HistPlotter
from time import sleep

import inputFiles
import QCDEstimation
outputFormats = ['png', 'pdf']
outputFolder = '/storage/results/plots/ElectronHad/'
saveAs = HistPlotter.saveAs
triggers = ['HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30','HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30',
                'HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30', 'HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30',
                'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30', 'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30',
                'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30', 'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30']
    
triggerVariables = ['jet_pt', 'jet_eta', 'jet_phi']
triggerModifiers = ['visited', 'fired']

def compareQCDControlRegionsInData(dataHists, bJetBins):
#    saveAs = HistPlotter.saveAs
    
    leg = TLegend(0.696, 0.35, 0.94, 0.92);
    leg.SetBorderSize(0);
    leg.SetLineStyle(0);
    leg.SetTextFont(42);
    leg.SetFillStyle(0);

    AddLegendEntry = leg.AddEntry 
    alreadyAdded = False
        
    for bin in bJetBins:
        normConv = dataHists['topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets_' + bin]
        normContr = dataHists['topReconstruction/backgroundShape/mttbar_antiID_withMETAndAsymJets_' + bin]
        normAnti = dataHists['topReconstruction/backgroundShape/mttbar_antiIsolated_withMETAndAsymJets_' + bin]
        normConv.SetYTitle("a.u/50GeV");
    
        normConv.Sumw2()
        normContr.Sumw2()
        normAnti.Sumw2()
        
        
        normConv = HistPlotter.normalise(normConv)
        normContr = HistPlotter.normalise(normContr)
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
        saveAs(c, 'shapeRatio_conversions_NonIsolatedElectrons' + '_' + bin , outputFormats)
        del c
    
#        diff = normConv.Clone()
#        diff.Divide(normContr)
#    
#        c = TCanvas("cname2", 'cname2', 1920, 1080)
#        diff.SetYTitle("conversions/fake electrons /50GeV");
#        diff.Draw('error')
#        line.Draw('same')
#        saveAs(c, 'shapeRatio_conversions_fakeElectrons' + '_' + bin , outputFormat)
#        del c
#        
        c = TCanvas("cname3", 'cname3', 1920, 1080)
    
        normConv.SetFillColor(kYellow)
        normConv.SetFillStyle(1001)
        
        normContr.SetFillColor(kAzure - 2)
        normContr.SetFillStyle(3005)
    
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
        saveAs(c, 'shape_comparison' + '_' + bin , outputFormats)
        del c
        
    del leg


def plotQCDEstimationFits(allHists, bJetBins):
#    saveAs = HistPlotter.saveAs
    
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

def plotTopReconstruction(files):
    pass

def plotMttbar():
    pass

def getControlRegionsFor(hist):
    pass        

def plotHLTStudy(hists, suffix = '', rebin = 1):
    
    plots = ['HLTStudy/' + trigger + '/' + variable for trigger in triggers for variable in triggerVariables]
#    saveAs = HistPlotter.saveAs
    
    for plot in plots:
        fired = hists[plot + '_' + 'fired']
        visited = hists[plot + '_' + 'visited']
        
        fired.Sumw2()
        visited.Sumw2()
        
        fired.Rebin(rebin)
        visited.Rebin(rebin)
        
        xlimits = [10,100]
        xTitle = 'jet p_{T} (GeV)'
        yTitle = 'efficiency/(GeV)'
        if 'jet_pt' in plot:
            xlimits = [10,100]
            xTitle = 'jet p_{T} (GeV)'
            yTitle = 'efficiency/(%d GeV)' % (1*rebin)
        elif 'jet_eta' in plot:
            xlimits = [-3,3]
            xTitle = 'jet #eta (GeV)'
            yTitle = 'efficiency/(%0.1f)' % (0.1*rebin)
        elif 'jet_phi' in plot:
            xlimits = [-4,4]
            xTitle = 'jet #phi (GeV)'
            yTitle = 'efficiency/(%0.1f)' % (0.1*rebin)
        
        fired.GetXaxis().SetRangeUser(xlimits[0], xlimits[1])
        visited.GetXaxis().SetRangeUser(xlimits[0], xlimits[1])
        
        efficiency = TEfficiency(fired, visited)
        
        saveName = plot + '_' + 'efficiency'
        if not suffix == '':
            saveName += '_' + suffix
        saveName = saveName.replace('Jet30/', 'Jet30_')
        
        

#        if 'jet_pt' in plot:
#            efficiency.SetTitle("trigger efficiency;jet p_{T} (GeV);#epsilon/(GeV)")
#        elif 'jet_eta' in plot:
#            efficiency.SetTitle("trigger efficiency;jet #eta;#epsilon/(0.1)")
#        elif 'jet_phi' in plot:
#            efficiency.SetTitle("trigger efficiency;jet #Phi;#epsilon/(0.1)")
        
        c = TCanvas("cname", 'cname', 1920, 1080)
    
        hFrame = (c.cd()).DrawFrame(xlimits[0],-.1,xlimits[1],1.1)
        hFrame.GetXaxis().SetTitle(xTitle)
        hFrame.GetYaxis().SetTitle(yTitle)
        upper = TLine(xlimits[0],1.,xlimits[1],1.)
        lower = TLine(xlimits[0],0.,xlimits[1],0.)
        upper.SetLineColor(2)
        lower.SetLineColor(4)
        upper.DrawLine(xlimits[0],1.,xlimits[1],1.) ;
        lower.DrawLine(xlimits[0],0.,xlimits[1],0.) ;
#        efficiency.GetXaxis().SetRangeUser(xlimits[0], xlimits[1])
        efficiency.Draw('SAMEP0')
        saveAs(c, saveName, outputFolder = outputFolder)
        del c
    
    
def makeQCDErrorPlot(files):
    errors = QCDEstimation.getShapeErrorHistogram('topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets_1orMoreBtag', files)
    systematicEstimationError = QCDEstimation.doClosureTestFor(files, 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_1btag')
    qcdEstimate = QCDEstimation.estimateQCDFor('', files['data'], 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_1btag')
    print 'QCD estimation:', qcdEstimate
    print 'Systematic uncertainty for this b-jet bin:', systematicEstimationError
    gStyle.SetErrorX(0.5);
  
    if errors:
        errors.GetXaxis().SetRangeUser(250, 3000)
        
        c = TCanvas("cname3", 'cname3', 1920, 1080)
        
        errors.SetFillColor(kBlue);
        
#        errors.DrawCopy("hist");
        errors.SetFillColor(kBlack)
        errors.SetMarkerStyle(0)
#        errors.SetFillStyle(1001)
        errors.SetFillStyle(3013);
        errors.DrawCopy('E2')
        saveAs(c, 'shape_errors_0orMoreBtag' , outputFormats)
    gStyle.SetErrorX(0.);
        
if __name__ == '__main__':
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
    
    files = inputFiles.files

    histsNames = [
                  'topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_antiIsolated_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_controlRegion_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_antiID_withMETAndAsymJets'
#                  "QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts",
                  ]

#    triggerPlots = ['HLTStudy/' + trigger + '/' + variable + '_' + modifier for trigger in triggers for variable in triggerVariables for modifier in triggerModifiers]
    HistPlotter.setStyle()
    hists = HistGetter.getHistsFromFiles(histsNames, files, bJetBins=HistPlotter.allBjetBins)
#    hists = HistGetter.getHistsFromFiles(triggerPlots, files)
#    hists = HistGetter.addSampleSum( hists )
    
#    hists = applyDefaultStylesAndColors( hists )
    
    hists = HistPlotter.rebin(hists, 50, 'mttbar*')
    hists = HistPlotter.setXRange(hists, (300, 1800), 'mttbar*')
#    
    hists = HistPlotter.setXTitle(hists, 'm(t#bar{t})/GeV', 'mttbar*')
    hists = HistPlotter.setYTitle(hists, 'Events/(50 GeV)', 'mttbar*')
    
#    hists = HistGetter.makeMCStack( hists )
#    hists = makeDetailedMCStack( hists )
    
    compareQCDControlRegionsInData(dataHists=hists['data'], bJetBins=HistPlotter.inclusiveBjetBins)
    makeQCDErrorPlot(files)
#    plotHLTStudy(hists['data'], rebin = 2)
#    plotHLTStudy(hists['data2'], 'Calo', rebin = 2)
    
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
    
    
