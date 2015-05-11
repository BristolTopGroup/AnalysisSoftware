#!/bin/bash -e
# Usage:
# condor.sh --operation=test|single|analysis [--noop, --energy, --process, --sample, --mode, --cmssw]
# 
# mandatory:
# --operation=test|single|analysis
#     test: set of default parameters just for testing
#     single: run just one job based on the parameters (need to specify energy, mode, sample & cores)
#     analysis: run all jobs for a given centre-of-mass energy (need to specify energy & cores)
#     
# optional parameters:
# --cores: number of cores to be used per condor job
# --noop: no operation flag. Will create condor job template, but won't submit the jobs
# --energy=<centre-of-mass energy> 
# --process <condor process number> 
# --sample=<valid sample> (TTJet, etc) 
# --mode=<valid mode of operation> (central, JES_up, etc)
# --cmssw=53X|73X|74X
NOW=$(date +"%d-%m-%Y")
# remove Analysis.tar in case it exists. We want to ship the latest code!
if [ -f "Analysis.tar" ]; then
	rm -f Analysis.tar
fi
tar -cf Analysis.tar BristolAnalysis/Tools TopQuarkAnalysis/TopHitFit/ --exclude="Debug*" --exclude="Release*" --exclude="Test*" --exclude="build*" --exclude="CMakeFiles" --exclude=".git*"
memory_per_job=800
total_memory=$memory_per_job
echo "$@"
cores=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_cores`
if [ $cores -gt 1 ]; then
	total_memory=$memory_per_job*$cores
fi
operation=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_operation`
energy=`BristolAnalysis/Tools/condor/job_mapper "$@"  --return_energy`
cmssw_version=`BristolAnalysis/Tools/condor/job_mapper "$@"  --return_cmssw`
noop=`BristolAnalysis/Tools/condor/job_mapper "$@"  --return_noop`

job_description="job.description.$NOW"
cp BristolAnalysis/Tools/condor/job.description.template $job_description;
# inject all parameters into job.description arguments
sed -i "s/%total_memory%/${total_memory}/g" $job_description

if [ $operation == "test" ] || [ $operation == "single" ]; then
	echo "Running operation=$operation";
	
	mode=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_mode`
	sample=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_sample`
	
	set1="--operation=$operation --cores=$cores --mode=$mode --sample=$sample"
	set2="--sample=$sample --energy=$energy --cmssw=$cmssw_version"
	other_params="$set1 $set2"
	
	sed -i "s/%other_params%/${other_params}/g" $job_description
	sed -i "s/%n_jobs%/1/g" $job_description
	
fi
if [ $operation == "single-sample-analysis" ]; then
	echo "Running single sample analysis";
	
	sample=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_sample`
	
	set1="--operation=$operation --cores=$cores --sample=$sample"
	set2="--sample=$sample --energy=$energy --cmssw=$cmssw_version"
	other_params="$set1 $set2"
	n_jobs=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_n_jobs`
	
	sed -i "s/%other_params%/${other_params}/g" $job_description
	sed -i "s/%n_jobs%/${n_jobs}/g" $job_description
fi

if [ $operation == "analysis" ]; then
	echo "lets do analysis"
		
	set1="--operation=$operation --cores=$cores"
	set2="--energy=$energy --cmssw=$cmssw_version"
	other_params="$set1 $set2"
	n_jobs=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_n_jobs`
	
	sed -i "s/%other_params%/${other_params}/g" $job_description
	sed -i "s/%n_jobs%/${n_jobs}/g" $job_description
fi
if [ $noop == "0" ]; then
	echo "Submitting job(s) to condor"
	condor_submit $job_description
else
	echo "Nothing else to do"
fi
