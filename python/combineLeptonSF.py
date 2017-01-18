from ROOT import gROOT, TFile, TH1F

class scaleFactorsForPeriod:
	def __init__(self, fileName, scaleFactorName, lumi):
		self.fileName = fileName
		self.scaleFactorName = scaleFactorName
		self.lumi = lumi

doIsolationSF = True

if __name__ == '__main__':

	outputFileName = None
	scaleFactorsToAverage = []
	if doIsolationSF:
		scaleFactorsToAverage = [
			scaleFactorsForPeriod( '../data/EfficienciesAndSF_Isolation_BCDEF.root', 'TightISO_TightID_pt_eta/abseta_pt_ratio', 20.1 ),
			scaleFactorsForPeriod( '../data/EfficienciesAndSF_Isolation_GH.root', 'TightISO_TightID_pt_eta/abseta_pt_ratio', 16.3 )
		]
		outputFileName = '../data/EfficienciesAndSF_Isolation_COMBINED.root'
	else:
		scaleFactorsToAverage = [
			scaleFactorsForPeriod( '../data/EfficienciesAndSF_BCDEF.root', 'MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/abseta_pt_ratio', 20.1 ),
			scaleFactorsForPeriod( '../data/EfficienciesAndSF_GH.root', 'MC_NUM_TightID_DEN_genTracks_PAR_pt_eta/abseta_pt_ratio', 16.3 )
		]
		outputFileName = '../data/EfficienciesAndSF_COMBINED.root'


	firstScaleFactor = True
	averagedScalefactors = None
	sumLumi = 0
	for scaleFactors in scaleFactorsToAverage:


		inputFile = TFile(scaleFactors.fileName)

		gROOT.cd()
		scaleFactors.sf_hist = inputFile.Get(scaleFactors.scaleFactorName).Clone()
		sumLumi += scaleFactors.lumi
		if firstScaleFactor:
			firstScaleFactor = False
			averagedScalefactors = scaleFactors.sf_hist.Clone()
			averagedScalefactors.Scale( scaleFactors.lumi )
		else:
			averagedScalefactors.Add( scaleFactors.sf_hist, scaleFactors.lumi )
		inputFile.Close()

	averagedScalefactors.Scale(1/sumLumi)

	outputFile = TFile(outputFileName, 'RECREATE')
	averagedScalefactors.Write('abseta_pt_ratio')
	outputFile.Close()