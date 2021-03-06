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
scramv1 project CMSSW CMSSW_8_0_14
cd CMSSW_8_0_14/src/
eval `scramv1 runtime -sh`
# get analysis software
tar -xf ../../Analysis.tar
# scram b -j8 #needs to be steered by condor.sh
# otherwise we risk to be evicted
scram b
echo "I will run the following executable now:"
which $exe

#figure out what I want to run
n_cores=`BristolAnalysis/Tools/condor/job_mapper "$@"  --return_cores`
# if n_cores > 1 we need to do a for loop and overwrite 
# the process parameter
process=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_process`
# TODO: FIXME: create CMSSW area based on this variable
# the tricky part: job_mapper cannot be used before setting up CMSSW
cmssw_version=`BristolAnalysis/Tools/condor/job_mapper "$@"  --return_cmssw`
local_process=$process
range_for_loop=`expr "$n_cores" + "$process"`
while [ $local_process -lt $range_for_loop ] ; do
	# do something
	sample=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_sample --process $local_process`
	analysisMode=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_mode --process $local_process`
	energy=`BristolAnalysis/Tools/condor/job_mapper "$@"  --return_energy`
	operation=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_operation`
	splitTTJet=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_splitTTJet`
	ntupleToProcess=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_ntupleToProcess --process $local_process`

	echo "I will run sample=${sample} in mode=${analysisMode} for centre-of-mass energy of ${energy} TeV"
	echo "Analysing ntuple number "$ntupleToProcess

	python_config=master_2015_cfg.py
	if [ $operation == "test" ]; then
		python_config=test_cfg.py
	fi

	log_file=${sample}_${analysisMode}_${energy}TeV_${cmssw_version}.log
	if [ $ntupleToProcess -ne -1 ]; then
		log_file=${sample}_${analysisMode}_${ntupleToProcess}_${energy}TeV_${cmssw_version}.log
	fi

	sample="$sample" analysisMode="$analysisMode" ntupleToProcess=$ntupleToProcess ${exe} ${toolsFolder}python/${python_config} ${TQAFPath} &> $log_file &
	
	let local_process+=1
done
wait
echo "ls"
ls -trlh
echo "All done"
# copy outputs to initial job directory (everything else is ignored)
cp *.log ../../.
cp *.root ../../.
