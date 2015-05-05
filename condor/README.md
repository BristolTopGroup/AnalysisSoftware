# Submitting analysis jobs with HTCondor

## Contents of this folder
 - condor.sh: main script
 - job_mapper: python job mapper script, creates a map for a given set of options (1 entry per condor job)
 - job.description.template: condor job description template, filled by condor.sh
 - job.sh: job script, executed by the condor job
 - README.md: this readme
 
## condor.sh
The condor.sh script is the main script to prepare and submit jobs to a HTCondor cluster.
*Requirements*: 
 - Analysis Software set up within CMSSW
 - to be executed from a condor scheduler
 - to be executed from the CMSSW src directory ($CMSSW_BASE/src).
 
To run just a test:
```shell
BristolAnalysis/Tools/condor/condor.sh --operation=test
```
to run one mode for a single sample
```shell
BristolAnalysis/Tools/condor/condor.sh --operation=single --mode=central --energy=7 --sample=TTJet --cores 1
```
to run all modes for a single sample
```shell
BristolAnalysis/Tools/condor/condor.sh --operation=single-sample-analysis --energy=7 --sample=TTJet --cores 1
```
to run the full analysis (many jobs!)
```shell
BristolAnalysis/Tools/condor/condor.sh --operation=analysis --energy=7 --cores 1
```