#Step 1: get histograms of the distribution you are interested in
#Step 2: Pass the distributions with starting values to RooFit
#Step 3: ???
#Step 4: Profit

import FILES
import tools.ROOTFileReader as FileReader
import tools.PlottingUtilities as plotting
from ROOT import *
gROOT.SetBatch(True)
gROOT.ProcessLine('gErrorIgnoreLevel = 5001;')
    
    
availableSamples = FILES.samplesToLoad
files = {'SingleElectron': '/home/kreczko/Workspace/Analysis/BristolAnalysisTools/ElectronHad_99999pb_PFElectron_PFMuon_PF2PATJets_PFMET.root'}

variables = [
             'electron_pT',
             'electron_eta',
             'electron_phi',
             "electron_pfIsolation_03",
             "electron_pfIsolation_04",
             "electron_pfIsolation_05",
             "electron_dPhi_in",
             "electron_dEta_in",
             "electron_HadOverEM",
             "electron_mvaTrigV0",
             "electron_mvaNonTrigV0",
             "electron_dB",
'electron_sigma_ietaieta'
             ]

triggers = [
            'HLT_Ele25_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_TriCentralPFNoPUJet30',
            'HLT_Ele25_CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30',
            'HLT_Ele25_CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT_TriCentralPFNoPUJet30',
            'HLT_Ele25_CaloIdVT_TrkIdT_TriCentralPFNoPUJet30',
            ]

limits = {
          'electron_pT':(0, 200),
             'electron_eta':(-3, 3),
             'electron_phi':(-4, 4),
             "electron_pfIsolation_03":(0, 2),
             "electron_pfIsolation_04":(0, 2),
             "electron_pfIsolation_05":(0, 2),
             "electron_dPhi_in":(-0.2, 0.2),
             "electron_dEta_in":(-0.02, 0.02),
             "electron_HadOverEM":(0, 0.2),
             "electron_mvaTrigV0":(-1.1, 1.1),
             "electron_mvaNonTrigV0":(-1.1, 1.1),
             "electron_dB":(0, 0.05),
             'electron_sigma_ietaieta':(0, 0.05)
          }

rebins = {
          'electron_pT':5,
             'electron_eta':10,
             'electron_phi':10,
             "electron_pfIsolation_03":5,
             "electron_pfIsolation_04":5,
             "electron_pfIsolation_05":5,
             "electron_dPhi_in":1,
             "electron_dEta_in":1,
             "electron_HadOverEM":1,
             "electron_mvaTrigV0":5,
             "electron_mvaNonTrigV0":5,
             "electron_dB":1,
             'electron_sigma_ietaieta':1
          }

titles = {
          'electron_pT':'Events/(5 GeV)',
             'electron_eta':'Events/(0.5)',
             'electron_phi':'Events/(0.5)',
             "electron_pfIsolation_03":'Events/(0.05)',
             "electron_pfIsolation_04":'Events/(0.05)',
             "electron_pfIsolation_05":'Events/(0.05)',
             "electron_dPhi_in":'Events/(0.01)',
             "electron_dEta_in":'Events/(0.001)',
             "electron_HadOverEM":'Events/(0.01)',
             "electron_mvaTrigV0":'Events/(0.05)',
             "electron_mvaNonTrigV0":'Events/(0.05)',
             "electron_dB":'Events/(0.001 cm)',
             'electron_sigma_ietaieta':'Events/(0.001)'
          }

histograms = ['HLTQCDStudy/' + trigger + '/' + variable for variable in variables for trigger in triggers]

hists = FileReader.getHistogramsFromFiles(histograms, files)
plotting.setStyle()
for variable in variables:
    hists = plotting.rebin(hists, rebins[variable], '*' + variable)
    hists = plotting.setXRange(hists, limits=limits[variable], histname='*' + variable)
    hists = plotting.setYTitle(hists, title=titles[variable],  histname='*' + variable)

labels = [
          'CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT',
              'CaloIdVT_CaloIsoVL_TrkIdVL_TrkIsoT',
              'CaloIdVL_CaloIsoT_TrkIdVL_TrkIsoT',
              'CaloIdVT_TrkIdT'
          ]

styles = [
          {'color': kBlack, 'fill': 1001},
          {'color': kRed, 'fill': 3004},
          {'color': kBlue, 'fill': 1001},
          {'color': kGreen + 3, 'fill': 1001},
          ]

electron_pT = []
electron_eta = []
electron_phi = []
maxfactor = 1.3
for variable in variables:
    compare = []
    app = compare.append
    for hist in histograms:
        if variable in hist:
            app(hists['SingleElectron'][hist])
    if 'Isolation' in variable:
        maxfactor = 2
    elif 'mvaTrigV0' in variable:
        maxfactor = 1.5
    else:
        maxfactor = 1.3
    canvas, legend = plotting.compareShapes(compare, labels, styles, maxfactor)
    legend.Draw()    
    plotting.saveAs(canvas, variable, outputFolder='/storage/results/plots/HLTQCDAnalysis/')
