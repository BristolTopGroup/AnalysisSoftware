import glob
import subprocess

# https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt
# jsonFile = 'Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON.txt'
# https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/PileUp/pileup_latest.txt

# jsonFile = 'https://cms-service-dqm.web.cern.ch/cms-service-dqm/CAF/certification/Collisions16/13TeV/ReReco/Final/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
jsonFile = 'Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON.txt'
pileupJson = 'pileup_latest.txt'

minBiasXsec = 69200
uncertainty = 0.062#%

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
                     '--maxPileupBin', '75',
                     '--numPileupBins', '75',
                     '../data/PileUp_2015_truth_%s.root' % variation
                     ])
