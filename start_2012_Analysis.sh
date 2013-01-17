#!/bin/bash
#setup the machine you are using
machine="soolin"
#machine="stardust"
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
#sample="QCD_20to30_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_central.log &
#sample="QCD_20to30_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_JES_up.log &
#sample="QCD_20to30_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_JES_down.log &
#sample="QCD_20to30_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_PU_down.log &
#sample="QCD_20to30_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_PU_up.log &
#sample="QCD_20to30_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_BJet_down.log &
#sample="QCD_20to30_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_BJet_up.log &
#sample="QCD_20to30_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_LightJet_down.log &
#sample="QCD_20to30_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_LightJet_up.log &

#sample="QCD_30to80_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_central.log &
#sample="QCD_30to80_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_JES_up.log &
#sample="QCD_30to80_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_JES_down.log &
#sample="QCD_30to80_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_PU_down.log &
#sample="QCD_30to80_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_PU_up.log &
#sample="QCD_30to80_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_BJet_down.log &
#sample="QCD_30to80_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_BJet_up.log &
#sample="QCD_30to80_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_LightJet_down.log &
#sample="QCD_30to80_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_LightJet_up.log &

#sample="QCD_80to170_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_central.log &
#sample="QCD_80to170_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_JES_up.log &
#sample="QCD_80to170_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_JES_down.log &
#sample="QCD_80to170_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_PU_down.log &
#sample="QCD_80to170_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_PU_up.log &
#sample="QCD_80to170_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_BJet_down.log &
#sample="QCD_80to170_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_BJet_up.log &
#sample="QCD_80to170_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_LightJet_down.log &
#sample="QCD_80to170_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_LightJet_up.log &

#sample="QCD_170to250_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_BCtoE_central.log &
#sample="QCD_170to250_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_BCtoE_JES_up.log &
#sample="QCD_170to250_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_BCtoE_JES_down.log &
#sample="QCD_170to250_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_BCtoE_PU_down.log &
#sample="QCD_170to250_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_BCtoE_PU_up.log &
#sample="QCD_170to250_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_BCtoE_BJet_down.log &
#sample="QCD_170to250_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_BCtoE_BJet_up.log &
#sample="QCD_170to250_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_BCtoE_LightJet_down.log &
#sample="QCD_170to250_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_BCtoE_LightJet_up.log &

#sample="QCD_250to350_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_BCtoE_central.log &
#sample="QCD_250to350_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_BCtoE_JES_up.log &
#sample="QCD_250to350_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_BCtoE_JES_down.log &
#sample="QCD_250to350_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_BCtoE_PU_down.log &
#sample="QCD_250to350_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_BCtoE_PU_up.log &
#sample="QCD_250to350_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_BCtoE_BJet_down.log &
#sample="QCD_250to350_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_BCtoE_BJet_up.log &
#sample="QCD_250to350_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_BCtoE_LightJet_down.log &
#sample="QCD_250to350_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_BCtoE_LightJet_up.log &

#sample="QCD_350_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_BCtoE_central.log &
#sample="QCD_350_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_BCtoE_JES_up.log &
#sample="QCD_350_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_BCtoE_JES_down.log &
#sample="QCD_350_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_BCtoE_PU_down.log &
#sample="QCD_350_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_BCtoE_PU_up.log &
#sample="QCD_350_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_BCtoE_BJet_down.log &
#sample="QCD_350_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_BCtoE_BJet_up.log &
#sample="QCD_350_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_BCtoE_LightJet_down.log &
#sample="QCD_350_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_BCtoE_LightJet_up.log &

#sample="QCD_20to30_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_central.log &
#sample="QCD_20to30_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_JES_up.log &
#sample="QCD_20to30_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_JES_down.log &
#sample="QCD_20to30_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_PU_down.log &
#sample="QCD_20to30_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_PU_up.log &
#sample="QCD_20to30_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_BJet_down.log &
#sample="QCD_20to30_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_BJet_up.log &
#sample="QCD_20to30_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_LightJet_down.log &
#sample="QCD_20to30_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_LightJet_up.log &

#sample="QCD_30to80_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_central.log &
#sample="QCD_30to80_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_JES_up.log &
#sample="QCD_30to80_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_JES_down.log &
#sample="QCD_30to80_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_PU_down.log &
#sample="QCD_30to80_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_PU_up.log &
#sample="QCD_30to80_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_BJet_down.log &
#sample="QCD_30to80_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_BJet_up.log &
#sample="QCD_30to80_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_LightJet_down.log &
#sample="QCD_30to80_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_LightJet_up.log &

#sample="QCD_80to170_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_central.log &
#sample="QCD_80to170_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_JES_up.log &
#sample="QCD_80to170_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_JES_down.log &
#sample="QCD_80to170_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_PU_down.log &
#sample="QCD_80to170_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_PU_up.log &
#sample="QCD_80to170_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_BJet_down.log &
#sample="QCD_80to170_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_BJet_up.log &
#sample="QCD_80to170_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_LightJet_down.log &
#sample="QCD_80to170_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_LightJet_up.log &

#sample="QCD_170to250_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_central.log &
#sample="QCD_170to250_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_JES_up.log &
#sample="QCD_170to250_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_JES_down.log &
#sample="QCD_170to250_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_PU_down.log &
#sample="QCD_170to250_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_PU_up.log &
#sample="QCD_170to250_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_BJet_down.log &
#sample="QCD_170to250_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_BJet_up.log &
#sample="QCD_170to250_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_LightJet_down.log &
#sample="QCD_170to250_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_LightJet_up.log &

#sample="QCD_250to350_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_central.log &
#sample="QCD_250to350_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_JES_up.log &
#sample="QCD_250to350_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_JES_down.log &
#sample="QCD_250to350_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_PU_down.log &
#sample="QCD_250to350_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_PU_up.log &
#sample="QCD_250to350_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_BJet_down.log &
#sample="QCD_250to350_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_BJet_up.log &
#sample="QCD_250to350_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_LightJet_down.log &
#sample="QCD_250to350_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_LightJet_up.log &

#sample="QCD_350_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_central.log &
#sample="QCD_350_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_JES_up.log &
#sample="QCD_350_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_JES_down.log &
#sample="QCD_350_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_PU_down.log &
#sample="QCD_350_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_PU_up.log &
#sample="QCD_350_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_BJet_down.log &
#sample="QCD_350_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_BJet_up.log &
#sample="QCD_350_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_LightJet_down.log &
#sample="QCD_350_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_LightJet_up.log &

#sample="GJets_200To400" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_200To400_central.log &
#sample="GJets_200To400" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_200To400_JES_up.log &
#sample="GJets_200To400" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_200To400_JES_down.log &
#sample="GJets_200To400" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_200To400_PU_down.log &
#sample="GJets_200To400" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_200To400_PU_up.log &
#sample="GJets_200To400" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_200To400_BJet_down.log &
#sample="GJets_200To400" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_200To400_BJet_up.log &
#sample="GJets_200To400" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_200To400_LightJet_down.log &
#sample="GJets_200To400" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_200To400_LightJet_up.log &

#sample="GJets_400" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_400_central.log &
#sample="GJets_400" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_400_JES_up.log &
#sample="GJets_400" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_400_JES_down.log &
#sample="GJets_400" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_400_PU_down.log &
#sample="GJets_400" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_400_PU_up.log &
#sample="GJets_400" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_400_BJet_down.log &
#sample="GJets_400" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_400_BJet_up.log &
#sample="GJets_400" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_400_LightJet_down.log &
#sample="GJets_400" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> GJets_400_LightJet_up.log &

#sample="QCD_Pt-15to20_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-15to20_Mu_central.log &
#sample="QCD_Pt-15to20_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-15to20_Mu_JES_up.log &
#sample="QCD_Pt-15to20_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-15to20_Mu_JES_down.log &
#sample="QCD_Pt-15to20_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-15to20_Mu_PU_down.log &
#sample="QCD_Pt-15to20_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-15to20_Mu_PU_up.log &
#sample="QCD_Pt-15to20_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-15to20_Mu_BJet_down.log &
#sample="QCD_Pt-15to20_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-15to20_Mu_BJet_up.log &
#sample="QCD_Pt-15to20_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-15to20_Mu_LightJet_down.log &
#sample="QCD_Pt-15to20_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-15to20_Mu_LightJet_up.log &

#sample="QCD_Pt-20to30_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-20to30_Mu_central.log &
#sample="QCD_Pt-20to30_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-20to30_Mu_JES_up.log &
#sample="QCD_Pt-20to30_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-20to30_Mu_JES_down.log &
#sample="QCD_Pt-20to30_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-20to30_Mu_PU_down.log &
#sample="QCD_Pt-20to30_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-20to30_Mu_PU_up.log &
#sample="QCD_Pt-20to30_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-20to30_Mu_BJet_down.log &
#sample="QCD_Pt-20to30_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-20to30_Mu_BJet_up.log &
#sample="QCD_Pt-20to30_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-20to30_Mu_LightJet_down.log &
#sample="QCD_Pt-20to30_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-20to30_Mu_LightJet_up.log &

#sample="QCD_Pt-30to50_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-30to50_Mu_central.log &
#sample="QCD_Pt-30to50_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-30to50_Mu_JES_up.log &
#sample="QCD_Pt-30to50_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-30to50_Mu_JES_down.log &
#sample="QCD_Pt-30to50_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-30to50_Mu_PU_down.log &
#sample="QCD_Pt-30to50_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-30to50_Mu_PU_up.log &
#sample="QCD_Pt-30to50_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-30to50_Mu_BJet_down.log &
#sample="QCD_Pt-30to50_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-30to50_Mu_BJet_up.log &
#sample="QCD_Pt-30to50_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-30to50_Mu_LightJet_down.log &
#sample="QCD_Pt-30to50_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-30to50_Mu_LightJet_up.log &

#sample="QCD_Pt-50to80_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-50to80_Mu_central.log &
#sample="QCD_Pt-50to80_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-50to80_Mu_JES_up.log &
#sample="QCD_Pt-50to80_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-50to80_Mu_JES_down.log &
#sample="QCD_Pt-50to80_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-50to80_Mu_PU_down.log &
#sample="QCD_Pt-50to80_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-50to80_Mu_PU_up.log &
#sample="QCD_Pt-50to80_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-50to80_Mu_BJet_down.log &
#sample="QCD_Pt-50to80_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-50to80_Mu_BJet_up.log &
#sample="QCD_Pt-50to80_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-50to80_Mu_LightJet_down.log &
#sample="QCD_Pt-50to80_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-50to80_Mu_LightJet_up.log &

#sample="QCD_Pt-80to120_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-80to120_Mu_central.log &
#sample="QCD_Pt-80to120_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-80to120_Mu_JES_up.log &
#sample="QCD_Pt-80to120_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-80to120_Mu_JES_down.log &
#sample="QCD_Pt-80to120_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-80to120_Mu_PU_down.log &
#sample="QCD_Pt-80to120_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-80to120_Mu_PU_up.log &
#sample="QCD_Pt-80to120_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-80to120_Mu_BJet_down.log &
#sample="QCD_Pt-80to120_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-80to120_Mu_BJet_up.log &
#sample="QCD_Pt-80to120_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-80to120_Mu_LightJet_down.log &
#sample="QCD_Pt-80to120_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-80to120_Mu_LightJet_up.log &

#sample="QCD_Pt-120to170_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-120to170_Mu_central.log &
#sample="QCD_Pt-120to170_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-120to170_Mu_JES_up.log &
#sample="QCD_Pt-120to170_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-120to170_Mu_JES_down.log &
#sample="QCD_Pt-120to170_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-120to170_Mu_PU_down.log &
#sample="QCD_Pt-120to170_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-120to170_Mu_PU_up.log &
#sample="QCD_Pt-120to170_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-120to170_Mu_BJet_down.log &
#sample="QCD_Pt-120to170_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-120to170_Mu_BJet_up.log &
#sample="QCD_Pt-120to170_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-120to170_Mu_LightJet_down.log &
#sample="QCD_Pt-120to170_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-120to170_Mu_LightJet_up.log &

#sample="QCD_Pt-170to300_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-170to300_Mu_central.log &
#sample="QCD_Pt-170to300_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-170to300_Mu_JES_up.log &
#sample="QCD_Pt-170to300_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-170to300_Mu_JES_down.log &
#sample="QCD_Pt-170to300_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-170to300_Mu_PU_down.log &
#sample="QCD_Pt-170to300_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-170to300_Mu_PU_up.log &
#sample="QCD_Pt-170to300_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-170to300_Mu_BJet_down.log &
#sample="QCD_Pt-170to300_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-170to300_Mu_BJet_up.log &
#sample="QCD_Pt-170to300_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-170to300_Mu_LightJet_down.log &
#sample="QCD_Pt-170to300_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-170to300_Mu_LightJet_up.log &

#sample="QCD_Pt-300to470_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-300to470_Mu_central.log &
#sample="QCD_Pt-300to470_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-300to470_Mu_JES_up.log &
#sample="QCD_Pt-300to470_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-300to470_Mu_JES_down.log &
#sample="QCD_Pt-300to470_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-300to470_Mu_PU_down.log &
#sample="QCD_Pt-300to470_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-300to470_Mu_PU_up.log &
#sample="QCD_Pt-300to470_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-300to470_Mu_BJet_down.log &
#sample="QCD_Pt-300to470_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-300to470_Mu_BJet_up.log &
#sample="QCD_Pt-300to470_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-300to470_Mu_LightJet_down.log &
#sample="QCD_Pt-300to470_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-300to470_Mu_LightJet_up.log &

#sample="QCD_Pt-470to600_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-470to600_Mu_central.log &
#sample="QCD_Pt-470to600_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-470to600_Mu_JES_up.log &
#sample="QCD_Pt-470to600_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-470to600_Mu_JES_down.log &
#sample="QCD_Pt-470to600_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-470to600_Mu_PU_down.log &
#sample="QCD_Pt-470to600_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-470to600_Mu_PU_up.log &
#sample="QCD_Pt-470to600_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-470to600_Mu_BJet_down.log &
#sample="QCD_Pt-470to600_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-470to600_Mu_BJet_up.log &
#sample="QCD_Pt-470to600_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-470to600_Mu_LightJet_down.log &
#sample="QCD_Pt-470to600_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-470to600_Mu_LightJet_up.log &

#sample="QCD_Pt-600to800_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-600to800_Mu_central.log &
#sample="QCD_Pt-600to800_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-600to800_Mu_JES_up.log &
#sample="QCD_Pt-600to800_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-600to800_Mu_JES_down.log &
#sample="QCD_Pt-600to800_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-600to800_Mu_PU_down.log &
#sample="QCD_Pt-600to800_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-600to800_Mu_PU_up.log &
#sample="QCD_Pt-600to800_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-600to800_Mu_BJet_down.log &
#sample="QCD_Pt-600to800_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-600to800_Mu_BJet_up.log &
#sample="QCD_Pt-600to800_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-600to800_Mu_LightJet_down.log &
#sample="QCD_Pt-600to800_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-600to800_Mu_LightJet_up.log &

#sample="QCD_Pt-800to1000_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-800to1000_Mu_central.log &
#sample="QCD_Pt-800to1000_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-800to1000_Mu_JES_up.log &
#sample="QCD_Pt-800to1000_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-800to1000_Mu_JES_down.log &
#sample="QCD_Pt-800to1000_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-800to1000_Mu_PU_down.log &
#sample="QCD_Pt-800to1000_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-800to1000_Mu_PU_up.log &
#sample="QCD_Pt-800to1000_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-800to1000_Mu_BJet_down.log &
#sample="QCD_Pt-800to1000_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-800to1000_Mu_BJet_up.log &
#sample="QCD_Pt-800to1000_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-800to1000_Mu_LightJet_down.log &
#sample="QCD_Pt-800to1000_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-800to1000_Mu_LightJet_up.log &

#sample="QCD_Pt-1000_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-1000_Mu_central.log &
#sample="QCD_Pt-1000_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-1000_Mu_JES_up.log &
#sample="QCD_Pt-1000_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-1000_Mu_JES_down.log &
#sample="QCD_Pt-1000_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-1000_Mu_PU_down.log &
#sample="QCD_Pt-1000_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-1000_Mu_PU_up.log &
#sample="QCD_Pt-1000_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-1000_Mu_BJet_down.log &
#sample="QCD_Pt-1000_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-1000_Mu_BJet_up.log &
#sample="QCD_Pt-1000_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-1000_Mu_LightJet_down.log &
#sample="QCD_Pt-1000_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> QCD_Pt-1000_Mu_LightJet_up.log &

#sample="W1Jet" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W1Jet_central.log &
#sample="W1Jet" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W1Jet_JES_up.log &
#sample="W1Jet" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W1Jet_JES_down.log &
#sample="W1Jet" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W1Jet_PU_down.log &
#sample="W1Jet" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W1Jet_PU_up.log &
#sample="W1Jet" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W1Jet_BJet_down.log &
#sample="W1Jet" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W1Jet_BJet_up.log &
#sample="W1Jet" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W1Jet_LightJet_down.log &
#sample="W1Jet" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W1Jet_LightJet_up.log &

#sample="W2Jets" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W2Jets_central.log &
#sample="W2Jets" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W2Jets_JES_up.log &
#sample="W2Jets" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W2Jets_JES_down.log &
#sample="W2Jets" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W2Jets_PU_down.log &
#sample="W2Jets" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W2Jets_PU_up.log &
#sample="W2Jets" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W2Jets_BJet_down.log &
#sample="W2Jets" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W2Jets_BJet_up.log &
#sample="W2Jets" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W2Jets_LightJet_down.log &
#sample="W2Jets" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W2Jets_LightJet_up.log &

#sample="W3Jets" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W3Jets_central.log &
#sample="W3Jets" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W3Jets_JES_up.log &
#sample="W3Jets" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W3Jets_JES_down.log &
#sample="W3Jets" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W3Jets_PU_down.log &
#sample="W3Jets" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W3Jets_PU_up.log &
#sample="W3Jets" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W3Jets_BJet_down.log &
#sample="W3Jets" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W3Jets_BJet_up.log &
#sample="W3Jets" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W3Jets_LightJet_down.log &
#sample="W3Jets" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W3Jets_LightJet_up.log &

#sample="W4Jets" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W4Jets_central.log &
#sample="W4Jets" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W4Jets_JES_up.log &
#sample="W4Jets" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W4Jets_JES_down.log &
#sample="W4Jets" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W4Jets_PU_down.log &
#sample="W4Jets" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W4Jets_PU_up.log &
#sample="W4Jets" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W4Jets_BJet_down.log &
#sample="W4Jets" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W4Jets_BJet_up.log &
#sample="W4Jets" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W4Jets_LightJet_down.log &
#sample="W4Jets" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> W4Jets_LightJet_up.log &

#sample="DY1JetsToLL" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY1JetsToLL_central.log &
#sample="DY1JetsToLL" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY1JetsToLL_JES_up.log &
#sample="DY1JetsToLL" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY1JetsToLL_JES_down.log &
#sample="DY1JetsToLL" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY1JetsToLL_PU_down.log &
#sample="DY1JetsToLL" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY1JetsToLL_PU_up.log &
#sample="DY1JetsToLL" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY1JetsToLL_BJet_down.log &
#sample="DY1JetsToLL" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY1JetsToLL_BJet_up.log &
#sample="DY1JetsToLL" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY1JetsToLL_LightJet_down.log &
#sample="DY1JetsToLL" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY1JetsToLL_LightJet_up.log &

#sample="DY2JetsToLL" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY2JetsToLL_central.log &
#sample="DY2JetsToLL" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY2JetsToLL_JES_up.log &
#sample="DY2JetsToLL" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY2JetsToLL_JES_down.log &
#sample="DY2JetsToLL" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY2JetsToLL_PU_down.log &
#sample="DY2JetsToLL" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY2JetsToLL_PU_up.log &
#sample="DY2JetsToLL" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY2JetsToLL_BJet_down.log &
#sample="DY2JetsToLL" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY2JetsToLL_BJet_up.log &
#sample="DY2JetsToLL" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY2JetsToLL_LightJet_down.log &
#sample="DY2JetsToLL" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY2JetsToLL_LightJet_up.log &

#sample="DY3JetsToLL" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY3JetsToLL_central.log &
#sample="DY3JetsToLL" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY3JetsToLL_JES_up.log &
#sample="DY3JetsToLL" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY3JetsToLL_JES_down.log &
#sample="DY3JetsToLL" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY3JetsToLL_PU_down.log &
#sample="DY3JetsToLL" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY3JetsToLL_PU_up.log &
#sample="DY3JetsToLL" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY3JetsToLL_BJet_down.log &
#sample="DY3JetsToLL" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY3JetsToLL_BJet_up.log &
#sample="DY3JetsToLL" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY3JetsToLL_LightJet_down.log &
#sample="DY3JetsToLL" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY3JetsToLL_LightJet_up.log &

#sample="DY4JetsToLL" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY4JetsToLL_central.log &
#sample="DY4JetsToLL" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY4JetsToLL_JES_up.log &
#sample="DY4JetsToLL" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY4JetsToLL_JES_down.log &
#sample="DY4JetsToLL" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY4JetsToLL_PU_down.log &
#sample="DY4JetsToLL" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY4JetsToLL_PU_up.log &
#sample="DY4JetsToLL" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY4JetsToLL_BJet_down.log &
#sample="DY4JetsToLL" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY4JetsToLL_BJet_up.log &
#sample="DY4JetsToLL" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY4JetsToLL_LightJet_down.log &
#sample="DY4JetsToLL" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> DY4JetsToLL_LightJet_up.log &

#sample="T_tW" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_tW_central.log &
#sample="T_tW" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_tW_JES_up.log &
#sample="T_tW" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_tW_JES_down.log &
#sample="T_tW" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_tW_PU_down.log &
#sample="T_tW" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_tW_PU_up.log &
#sample="T_tW" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_tW_BJet_down.log &
#sample="T_tW" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_tW_BJet_up.log &
#sample="T_tW" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_tW_LightJet_down.log &
#sample="T_tW" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_tW_LightJet_up.log &

#sample="T_t" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_t_central.log &
#sample="T_t" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_t_JES_up.log &
#sample="T_t" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_t_JES_down.log &
#sample="T_t" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_t_PU_down.log &
#sample="T_t" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_t_PU_up.log &
#sample="T_t" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_t_BJet_down.log &
#sample="T_t" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_t_BJet_up.log &
#sample="T_t" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_t_LightJet_down.log &
#sample="T_t" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_t_LightJet_up.log &

#sample="T_s" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_s_central.log &
#sample="T_s" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_s_JES_up.log &
#sample="T_s" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_s_JES_down.log &
#sample="T_s" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_s_PU_down.log &
#sample="T_s" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_s_PU_up.log &
#sample="T_s" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_s_BJet_down.log &
#sample="T_s" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_s_BJet_up.log &
#sample="T_s" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_s_LightJet_down.log &
#sample="T_s" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> T_s_LightJet_up.log &

#sample="Tbar_tW" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_tW_central.log &
#sample="Tbar_tW" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_tW_JES_up.log &
#sample="Tbar_tW" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_tW_JES_down.log &
#sample="Tbar_tW" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_tW_PU_down.log &
#sample="Tbar_tW" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_tW_PU_up.log &
#sample="Tbar_tW" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_tW_BJet_down.log &
#sample="Tbar_tW" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_tW_BJet_up.log &
#sample="Tbar_tW" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_tW_LightJet_down.log &
#sample="Tbar_tW" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_tW_LightJet_up.log &

#sample="Tbar_t" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_t_central.log &
#sample="Tbar_t" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_t_JES_up.log &
#sample="Tbar_t" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_t_JES_down.log &
#sample="Tbar_t" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_t_PU_down.log &
#sample="Tbar_t" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_t_PU_up.log &
#sample="Tbar_t" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_t_BJet_down.log &
#sample="Tbar_t" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_t_BJet_up.log &
#sample="Tbar_t" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_t_LightJet_down.log &
#sample="Tbar_t" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_t_LightJet_up.log &

#sample="Tbar_s" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_s_central.log &
#sample="Tbar_s" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_s_JES_up.log &
#sample="Tbar_s" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_s_JES_down.log &
#sample="Tbar_s" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_s_PU_down.log &
#sample="Tbar_s" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_s_PU_up.log &
#sample="Tbar_s" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_s_BJet_down.log &
#sample="Tbar_s" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_s_BJet_up.log &
#sample="Tbar_s" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_s_LightJet_down.log &
#sample="Tbar_s" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> Tbar_s_LightJet_up.log &

#sample="TTJet" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_central.log &
#sample="TTJet" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_JES_up.log &
#sample="TTJet" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_JES_down.log &
#sample="TTJet" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_PU_down.log &
#sample="TTJet" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_PU_up.log &
#sample="TTJet" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_BJet_down.log &
#sample="TTJet" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_BJet_up.log &
#sample="TTJet" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_LightJet_down.log &
#sample="TTJet" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_LightJet_up.log &

#sample="SingleElectron" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> SingleElectron_central.log &
#sample="SingleElectron" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> SingleElectron_JES_up.log &
#sample="SingleElectron" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> SingleElectron_JES_down.log &

#sample="SingleMu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> SingleMu_central.log &
#sample="SingleMu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> SingleMu_JES_up.log &
#sample="SingleMu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> SingleMu_JES_down.log &

#PDFs with odd numbers
#for i in $(seq 22)
#do
#if [ $(($i % 2)) -eq 1 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}"
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done
#for i in $(seq 22 44)
#do
#if [ $(($i % 2)) -eq 1 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}"
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done

#PDFs with even numbers
#for i in $(seq 22)
#do
#if [ $(($i % 2)) -eq 0 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}"
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done

#PDFs with even numbers
#for i in $(seq 23 44)
#do
#if [ $(($i % 2)) -eq 0 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}"
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done

#other generators:
#sample="TTJets-mcatnlo" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJets-mcatnlo_central.log &
#sample="TTJets-powheg" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJets-powheg_central.log &

#systematics
#sample="TTJets-matchingdown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJets-matchingdown_central.log &
#sample="TTJets-matchingup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJets-matchingup_central.log &
#sample="TTJets-scaledown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJets-scaledown_central.log &
#sample="TTJets-scaleup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJets-scaleup_central.log &

#sample="WJets-matchingdown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> WJets-matchingdown_central.log &
#sample="WJets-matchingup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> WJets-matchingup_central.log &
#sample="WJets-scaledown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> WJets-scaledown_central.log &
#sample="WJets-scaleup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> WJets-scaleup_central.log &

#sample="ZJets-matchingdown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> ZJets-matchingdown_central.log &
#sample="ZJets-matchingup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> ZJets-matchingup_central.log &
#sample="ZJets-scaledown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> ZJets-scaledown_central.log &
#sample="ZJets-scaleup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> ZJets-scaleup_central.log &

#systematics
#sample="TTJets-matchingdown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJets-matchingdown_PU_down.log &
#sample="TTJets-matchingup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJets-matchingup_PU_down.log &
#sample="TTJets-scaledown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJets-scaledown_PU_down.log &
#sample="TTJets-scaleup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> TTJets-scaleup_PU_down.log &

#sample="WJets-matchingdown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> WJets-matchingdown_PU_down.log &
#sample="WJets-matchingup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> WJets-matchingup_PU_down.log &
#sample="WJets-scaledown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> WJets-scaledown_PU_down.log &
#sample="WJets-scaleup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> WJets-scaleup_PU_down.log &

#sample="ZJets-matchingdown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> ZJets-matchingdown_PU_down.log &
#sample="ZJets-matchingup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> ZJets-matchingup_PU_down.log &
#sample="ZJets-scaledown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> ZJets-scaledown_PU_down.log &
#sample="ZJets-scaleup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2012_cfg.py ${TQAFPath} &> ZJets-scaleup_PU_down.log &
