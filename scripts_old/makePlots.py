from __future__ import division
from ROOT import *
import HistGetter
import HistPlotter
from time import sleep
from math import isnan

import inputFiles
import QCDEstimation
outputFormats = ['png', 'pdf']
outputFolder = '/storage/results/plots/ElectronHad/'
saveAs = HistPlotter.saveAs
triggers = [
#            'HLT_Ele25_CaloIdVT_TrkIdT_CentralJet30',
#            'HLT_Ele25_CaloIdVT_TrkIdT_DiCentralJet30',
              'HLT_Ele25_CaloIdVT_TrkIdT_TriCentralJet30', 
#                'HLT_Ele25_CaloIdVT_TrkIdT_QuadCentralJet30',
#                'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_CentralJet30', 
#                'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_DiCentralJet30',
                'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralJet30',
               'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFJet30',  
#               'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralPFJet30',
#                'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_QuadCentralJet30'
                ]
    
triggerVariables = ['jet_pt', 
                    'jet_eta', 
                    'jet_phi',
                    'jet_eta_PtGT30',
                    'jet_phi_PtGT30',
                    'jet_eta_PtGT35',
                    'jet_phi_PtGT35',
                    'jet_eta_PtGT40',
                    'jet_phi_PtGT40',
                    'jet_eta_PtGT45',
                    'jet_phi_PtGT45']
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
#        normConv = dataHists['topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets_' + bin]
#        normContr = dataHists['topReconstruction/backgroundShape/mttbar_antiID_withMETAndAsymJets_' + bin]
#        normAnti = dataHists['topReconstruction/backgroundShape/mttbar_antiIsolated_withMETAndAsymJets_' + bin]
        normConv = dataHists['topReconstruction/backgroundShape/mttbar_3jets_conversions_withMETAndAsymJets_' + bin]
        normContr = dataHists['topReconstruction/backgroundShape/mttbar_3jets_antiID_withMETAndAsymJets_' + bin]
        normAnti = dataHists['topReconstruction/backgroundShape/mttbar_3jets_antiIsolated_withMETAndAsymJets_' + bin]
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
    
    
        c = TCanvas("cname", 'cname', 1200, 900)
        diff.SetYTitle("conversions/non-iso electrons /50GeV");
        diff.Draw('error')
        line.Draw('same')
        saveAs(c, 'shapeRatio_conversions_NonIsolatedElectrons' + '_' + bin , outputFormats)
        del c
    
#        diff = normConv.Clone()
#        diff.Divide(normContr)
#    
#        c = TCanvas("cname2", 'cname2', 1200, 900)
#        diff.SetYTitle("conversions/fake electrons /50GeV");
#        diff.Draw('error')
#        line.Draw('same')
#        saveAs(c, 'shapeRatio_conversions_fakeElectrons' + '_' + bin , outputFormat)
#        del c
#        
        c = TCanvas("cname3", 'cname3', 1200, 900)
    
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
        saveAs(c, 'shape_comparison_3jets' + '_' + bin , outputFormats)
        del c
        
    del leg

def plotControlRegionComparision(normConv, normContr, normAnti):
    pass

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
    data = hists['data']
    ttbar = hists['ttbar']
    
    #create a dictionary for each sample
    # dic[trigger] = {'fitfunction':'', 'parameters':[(param, error), ...], 'sfFunction':'', sfParams}
    
    plots = ['HLTStudy/' + trigger + '/' + variable for trigger in triggers for variable in triggerVariables]
#    saveAs = HistPlotter.saveAs
    efficiency = {}
    mc_efficiency = {}
    infos = {}
    mcInfos = {}
    gStyle.SetOptFit( 112 );
    for jetbin in HistPlotter.allJetBins:
        for plot in plots:
            if 'Quad' in plot:
                if not '4' in jetbin:
                    continue
            
            elif 'Tri' in plot:
                if not '3jets' in jetbin and not '4' in jetbin:
                    continue
            elif 'Di' in plot:
                if not '2jet' in jetbin:
                    continue
            else:
                if not '1jet' in jetbin:
                    continue 
            print plot + '_' + jetbin
            fired = data[plot + '_' + 'fired_' + jetbin]
            visited = data[plot + '_' + 'visited_' + jetbin]
            mc_fired = ttbar[plot + '_' + 'fired_' + jetbin]
            mc_visited = ttbar[plot + '_' + 'visited_' + jetbin]
            
            fired.Sumw2()
            visited.Sumw2()
            mc_fired.Sumw2()
            mc_visited.Sumw2()
            
            if not infos.has_key(plot + '_' + jetbin):
                infos[plot + '_' + jetbin] = {}
                mcInfos[plot + '_' + jetbin] = {}
#            print fired.GetNbinsX()
#            print visited.GetNbinsX()
#            print mc_fired.GetNbinsX()
#            print mc_visited.GetNbinsX()
#            
#            print fired.GetNbinsX()
#            print visited.GetNbinsX()
#            print mc_fired.GetNbinsX()
#            print mc_visited.GetNbinsX()
        
            xlimits = [10,200]
            xTitle = 'jet p_{T} (GeV)'
            yTitle = 'efficiency/(GeV)'
            fitfunction = ''
            decimals = 2
            if 'jet_pt' in plot:
                xlimits = [10,200]
                xTitle = 'jet p_{T} (GeV)'
                yTitle = 'efficiency/(%d GeV)' % (1*rebin)
                fitfunction = "[0]*exp([1]*exp([2]*x))"
                scaleFactorFitFunction = "[0]*exp([1]*exp([2]*x) - [3]*exp([4]*x))"
                fitRange = [20,200]
            elif 'jet_eta' in plot:
                xlimits = [-3,3]
                xTitle = 'jet #eta (GeV)'
                yTitle = 'efficiency/(%0.1f)' % (0.1*rebin)
                fitfunction = '[0]*x*x + [1]*x + [2]'
                scaleFactorFitFunction = fitfunction
                fitRange = [-3,3]
            elif 'jet_phi' in plot:
                xlimits = [-4.,4.]
                xTitle = 'jet #phi (GeV)'
                yTitle = 'efficiency/(%0.1f)' % (0.1*rebin)
                fitfunction = '[0]'
                decimals = 4
                scaleFactorFitFunction = fitfunction
                fitRange = [-3.1,3.1]
        
            fired.GetXaxis().SetRangeUser(xlimits[0], xlimits[1])
            visited.GetXaxis().SetRangeUser(xlimits[0], xlimits[1])
            mc_fired.GetXaxis().SetRangeUser(xlimits[0], xlimits[1])
            mc_visited.GetXaxis().SetRangeUser(xlimits[0], xlimits[1])

            fired.Rebin(rebin)
            visited.Rebin(rebin)
            mc_fired.Rebin(rebin)
            mc_visited.Rebin(rebin)
        
#            print fired.GetNbinsX()
#            print visited.GetNbinsX()
#            print mc_fired.GetNbinsX()
#            print mc_visited.GetNbinsX()
            efficiency[plot + jetbin] = TEfficiency(fired, visited)
            mc_efficiency[plot + jetbin] = TEfficiency(mc_fired, mc_visited)
            eff = efficiency[plot + jetbin].Clone("Copy")
            mceff = mc_efficiency[plot + jetbin].Clone("CopyMC")
#            efficiency.Rebin(rebin)
#            mc_efficiency.Rebin(rebin)
            f1= TF1("f1",fitfunction,fitRange[0],fitRange[1]);
            f2= TF1("f2",fitfunction,fitRange[0],fitRange[1]);
            f3= TF1("f3",scaleFactorFitFunction,fitRange[0],fitRange[1]);

            if 'jet_pt' in plot:
                f1.SetParLimits(0,0.8,1.0);
                f1.SetParLimits(1,-100,-1);
                f1.SetParLimits(2,-1,-0.01);
            
                f2.SetParLimits(0,0.8,1.0);
                f2.SetParLimits(1,-100,-1);
                f2.SetParLimits(2,-1,-0.01);
                
                f3.SetParLimits(2,-1,-0.01);
                
                if "_CentralJet" in plot:
                    f1.SetParLimits(0,0,1.0);
                    f2.SetParLimits(0,0,1.0);
                
#            if 'jet_eta' in plot:
#                f1.SetParLimits(0,0.2,1.0);
#                f1.SetParLimits(1,-100,-1);
#                f1.SetParLimits(2,-1,-0.01);
            
#                f2.SetParLimits(0,0.2,1.0);
#                f2.SetParLimits(1,-100,-1);
#                f2.SetParLimits(2,-1,-0.01);
            
            f1.SetLineWidth(2);
            f1.SetLineColor(1);
            f2.SetLineWidth(2);
            f2.SetLineColor(2);
            f3.SetLineWidth(2);
            f3.SetLineColor(2);
            
            efficiency[plot + jetbin].Fit(f1);
            mc_efficiency[plot + jetbin].Fit(f2);
            axis = mceff.GetTotalHistogram().GetXaxis()
            
            scaleFactor = TH1F(fired.Clone("MCfired"))
            scaleFactor.Multiply(mc_visited)
            scaleFactor.Divide(visited)
            scaleFactor.Divide(mc_fired)
#            scaleFactor = TH1D("sf", "sf",axis.GetNbins(), axis.GetXmin(), axis.GetXmax())
#            print plot + '_' + jetbin
#            for bin in range(1, axis.GetNbins()):
#                effMC = mceff.GetEfficiency(bin)
#                effData = eff.GetEfficiency(bin)
#                #print effMC, effData
#                sf = 0
#                if not effData == 0 and not isnan(effData) and not isnan(effMC):
#                    sf = effMC/effData
#                    
#                scaleFactor.SetBinContent(bin, sf)
            
#            scaleFactor = mc_hist.Clone("SF")
#            scaleFactor.Divide(data_hist);
            
            mceff.SetLineColor(2)
            mceff.SetMarkerColor(2)
            mceff.SetMarkerStyle(22)
        
            saveName = plot + '_' + 'efficiency'
            if not suffix == '':
                saveName += '_' + suffix
            saveName = saveName.replace('Jet30/', 'Jet30_')
        
#            c = TCanvas("cname" + plot + jetbin, 'cname', 900, 900)
            c = TCanvas("cname" + plot + jetbin, 'cname', 1080, 1080)
            
#            pad1 = TPad("pad1", "The pad with the function",0.03,0.2,0.98,0.98);
#            pad2 = TPad("pad2","The pad with the histogram",0.03,0.02,0.98,0.19);
            pad1 = TPad("pad1", "The pad with the function",0.0,0.2,1.0,1.0);
            pad2 = TPad("pad2","The pad with the histogram",0.0,0.0,1.0,0.2);
            pad1.Draw()
            pad2.Draw()
            
            pad1.cd()
            leg = TLegend(0.7, 0.2, 0.8, 0.3)
            leg.SetBorderSize(0);
            leg.SetLineStyle(0);
            leg.SetTextFont(42);
            leg.SetFillStyle(0);
            leg.AddEntry(eff, 'data', 'P')
            leg.AddEntry(mceff, 'MC', 'P')
    
            hFrame = (pad1.cd()).DrawFrame(xlimits[0],-.1,xlimits[1],1.1)
            hFrame.GetXaxis().SetTitle(xTitle)
            hFrame.GetYaxis().SetTitle(yTitle)
            hFrame.Draw()
            upper = TLine(xlimits[0],1.,xlimits[1],1.)
            lower = TLine(xlimits[0],0.,xlimits[1],0.)
            cut = TLine(30., 0., 30., 1.)
            cut.SetLineColor(1)
            upper.SetLineColor(4)
            lower.SetLineColor(4)
            upper.DrawLine(xlimits[0],1.,xlimits[1],1.) ;
            lower.DrawLine(xlimits[0],0.,xlimits[1],0.) ;
            cut.DrawLine(30., 0., 30., 1.)
            
            
            eff.Draw('SAMEP0')
            mceff.Draw('SAMEP0')
            leg.Draw('same')
            f1.DrawCopy('same')
            f2.DrawCopy('same')
#            
            tex = TLatex(0.18,1,"CMS Preliminary 2011,  #sqrt{s} = 7 TeV, L = 4.69 fb^{-1}");
            tex.SetNDC();
            tex.SetTextAlign(13);
            tex.SetTextFont(42);
            tex.SetTextSize(0.04);
            tex.SetLineWidth(2);
            tex.Draw();
            
            #f1.GetFunction? If yes, color-code the functions
            strFit = fitfunction
            strFit = strFit.replace('[0]', str(round(f1.GetParameter(0), decimals)))
            strFit = strFit.replace('[1]', str(round(f1.GetParameter(1), decimals)))
            strFit = strFit.replace('[2]', str(round(f1.GetParameter(2), decimals)))
                                         
                                         
            #replace parameters
            tex2 = TLatex(0.18,0.18,strFit);
            tex2.SetNDC();
            tex2.SetTextAlign(13);
            tex2.SetTextFont(42);
            tex2.SetTextSize(0.04);
            tex2.SetLineWidth(2);
            tex2.Draw();
            
            strFit = fitfunction
            strFit = strFit.replace('[0]', str(round(f2.GetParameter(0), decimals)))
            strFit = strFit.replace('[1]', str(round(f2.GetParameter(1), decimals)))
            strFit = strFit.replace('[2]', str(round(f2.GetParameter(2), decimals)))
                                         
                                         
            #replace parameters
            tex3 = TLatex(0.57,0.18,strFit);
            tex3.SetNDC();
            tex3.SetTextAlign(13);
            tex3.SetTextFont(42);
            tex3.SetTextSize(0.04);
            tex3.SetLineWidth(2);
            tex3.SetTextColor(2)
            tex3.Draw();
            
            pad2.cd()
            hFrame = (pad2.cd()).DrawFrame(xlimits[0],0,xlimits[1],2)
            hFrame.GetXaxis().SetTitle(xTitle)
            hFrame.GetYaxis().SetTitle(yTitle)
            hFrame.Draw()
            upper = TLine(xlimits[0],1.,xlimits[1],1.)
#            lower = TLine(xlimits[0],0.,xlimits[1],0.)
            cut = TLine(30., 0., 30., 2.)
            cut.SetLineColor(1)
            upper.SetLineColor(1)
            lower.SetLineColor(4)
            upper.DrawLine(xlimits[0],1.,xlimits[1],1.) ;
#            lower.DrawLine(xlimits[0],0.,xlimits[1],0.) ;
            cut.DrawLine(30., 0., 30., 2.)
            
            #replace this fit with results from the data and MC fit
#            sfitfunction = f1/f2
            #scaleFactor.Fit(f3)
            scaleFactor.Fit(f3)
#            print 'here', f3.GetParameter(0), f3.GetParameter(1), f3.GetParameter(2)
            scaleFactor.GetYaxis().SetRangeUser(0., 3.);
            scaleFactor.SetLineColor(4)
            scaleFactor.SetMarkerColor(4)
            scaleFactor.Draw('SAMEP0')
            strFit = fitfunction
            strFit = strFit.replace('[0]', str(round(f3.GetParameter(0), decimals)))
            strFit = strFit.replace('[1]', str(round(f3.GetParameter(1), decimals)))
            strFit = strFit.replace('[2]', str(round(f3.GetParameter(2), decimals)))
            #replace parameters
            tex4 = TLatex(0.18,0.78,strFit);
            tex4.SetNDC();
            tex4.SetTextAlign(13);
            tex4.SetTextFont(42);
            tex4.SetTextSize(0.2);
            tex4.SetLineWidth(2);
            tex4.SetTextColor(2)
            tex4.Draw();
            f3.DrawCopy("same")
            
            #infos
            infos[plot + '_' + jetbin]['fitFunction'] = fitfunction
            infos[plot + '_' + jetbin]['sfFitFunction'] = scaleFactorFitFunction
            mcInfos[plot + '_' + jetbin]['fitFunction'] = fitfunction
            infos[plot + '_' + jetbin]['chi2'] = f1.GetChisquare()
            infos[plot + '_' + jetbin]['ndof'] = f1.GetNumberFreeParameters()
            infos[plot + '_' + jetbin]['sfchi2'] = f1.GetChisquare()
            infos[plot + '_' + jetbin]['sfndof'] = f1.GetNumberFreeParameters()
            infos[plot + '_' + jetbin]['params'] = []
            infos[plot + '_' + jetbin]['sfParams'] = []
            mcInfos[plot + '_' + jetbin]['params'] = []
            mcInfos[plot + '_' + jetbin]['chi2'] = f1.GetChisquare()
            mcInfos[plot + '_' + jetbin]['ndof'] = f1.GetNumberFreeParameters()
            
            for i in range(f1.GetNpar()):
                par, err = f1.GetParameter(i), f1.GetParError(i)
                infos[plot + '_' + jetbin]['params'].append((par, err))
                
            for i in range(f2.GetNpar()):
                par, err = f2.GetParameter(i), f2.GetParError(i)
                mcInfos[plot + '_' + jetbin]['params'].append((par, err))
                
            for i in range(f3.GetNpar()):
                par, err = f3.GetParameter(i), f3.GetParError(i)
                infos[plot + '_' + jetbin]['sfParams'].append((par, err))
            
#            eff.DrawClone('P0')
#            f1.Draw("SAME");
            
            saveAs(c, saveName + '_' + jetbin + '_Summer11', outputFolder = outputFolder,outputFormats= ['png'])
            del hFrame
            del c
            del f1
            del f2
            del f3
    del efficiency
    del mc_efficiency
    return infos, mcInfos
    
    
def makeQCDErrorPlot(files, hists):
    errors = QCDEstimation.getShapeErrorHistogram('topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets_0orMoreBtag', files)
#    systematicEstimationError = QCDEstimation.doClosureTestFor(files, 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_1btag')
#    qcdEstimate = QCDEstimation.estimateQCDFor('', files['data'], 'QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts_1btag')
#    print 'QCD estimation:', qcdEstimate
#    print 'Systematic uncertainty for this b-jet bin:', systematicEstimationError
    gStyle.SetErrorX(0.5);
    
    if errors:
        errors.GetXaxis().SetRangeUser(250, 3000)
        
        c = TCanvas("cname3", 'cname3', 1200, 900)
        
#        errors.DrawCopy("hist");
        errors.SetFillColor(kBlack)
        errors.SetMarkerStyle(0)
#        errors.SetFillStyle(1001)
        errors.SetFillStyle(3013);
        errors.DrawCopy('E2')
        saveAs(c, 'shape_errors_0orMoreBtag' , outputFormats)
        
        controlHist = hists['data']['topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets_0orMoreBtag']
        nTotalControl = controlHist.Integral()
        shapeError = 0
        errorlist = []
        binlist = []
        for bin in range(1, errors.GetNbinsX()):
            err = fabs(errors.GetBinContent(bin) + errors.GetBinError(bin))
            errorlist.append(err)
            binValue = errors.GetBinCenter(bin) - errors.GetBinWidth(bin)/2
            binlist.append(binValue)
            controlEntries = controlHist.GetBinContent(bin)
            
            shapeError += (err*controlEntries/nTotalControl)**2
#            if binValue >= 200 and binValue <= 2000:
#                print 'relative error:', err
#                print 'bin start:', binValue
        print 'Total shape error: ', sqrt(shapeError)
        print binlist
        print errorlist
    gStyle.SetErrorX(0.);
    
def compareShapesTwoData(dataOld, dataNew):
    hists = ['topReconstruction/mttbar_withMETAndAsymJets',
             'topReconstruction/mttbar_3jets_withMETAndAsymJets',
             ]
    histsOld = HistGetter.getHistsFromFiles(hists, {'data': dataOld}, bJetBins=HistPlotter.allBjetBins)
    histsNew = HistGetter.getHistsFromFiles(hists, {'data': dataNew}, bJetBins=HistPlotter.allBjetBins)
    
    
    leg = TLegend(0.696, 0.35, 0.94, 0.92);
    leg.SetBorderSize(0);
    leg.SetLineStyle(0);
    leg.SetTextFont(42);
    leg.SetFillStyle(0);

    AddLegendEntry = leg.AddEntry 
    alreadyAdded = False
    
    for bin in HistPlotter.allBjetBins:
        for hist in hists:
            current = hist + '_' + bin
            currentOld = histsOld['data'][current]
            currentNew = histsNew['data'][current]
            currentOld.Rebin(50)
            currentNew.Rebin(50)
            if currentOld.Integral() > 0:
                currentOld.Scale(1000/currentOld.Integral())
            if currentNew.Integral():
                currentNew.Scale(1000/currentNew.Integral())
            
            c = TCanvas("cname3", 'cname3', 1200, 900)
            c.SetLogy(1)
            currentOld.SetFillColor(2)
            currentOld.SetFillStyle(3004)
        
            currentNew.SetFillColor(4)
            currentNew.SetFillStyle(3005)
    
            currentOld.GetXaxis().SetRangeUser(250, 3000);
            currentOld.Draw('hist')
            currentNew.Draw('hist same')
    
            if not alreadyAdded:
                AddLegendEntry(currentOld, "old JEC", "f");
                AddLegendEntry(currentNew, "new JEC", "f");
            alreadyAdded = True
        
            leg.Draw()
            saveAs(c, current + '_shape_comparison' , outputFormats)
            del c
        
def printHLTOutput(infos, mcInfos):
#    TODO: create dictionary scaleFunction[trigger][ptThreshold][function|error]
    keys = infos.keys()
    previous = ''
    for hlt in sorted(keys):
        trigger = hlt.split('/')[1]
        suffix = hlt.split('/')[-1]
        jetbin = suffix.split('_')[-1]
        variable = suffix.split('_')[1]
        ptThreshold = ''
        if 'PtGT' in suffix:
            ptThreshold = '(' + suffix.split('_')[2] + ')'
        if not trigger == previous:
            print '='*10, trigger, '='*10
        previous = trigger
        print '='*30
        print 'Fit functions for jet bin', jetbin, 'and variable', variable, ptThreshold
#        print 'Fit function for data'
        fitfunction = infos[hlt]['sfFitFunction']
        params = infos[hlt]['sfParams']
        function = fitfunction
        upperFunction = function
        lowerFunction = function
        errorFunction = function
        for i in range(len(params)):
            param = params[i][0]
            err = params[i][1]
            upper = param + err
            lower = param - err
            if not 'pol' in function:
                function = function.replace('[%d]'%i, str(param))
                upperFunction = upperFunction.replace('[%d]'%i, str(upper))
                lowerFunction = lowerFunction.replace('[%d]'%i, str(lower))
                errorFunction = errorFunction.replace('[%d]'%i, '&pm;' + str(err))
                
#        print 'Upper boundary'
#        print upperFunction
#        chi2 = infos[hlt]['sfchi2']
#        ndof = infos[hlt]['sfndof']
#        print 'central fit, chi2:', chi2, 'ndof:', ndof, 'chi2/ndof', chi2/ndof
#        print function
#        print 'Lower boundary'
#        print lowerFunction
        
        fitRange = [-3.1,3.1]
        if variable == 'pt':
            fitRange = [20,200]
        elif variable == 'eta':
            fitRange = [-3,3]
            
        print 'variable', variable
        print '| =%s= | %s | %s |' % (trigger, function, errorFunction) 
            
        
        c = TCanvas("cname", 'cname', 1200, 900)
        f1 = TF1('f1', upperFunction, fitRange[0], fitRange[1])
        f2 = TF1('f2', function, fitRange[0], fitRange[1])
        f3 = TF1('f3', lowerFunction, fitRange[0], fitRange[1])
        f1.SetLineColor(2)
        f2.SetLineColor(1)
        f3.SetLineColor(4)
        
        f1.Draw()
        f2.Draw('same')
        f3.Draw('same')
        
        saveAs(c, hlt + '_scaleFactor', 
               outputFolder = outputFolder,outputFormats= ['png'])
        
        
        
        
if __name__ == '__main__':
    gROOT.SetBatch(True)
    gROOT.ProcessLine('gErrorIgnoreLevel = 5001;')
    
    files = inputFiles.files
    
    hltFiles = {}
    hltFiles['data'] = '/storage/results/histogramFiles/data_1959.75pb_PFElectron_PF2PATJets_PFMET.root'
    hltFiles['ttbar'] = '/storage/results/histogramFiles/TTJet_1959.75pb_PFElectron_PF2PATJets_PFMET.root'
    
    hltFiles['data'] = inputFiles.files['data']
    hltFiles['ttbar'] = inputFiles.files['ttbar']

    histsNames = [
                  'topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_3jets_conversions_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_antiIsolated_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_3jets_antiIsolated_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_controlRegion_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_antiID_withMETAndAsymJets',
                  'topReconstruction/backgroundShape/mttbar_3jets_antiID_withMETAndAsymJets'
#                  "QCDStudy/PFIsolation_WithMETCutAndAsymJetCuts",
                  ]

    triggerPlots = ['HLTStudy/' + trigger + '/' + variable + '_' + modifier for trigger in triggers for variable in triggerVariables for modifier in triggerModifiers]
    HistPlotter.setStyle()
#    hists = HistGetter.getHistsFromFiles(histsNames, files, bJetBins=HistPlotter.allBjetBins)
    hists = HistGetter.getHistsFromFiles(triggerPlots, hltFiles, jetBins=HistPlotter.allJetBins)
#    hists = HistGetter.addSampleSum( hists )
#    
#    hists = HistPlotter.applyDefaultStylesAndColors( hists )
#    
#    hists = HistPlotter.rebin(hists, 50, 'mttbar*')
#    hists = HistPlotter.setXRange(hists, (300, 1800), 'mttbar*')
###    
#    hists = HistPlotter.setXTitle(hists, 'm(t#bar{t})/GeV', 'mttbar*')
#    hists = HistPlotter.setYTitle(hists, 'Events/(50 GeV)', 'mttbar*')
#    
#    hists = HistGetter.makeMCStack( hists )
#    hists = HistGetter.makeDetailedMCStack( hists )
    
#    compareQCDControlRegionsInData(dataHists=hists['data'], bJetBins=HistPlotter.inclusiveBjetBins)
#    compareShapesTwoData(
#                         '/storage/results/histogramFiles/PAS3/data_1091.45pb_PFElectron_PF2PATJets_PFMET.root',
#                         '/storage/results/histogramFiles/CiCElectron ID/data_1611.95pb_PFElectron_PF2PATJets_PFMET.root'
#                         )
#    makeQCDErrorPlot(files, hists)
    infos, mcInfos =  plotHLTStudy(hists, rebin = 5)
    printHLTOutput(infos, mcInfos)
    
#    
#    c = TCanvas("cname4", 'cname4', 1200, 900)
#    histname = 'topReconstruction/backgroundShape/mttbar_conversions_withMETAndAsymJets_0orMoreBtag'
#    hist = hists['allMCDetailed'][histname]
#    hists['enri1'][histname].GetYaxis().SetRangeUser(0, hist.GetMaximum()*1.4);
#    hists['enri1'][histname].Draw('hist')
#    hist.Draw('hist same')
#    
#    entries = hists['enri3'][histname].GetEntries()
#    events = hists['enri3'][histname].Integral()
#    print entries, events, events/entries
#    print QCDEstimation.getIntegral(hists['enri3'][histname], (350, 400))
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
#    c.RedrawAxis()
#    
#    c.SaveAs('detailed_MC_conversions.png')
    
    
