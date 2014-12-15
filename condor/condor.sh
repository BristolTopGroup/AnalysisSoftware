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
tar -cf Analysis.tar BristolAnalysis/Tools
memory_per_job=2000
total_memory=$memory_per_job
cores=BristolAnalysis/Tools/condor/job_mapper "$@" --return_cores
if [ $cores -gt 1 ]; then
	total_memory=$memory_per_job*$cores
fi
# inject all parameters into job.description arguments
condor_submit BristolAnalysis/Tools/condor/job.description
