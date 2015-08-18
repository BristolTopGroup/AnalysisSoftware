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

base = '/hdfs/TopQuarkGroup/run2/ntuples/v25/'
mc_files = glob.glob(base + 'TTJets_PowhegPythia8/*001.root')
data_files_el = glob.glob(base + 'SingleElectron_*/*.root')
data_files_mu = glob.glob(base + 'SingleMuon_*/*.root')


pu_mc_true = Hist(60, 0.5, 60.5, name='pu_mc_true')
pu_mc_reco = Hist(60, 0.5, 60.5, name='pu_mc_reco')
pu_el_reco = Hist(60, 0.5, 60.5, name='pu_el_reco')
pu_mu_reco = Hist(60, 0.5, 60.5, name='pu_mu_reco')

mc_chain = TreeChain("nTupleTree/tree", mc_files)
mc_chain.Draw('Event.PileUpInteractions', hist=pu_mc_true)
mc_chain.Draw('Event.NRecoVertices', hist=pu_mc_reco)

el_chain = TreeChain("nTupleTree/tree", data_files_el)
el_chain.Draw('Event.NRecoVertices', hist=pu_el_reco)
mu_chain = TreeChain("nTupleTree/tree", data_files_mu)
mu_chain.Draw('Event.NRecoVertices', hist=pu_mu_reco)

pu_data_reco = pu_el_reco.Clone('pu_data_reco')
pu_data_reco += pu_mu_reco

hists = [pu_mc_reco, pu_mc_true, pu_mu_reco, pu_el_reco, pu_data_reco]
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

with File('data/PileUp_2015_truth.root', 'recreate') as f:
    for h in hists:
        h.write()

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
