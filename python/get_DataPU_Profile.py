import glob
import subprocess

# https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt
jsonFile = 'Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt'
# https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/PileUp/pileup_latest.txt
pileupJson = 'pileup_latest.txt'
minBiasXsec = 69200
uncertainty = 0.046#%

variations = {
'central' : minBiasXsec,
'up' : (1 + uncertainty) * minBiasXsec,
'down' : (1 - uncertainty) * minBiasXsec
}

for variation, xsec in variations.iteritems():
    print variation, xsec

    subprocess.call(['pileupCalc.py',
                    '-i',jsonFile,
                    '--inputLumiJSON', pileupJson,
                    '--calcMode', 'true',
                     '--minBiasXsec', '%s' % xsec,
                     '--maxPileupBin', '50',
                     '--numPileupBins', '50',
                     'BristolAnalysis/Tools/data/PileUp_2015_truth_%s.root' % variation
                     ])
