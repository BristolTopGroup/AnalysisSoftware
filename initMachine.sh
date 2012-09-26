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

