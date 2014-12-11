#!/bin/bash
# set up ENV
exe=BAT
toolsFolder=BristolAnalysis/Tools/
VO_CMS_SW_DIR=/cvmfs/cms.cern.ch
host=`hostname`
# some information for debugging
echo "I am running on $host"
echo "I got the following parameters: $@"
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
echo "I will run the following executable now:"
which $exe

#figure out what I want to run
sample=BristolAnalysis/Tools/condor/job_mapper "$@" --return_sample
analysisMode=BristolAnalysis/Tools/condor/job_mapper "$@" --return_mode
echo "I will run sample=${sample} in mode=${analysisMode}"
${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> ${sample}_${analysisMode}.log &
wait
# copy outputs to initial job directory (everything else is ignored)
cp *.log ../../.
cp *.root ../../.
