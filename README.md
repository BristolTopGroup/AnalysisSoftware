AnalysisSoftware
================

Analysis software for TTbar+MET differential cross section analysis (TOP-12-042)

## General Recipe

To setup the code:

```
export SCRAM_ARCH=slc5_amd64_gcc462
cmsrel CMSSW_6_0_0
cd CMSSW_6_0_0/src
cmsenv

#get the code from the repository
git clone git@github.com:BristolTopGroup/AnalysisSoftware BristolAnalysis/Tools

#compile
scram b -j2

hash -r #or rehash in case that BAT cannot be found

#test run the code:
nohup BAT BristolAnalysis/Tools/python/test_cfg.py &> test.log &
```

## Notes
More information can be found at [Bristol Analysis Tools twiki page](https://twiki.cern.ch/twiki/bin/view/CMS/BristolAnalysisTools)

## Bugs
Please report any problems on our [issues page](https://github.com/BristolTopGroup/AnalysisSoftware/issues).
