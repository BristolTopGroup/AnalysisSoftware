AnalysisSoftware
================

Analysis software for TTbar+X differential cross section analysis (TOP-12-042)

## General Recipe

To setup the standalone version of the code:

```
#get the code from the repository
git clone https://github.com/BristolTopGroup/AnalysisSoftware.git AnalysisSoftware
cd AnalysisSoftware

#create the makefile using cmake (install it if you don't have it: https://github.com/Kitware/CMake)
cmake CMakeLists.txt

#compile and build
make -j24

#adjust the toolsFolder environment variable
export toolsFolder=$PWD/

#test run the code:
nohup ./AnalysisSoftware python/test_cfg.py &> test.log &
```


To setup the code using CMSSW:

```
export SCRAM_ARCH=slc5_amd64_gcc462
cmsrel CMSSW_6_0_0
cd CMSSW_6_0_0/src
cmsenv

#get the code from the repository
git clone https://github.com/BristolTopGroup/AnalysisSoftware.git BristolAnalysis/Tools

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
