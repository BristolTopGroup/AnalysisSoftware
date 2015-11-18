import glob
import subprocess

jsonFile = '/hdfs/TopQuarkGroup/run2/json/Cert_246908-258159_13TeV_PromptReco_Collisions15_25ns_JSON_v3.txt'
pileupJson = '/hdfs/TopQuarkGroup/run2/json/pileup/pileup_JSON_11-02-2015.txt'
minBiasXsec = 69000
uncertainty = 0.05#%

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
