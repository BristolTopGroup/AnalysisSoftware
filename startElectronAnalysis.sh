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
sample="QCD_20to30_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_central.log &
sample="QCD_20to30_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_JES_up.log &
sample="QCD_20to30_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_JES_down.log &
sample="QCD_20to30_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_PU_down.log &
sample="QCD_20to30_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_PU_up.log &
sample="QCD_20to30_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_BJet_down.log &
sample="QCD_20to30_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_BJet_up.log &
sample="QCD_20to30_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_LightJet_down.log &
sample="QCD_20to30_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_BCtoE_LightJet_up.log &

sample="QCD_30to80_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_central.log &
sample="QCD_30to80_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_JES_up.log &
sample="QCD_30to80_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_JES_down.log &
sample="QCD_30to80_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_PU_down.log &
sample="QCD_30to80_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_PU_up.log &
sample="QCD_30to80_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_BJet_down.log &
sample="QCD_30to80_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_BJet_up.log &
sample="QCD_30to80_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_LightJet_down.log &
sample="QCD_30to80_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_BCtoE_LightJet_up.log &

sample="QCD_80to170_BCtoE" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_central.log &
sample="QCD_80to170_BCtoE" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_JES_up.log &
sample="QCD_80to170_BCtoE" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_JES_down.log &
sample="QCD_80to170_BCtoE" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_PU_down.log &
sample="QCD_80to170_BCtoE" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_PU_up.log &
sample="QCD_80to170_BCtoE" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_BJet_down.log &
sample="QCD_80to170_BCtoE" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_BJet_up.log &
sample="QCD_80to170_BCtoE" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_LightJet_down.log &
sample="QCD_80to170_BCtoE" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_BCtoE_LightJet_up.log &

sample="QCD_20to30_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_central.log &
sample="QCD_20to30_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_JES_up.log &
sample="QCD_20to30_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_JES_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_PU_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_PU_up.log &
sample="QCD_20to30_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_BJet_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_BJet_up.log &
sample="QCD_20to30_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_LightJet_down.log &
sample="QCD_20to30_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_20to30_EMEnriched_LightJet_up.log &

sample="QCD_30to80_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_central.log &
sample="QCD_30to80_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_JES_up.log &
sample="QCD_30to80_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_JES_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_PU_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_PU_up.log &
sample="QCD_30to80_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_BJet_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_BJet_up.log &
sample="QCD_30to80_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_LightJet_down.log &
sample="QCD_30to80_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_30to80_EMEnriched_LightJet_up.log &

sample="QCD_80to170_EMEnriched" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_central.log &
sample="QCD_80to170_EMEnriched" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_JES_up.log &
sample="QCD_80to170_EMEnriched" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_JES_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_PU_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_PU_up.log &
sample="QCD_80to170_EMEnriched" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_BJet_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_BJet_up.log &
sample="QCD_80to170_EMEnriched" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_LightJet_down.log &
sample="QCD_80to170_EMEnriched" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_80to170_EMEnriched_LightJet_up.log &

sample="GJets_40To100" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_40To100_central.log &
sample="GJets_40To100" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_40To100_JES_up.log &
sample="GJets_40To100" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_40To100_JES_down.log &
sample="GJets_40To100" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_40To100_PU_down.log &
sample="GJets_40To100" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_40To100_PU_up.log &
sample="GJets_40To100" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_40To100_BJet_down.log &
sample="GJets_40To100" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_40To100_BJet_up.log &
sample="GJets_40To100" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_40To100_LightJet_down.log &
sample="GJets_40To100" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_40To100_LightJet_up.log &

sample="GJets_100To200" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_100To200_central.log &
sample="GJets_100To200" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_100To200_JES_up.log &
sample="GJets_100To200" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_100To200_JES_down.log &
sample="GJets_100To200" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_100To200_PU_down.log &
sample="GJets_100To200" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_100To200_PU_up.log &
sample="GJets_100To200" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_100To200_BJet_down.log &
sample="GJets_100To200" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_100To200_BJet_up.log &
sample="GJets_100To200" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_100To200_LightJet_down.log &
sample="GJets_100To200" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_100To200_LightJet_up.log &

sample="GJets_200" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_200_central.log &
sample="GJets_200" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_200_JES_up.log &
sample="GJets_200" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_200_JES_down.log &
sample="GJets_200" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_200_PU_down.log &
sample="GJets_200" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_200_PU_up.log &
sample="GJets_200" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_200_BJet_down.log &
sample="GJets_200" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_200_BJet_up.log &
sample="GJets_200" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_200_LightJet_down.log &
sample="GJets_200" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> GJets_200_LightJet_up.log &

sample="QCD_Mu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_Mu_central.log &
sample="QCD_Mu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_Mu_JES_up.log &
sample="QCD_Mu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_Mu_JES_down.log &
sample="QCD_Mu" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_Mu_PU_down.log &
sample="QCD_Mu" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_Mu_PU_up.log &
sample="QCD_Mu" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_Mu_BJet_down.log &
sample="QCD_Mu" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_Mu_BJet_up.log &
sample="QCD_Mu" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_Mu_LightJet_down.log &
sample="QCD_Mu" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> QCD_Mu_LightJet_up.log &

sample="W1Jet" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W1Jet_central.log &
sample="W1Jet" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W1Jet_JES_up.log &
sample="W1Jet" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W1Jet_JES_down.log &
sample="W1Jet" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W1Jet_PU_down.log &
sample="W1Jet" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W1Jet_PU_up.log &
sample="W1Jet" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W1Jet_BJet_down.log &
sample="W1Jet" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W1Jet_BJet_up.log &
sample="W1Jet" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W1Jet_LightJet_down.log &
sample="W1Jet" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W1Jet_LightJet_up.log &

sample="W2Jets" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W2Jets_central.log &
sample="W2Jets" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W2Jets_JES_up.log &
sample="W2Jets" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W2Jets_JES_down.log &
sample="W2Jets" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W2Jets_PU_down.log &
sample="W2Jets" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W2Jets_PU_up.log &
sample="W2Jets" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W2Jets_BJet_down.log &
sample="W2Jets" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W2Jets_BJet_up.log &
sample="W2Jets" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W2Jets_LightJet_down.log &
sample="W2Jets" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W2Jets_LightJet_up.log &

sample="W3Jets" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W3Jets_central.log &
sample="W3Jets" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W3Jets_JES_up.log &
sample="W3Jets" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W3Jets_JES_down.log &
sample="W3Jets" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W3Jets_PU_down.log &
sample="W3Jets" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W3Jets_PU_up.log &
sample="W3Jets" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W3Jets_BJet_down.log &
sample="W3Jets" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W3Jets_BJet_up.log &
sample="W3Jets" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W3Jets_LightJet_down.log &
sample="W3Jets" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W3Jets_LightJet_up.log &

sample="W4Jets" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W4Jets_central.log &
sample="W4Jets" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W4Jets_JES_up.log &
sample="W4Jets" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W4Jets_JES_down.log &
sample="W4Jets" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W4Jets_PU_down.log &
sample="W4Jets" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W4Jets_PU_up.log &
sample="W4Jets" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W4Jets_BJet_down.log &
sample="W4Jets" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W4Jets_BJet_up.log &
sample="W4Jets" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W4Jets_LightJet_down.log &
sample="W4Jets" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> W4Jets_LightJet_up.log &

sample="DYJetsToLL" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> DYJetsToLL_central.log &
sample="DYJetsToLL" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> DYJetsToLL_JES_up.log &
sample="DYJetsToLL" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> DYJetsToLL_JES_down.log &
sample="DYJetsToLL" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> DYJetsToLL_PU_down.log &
sample="DYJetsToLL" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> DYJetsToLL_PU_up.log &
sample="DYJetsToLL" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> DYJetsToLL_BJet_down.log &
sample="DYJetsToLL" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> DYJetsToLL_BJet_up.log &
sample="DYJetsToLL" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> DYJetsToLL_LightJet_down.log &
sample="DYJetsToLL" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> DYJetsToLL_LightJet_up.log &

sample="TTbarZIncl" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarZIncl_central.log &
sample="TTbarZIncl" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarZIncl_JES_up.log &
sample="TTbarZIncl" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarZIncl_JES_down.log &
sample="TTbarZIncl" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarZIncl_PU_down.log &
sample="TTbarZIncl" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarZIncl_PU_up.log &
sample="TTbarZIncl" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarZIncl_BJet_down.log &
sample="TTbarZIncl" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarZIncl_BJet_up.log &
sample="TTbarZIncl" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarZIncl_LightJet_down.log &
sample="TTbarZIncl" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarZIncl_LightJet_up.log &

sample="TTbarInclWIncl" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarInclWIncl_central.log &
sample="TTbarInclWIncl" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarInclWIncl_JES_up.log &
sample="TTbarInclWIncl" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarInclWIncl_JES_down.log &
sample="TTbarInclWIncl" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarInclWIncl_PU_down.log &
sample="TTbarInclWIncl" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarInclWIncl_PU_up.log &
sample="TTbarInclWIncl" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarInclWIncl_BJet_down.log &
sample="TTbarInclWIncl" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarInclWIncl_BJet_up.log &
sample="TTbarInclWIncl" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarInclWIncl_LightJet_down.log &
sample="TTbarInclWIncl" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTbarInclWIncl_LightJet_up.log &

sample="WW" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WW_central.log &
sample="WW" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WW_JES_up.log &
sample="WW" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WW_JES_down.log &
sample="WW" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WW_PU_down.log &
sample="WW" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WW_PU_up.log &
sample="WW" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WW_BJet_down.log &
sample="WW" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WW_BJet_up.log &
sample="WW" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WW_LightJet_down.log &
sample="WW" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WW_LightJet_up.log &

sample="WZ" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WZ_central.log &
sample="WZ" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WZ_JES_up.log &
sample="WZ" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WZ_JES_down.log &
sample="WZ" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WZ_PU_down.log &
sample="WZ" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WZ_PU_up.log &
sample="WZ" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WZ_BJet_down.log &
sample="WZ" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WZ_BJet_up.log &
sample="WZ" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WZ_LightJet_down.log &
sample="WZ" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WZ_LightJet_up.log &

sample="ZZ" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZZ_central.log &
sample="ZZ" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZZ_JES_up.log &
sample="ZZ" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZZ_JES_down.log &
sample="ZZ" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZZ_PU_down.log &
sample="ZZ" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZZ_PU_up.log &
sample="ZZ" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZZ_BJet_down.log &
sample="ZZ" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZZ_BJet_up.log &
sample="ZZ" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZZ_LightJet_down.log &
sample="ZZ" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZZ_LightJet_up.log &

sample="T_tW" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_tW_central.log &
sample="T_tW" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_tW_JES_up.log &
sample="T_tW" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_tW_JES_down.log &
sample="T_tW" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_tW_PU_down.log &
sample="T_tW" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_tW_PU_up.log &
sample="T_tW" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_tW_BJet_down.log &
sample="T_tW" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_tW_BJet_up.log &
sample="T_tW" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_tW_LightJet_down.log &
sample="T_tW" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_tW_LightJet_up.log &

sample="T_t" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_t_central.log &
sample="T_t" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_t_JES_up.log &
sample="T_t" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_t_JES_down.log &
sample="T_t" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_t_PU_down.log &
sample="T_t" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_t_PU_up.log &
sample="T_t" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_t_BJet_down.log &
sample="T_t" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_t_BJet_up.log &
sample="T_t" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_t_LightJet_down.log &
sample="T_t" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_t_LightJet_up.log &

sample="T_s" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_s_central.log &
sample="T_s" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_s_JES_up.log &
sample="T_s" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_s_JES_down.log &
sample="T_s" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_s_PU_down.log &
sample="T_s" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_s_PU_up.log &
sample="T_s" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_s_BJet_down.log &
sample="T_s" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_s_BJet_up.log &
sample="T_s" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_s_LightJet_down.log &
sample="T_s" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> T_s_LightJet_up.log &

sample="Tbar_tW" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_tW_central.log &
sample="Tbar_tW" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_tW_JES_up.log &
sample="Tbar_tW" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_tW_JES_down.log &
sample="Tbar_tW" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_tW_PU_down.log &
sample="Tbar_tW" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_tW_PU_up.log &
sample="Tbar_tW" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_tW_BJet_down.log &
sample="Tbar_tW" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_tW_BJet_up.log &
sample="Tbar_tW" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_tW_LightJet_down.log &
sample="Tbar_tW" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_tW_LightJet_up.log &

sample="Tbar_t" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_t_central.log &
sample="Tbar_t" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_t_JES_up.log &
sample="Tbar_t" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_t_JES_down.log &
sample="Tbar_t" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_t_PU_down.log &
sample="Tbar_t" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_t_PU_up.log &
sample="Tbar_t" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_t_BJet_down.log &
sample="Tbar_t" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_t_BJet_up.log &
sample="Tbar_t" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_t_LightJet_down.log &
sample="Tbar_t" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_t_LightJet_up.log &

sample="Tbar_s" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_s_central.log &
sample="Tbar_s" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_s_JES_up.log &
sample="Tbar_s" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_s_JES_down.log &
sample="Tbar_s" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_s_PU_down.log &
sample="Tbar_s" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_s_PU_up.log &
sample="Tbar_s" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_s_BJet_down.log &
sample="Tbar_s" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_s_BJet_up.log &
sample="Tbar_s" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_s_LightJet_down.log &
sample="Tbar_s" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> Tbar_s_LightJet_up.log &

sample="TTJet" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_central.log &
sample="TTJet" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_JES_up.log &
sample="TTJet" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_JES_down.log &
sample="TTJet" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_PU_down.log &
sample="TTJet" analysisMode="PU_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_PU_up.log &
sample="TTJet" analysisMode="BJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_BJet_down.log &
sample="TTJet" analysisMode="BJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_BJet_up.log &
sample="TTJet" analysisMode="LightJet_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_LightJet_down.log &
sample="TTJet" analysisMode="LightJet_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_LightJet_up.log &

sample="ElectronHad" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ElectronHad_central.log &
sample="ElectronHad" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ElectronHad_JES_up.log &
sample="ElectronHad" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ElectronHad_JES_down.log &

sample="SingleMu" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> SingleMu_central.log &
sample="SingleMu" analysisMode="JES_up" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> SingleMu_JES_up.log &
sample="SingleMu" analysisMode="JES_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> SingleMu_JES_down.log &

#PDFs with odd numbers
for i in $(seq 22)
do
if [ $(($i % 2)) -eq 1 ]
then
testi="PDFWeights_${i}"
echo "Starting ${testi}"
sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
fi
done
for i in $(seq 22 44)
do
if [ $(($i % 2)) -eq 1 ]
then
testi="PDFWeights_${i}"
echo "Starting ${testi}"
sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
fi
done

#PDFs with even numbers
for i in $(seq 22)
do
if [ $(($i % 2)) -eq 0 ]
then
testi="PDFWeights_${i}"
echo "Starting ${testi}"
sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
fi
done

#PDFs with even numbers
for i in $(seq 23 44)
do
if [ $(($i % 2)) -eq 0 ]
then
testi="PDFWeights_${i}"
echo "Starting ${testi}"
sample=TTJet analysisMode=${testi} nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJet_${testi}.log &
fi
done

#other generators:
sample="TTJets-mcatnlo" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-mcatnlo_central.log &
sample="TTJets-pythia" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-pythia_central.log &
sample="TTJets-powheg" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-powheg_central.log &

sample="TTJets-mcatnlo" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-mcatnlo_PU_down.log &
sample="TTJets-pythia" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-pythia_PU_down.log &
sample="TTJets-powheg" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-powheg_PU_down.log &

#systematics
sample="TTJets-matchingdown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-matchingdown_central.log &
sample="TTJets-matchingup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-matchingup_central.log &
sample="TTJets-scaledown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-scaledown_central.log &
sample="TTJets-scaleup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-scaleup_central.log &

sample="WJets-matchingdown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WJets-matchingdown_central.log &
sample="WJets-matchingup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WJets-matchingup_central.log &
sample="WJets-scaledown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WJets-scaledown_central.log &
sample="WJets-scaleup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WJets-scaleup_central.log &

sample="ZJets-matchingdown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZJets-matchingdown_central.log &
sample="ZJets-matchingup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZJets-matchingup_central.log &
sample="ZJets-scaledown" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZJets-scaledown_central.log &
sample="ZJets-scaleup" analysisMode="central" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZJets-scaleup_central.log &

#other generators:
sample="TTJets-mcatnlo" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-mcatnlo_PU_down.log &
sample="TTJets-pythia" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-pythia_PU_down.log &
sample="TTJets-powheg" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-powheg_PU_down.log &

#systematics
sample="TTJets-matchingdown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-matchingdown_PU_down.log &
sample="TTJets-matchingup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-matchingup_PU_down.log &
sample="TTJets-scaledown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-scaledown_PU_down.log &
sample="TTJets-scaleup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> TTJets-scaleup_PU_down.log &

sample="WJets-matchingdown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WJets-matchingdown_PU_down.log &
sample="WJets-matchingup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WJets-matchingup_PU_down.log &
sample="WJets-scaledown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WJets-scaledown_PU_down.log &
sample="WJets-scaleup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> WJets-scaleup_PU_down.log &

sample="ZJets-matchingdown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZJets-matchingdown_PU_down.log &
sample="ZJets-matchingup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZJets-matchingup_PU_down.log &
sample="ZJets-scaledown" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZJets-scaledown_PU_down.log &
sample="ZJets-scaleup" analysisMode="PU_down" nohup ${exe} ${toolsFolder}python/master_cfg.py ${TQAFPath} &> ZJets-scaleup_PU_down.log &
