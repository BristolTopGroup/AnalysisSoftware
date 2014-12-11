#!/bin/bash
# set up ENV
exe=BAT
toolsFolder=BristolAnalysis/Tools/
VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
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
which $exe

sample="QCD_80to170_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_central.log &
wait
# copy outputs to initial job directory (everything else is ignored)
cp *.log ../../.
cp *.root ../../.
