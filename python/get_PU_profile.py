'''
    Script to generate the PU distribution from reco vertices for the Spring15
    MC (array) as well as the data file. This method is not the official
    prescription and should be only used when official prescription is not
    available.
'''
from __future__ import print_function
from rootpy.tree import TreeChain
from rootpy.io import File
from rootpy.plotting import Hist
import glob

base = '/hdfs/TopQuarkGroup/run2/atOutput/13TeV/50ns/'
mc_samples = ['SingleTop_tree.root', 'TTJets_PowhegPythia8_tree.root', 'VJets_tree.root', 'QCD_Muon_tree.root', 'QCD_Electron_tree.root']
mc_files = glob.glob(base + 'TTJets_PowhegPythia8_tree.root')
data_files_el = glob.glob(base + 'data_electron_tree.root')
data_files_mu = glob.glob(base + 'data_muon_tree.root')


pu_el_mc_reco = Hist(60, 0.5, 60.5, name='pu_el_mc_reco')
pu_mu_mc_reco = Hist(60, 0.5, 60.5, name='pu_mu_mc_reco')
pu_el_reco = Hist(60, 0.5, 60.5, name='pu_el_reco')
pu_mu_reco = Hist(60, 0.5, 60.5, name='pu_mu_reco')


el_mc_chain = TreeChain("TTbar_plus_X_analysis/EPlusJets/Ref selection/Pileup/Pileup", mc_files)
el_mc_chain.Draw('NVertices', 'EventWeight * LeptonEfficiencyCorrection', hist=pu_el_mc_reco)
mu_mc_chain = TreeChain("TTbar_plus_X_analysis/MuPlusJets/Ref selection/Pileup/Pileup", mc_files)
mu_mc_chain.Draw('NVertices', 'EventWeight * LeptonEfficiencyCorrection', hist=pu_mu_mc_reco)

el_chain = TreeChain("TTbar_plus_X_analysis/EPlusJets/Ref selection/Pileup/Pileup", data_files_el)
el_chain.Draw('NVertices', hist=pu_el_reco)
mu_chain = TreeChain("TTbar_plus_X_analysis/MuPlusJets/Ref selection/Pileup/Pileup", data_files_mu)
mu_chain.Draw('NVertices', hist=pu_mu_reco)

pu_mc_reco = pu_el_mc_reco.Clone('pu_mc_reco')
pu_mc_reco += pu_mu_mc_reco

pu_data_reco = pu_el_reco.Clone('pu_data_reco')
pu_data_reco += pu_mu_reco

hists = [pu_mc_reco, pu_data_reco]
norm_hists = {}

for hist in hists:
    name = hist.GetName() + '_norm'
    h = hist.Clone(name)
    h.Scale(1 / h.integral())
    norm_hists[name] = h
# rename so it is compatible to hardcoded name
pileup = pu_data_reco.Clone('pileup')
hists.append(pileup)
hists.extend(norm_hists.values())

h_weight = norm_hists['pu_data_reco_norm'] / norm_hists['pu_mc_reco_norm']
# print h_weight.GetMean()

with File('data/PileUp_2015_truth.root', 'recreate') as f:
    for h in hists:
        h.write()
    h_weight.write()
# now for the code
print('To be copied to interface/EventWeightProvider.h')
header = 'const boost::array<double, 60> Spring2015_50ns = { {\n'
footer = '} };'
body = ''
values_mc = list(norm_hists['pu_mc_reco_norm'].y())
for v in values_mc:
    body += '\t{0},\n'.format(v)
body = body.rstrip(',\n')
print(header + body + footer)
