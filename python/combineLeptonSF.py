from ROOT import gROOT, TFile, TH1F

if __name__ == '__main__':
	muonSFFile = 'BristolAnalysis/Tools/data/SingleMuonTrigger_Z_RunBCD_prompt80X_7p65.root'

	runBEff = 'IsoMu22_OR_IsoTkMu22_PtEtaBins_Run273158_to_274093/efficienciesDATA/abseta_pt_DATA'
	runBLumi = 0.5

	runCDEff = 'IsoMu22_OR_IsoTkMu22_PtEtaBins_Run274094_to_276097/efficienciesDATA/abseta_pt_DATA'
	runCDLumi = 15

	inputFile = TFile(muonSFFile)

	runB_hist = inputFile.Get(runBEff)
	runCD_hist = inputFile.Get(runCDEff)


	for xBin in range(1,runB_hist.GetNbinsX()+1):
		for yBin in range(1,runB_hist.GetNbinsY()+1):
			runB_eff = runB_hist.GetBinContent(xBin,yBin)
			runC_eff = runCD_hist.GetBinContent(xBin,yBin)
			weightedAverage = ( runB_eff * runBLumi + runC_eff * runCDLumi ) / ( runBLumi + runCDLumi )
			print xBin, yBin, runB_eff, runC_eff, runBLumi, runCDLumi, weightedAverage

			runCD_hist.SetBinContent( xBin, yBin, weightedAverage )
	# runB_hist.Draw('COLZ TEXTE')
	# raw_input("...")

	outputFileName = 'BristolAnalysis/Tools/data/SingleMuonTrigger_Combined.root'
	# runCD_hist.Draw('COLZ TEXTE')
	# raw_input("...")
	outputFile = TFile(outputFileName, 'RECREATE')
	runCD_hist.Write('abseta_pt_DATA')
	outputFile.Close()