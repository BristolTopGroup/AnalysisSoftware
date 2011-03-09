from HistGetter import *
from HistPlotter import *
from time import sleep
from ROOT import *

if __name__ == '__main__':
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 1001;')
    
    files = {'data':"/storage/results/QCD_Estimate/Preapproval3/data_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'ttbar' : "/storage/results/QCD_Estimate/Preapproval3/ttjet_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'wjets' : "/storage/results/QCD_Estimate/Preapproval3/wj_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'zjets' : "/storage/results/QCD_Estimate/Preapproval3/zj_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'bce1' : "/storage/results/QCD_Estimate/Preapproval3/bce1_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'bce2' : "/storage/results/QCD_Estimate/Preapproval3/bce2_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'bce3' : "/storage/results/QCD_Estimate/Preapproval3/bce3_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'enri1' : "/storage/results/QCD_Estimate/Preapproval3/enri1_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'enri2' : "/storage/results/QCD_Estimate/Preapproval3/enri2_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'enri3' : "/storage/results/QCD_Estimate/Preapproval3/enri3_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'pj1' : "/storage/results/QCD_Estimate/Preapproval3/pj1_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'pj2' : "/storage/results/QCD_Estimate/Preapproval3/pj2_36.145pb_PFElectron_PF2PATJets_PFMET.root",
    'pj3' : "/storage/results/QCD_Estimate/Preapproval3/pj3_36.145pb_PFElectron_PF2PATJets_PFMET.root"}

    histsNames = ['mttbar_conversions_allBtags',
                        'mttbar_conversions_withMETAndAsymJets_allBtags',
                        'mttbar_antiIsolated_withMETAndAsymJets_allBtags',
                        'mttbar_antiIsolated_allBtags',
                        'mttbar_controlRegion_withMETAndAsymJets_allBtags',
                        'mttbar_controlRegion_allBtags']

#    HistGetter.samplefiles = files
#    HG = HistGetter()
    setStyle()
    hists = getHistsFromFiles( histsNames, files )
    hists = addSampleSum( hists )
    hists = applyDefaultStylesAndColors( hists )
    
    hists = rebin( hists, 50, 'mttbar*' )
    hists = setXRange( hists, ( 300, 1800 ), 'mttbar*' )
    
    hists = setXTitle( hists, 'm(t#bar{t})/GeV', 'mttbar*' )
    hists = setYTitle( hists, 'Events/(50 GeV)', 'mttbar*' )
    
    hists = makeDetailedMCStack( hists )
    
    normConv = hists['data']['mttbar_conversions_withMETAndAsymJets_allBtags']
    normContr = hists['data']['mttbar_controlRegion_withMETAndAsymJets_allBtags']
    normAnti = hists['data']['mttbar_antiIsolated_withMETAndAsymJets_allBtags']
    
    normConv.SetYTitle("a.u/50GeV");
    
    normConv.Sumw2()
    normContr.Sumw2()
    normAnti.Sumw2()
    
    normConv.Scale(1/normConv.Integral())
    normContr.Scale(1/normContr.Integral())
    normAnti.Scale(1/normAnti.Integral())
    
    diff = normConv.Clone()
    diff.Divide(normAnti)
    line = TLine(300, 1, 1800, 1)
    line.SetLineColor(1)
    line.SetLineWidth(4)
    
    
    c = TCanvas("cname", 'cname', 1920, 1080)
    diff.SetYTitle("conversions/non-iso electrons /50GeV");
    diff.Draw('error')
    line.Draw('same')
    c.SaveAs('shapeRatio_conversions_NonIsolatedElectrons.png')
    
    diff = normConv.Clone()
    diff.Divide(normContr)
    
    c = TCanvas("cname2", 'cname2', 1920, 1080)
    diff.SetYTitle("conversions/fake electrons /50GeV");
    diff.Draw('error')
    line.Draw('same')
    c.SaveAs('shapeRatio_conversions_fakeElectrons.png')
    
    c = TCanvas("cname3", 'cname3', 1920, 1080)
    
    normConv.SetFillColor(kYellow)
    normConv.SetFillStyle( 1001 )
    
    normContr.SetFillColor(kAzure - 2)
    normContr.SetFillStyle( 3005 )
    
    normAnti.SetFillColor(kRed + 1)
    normAnti.SetFillStyle( 3004 )
    normConv.GetYaxis().SetRangeUser(0, 0.2);
    normConv.Draw('hist')
    normContr.Draw('hist same')
    normAnti.Draw('hist same')
    
    leg = TLegend(0.696, 0.35, 0.94, 0.92);
    leg.SetBorderSize(0);
    leg.SetLineStyle(0);
    leg.SetTextFont(42);
    leg.SetFillStyle(0);

    leg.AddEntry(normConv, "conversions", "f");
    leg.AddEntry(normContr, "fake electrons", "f");
    leg.AddEntry(normAnti, "non isolated electrons", "f");
    leg.Draw()
    
    c.SaveAs('shape_comparison.png')
    
    c = TCanvas("cname4", 'cname4', 1920, 1080)
    histname = 'mttbar_antiIsolated_withMETAndAsymJets_allBtags'
    hist = hists['allMCDetailed'][histname]
    hists['enri1'][histname].GetYaxis().SetRangeUser(0, hist.GetMaximum()*1.4);
    hists['enri1'][histname].Draw('hist')
    hist.Draw('hist same')
    
    leg = TLegend(0.696, 0.35, 0.94, 0.92);
    leg.SetBorderSize(0);
    leg.SetLineStyle(0);
    leg.SetTextFont(42);
    leg.SetFillStyle(0);

    leg.AddEntry(hists['enri1'][histname], "QCD EMEnr. 20to30", "f");
    leg.AddEntry(hists['enri2'][histname], "QCD EMEnr. 30to80", "f");
    leg.AddEntry(hists['enri3'][histname], "QCD EMEnr. 80to170", "f");
    leg.AddEntry(hists['bce1'][histname], "QCD BCToE. 20to30", "f");
    leg.AddEntry(hists['bce2'][histname], "QCD BCToE. 30to80", "f");
    leg.AddEntry(hists['bce3'][histname], "QCD BCToE. 80to170", "f");
    leg.AddEntry(hists['pj1'][histname], "QCD #gamma + jets 40to100", "f");
    leg.AddEntry(hists['pj2'][histname], "QCD #gamma + jets  100to200", "f");
    leg.AddEntry(hists['pj3'][histname], "QCD #gamma + jets  200toINF", "f");
    leg.Draw()
    
    c.SaveAs('detailed_MC_NonIsoElectrons.png')
    
    
