#!/bin/bash
# TODO: 
# - get number of cores as parameter
# - get operation (single or analysis)
# parameters:
# - energy
# - operation = 'analysis|test|single'
# - cores
# if operation == single
# - mode (an analysis mode)
# - sample (which sample to analyse)
# or at least the workload
NOW=$(date +"%d-%m-%Y")
tar -cf Analysis.tar BristolAnalysis/Tools --exclude="Debug*" --exclude="Release*" --exclude="Test*" --exclude="build*" --exclude="CMakeFiles" --exclude=".git*"
memory_per_job=2000
total_memory=$memory_per_job
echo "$@"
cores=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_cores`
if [ $cores -gt 1 ]; then
	total_memory=$memory_per_job*$cores
fi
operation=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_operation`
energy=`BristolAnalysis/Tools/condor/job_mapper "$@"  --return_energy`
cmssw_version=`BristolAnalysis/Tools/condor/job_mapper "$@"  --return_cmssw`

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

if [ $operation == "analysis" ]; then
	echo "lets do analysis"
		
	set1="--operation=$operation --cores=$cores"
	set2="--energy=$energy --cmssw=$cmssw_version"
	other_params="$set1 $set2"
	n_jobs=`BristolAnalysis/Tools/condor/job_mapper "$@" --return_n_jobs`
	
	sed -i "s/%other_params%/${other_params}/g" $job_description
	sed -i "s/%n_jobs%/${n_jobs}/g" $job_description
fi
condor_submit $job_description
