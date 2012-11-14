#!/bin/bash
#setup the machine you are using
machine="soolin"
exe=BAT
toolsFolder=BristolAnalysis/Tools/

#private machine
machine="stardust"
if [ ${machine} == "stardust" ]
then
toolsFolder=/storage/Workspace/Analysis/AnalysisSoftware/
exe=${toolsFolder}Release/AnalysisSoftware
TQAFPath="--TQAFPath=../"
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/storage/Workspace/Analysis/TopQuarkAnalysis/Release
fi

MAX_EVENTS=5000

export toolsFolder=${toolsFolder}
export exe=${exe}
export TQAFPath=${TQAFPath}

sample="ElectronHad" analysisMode="Test" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} --maxEvents=${MAX_EVENTS} &> test_data.log &
#sample="TTJet" analysisMode="Test" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} --maxEvents=${MAX_EVENTS} &> test_mc.log &
