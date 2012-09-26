#!/bin/bash
#setup the machine you are using
#machine="soolin"
machine="stardust"
exe=BAT
toolsFolder=BristolAnalysis/Tools/
toolsFolder=BristolAnalysis/Tools/ 
if [ ${machine} == "stardust" ]
then
toolsFolder=/storage/Workspace/Analysis/AnalysisTools/
exe=${toolsFolder}Release/AnalysisTools
TQAFPath="--TQAFPath=../"
fi

export toolsFolder=${toolsFolder}
export exe=${exe}
export TQAFPath=${TQAFPath}

#other generators:
sample="TTJets-mcatnlo" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-mcatnlo_central.log &
sample="TTJets-pythia" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-pythia_central.log &
sample="TTJets-powheg" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-powheg_central.log &
