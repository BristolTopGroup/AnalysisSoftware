#!/bin/bash
# TODO: get number of cores as parameter
# or at least the workload
tar -cf Analysis.tar BristolAnalysis/Tools
condor_submit BristolAnalysis/Tools/condor/job.description
