'''
Created on 08 May 2015

@author: Douglas Burns

Plots CSV, MW.vs.MT, Neutrino Chi Test.

'''

import ROOT 
from ROOT import gROOT, gPad, gStyle, TChain, TFile, TTree, TMath, TH1, TH1F, TH1D, TH2F, TCanvas, TPad, TAxis, TLegend, TLatex, kRed, kBlue, kGreen
import math

if __name__ == '__main__':


	########## 			SETUP 			##########
	gStyle.SetOptStat("")
	input_file = "/storage/ec6821/AnalysisTools/CMSSW_7_4_5/src/tree_TTJets_amcatnloFXFX_5000pb_PFElectron_PFMuon_PF2PATJets_MET.root"

	LightJetCSVHist = TH1F("CSV for LightJets","CSV for LightJets", 100, 0, 1.0)
	BJetCSVHist = TH1F("CSV for BJets","CSV for BJets", 100, 0, 1.0)
	
	TrueHadronicMTvsMWHist = TH2F("MT vs MW True Reconstructed B(hadronic)","M_{t} vs M_{W} Correct B; M_{W} (GeV); M_{t} (GeV)", 500, 0, 500, 500, 0, 500)
	TrueLeptonicMTvsMWHist = TH2F("MT vs MW True Reconstructed B(leptonic)","M_{t} vs M_{W} Correct B; M_{W} (GeV); M_{t} (GeV)", 100, 0, 500, 200, 0, 500)
	FalseHadronicMTvsMWHist = TH2F("MT vs MW False Reconstructed B(hadronic)","M_{t} vs M_{W} False Hadronic B; M_{W} (GeV); M_{t} (GeV)", 500, 0, 500, 500, 0, 500)
	FalseLeptonicMTvsMWHist = TH2F("MT vs MW False Reconstructed B(leptonic)","M_{t} vs M_{W} False B; M_{W} (GeV); M_{t} (GeV)", 100, 0, 500, 500, 0, 500)
	TrueHadFalseLepMTvsMWHist = TH2F("MT vs MW True Had B False Lep B","M_{t} vs M_{W} Correct Hadronic B; M_{W} (GeV); M_{t} (GeV)", 500, 0, 500, 500, 0, 500)
	
	NeutrinoChiSqLeptonicBHist = TH1F("NeutrinoChiSqLeptonicB","NeutrinoChiSqLeptonicB", 50, 0, 20)
	NeutrinoChiSqWHist = TH1F("NeutrinoChiSqW","NeutrinoChiSqW", 50, 0, 20)
	NeutrinoChiSqFalseBHist = TH1F("NeutrinoChiSqFalseB","NeutrinoChiSqFalseB", 50, 0, 20)

	TrueLikelihoodMassvsBtagHist = TH2F("Likelihood B Tag vs Mass True","Likelihood B Tag vs Mass True", 200, -10, 2, 200, -10, 0)
	FalseLikelihoodMassvsBtagHist = TH2F("Likelihood B Tag vs Mass False","Likelihood B Tag vs Mass False", 200, -10, 2, 200, -10, 0)

	CSVTree = "likelihood/CSV"
	CSVChain = TChain(CSVTree)
	CSVChain.Add(input_file)

	TopRecoTree = "likelihood/TopReco"
	TopRecoChain = TChain(TopRecoTree)
	TopRecoChain.Add(input_file)

	NeutrinoTree = "likelihood/Neutrino"
	NeutrinoChain = TChain(NeutrinoTree)
	NeutrinoChain.Add(input_file)

	LikelihoodTree = "TTbar_plus_X_analysis/EPlusJets/Ref selection/LikelihoodReco/Discriminator"
	LikelihoodChain = TChain(LikelihoodTree)
	LikelihoodChain.Add(input_file)

	CSVChain.AddFriend(TopRecoChain)
	CSVChain.AddFriend(NeutrinoChain)
	CSVChain.AddFriend(LikelihoodChain)
	CSVChain.SetBranchStatus("*",1)


	########## 			FILL HISTOGRAMS 		##########

	for event in CSVChain:


		Jet_CSV = event.__getattr__("Jets")
		TypeOfJet = event.__getattr__("TypeofJet")#1 for light, 2 for b

		for JetIndex in range (0,len(TypeOfJet)):
			if (TypeOfJet[JetIndex] == 1):
				LightJetCSVHist.Fill(Jet_CSV[JetIndex])
			if (TypeOfJet[JetIndex] == 2):
				BJetCSVHist.Fill(Jet_CSV[JetIndex])


		TrueReconstruction = event.__getattr__("CorrectReconstruction")
		LeptonicTopmass = event.__getattr__("LeptonicTop_Mass")
		HadronicTopmass = event.__getattr__("HadronicTop_Mass")
		LeptonicWmass = event.__getattr__("LeptonicW_Mass")
		HadronicWmass = event.__getattr__("HadronicW_Mass")

		for jetcombinationIndex in range (0, len(TrueReconstruction)):

			if (TrueReconstruction[jetcombinationIndex] == 2 or TrueReconstruction[jetcombinationIndex] == 1):
				TrueHadFalseLepMTvsMWHist.Fill(HadronicWmass[jetcombinationIndex],HadronicTopmass[jetcombinationIndex])

				if (TrueReconstruction[jetcombinationIndex] == 2):
					TrueHadronicMTvsMWHist.Fill(HadronicWmass[jetcombinationIndex],HadronicTopmass[jetcombinationIndex])
					TrueLeptonicMTvsMWHist.Fill(LeptonicWmass[jetcombinationIndex],LeptonicTopmass[jetcombinationIndex])

			elif (TrueReconstruction[jetcombinationIndex] == 0):
				FalseHadronicMTvsMWHist.Fill(HadronicWmass[jetcombinationIndex],HadronicTopmass[jetcombinationIndex])
				FalseLeptonicMTvsMWHist.Fill(LeptonicWmass[jetcombinationIndex],LeptonicTopmass[jetcombinationIndex])


		NuReconstruction = event.__getattr__("ReconstructedNeutrino")
		ChiSq = event.__getattr__("NuChiSq")

		for NuIndex in range (0, len(NuReconstruction)):

			if ( ChiSq[NuIndex] < 0 ) : continue

			if (NuReconstruction[NuIndex] == 1):
				NeutrinoChiSqLeptonicBHist.Fill(ChiSq[NuIndex])
			elif (NuReconstruction[NuIndex] == 2):
				NeutrinoChiSqFalseBHist.Fill(ChiSq[NuIndex])
				NeutrinoChiSqWHist.Fill(ChiSq[NuIndex])
			elif (NuReconstruction[NuIndex] == 3):
				NeutrinoChiSqWHist.Fill(ChiSq[NuIndex])


		LikelihoodBTag = event.__getattr__("CSVDiscrimBest")
		LikelihoodMass = event.__getattr__("MassDiscrimBest")
		EventReconstruction = event.__getattr__("EventReconstruction")

		if (EventReconstruction == 1):
			TrueLikelihoodMassvsBtagHist.Fill(LikelihoodMass,LikelihoodBTag)
		if (EventReconstruction == 0):
			FalseLikelihoodMassvsBtagHist.Fill(LikelihoodMass,LikelihoodBTag)

	########## 			NORMALISING 			##########

	integral = LightJetCSVHist.Integral()
	LightJetCSVHist.Scale(1/integral)
	integral = BJetCSVHist.Integral()
	BJetCSVHist.Scale(1/integral)

	integral = NeutrinoChiSqLeptonicBHist.Integral(0,51)
	print NeutrinoChiSqLeptonicBHist.GetBinContent(0),NeutrinoChiSqLeptonicBHist.GetBinContent(50),NeutrinoChiSqLeptonicBHist.GetBinContent(51)
	print NeutrinoChiSqLeptonicBHist.Integral(0,51)
	print integral
	NeutrinoChiSqLeptonicBHist.Scale(1/integral)
	print NeutrinoChiSqLeptonicBHist.Integral(),NeutrinoChiSqLeptonicBHist.Integral(0,51)
	integral = NeutrinoChiSqWHist.Integral(0,51)
	NeutrinoChiSqWHist.Scale(1/integral)
	integral = NeutrinoChiSqFalseBHist.Integral(0,51)
	print NeutrinoChiSqFalseBHist.GetBinContent(0),NeutrinoChiSqFalseBHist.GetBinContent(50),NeutrinoChiSqFalseBHist.GetBinContent(51)
	print NeutrinoChiSqFalseBHist.Integral(0,51)
	print integral
	NeutrinoChiSqFalseBHist.Scale(1/integral)
	print NeutrinoChiSqFalseBHist.Integral(),NeutrinoChiSqFalseBHist.Integral(0,51)

	integral = TrueHadronicMTvsMWHist.Integral()
	TrueHadronicMTvsMWHist.Scale(1/integral)
	integral = TrueLeptonicMTvsMWHist.Integral()
	TrueLeptonicMTvsMWHist.Scale(1/integral)
	integral = FalseHadronicMTvsMWHist.Integral()
	FalseHadronicMTvsMWHist.Scale(1/integral)
	integral = FalseLeptonicMTvsMWHist.Integral()
	FalseLeptonicMTvsMWHist.Scale(1/integral)
	integral = TrueHadFalseLepMTvsMWHist.Integral()
	TrueHadFalseLepMTvsMWHist.Scale(1/integral)



	########## 				PLOTTING 			##########


	file = TFile("LikelihoodInputAnalyserOutput.root", "RECREATE")

	########## 				CSV 				##########

	CSVCanvas = TCanvas("CSV","CSV", 0, 0, 800, 600)
	CSVleg = TLegend(0.15,0.7,0.5,0.88)#xmin,ymin,xmax,ymax (in % of canvas)
	CSVleg.SetFillColor(0)
	CSVleg.SetLineColor(0)

	BJetCSVHist.SetLineColor(kRed)
	BJetCSVHist.SetTitle("Jet CSV; CSVv2 b-tag; Normalised Events")
	BJetCSVHist.GetYaxis().SetTitleOffset(1.2)
	CSVleg.AddEntry(BJetCSVHist, "B Jets" ,"le")
	BJetCSVHist.Draw()

	LightJetCSVHist.SetLineColor(kBlue)
	CSVleg.AddEntry(LightJetCSVHist, "Light Jets" ,"le")
	LightJetCSVHist.Draw("same")

	CSVleg.Draw()
	CSVCanvas.Update()
	CSVCanvas.SaveAs("plots/AllJetCSV.png")

	LightJetCSVHist.Write()
	BJetCSVHist.Write()
	CSVCanvas.Write()


	########## 				MT vs MW 			##########

	M1Canvas = TCanvas("True Hadronic","Correct_HadB_MTvsMW", 0, 0, 1000, 800)
	TrueHadronicMTvsMWHist.Draw("colz")
	TrueHadronicMTvsMWHist.GetYaxis().SetTitleOffset(1.2)
	M1Canvas.SetRightMargin(0.9)#TAttPad
	# M1Canvas.SetLogz()
	M1Canvas.Update()
	M1Canvas.SaveAs("plots/Correct_HadB_MTvsMW.png")

	# M2Canvas = TCanvas("True Leptonic","Correct_LepB_MTvsMW", 0, 0, 1000, 800)
	# TrueLeptonicMTvsMWHist.Draw("colz")
	# TrueLeptonicMTvsMWHist.GetYaxis().SetTitleOffset(1.2)
	# M2Canvas.Update()
	# M2Canvas.SaveAs("plots/Correct_LepB_MTvsMW.png")

	M3Canvas = TCanvas("False Hadronic","Incorrect_HadB_MTvsMW", 0, 0, 1000, 800)
	# FalseHadronicMTvsMWHist.Draw("Surf2")
	FalseHadronicMTvsMWHist.Draw("colz")
	FalseHadronicMTvsMWHist.GetYaxis().SetTitleOffset(1.2)
	# FalseHadronicMTvsMWHist.SetMinimum(0)
	# FalseHadronicMTvsMWHist.SetMaximum(0.001)
	# M3Canvas.SetLogz()
	M3Canvas.Update()
	M3Canvas.SaveAs("plots/Incorrect_HadB_MTvsMW.png")

	# M4Canvas = TCanvas("False Leptonic","Incorrect_LepB_MTvsMW", 0, 0, 1000, 800)
	# FalseLeptonicMTvsMWHist.Draw("colz")
	# FalseLeptonicMTvsMWHist.GetYaxis().SetTitleOffset(1.2)
	# M4Canvas.Update()
	# M4Canvas.SaveAs("plots/Incorrect_LepB_MTvsMW.png")

	M5Canvas = TCanvas("False Leptonic True Hadronic","Correct_HadB_Incorrect_LepB_TrueHadMTvsMW", 0, 0, 1000, 800)
	# TrueHadFalseLepMTvsMWHist.Draw("Surf2")
	TrueHadFalseLepMTvsMWHist.Draw("colz")
	TrueHadFalseLepMTvsMWHist.GetYaxis().SetTitleOffset(1.2)
	# FalseHadronicMTvsMWHist.SetMinimum(0)
	# FalseHadronicMTvsMWHist.SetMaximum(0.001)
	# M5Canvas.SetLogz()
	M5Canvas.Update()
	M5Canvas.SaveAs("plots/Correct_HadB_Incorrect_LepB_TrueHadMTvsMW.png")

	TrueHadronicMTvsMWHist.Write()
	TrueLeptonicMTvsMWHist.Write()
	FalseHadronicMTvsMWHist.Write()
	FalseLeptonicMTvsMWHist.Write()
	TrueHadFalseLepMTvsMWHist.Write()


	projTrueMw = TH1D(TrueHadFalseLepMTvsMWHist.ProjectionX())
	projTrueMt = TH1D(TrueHadFalseLepMTvsMWHist.ProjectionY())	
	projFalseMw = TH1D(FalseHadronicMTvsMWHist.ProjectionX())
	projFalseMt = TH1D(FalseHadronicMTvsMWHist.ProjectionY())

	M6Canvas = TCanvas("ProjectionWMass","ProjectionWMass", 0, 0, 1000, 800)
	projWleg = TLegend(0.5,0.7,0.88,0.88)
	projWleg.SetFillColor(0)
	projWleg.SetLineColor(0)
	projTrueMw.SetTitle("W Mass Projection; Mass / GeV; Normalised Number of Events")
	projTrueMw.GetYaxis().SetTitleOffset(1.2)
	projTrueMw.SetLineColor(kBlue)
	projTrueMw.Draw()
	projFalseMw.SetLineColor(kRed)
	projFalseMw.Draw("same")
	projWleg.AddEntry(projTrueMw, "True Mw Projection" ,"le")
	projWleg.AddEntry(projFalseMw, "False Mw Projection" ,"le")
	projWleg.Draw()
	M6Canvas.Update()
	M6Canvas.SaveAs("plots/ProjectionWMass.png")


	M7Canvas = TCanvas("ProjectionTopMass","ProjectionTopMass", 0, 0, 1000, 800)
	projtleg = TLegend(0.5,0.7,0.88,0.88)
	projtleg.SetFillColor(0)
	projtleg.SetLineColor(0)
	projTrueMt.SetTitle("Top Mass Projection; Mass / GeV; Normalised Number of Events")
	projTrueMt.GetYaxis().SetTitleOffset(1.2)
	projTrueMt.SetLineColor(kBlue)
	projTrueMt.Draw()
	projFalseMt.SetLineColor(kRed)
	projFalseMt.Draw("same")
	projtleg.AddEntry(projTrueMt, "True Mt Projection" ,"le")
	projtleg.AddEntry(projFalseMt, "False Mt Projection" ,"le")
	projtleg.Draw()
	M7Canvas.Update()
	M7Canvas.SaveAs("plots/ProjectionTopMass.png")


	projTrueMw.Write()
	projTrueMt.Write()
	projFalseMw.Write()
	projFalseMt.Write()



	M1Canvas.Write()
	# M2Canvas.Write()
	M3Canvas.Write()
	# M4Canvas.Write()
	M5Canvas.Write()
	M6Canvas.Write()
	M7Canvas.Write()

	########## 			Likelihood Mass vs B-Tag			##########

	# L1Canvas = TCanvas("True Mass vs B-Tag","True Mass vs B-Tag", 0, 0, 1000, 800)
	# TrueLikelihoodMassvsBtagHist.Draw("colz")
	# TrueLikelihoodMassvsBtagHist.GetYaxis().SetTitleOffset(1.2)
	# L1Canvas.SetRightMargin(0.9)
	# L1Canvas.SaveAs("plots/TrueMvBTag.png")

	# L2Canvas = TCanvas("False Mass vs B-Tag","False Mass vs B-Tag", 0, 0, 1000, 800)
	# FalseLikelihoodMassvsBtagHist.Draw("colz")
	# FalseLikelihoodMassvsBtagHist.GetYaxis().SetTitleOffset(1.2)
	# L2Canvas.SaveAs("plots/FalseMvsBTag.png")

	# TrueLikelihoodMassvsBtagHist.Write()
	# FalseLikelihoodMassvsBtagHist.Write()

	# L1Canvas.Write()
	# L2Canvas.Write()


	########## 			Neutrino Chi Test 		##########

	NeutrinoCanvas = TCanvas("NeutrinoChi2","NeutrinoChi2", 0, 0, 800, 600)
	Neutrinoleg = TLegend(0.15,0.7,0.5,0.88)#xmin,ymin,xmax,ymax (in % of canvas)
	Neutrinoleg.SetFillColor(0)
	Neutrinoleg.SetLineColor(0)

	NeutrinoChiSqLeptonicBHist.SetLineColor(kRed)
	NeutrinoChiSqLeptonicBHist.SetTitle("Neutrino Chi2; Chi2; Normalised Number of Events")
	NeutrinoChiSqLeptonicBHist.GetYaxis().SetTitleOffset(1.2)
	Neutrinoleg.AddEntry(NeutrinoChiSqLeptonicBHist, "Correct Leptonic B" ,"le")
	NeutrinoChiSqLeptonicBHist.Draw()

	NeutrinoChiSqFalseBHist.SetLineColor(kBlue)
	Neutrinoleg.AddEntry(NeutrinoChiSqFalseBHist, "Wrong Leptonic B Using B Jets" ,"le")
	NeutrinoChiSqFalseBHist.Draw("same")

	# NeutrinoChiSqWHist.SetLineColor(kGreen)
	# Neutrinoleg.AddEntry(NeutrinoChiSqWHist, "Wrong Leptonic B Using All Jets" ,"le")
	# NeutrinoChiSqWHist.Draw("same")

	Neutrinoleg.Draw()
	NeutrinoCanvas.SetLogy()
	NeutrinoCanvas.Update()
	NeutrinoCanvas.SaveAs("plots/NeutrinoChi2.png")

	NeutrinoChiSqLeptonicBHist.Write()
	# NeutrinoChiSqWHist.Write()
	NeutrinoChiSqFalseBHist.Write()
	NeutrinoCanvas.Write()