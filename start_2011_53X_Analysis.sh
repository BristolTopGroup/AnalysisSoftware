#!/bin/bash
#setup the machine you are using
machine="soolin"
#machine="stardust"
exe=BAT
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
sample="QCD_20to30_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_central.log &
sample="QCD_20to30_BCtoE" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_Electron_up.log &
sample="QCD_20to30_BCtoE" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_Electron_down.log &
sample="QCD_20to30_BCtoE" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_Muon_up.log &
sample="QCD_20to30_BCtoE" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_Muon_down.log &
sample="QCD_20to30_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_JES_up.log &
sample="QCD_20to30_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_JES_down.log &
sample="QCD_20to30_BCtoE" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_JER_up.log &
wait
sample="QCD_20to30_BCtoE" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_JER_down.log &
sample="QCD_20to30_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_PU_down.log &
sample="QCD_20to30_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_PU_up.log &
sample="QCD_20to30_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_BJet_down.log &
sample="QCD_20to30_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_BJet_up.log &
sample="QCD_20to30_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_LightJet_down.log &
sample="QCD_20to30_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_LightJet_up.log &
wait

sample="QCD_30to80_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_central.log &
sample="QCD_30to80_BCtoE" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_Electron_up.log &
sample="QCD_30to80_BCtoE" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_Electron_down.log &
sample="QCD_30to80_BCtoE" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_Muon_up.log &
sample="QCD_30to80_BCtoE" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_Muon_down.log &
sample="QCD_30to80_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_JES_up.log &
sample="QCD_30to80_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_JES_down.log &
sample="QCD_30to80_BCtoE" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_JER_up.log &
wait
sample="QCD_30to80_BCtoE" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_JER_down.log &
sample="QCD_30to80_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_PU_down.log &
sample="QCD_30to80_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_PU_up.log &
sample="QCD_30to80_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_BJet_down.log &
sample="QCD_30to80_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_BJet_up.log &
sample="QCD_30to80_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_LightJet_down.log &
sample="QCD_30to80_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_LightJet_up.log &
wait

sample="QCD_80to170_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_central.log &
sample="QCD_80to170_BCtoE" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_Electron_up.log &
sample="QCD_80to170_BCtoE" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_Electron_down.log &
sample="QCD_80to170_BCtoE" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_Muon_up.log &
sample="QCD_80to170_BCtoE" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_Muon_down.log &
sample="QCD_80to170_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_JES_up.log &
sample="QCD_80to170_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_JES_down.log &
sample="QCD_80to170_BCtoE" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_JER_up.log &
wait
sample="QCD_80to170_BCtoE" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_JER_down.log &
sample="QCD_80to170_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_PU_down.log &
sample="QCD_80to170_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_PU_up.log &
sample="QCD_80to170_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_BJet_down.log &
sample="QCD_80to170_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_BJet_up.log &
sample="QCD_80to170_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_LightJet_down.log &
sample="QCD_80to170_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_LightJet_up.log &
wait

sample="QCD_20to30_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_central.log &
sample="QCD_20to30_EMEnriched" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_Electron_up.log &
sample="QCD_20to30_EMEnriched" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_Electron_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_Muon_up.log &
sample="QCD_20to30_EMEnriched" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_Muon_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_JES_up.log &
sample="QCD_20to30_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_JES_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_JER_up.log &
wait
sample="QCD_20to30_EMEnriched" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_JER_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_PU_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_PU_up.log &
sample="QCD_20to30_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_BJet_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_BJet_up.log &
sample="QCD_20to30_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_LightJet_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_LightJet_up.log &
wait

sample="QCD_30to80_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_central.log &
sample="QCD_30to80_EMEnriched" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_Electron_up.log &
sample="QCD_30to80_EMEnriched" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_Electron_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_Muon_up.log &
sample="QCD_30to80_EMEnriched" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_Muon_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_JES_up.log &
sample="QCD_30to80_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_JES_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_JER_up.log &
wait
sample="QCD_30to80_EMEnriched" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_JER_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_PU_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_PU_up.log &
sample="QCD_30to80_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_BJet_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_BJet_up.log &
sample="QCD_30to80_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_LightJet_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_LightJet_up.log &
wait

sample="QCD_80to170_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_central.log &
sample="QCD_80to170_EMEnriched" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_Electron_up.log &
sample="QCD_80to170_EMEnriched" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_Electron_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_Muon_up.log &
sample="QCD_80to170_EMEnriched" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_Muon_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_JES_up.log &
sample="QCD_80to170_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_JES_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_JER_up.log &
wait
sample="QCD_80to170_EMEnriched" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_JER_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_PU_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_PU_up.log &
sample="QCD_80to170_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_BJet_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_BJet_up.log &
sample="QCD_80to170_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_LightJet_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_LightJet_up.log &
wait

sample="QCD_170to250_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_central.log &
sample="QCD_170to250_EMEnriched" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_Electron_up.log &
sample="QCD_170to250_EMEnriched" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_Electron_down.log &
sample="QCD_170to250_EMEnriched" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_Muon_up.log &
sample="QCD_170to250_EMEnriched" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_Muon_down.log &
sample="QCD_170to250_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_JES_up.log &
sample="QCD_170to250_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_JES_down.log &
sample="QCD_170to250_EMEnriched" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_JER_up.log &
wait
sample="QCD_170to250_EMEnriched" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_JER_down.log &
sample="QCD_170to250_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_PU_down.log &
sample="QCD_170to250_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_PU_up.log &
sample="QCD_170to250_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_BJet_down.log &
sample="QCD_170to250_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_BJet_up.log &
sample="QCD_170to250_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_LightJet_down.log &
sample="QCD_170to250_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to250_EMEnriched_LightJet_up.log &
wait

sample="QCD_250to350_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_central.log &
sample="QCD_250to350_EMEnriched" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_Electron_up.log &
sample="QCD_250to350_EMEnriched" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_Electron_down.log &
sample="QCD_250to350_EMEnriched" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_Muon_up.log &
sample="QCD_250to350_EMEnriched" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_Muon_down.log &
sample="QCD_250to350_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_JES_up.log &
sample="QCD_250to350_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_JES_down.log &
sample="QCD_250to350_EMEnriched" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_JER_up.log &
wait
sample="QCD_250to350_EMEnriched" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_JER_down.log &
sample="QCD_250to350_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_PU_down.log &
sample="QCD_250to350_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_PU_up.log &
sample="QCD_250to350_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_BJet_down.log &
sample="QCD_250to350_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_BJet_up.log &
sample="QCD_250to350_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_LightJet_down.log &
sample="QCD_250to350_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_250to350_EMEnriched_LightJet_up.log &
wait

sample="QCD_350_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_central.log &
sample="QCD_350_EMEnriched" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_ELectron_up.log &
sample="QCD_350_EMEnriched" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_Electron_down.log &
sample="QCD_350_EMEnriched" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_Muon_up.log &
sample="QCD_350_EMEnriched" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_Muon_down.log &
sample="QCD_350_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_JES_up.log &
sample="QCD_350_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_JES_down.log &
sample="QCD_350_EMEnriched" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_JER_up.log &
wait
sample="QCD_350_EMEnriched" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_JER_down.log &
sample="QCD_350_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_PU_down.log &
sample="QCD_350_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_PU_up.log &
sample="QCD_350_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_BJet_down.log &
sample="QCD_350_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_BJet_up.log &
sample="QCD_350_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_LightJet_down.log &
sample="QCD_350_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_350_EMEnriched_LightJet_up.log &
wait

#sample="GJets_40To100" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_central.log &
#sample="GJets_40To100" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_Electron_up.log &
#sample="GJets_40To100" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_Electron_down.log &
#sample="GJets_40To100" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_Muon_up.log &
#sample="GJets_40To100" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_Muon_down.log &
#sample="GJets_40To100" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_JES_up.log &
#sample="GJets_40To100" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_JES_down.log &
#sample="GJets_40To100" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_JER_up.log &
#wait
#sample="GJets_40To100" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_JER_down.log &
#sample="GJets_40To100" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_PU_down.log &
#sample="GJets_40To100" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_BJet_down.log &
#sample="GJets_40To100" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_PU_up.log &
#sample="GJets_40To100" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_BJet_up.log &
#sample="GJets_40To100" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_LightJet_down.log &
#sample="GJets_40To100" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_40To100_LightJet_up.log &
#wait

sample="GJets_100To200" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_central.log &
sample="GJets_100To200" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_Electron_up.log &
sample="GJets_100To200" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_Electron_down.log &
sample="GJets_100To200" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_Muon_up.log &
sample="GJets_100To200" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_Muon_down.log &
sample="GJets_100To200" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_JES_up.log &
sample="GJets_100To200" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_JES_down.log &
sample="GJets_100To200" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_JER_up.log &
wait
sample="GJets_100To200" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_JER_down.log &
sample="GJets_100To200" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_PU_down.log &
sample="GJets_100To200" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_PU_up.log &
sample="GJets_100To200" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_BJet_down.log &
sample="GJets_100To200" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_BJet_up.log &
sample="GJets_100To200" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_LightJet_down.log &
sample="GJets_100To200" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_100To200_LightJet_up.log &
wait

#sample="GJets_200" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_central.log &
#sample="GJets_200" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_Electron_up.log &
#sample="GJets_200" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_Electron_down.log &
#sample="GJets_200" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_Muon_up.log &
#sample="GJets_200" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_Muon_down.log &
#sample="GJets_200" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_JES_up.log &
#sample="GJets_200" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_JES_down.log &
#sample="GJets_200" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_JER_up.log &
#wait
#sample="GJets_200" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_JER_down.log &
#sample="GJets_200" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_PU_down.log &
#sample="GJets_200" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_PU_up.log &
#sample="GJets_200" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_BJet_down.log &
#sample="GJets_200" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_BJet_up.log &
#sample="GJets_200" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_LightJet_down.log &
#sample="GJets_200" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> GJets_200_LightJet_up.log &
#wait

sample="QCD_15to20_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_central.log &
sample="QCD_15to20_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_Electron_up.log &
sample="QCD_15to20_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_Electron_down.log &
sample="QCD_15to20_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_Muon_up.log &
sample="QCD_15to20_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_Muon_down.log &
sample="QCD_15to20_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_JES_up.log &
sample="QCD_15to20_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_JES_down.log &
sample="QCD_15to20_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_JER_up.log &
wait
sample="QCD_15to20_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_JER_down.log &
sample="QCD_15to20_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_PU_down.log &
sample="QCD_15to20_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_PU_up.log &
sample="QCD_15to20_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_BJet_down.log &
sample="QCD_15to20_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_BJet_up.log &
sample="QCD_15to20_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_LightJet_down.log &
sample="QCD_15to20_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_15to20_Mu_LightJet_up.log &
wait

sample="QCD_20to30_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_central.log &
sample="QCD_20to30_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_Electron_up.log &
sample="QCD_20to30_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_Electron_down.log &
sample="QCD_20to30_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_Muon_up.log &
sample="QCD_20to30_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_Muon_down.log &
sample="QCD_20to30_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_JES_up.log &
sample="QCD_20to30_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_JES_down.log &
sample="QCD_20to30_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_JER_up.log &
wait
sample="QCD_20to30_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_JER_down.log &
sample="QCD_20to30_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_PU_down.log &
sample="QCD_20to30_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_PU_up.log &
sample="QCD_20to30_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_BJet_down.log &
sample="QCD_20to30_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_BJet_up.log &
sample="QCD_20to30_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_LightJet_down.log &
sample="QCD_20to30_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_20to30_Mu_LightJet_up.log &
wait

sample="QCD_30to50_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_central.log &
sample="QCD_30to50_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_Electron_up.log &
sample="QCD_30to50_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_Electron_down.log &
sample="QCD_30to50_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_Muon_up.log &
sample="QCD_30to50_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_Muon_down.log &
sample="QCD_30to50_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_JES_up.log &
sample="QCD_30to50_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_JES_down.log &
sample="QCD_30to50_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_JER_up.log &
wait
sample="QCD_30to50_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_JER_down.log &
sample="QCD_30to50_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_PU_down.log &
sample="QCD_30to50_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_PU_up.log &
sample="QCD_30to50_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_BJet_down.log &
sample="QCD_30to50_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_BJet_up.log &
sample="QCD_30to50_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_LightJet_down.log &
sample="QCD_30to50_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_30to50_Mu_LightJet_up.log &
wait

sample="QCD_50to80_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_central.log &
sample="QCD_50to80_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_Electron_up.log &
sample="QCD_50to80_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_Electron_down.log &
sample="QCD_50to80_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_Muon_up.log &
sample="QCD_50to80_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_Muon_down.log &
sample="QCD_50to80_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_JES_up.log &
sample="QCD_50to80_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_JES_down.log &
sample="QCD_50to80_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_JER_up.log &
wait
sample="QCD_50to80_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_JER_down.log &
sample="QCD_50to80_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_PU_down.log &
sample="QCD_50to80_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_PU_up.log &
sample="QCD_50to80_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_BJet_down.log &
sample="QCD_50to80_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_BJet_up.log &
sample="QCD_50to80_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_LightJet_down.log &
sample="QCD_50to80_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_50to80_Mu_LightJet_up.log &
wait

sample="QCD_80to120_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_central.log &
sample="QCD_80to120_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_Electron_up.log &
sample="QCD_80to120_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_Electron_down.log &
sample="QCD_80to120_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_Muon_up.log &
sample="QCD_80to120_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_Muon_down.log &
sample="QCD_80to120_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_JES_up.log &
sample="QCD_80to120_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_JES_down.log &
sample="QCD_80to120_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_JER_up.log &
wait
sample="QCD_80to120_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_JER_down.log &
sample="QCD_80to120_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_PU_down.log &
sample="QCD_80to120_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_PU_up.log &
sample="QCD_80to120_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_BJet_down.log &
sample="QCD_80to120_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_BJet_up.log &
sample="QCD_80to120_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_LightJet_down.log &
sample="QCD_80to120_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_80to120_Mu_LightJet_up.log &
wait

sample="QCD_120to170_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_central.log &
sample="QCD_120to170_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_Electron_up.log &
sample="QCD_120to170_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_Electron_down.log &
sample="QCD_120to170_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_Muon_up.log &
sample="QCD_120to170_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_Muon_down.log &
sample="QCD_120to170_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_JES_up.log &
sample="QCD_120to170_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_JES_down.log &
sample="QCD_120to170_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_JER_up.log &
wait
sample="QCD_120to170_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_JER_down.log &
sample="QCD_120to170_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_PU_down.log &
sample="QCD_120to170_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_PU_up.log &
sample="QCD_120to170_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_BJet_down.log &
sample="QCD_120to170_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_BJet_up.log &
sample="QCD_120to170_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_LightJet_down.log &
sample="QCD_120to170_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_120to170_Mu_LightJet_up.log &
wait

sample="QCD_170to300_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_central.log &
sample="QCD_170to300_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_Electron_up.log &
sample="QCD_170to300_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_Electron_down.log &
sample="QCD_170to300_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_Muon_up.log &
sample="QCD_170to300_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_Muon_down.log &
sample="QCD_170to300_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_JES_up.log &
sample="QCD_170to300_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_JES_down.log &
sample="QCD_170to300_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_JER_up.log &
wait
sample="QCD_170to300_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_JER_down.log &
sample="QCD_170to300_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_PU_down.log &
sample="QCD_170to300_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_PU_up.log &
sample="QCD_170to300_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_BJet_down.log &
sample="QCD_170to300_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_BJet_up.log &
sample="QCD_170to300_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_LightJet_down.log &
sample="QCD_170to300_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_170to300_Mu_LightJet_up.log &
wait

sample="QCD_300to470_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_central.log &
sample="QCD_300to470_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_Electron_up.log &
sample="QCD_300to470_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_Electron_down.log &
sample="QCD_300to470_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_Muon_up.log &
sample="QCD_300to470_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_Muon_down.log &
sample="QCD_300to470_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_JES_up.log &
sample="QCD_300to470_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_JES_down.log &
sample="QCD_300to470_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_JER_up.log &
wait
sample="QCD_300to470_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_JER_down.log &
sample="QCD_300to470_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_PU_down.log &
sample="QCD_300to470_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_PU_up.log &
sample="QCD_300to470_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_BJet_down.log &
sample="QCD_300to470_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_BJet_up.log &
sample="QCD_300to470_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_LightJet_down.log &
sample="QCD_300to470_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_300to470_Mu_LightJet_up.log &
wait

ample="QCD_470to600_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_central.log &
sample="QCD_470to600_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_Electron_up.log &
sample="QCD_470to600_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_Electron_down.log &
sample="QCD_470to600_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_Muon_up.log &
sample="QCD_470to600_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_Muon_down.log &
sample="QCD_470to600_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_JES_up.log &
sample="QCD_470to600_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_JES_down.log &
sample="QCD_470to600_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_JER_up.log &
wait
sample="QCD_470to600_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_JER_down.log &
sample="QCD_470to600_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_PU_down.log &
sample="QCD_470to600_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_PU_up.log &
sample="QCD_470to600_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_BJet_down.log &
sample="QCD_470to600_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_BJet_up.log &
sample="QCD_470to600_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_LightJet_down.log &
sample="QCD_470to600_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_470to600_Mu_LightJet_up.log &
wait

sample="QCD_600to800_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_central.log &
sample="QCD_600to800_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_Electron_up.log &
sample="QCD_600to800_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_Electron_down.log &
sample="QCD_600to800_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_Muon_up.log &
sample="QCD_600to800_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_Muon_down.log &
sample="QCD_600to800_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_JES_up.log &
sample="QCD_600to800_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_JES_down.log &
sample="QCD_600to800_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_JER_up.log &
wait
sample="QCD_600to800_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_JER_down.log &
sample="QCD_600to800_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_PU_down.log &
sample="QCD_600to800_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_PU_up.log &
sample="QCD_600to800_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_BJet_down.log &
sample="QCD_600to800_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_BJet_up.log &
sample="QCD_600to800_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_LightJet_down.log &
sample="QCD_600to800_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_600to800_Mu_LightJet_up.log &
wait

sample="QCD_800to1000_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_central.log &
sample="QCD_800to1000_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_Electron_up.log &
sample="QCD_800to1000_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_Electron_down.log &
sample="QCD_800to1000_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_Muon_up.log &
sample="QCD_800to1000_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_Muon_down.log &
sample="QCD_800to1000_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_JES_up.log &
sample="QCD_800to1000_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_JES_down.log &
sample="QCD_800to1000_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_JER_up.log &
wait
sample="QCD_800to1000_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_JER_down.log &
sample="QCD_800to1000_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_PU_down.log &
sample="QCD_800to1000_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_PU_up.log &
sample="QCD_800to1000_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_BJet_down.log &
sample="QCD_800to1000_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_BJet_up.log &
sample="QCD_800to1000_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_LightJet_down.log &
sample="QCD_800to1000_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_800to1000_Mu_LightJet_up.log &
wait

sample="QCD_1000_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_central.log &
sample="QCD_1000_Mu" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_Electron_up.log &
sample="QCD_1000_Mu" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_Electron_down.log &
sample="QCD_1000_Mu" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_Muon_up.log &
sample="QCD_1000_Mu" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_Muon_down.log &
sample="QCD_1000_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_JES_up.log &
sample="QCD_1000_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_JES_down.log &
sample="QCD_1000_Mu" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_JER_up.log &
wait
sample="QCD_1000_Mu" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_JER_down.log &
sample="QCD_1000_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_PU_down.log &
sample="QCD_1000_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_PU_up.log &
sample="QCD_1000_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_BJet_down.log &
sample="QCD_1000_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_BJet_up.log &
sample="QCD_1000_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_LightJet_down.log &
sample="QCD_1000_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> QCD_1000_Mu_LightJet_up.log &
wait

sample="WJets" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_central.log &
sample="WJets" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_Electron_up.log &
sample="WJets" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_Electron_down.log &
sample="WJets" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_Muon_up.log &
sample="WJets" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_Muon_down.log &
sample="WJets" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_JES_up.log &
sample="WJets" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_JES_down.log &
sample="WJets" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_JER_up.log &
wait
sample="WJets" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_JER_down.log &
sample="WJets" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_PU_down.log &
sample="WJets" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_PU_up.log &
sample="WJets" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_BJet_down.log &
sample="WJets" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_BJet_up.log &
sample="WJets" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_LightJet_down.log &
sample="WJets" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJet_LightJet_up.log &
wait

#sample="W1Jet" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_central.log &
#sample="W1Jet" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_Electron_up.log &
#sample="W1Jet" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_Electron_down.log &
#sample="W1Jet" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_Muon_up.log &
#sample="W1Jet" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_Muon_down.log &
#sample="W1Jet" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_JES_up.log &
#sample="W1Jet" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_JES_down.log &
#sample="W1Jet" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_JER_up.log &
#wait
#sample="W1Jet" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_JER_down.log &
#sample="W1Jet" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_PU_down.log &
#sample="W1Jet" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_PU_up.log &
#sample="W1Jet" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_BJet_down.log &
#sample="W1Jet" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_BJet_up.log &
#sample="W1Jet" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_LightJet_down.log &
#sample="W1Jet" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W1Jet_LightJet_up.log &
#wait

sample="W2Jets" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_central.log &
sample="W2Jets" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_Electron_up.log &
sample="W2Jets" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_Electron_down.log &
sample="W2Jets" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_Muon_up.log &
sample="W2Jets" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_Muon_down.log &
sample="W2Jets" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_JES_up.log &
sample="W2Jets" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_JES_down.log &
sample="W2Jets" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_JER_up.log &
wait
sample="W2Jets" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_JER_down.log &
sample="W2Jets" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_PU_down.log &
sample="W2Jets" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_PU_up.log &
sample="W2Jets" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_BJet_down.log &
sample="W2Jets" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_BJet_up.log &
sample="W2Jets" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_LightJet_down.log &
sample="W2Jets" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W2Jets_LightJet_up.log &
wait

#sample="W3Jets" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_central.log &
#sample="W3Jets" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_Electron_up.log &
#sample="W3Jets" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_Electron_down.log &
#sample="W3Jets" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_Muon_up.log &
#sample="W3Jets" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_Muon_down.log &
#sample="W3Jets" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_JES_up.log &
#sample="W3Jets" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_JES_down.log &
#sample="W3Jets" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_JER_up.log &
#wait
#sample="W3Jets" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_JER_down.log &
#sample="W3Jets" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_PU_down.log &
#sample="W3Jets" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_PU_up.log &
#sample="W3Jets" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_BJet_down.log &
#sample="W3Jets" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_BJet_up.log &
#sample="W3Jets" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_LightJet_down.log &
#sample="W3Jets" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W3Jets_LightJet_up.log &
#wait

sample="W4Jets" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_central.log &
sample="W4Jets" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_Electron_up.log &
sample="W4Jets" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_Electron_down.log &
sample="W4Jets" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_Muon_up.log &
sample="W4Jets" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_Muon_down.log &
sample="W4Jets" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_JES_up.log &
sample="W4Jets" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_JES_down.log &
sample="W4Jets" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_JER_up.log &
wait
sample="W4Jets" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_JER_down.log &
sample="W4Jets" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_PU_down.log &
sample="W4Jets" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_PU_up.log &
sample="W4Jets" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_BJet_down.log &
sample="W4Jets" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_BJet_up.log &
sample="W4Jets" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_LightJet_down.log &
sample="W4Jets" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> W4Jets_LightJet_up.log &
wait

sample="DYJetsToLL" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_central.log &
sample="DYJetsToLL" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_Electron_up.log &
sample="DYJetsToLL" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_Electron_down.log &
sample="DYJetsToLL" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_Muon_up.log &
sample="DYJetsToLL" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_Muon_down.log &
sample="DYJetsToLL" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_JES_up.log &
sample="DYJetsToLL" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_JES_down.log &
sample="DYJetsToLL" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_JER_up.log &
wait
sample="DYJetsToLL" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_JER_down.log &
sample="DYJetsToLL" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_PU_down.log &
sample="DYJetsToLL" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_PU_up.log &
sample="DYJetsToLL" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_BJet_down.log &
sample="DYJetsToLL" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_BJet_up.log &
sample="DYJetsToLL" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_LightJet_down.log &
sample="DYJetsToLL" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DYJetsToLL_LightJet_up.log &
wait

#sample="DY1JetsToLL" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_central.log &
#sample="DY1JetsToLL" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_Electron_up.log &
#sample="DY1JetsToLL" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_Electron_down.log &
#sample="DY1JetsToLL" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_Muon_up.log &
#sample="DY1JetsToLL" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_Muon_down.log &
#sample="DY1JetsToLL" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_JES_up.log &
#sample="DY1JetsToLL" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_JES_down.log &
#sample="DY1JetsToLL" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_JER_up.log &
#wait
#sample="DY1JetsToLL" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_JER_down.log &
#sample="DY1JetsToLL" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_PU_down.log &
#sample="DY1JetsToLL" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_PU_up.log &
#sample="DY1JetsToLL" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_BJet_down.log &
#sample="DY1JetsToLL" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_BJet_up.log &
#sample="DY1JetsToLL" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_LightJet_down.log &
#sample="DY1JetsToLL" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY1JetsToLL_LightJet_up.log &
#wait

#sample="DY2JetsToLL" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_central.log &
#sample="DY2JetsToLL" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_Electron_up.log &
#sample="DY2JetsToLL" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_Electron_down.log &
#sample="DY2JetsToLL" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_Muon_up.log &
#sample="DY2JetsToLL" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_Muon_down.log &
#sample="DY2JetsToLL" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_JES_up.log &
#sample="DY2JetsToLL" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_JES_down.log &
#sample="DY2JetsToLL" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_JER_up.log &
#wait
#sample="DY2JetsToLL" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_JER_down.log &
#sample="DY2JetsToLL" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_PU_down.log &
#sample="DY2JetsToLL" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_PU_up.log &
#sample="DY2JetsToLL" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_BJet_down.log &
#sample="DY2JetsToLL" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_BJet_up.log &
#sample="DY2JetsToLL" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_LightJet_down.log &
#sample="DY2JetsToLL" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY2JetsToLL_LightJet_up.log &
#wait

#sample="DY3JetsToLL" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_central.log &
#sample="DY3JetsToLL" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_Electron_up.log &
#sample="DY3JetsToLL" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_Electron_down.log &
#sample="DY3JetsToLL" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_Muon_up.log &
#sample="DY3JetsToLL" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_Muon_down.log &
#sample="DY3JetsToLL" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_JES_up.log &
#sample="DY3JetsToLL" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_JES_down.log &
#sample="DY3JetsToLL" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_JER_up.log &
#wait
#sample="DY3JetsToLL" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_JER_down.log &
#sample="DY3JetsToLL" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_PU_down.log &
#sample="DY3JetsToLL" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_PU_up.log &
#sample="DY3JetsToLL" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_BJet_down.log &
#sample="DY3JetsToLL" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_BJet_up.log &
#sample="DY3JetsToLL" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_LightJet_down.log &
#sample="DY3JetsToLL" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY3JetsToLL_LightJet_up.log &
#wait

#sample="DY4JetsToLL" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_central.log &
#sample="DY4JetsToLL" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_Electron_up.log &
#sample="DY4JetsToLL" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_Electron_down.log &
#sample="DY4JetsToLL" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_Muon_up.log &
#sample="DY4JetsToLL" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_Muon_down.log &
#sample="DY4JetsToLL" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_JES_up.log &
#sample="DY4JetsToLL" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_JES_down.log &
#sample="DY4JetsToLL" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_JER_up.log &
#wait
#sample="DY4JetsToLL" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_JER_down.log &
#sample="DY4JetsToLL" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_PU_down.log &
#sample="DY4JetsToLL" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_PU_up.log &
#sample="DY4JetsToLL" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_BJet_down.log &
#sample="DY4JetsToLL" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_BJet_up.log &
#sample="DY4JetsToLL" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_LightJet_down.log &
#sample="DY4JetsToLL" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> DY4JetsToLL_LightJet_up.log &
#wait

sample="T_tW" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_central.log &
sample="T_tW" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_Electron_up.log &
sample="T_tW" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_Electron_down.log &
sample="T_tW" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_Muon_up.log &
sample="T_tW" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_Muon_down.log &
sample="T_tW" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_JES_up.log &
sample="T_tW" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_JES_down.log &
sample="T_tW" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_JER_up.log &
wait
sample="T_tW" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_JER_down.log &
sample="T_tW" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_PU_down.log &
sample="T_tW" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_PU_up.log &
sample="T_tW" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_BJet_down.log &
sample="T_tW" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_BJet_up.log &
sample="T_tW" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_LightJet_down.log &
sample="T_tW" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_tW_LightJet_up.log &
wait

sample="T_t" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_central.log &
sample="T_t" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_Electron_up.log &
sample="T_t" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_Electron_down.log &
sample="T_t" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_Muon_up.log &
sample="T_t" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_Muon_down.log &
sample="T_t" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_JES_up.log &
sample="T_t" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_JES_down.log &
sample="T_t" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_JER_up.log &
wait
sample="T_t" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_JER_down.log &
sample="T_t" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_PU_down.log &
sample="T_t" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_PU_up.log &
sample="T_t" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_BJet_down.log &
sample="T_t" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_BJet_up.log &
sample="T_t" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_LightJet_down.log &
sample="T_t" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_t_LightJet_up.log &
wait

sample="T_s" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_central.log &
sample="T_s" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_Electron_up.log &
sample="T_s" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_Electron_down.log &
sample="T_s" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_Muon_up.log &
sample="T_s" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_Muon_down.log &
sample="T_s" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_JES_up.log &
sample="T_s" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_JES_down.log &
sample="T_s" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_JER_up.log &
wait
sample="T_s" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_JER_down.log &
sample="T_s" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_PU_down.log &
sample="T_s" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_PU_up.log &
sample="T_s" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_BJet_down.log &
sample="T_s" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_BJet_up.log &
sample="T_s" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_LightJet_down.log &
sample="T_s" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> T_s_LightJet_up.log &
wait

sample="Tbar_tW" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_central.log &
sample="Tbar_tW" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_Electron_up.log &
sample="Tbar_tW" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_Electron_down.log &
sample="Tbar_tW" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_Muon_up.log &
sample="Tbar_tW" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_Muon_down.log &
sample="Tbar_tW" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_JES_up.log &
sample="Tbar_tW" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_JES_down.log &
sample="Tbar_tW" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_JER_up.log &
wait
sample="Tbar_tW" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_JER_down.log &
sample="Tbar_tW" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_PU_down.log &
sample="Tbar_tW" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_PU_up.log &
sample="Tbar_tW" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_BJet_down.log &
sample="Tbar_tW" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_BJet_up.log &
sample="Tbar_tW" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_LightJet_down.log &
sample="Tbar_tW" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_tW_LightJet_up.log &
wait

sample="Tbar_t" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_central.log &
sample="Tbar_t" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_Electron_up.log &
sample="Tbar_t" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_Electron_down.log &
sample="Tbar_t" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_Muon_up.log &
sample="Tbar_t" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_Muon_down.log &
sample="Tbar_t" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_JES_up.log &
sample="Tbar_t" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_JES_down.log &
sample="Tbar_t" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_JER_up.log &
wait
sample="Tbar_t" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_JER_down.log &
sample="Tbar_t" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_PU_down.log &
sample="Tbar_t" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_PU_up.log &
sample="Tbar_t" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_BJet_down.log &
sample="Tbar_t" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_BJet_up.log &
sample="Tbar_t" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_LightJet_down.log &
sample="Tbar_t" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_t_LightJet_up.log &
wait

sample="Tbar_s" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_central.log &
sample="Tbar_s" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_Electron_up.log &
sample="Tbar_s" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_Electron_down.log &
sample="Tbar_s" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_Muon_up.log &
sample="Tbar_s" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_Muon_down.log &
sample="Tbar_s" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_JES_up.log &
sample="Tbar_s" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_JES_down.log &
sample="Tbar_s" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_JER_up.log &
wait
sample="Tbar_s" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_JER_down.log &
sample="Tbar_s" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_PU_down.log &
sample="Tbar_s" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_PU_up.log &
sample="Tbar_s" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_BJet_down.log &
sample="Tbar_s" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_BJet_up.log &
sample="Tbar_s" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_LightJet_down.log &
sample="Tbar_s" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> Tbar_s_LightJet_up.log &
wait

sample="TTJet" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_central.log &
sample="TTJet" analysisMode="Electron_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_Electron_up.log &
sample="TTJet" analysisMode="Electron_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_Electron_down.log &
sample="TTJet" analysisMode="Muon_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_Muon_up.log &
sample="TTJet" analysisMode="Muon_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_Muon_down.log &
sample="TTJet" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_JES_up.log &
sample="TTJet" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_JES_down.log &
sample="TTJet" analysisMode="JetSmearing_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_JER_up.log &
wait
sample="TTJet" analysisMode="JetSmearing_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_JER_down.log &
sample="TTJet" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_PU_down.log &
sample="TTJet" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_PU_up.log &
sample="TTJet" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_BJet_down.log &
sample="TTJet" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_BJet_up.log &
sample="TTJet" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_LightJet_down.log &
sample="TTJet" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_LightJet_up.log &
wait

#data
sample="ElectronHad" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> ElectronHad_central.log &
sample="ElectronHad" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> ElectronHad_JES_up.log &
sample="ElectronHad" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> ElectronHad_JES_down.log &

sample="SingleMu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> SingleMu_central.log &
sample="SingleMu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> SingleMu_JES_up.log &
sample="SingleMu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> SingleMu_JES_down.log &
wait

#systematics
#sample="TTJets-matchingdown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJets-matchingdown_central.log &
#sample="TTJets-matchingup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJets-matchingup_central.log &
sample="TTJets-scaledown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJets-scaledown_central.log &
sample="TTJets-scaleup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJets-scaleup_central.log &

#sample="WJets-matchingdown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJets-matchingdown_central.log &
#sample="WJets-matchingup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJets-matchingup_central.log &
#sample="WJets-scaledown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJets-scaledown_central.log &
#sample="WJets-scaleup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> WJets-scaleup_central.log &
wait

#sample="ZJets-matchingdown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> ZJets-matchingdown_central.log &
#sample="ZJets-matchingup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> ZJets-matchingup_central.log &
#sample="ZJets-scaledown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> ZJets-scaledown_central.log &
#sample="ZJets-scaleup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> ZJets-scaleup_central.log &

#other generators: (last because they take the longest to run)
#sample="TTJets-mcatnlo" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJets-mcatnlo_central.log &
#sample="TTJets-pythia" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJets-pythia_central.log &
#sample="TTJets-powheg" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJets-powheg_central.log &
#wait

#PDFs with odd numbers
#for i in $(seq 11)
#do
#if [ $(($i % 2)) -eq 1 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}"
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done
#wait

#for i in $(seq 12 22)
#do
#if [ $(($i % 2)) -eq 1 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}"
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done
#wait
#
#for i in $(seq 22 33)
#do
#if [ $(($i % 2)) -eq 1 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}"
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done
#wait
#
#for i in $(seq 34 44)
#do
#if [ $(($i % 2)) -eq 1 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}"
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done
#wait
#
#PDFs with even numbers
#for i in $(seq 11)
#do
#if [ $(($i % 2)) -eq 0 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}"
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done
#wait
#
#for i in $(seq 11 22)
#do
#if [ $(($i % 2)) -eq 0 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}" 
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done
#wait
#
#for i in $(seq 23 33)
#do   
#if [ $(($i % 2)) -eq 0 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}" 
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done
#wait
#
#for i in $(seq 33 44)
#do   
#if [ $(($i % 2)) -eq 0 ]
#then
#testi="PDFWeights_${i}"
#echo "Starting ${testi}"
#sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_2011_53X_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
#fi
#done
#wait
