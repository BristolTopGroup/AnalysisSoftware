#!/bin/bash
# source CMSSW env
. $VO_CMS_SW_DIR/cmsset_default.sh
# get CMSSW
scramv1 project CMSSW CMSSW_6_2_12
cd CMSSW_6_2_12/src/
eval `scramv1 runtime -sh`
# get analysis software
tar -xf ../../Analysis.tar
# scram b -j8 #needs to be steered by condor.sh
# otherwise we risk to be evicted
scram b
which BAT

BAT BristolAnalysis/Tools/python/test_cfg.py &> test.log &
wait
# copy outputs to initial job directory (everything else is ignored)
cp *.log ../../.
cp *.root ../../.
