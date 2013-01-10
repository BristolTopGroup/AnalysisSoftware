
/*
 * BinningAnalyser.cpp
 */

#include "../../interface/Analysers/BinningAnalyser.h"

namespace BAT {

void BinningAnalyser::analyse(const EventPtr event) {

	if(!event->isRealData()){
	ePlusJetsSignalAnalysis(event);
	muPlusJetsSignalAnalysis(event);
	muPlusJetsMETAnalysis(event);
	ePlusJetsMETAnalysis(event);
	}
}

void BinningAnalyser::muPlusJetsMETAnalysis(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;

	if (topMuPlusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
			std::string prefix = METAlgorithm::prefixes.at(index);
			METAlgorithm::value metType = (METAlgorithm::value) index;
			if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
				continue;
			bool isMCOnlyMET = MET::isMCOnlyMETType(index);

			if (isMCOnlyMET && event->isRealData()) //these METs are MC only (Jet resolution systematics)
				continue;
			const METPointer met(event->MET(metType));
			histMan_->setCurrentHistogramFolder(histogramFolder_+"/MuonMET/"+prefix);
			if (index != METAlgorithm::GenMET && !event->isRealData()) {
				histMan_->H2D_BJetBinned("GenMET_vs_RecoMET")->Fill(event->GenMET()->et(), met->et(), weight_);
			}

		}
	}
}

void BinningAnalyser::ePlusJetsMETAnalysis(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;

	if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {
		for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
			std::string prefix = METAlgorithm::prefixes.at(index);
			METAlgorithm::value metType = (METAlgorithm::value) index;
			if (!MET::isAvailableInNTupleVersion(Globals::NTupleVersion, index))
				continue;
			bool isMCOnlyMET = MET::isMCOnlyMETType(index);

			if (isMCOnlyMET && event->isRealData()) //these METs are MC only (Jet resolution systematics)
				continue;
			const METPointer met(event->MET(metType));
			histMan_->setCurrentHistogramFolder(histogramFolder_+"/ElectronMET/"+prefix);
			if (index != METAlgorithm::GenMET && !event->isRealData()) {
				histMan_->H2D_BJetBinned("GenMET_vs_RecoMET")->Fill(event->GenMET()->et(), met->et(), weight_);
			}

		}
	}
}

void BinningAnalyser::muPlusJetsSignalAnalysis(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/MuPlusJets/");
	weight_ = event->weight() * prescale_ * scale_;

	if (topMuPlusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {

		const JetCollection jets(topMuPlusJetsRefSelection_->cleanedJets(event));
		const JetCollection genJets(event->GenJets());
		const JetCollection bJets(topMuPlusJetsRefSelection_->cleanedBJets(event));
		const MCParticleCollection genPart(event->GenParticles());
		const LeptonPointer signalLepton = topMuPlusJetsRefSelection_->signalLepton(event);
		const MuonPointer signalMuon(boost::static_pointer_cast<Muon>(signalLepton));
		const METPointer met(event->MET());

		ParticlePointer leptonic_W;
		leptonic_W = ParticlePointer(new Particle(*met + *signalLepton));

		ParticlePointer M3;
		ParticlePointer GeneratorM3;

		unsigned int numberOfGenJets(genJets.size());
		unsigned int numberOfJets(jets.size());
        unsigned int numberOfGenParts(genPart.size());

//cout << "gen jets: " << numberOfGenJets << endl;
//cout << "jets: " << numberOfJets << endl;
//cout << "parts: " << numberOfGenParts << endl;

//		unsigned int numberOfBJets(bJets.size());
		double HT = 0;
		double GenHT = 0;
		double HT_lepton = 0;
		double GenHT_lepton = 0;
		double HT_lepton_MET = 0;
		double GenHT_lepton_MET = 0;
		double GenNuPt = 0;
		double gendPhi = 0;

		double genLeptPt= 0;
		double genWPt = 0;
		bool oneMuon = false;
		double muonID = 0;
		double partonPt = 0;
		for (unsigned int i = 0; i < numberOfGenParts; i++) {
//	cout << "part: " << i << " id: " << genPart.at(i)->pdgId() << " mother:" << genPart.at(i)->motherIndex() << " pt: " << genPart.at(i)->pt() << " M(): " << genPart.at(i)->mass() << endl;

			// find the muon
			if(abs(genPart.at(i)->pdgId())==(13) && abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24)){
				oneMuon = true;
				muonID = genPart.at(i)->pdgId();
				genLeptPt = genPart.at(i)->pt();
				gendPhi = genPart.at(i)->deltaPhi(event->GenMET());
				genWPt = genPart.at(genPart.at(i)->motherIndex())->pt();
			}

			// find neutrino
			if(abs(genPart.at(i)->pdgId())==(14) && abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24)){
				GenNuPt = genPart.at(i)->pt();
			}

			// find additional lepton for veto
			if((muonID>1 && (genPart.at(i)->pdgId()==(-11) || genPart.at(i)->pdgId()==(-13) || genPart.at(i)->pdgId()==(-17))) || (muonID<-1 && (genPart.at(i)->pdgId()==(11) || genPart.at(i)->pdgId()==(13) || genPart.at(i)->pdgId()==(17)))){
				oneMuon = false;
		    }

			//parton level Ht
			if(((abs(genPart.at(i)->pdgId())>0 && abs(genPart.at(i)->pdgId()) < 6) && (abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(6) || abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24))) || ((genPart.at(i)->pdgId()==21 || (abs(genPart.at(i)->pdgId())>0 && abs(genPart.at(i)->pdgId()) < 6)) && genPart.at(i)->motherIndex()==4)){
				partonPt = partonPt + genPart.at(i)->pt();
			}

		}

		for (unsigned int i = 0; i < numberOfJets; ++i) {
			HT = HT + jets.at(i)->pt();
		}
		for (unsigned int i = 0; i < numberOfGenJets; ++i) {
			GenHT = GenHT + genJets.at(i)->pt();
		}

		//for M3 calc
		double ptmax = 0;
		double RecoM3 = 0;
		for (unsigned int a = 0; a < numberOfJets; ++a) {
			for (unsigned int b = 1; b < numberOfJets; ++b) {
				for (unsigned int c = 2; c < numberOfJets; ++c) {
					if(a!=b && a!=c){
						M3 = ParticlePointer(new Particle(*jets.at(a) + *jets.at(b)+ *jets.at(c)));
					    double pt =  M3->pt();

					    if(pt>ptmax){
					    	ptmax = pt;
					        RecoM3 = M3->mass();
					    }
					}
				}
			}
		}

		double ptGenMax = 0;
		double GenM3 = 0;
		for (unsigned int a = 0; a < numberOfGenJets; ++a) {
			for (unsigned int b = 1; b < numberOfGenJets; ++b) {
				for (unsigned int c = 2; c < numberOfGenJets; ++c) {
					if(a!=b && a!=c){
						GeneratorM3 = ParticlePointer(new Particle(*genJets.at(a) + *genJets.at(b)+ *genJets.at(c)));
					    double pt =  GeneratorM3->pt();

					    if(pt>ptGenMax){
					    	ptGenMax = pt;
					        GenM3 = GeneratorM3->mass();
					    }
					}
				}
			}
		}

		HT_lepton = HT + signalLepton->pt();
		GenHT_lepton = GenHT + genLeptPt;
		HT_lepton_MET = HT_lepton + met->pt();
		GenHT_lepton_MET = GenHT_lepton + event->GenMET()->et();

		histMan_->H2D_BJetBinned("GenHTPlusMETPt_vs_RecoHTPlusMetPt")->Fill(GenHT+event->GenMET()->pt(),HT+met->pt(), weight_);
		histMan_->H2D_BJetBinned("GenHT_vs_RecoHT")->Fill(GenHT,HT,  weight_);
		histMan_->H2D_BJetBinned("GenM3_vs_RecoM3")->Fill(GenM3,RecoM3,  weight_);

		if(oneMuon){
		histMan_->H2D_BJetBinned("GenLepPlusMETPt_vs_RecoLepPlusMetPt")->Fill(genLeptPt+event->GenMET()->pt(),signalLepton->pt()+met->pt(), weight_);
	    histMan_->H2D_BJetBinned("GendPhiLepMet_vs_RecodPhiLepMetPt")->Fill(gendPhi,signalLepton->deltaPhi(met), weight_);
		histMan_->H2D_BJetBinned("GenLepPt_vs_RecoLepPt")->Fill(genLeptPt, signalLepton->pt(),weight_);
		histMan_->H2D_BJetBinned("GenHT_lep_vs_RecoHT_lep")->Fill(GenHT_lepton,HT_lepton, weight_);
		histMan_->H2D_BJetBinned("GenHT_lep_met_vs_RecoHT_lep_met")->Fill(GenHT_lepton_MET,HT_lepton_MET,  weight_);
		histMan_->H2D_BJetBinned("Genleptonic_W_pt_vs_Recoleptonic_W_pt")->Fill(genWPt,leptonic_W->pt(),  weight_);
		histMan_->H2D_BJetBinned("GenNu_vs_RecoMET")->Fill(GenNuPt, met->pt(), weight_);
		histMan_->H2D_BJetBinned("GenParton_vs_RecoHT")->Fill(partonPt, HT, weight_);
		histMan_->H2D_BJetBinned("GenJetHT_vs_GenParton")->Fill(GenHT, partonPt, weight_);
		}


	}

}

void BinningAnalyser::ePlusJetsSignalAnalysis(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/EPlusJets/");
	weight_ = event->weight() * prescale_ * scale_;

	if (topEplusJetsRefSelection_->passesFullSelectionExceptLastTwoSteps(event)) {

		const JetCollection jets(topEplusJetsRefSelection_->cleanedJets(event));
		const JetCollection genJets(event->GenJets());
		const JetCollection bJets(topEplusJetsRefSelection_->cleanedBJets(event));
		const MCParticleCollection genPart(event->GenParticles());
		const LeptonPointer signalLepton = topEplusJetsRefSelection_->signalLepton(event);
		const ElectronPointer signalElectron(boost::static_pointer_cast<Electron>(signalLepton));
		const METPointer met(event->MET());

		ParticlePointer leptonic_W;
		leptonic_W = ParticlePointer(new Particle(*met + *signalLepton));

		ParticlePointer M3;
		ParticlePointer GeneratorM3;

		unsigned int numberOfGenJets(genJets.size());
		unsigned int numberOfJets(jets.size());
        unsigned int numberOfGenParts(genPart.size());

//		unsigned int numberOfBJets(bJets.size());
		double HT = 0;
		double GenHT = 0;
		double HT_lepton = 0;
		double GenHT_lepton = 0;
		double HT_lepton_MET = 0;
		double GenHT_lepton_MET = 0;
		double GenNuPt = 0;
		double gendPhi = 0;

		double genLeptPt= 0;
		double genWPt = 0;
		bool oneElectron = false;
		double electronID = 0;
		double partonPt = 0;

		for (unsigned int i = 0; i < numberOfGenParts; i++) {
//	cout << "part: " << i << " id: " << genPart.at(i)->pdgId() << " mother:" << genPart.at(i)->motherIndex() << " pt: " << genPart.at(i)->pt() << " M(): " << genPart.at(i)->mass() << endl;

			// find the electron
			if(abs(genPart.at(i)->pdgId())==(11) && abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24)){
				oneElectron = true;
				electronID = genPart.at(i)->pdgId();
				genLeptPt = genPart.at(i)->pt();
				gendPhi = genPart.at(i)->deltaPhi(event->GenMET());
				genWPt = genPart.at(genPart.at(i)->motherIndex())->pt();
			}

			// find neutrino
			if(abs(genPart.at(i)->pdgId())==(12) && abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24)){
				GenNuPt = genPart.at(i)->pt();
			}

			// find additional lepton for veto
			if((electronID>1 && (genPart.at(i)->pdgId()==(-11) || genPart.at(i)->pdgId()==(-13) || genPart.at(i)->pdgId()==(-17))) || (electronID<-1 && (genPart.at(i)->pdgId()==(11) || genPart.at(i)->pdgId()==(13) || genPart.at(i)->pdgId()==(17)))){
				oneElectron = false;
		    }

			//parton level Ht
			if(((abs(genPart.at(i)->pdgId())>0 && abs(genPart.at(i)->pdgId()) < 6) && (abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(6) || abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24))) || ((genPart.at(i)->pdgId()==21 || (abs(genPart.at(i)->pdgId())>0 && abs(genPart.at(i)->pdgId()) < 6)) && genPart.at(i)->motherIndex()==4)){
				partonPt = partonPt + genPart.at(i)->pt();
			}

		}
		//cout << genLeptPt << endl;

		for (unsigned int i = 0; i < numberOfJets; ++i) {
			HT = HT + jets.at(i)->pt();
		}
		for (unsigned int i = 0; i < numberOfGenJets; ++i) {
			GenHT = GenHT + genJets.at(i)->pt();
		}

		//for M3 calc
		double ptmax = 0;
		double RecoM3 = 0;
		for (unsigned int a = 0; a < numberOfJets; ++a) {
			for (unsigned int b = 1; b < numberOfJets; ++b) {
				for (unsigned int c = 2; c < numberOfJets; ++c) {
					if(a!=b && a!=c){
						M3 = ParticlePointer(new Particle(*jets.at(a) + *jets.at(b)+ *jets.at(c)));
					    double pt =  M3->pt();

					    if(pt>ptmax){
					    	ptmax = pt;
					        RecoM3 = M3->mass();
					    }
					}
				}
			}
		}

		double ptGenMax = 0;
		double GenM3 = 0;
		for (unsigned int a = 0; a < numberOfGenJets; ++a) {
			for (unsigned int b = 1; b < numberOfGenJets; ++b) {
				for (unsigned int c = 2; c < numberOfGenJets; ++c) {
					if(a!=b && a!=c){
						GeneratorM3 = ParticlePointer(new Particle(*genJets.at(a) + *genJets.at(b)+ *genJets.at(c)));
					    double pt =  GeneratorM3->pt();

					    if(pt>ptGenMax){
					    	ptGenMax = pt;
					        GenM3 = GeneratorM3->mass();
					    }
					}
				}
			}
		}

		HT_lepton = HT + signalLepton->pt();
		GenHT_lepton = GenHT + genLeptPt;
		HT_lepton_MET = HT_lepton + met->pt();
		GenHT_lepton_MET = GenHT_lepton + event->GenMET()->et();

		histMan_->H2D_BJetBinned("GenHTPlusMETPt_vs_RecoHTPlusMetPt")->Fill(GenHT+event->GenMET()->pt(),HT+met->pt(), weight_);
		histMan_->H2D_BJetBinned("GenHT_vs_RecoHT")->Fill(GenHT,HT,  weight_);
		histMan_->H2D_BJetBinned("GenM3_vs_RecoM3")->Fill(GenM3,RecoM3,  weight_);


		if(numberOfJets>0 && numberOfGenJets>0){
        histMan_->H2D_BJetBinned("GenJet1Pt_vs_RecoJet1Pt")->Fill(genJets.at(0)->pt(),jets.at(0)->pt(), weight_);}
        if(numberOfJets>1 && numberOfGenJets>1){
        histMan_->H2D_BJetBinned("GenJet2Pt_vs_RecoJet2Pt")->Fill(genJets.at(1)->pt(),jets.at(1)->pt(), weight_);}
        if(numberOfJets>2 && numberOfGenJets>2){
        histMan_->H2D_BJetBinned("GenJet3Pt_vs_RecoJet3Pt")->Fill(genJets.at(2)->pt(),jets.at(2)->pt(), weight_);}
        if(numberOfJets>3 && numberOfGenJets>3){
        histMan_->H2D_BJetBinned("GenJet4Pt_vs_RecoJet4Pt")->Fill(genJets.at(3)->pt(),jets.at(3)->pt(), weight_);}
        if(numberOfJets>4 && numberOfGenJets>4){
        histMan_->H2D_BJetBinned("GenJet5Pt_vs_RecoJet5Pt")->Fill(genJets.at(4)->pt(),jets.at(4)->pt(), weight_);}

		if(oneElectron){
			histMan_->H2D_BJetBinned("GenLepPlusMETPt_vs_RecoLepPlusMetPt")->Fill(genLeptPt+event->GenMET()->pt(),signalLepton->pt()+met->pt(), weight_);
		    histMan_->H2D_BJetBinned("GendPhiLepMet_vs_RecodPhiLepMetPt")->Fill(gendPhi,signalLepton->deltaPhi(met), weight_);
			histMan_->H2D_BJetBinned("GenLepPt_vs_RecoLepPt")->Fill(genLeptPt, signalLepton->pt(),weight_);
			histMan_->H2D_BJetBinned("GenHT_lep_vs_RecoHT_lep")->Fill(GenHT_lepton,HT_lepton, weight_);
			histMan_->H2D_BJetBinned("GenHT_lep_met_vs_RecoHT_lep_met")->Fill(GenHT_lepton_MET,HT_lepton_MET,  weight_);
			histMan_->H2D_BJetBinned("Genleptonic_W_pt_vs_Recoleptonic_W_pt")->Fill(genWPt,leptonic_W->pt(),  weight_);
			histMan_->H2D_BJetBinned("GenNu_vs_RecoMET")->Fill(GenNuPt, met->pt(), weight_);
			histMan_->H2D_BJetBinned("GenParton_vs_RecoHT")->Fill(partonPt, HT, weight_);
			histMan_->H2D_BJetBinned("GenJetHT_vs_GenParton")->Fill(GenHT, partonPt, weight_);
		}

	}

}

void BinningAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/EPlusJets/");

	histMan_->addH2D_BJetBinned("GenJet1Pt_vs_RecoJet1Pt", "GenJet1Pt_vs_RecoJet1Pt; Jet 1 PT_{Gen} [GeV]; Jet 1PT_{Reco} [GeV]", 500,
			0, 500, 500, 0, 500);
	histMan_->addH2D_BJetBinned("GenJet2Pt_vs_RecoJet2Pt", "GenJet2Pt_vs_RecoJet2Pt; Jet 2 PT_{Gen} [GeV]; Jet 2PT_{Reco} [GeV]", 500,
					0, 500, 500, 0, 500);
	histMan_->addH2D_BJetBinned("GenJet3Pt_vs_RecoJet3Pt", "GenJet3Pt_vs_RecoJet3Pt; Jet 3 PT_{Gen} [GeV]; Jet 3PT_{Reco} [GeV]", 500,
						0, 500, 500, 0, 500);
	histMan_->addH2D_BJetBinned("GenJet4Pt_vs_RecoJet4Pt", "GenJet4Pt_vs_RecoJet4Pt; Jet 4 PT_{Gen} [GeV]; Jet 4PT_{Reco} [GeV]", 500,
						0, 500, 500, 0, 500);
	histMan_->addH2D_BJetBinned("GenJet5Pt_vs_RecoJet5Pt", "GenJet5Pt_vs_RecoJet5Pt; Jet 5 PT_{Gen} [GeV]; Jet 5PT_{Reco} [GeV]", 500,
						0, 500, 500, 0, 500);

	histMan_->addH2D_BJetBinned("GenHTPlusMETPt_vs_RecoHTPlusMetPt", "GenHTPlusMETPt_vs_RecoHTPlusMETPt; HT+met_{Gen} [GeV]; HT+met_{Reco} [GeV]", 500,
			0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenLepPlusMETPt_vs_RecoLepPlusMetPt", "GenLepPlusMETPt_vs_RecoLepPlusMETPt; lepton+met pt_{Gen} [GeV]; lepton+met pt_{Reco} [GeV]", 500,
						0, 500, 500, 0, 500);
	histMan_->addH2D_BJetBinned("GenLepPt_vs_RecoLepPt", "GenLepPt_vs_RecoLepPt; Lep pt_{Gen} [GeV]; Lep pt_{Reco} [GeV]", 500,
						0, 500, 500, 0, 500);
	histMan_->addH2D_BJetBinned("GendPhiLepMet_vs_RecodPhiLepMetPt", "GendPhiLepMet_vs_RecodPhiLepMet; #Delta#Phi (lepton,MET)_{Gen}; #Delta#Phi (lepton,MET)_{Reco}", 400,
						-4., +4., 400, -4, +4);
	histMan_->addH2D_BJetBinned("GenHT_vs_RecoHT", "GenHT_vs_RecoHT; HT_{Gen} [GeV]; HT_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenHT_lep_vs_RecoHT_lep", "GenHT_lep_vs_RecoHT_lep; HT_lep_{Gen} [GeV]; HT_lep_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenHT_lep_met_vs_RecoHT_lep_met", "GenHT_lep_met_vs_RecoHT_lep_met; HT_lep_met_{Gen} [GeV]; HT_lep_met_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("Genleptonic_W_pt_vs_Recoleptonic_W_pt", "Genleptonic_W_pt_vs_Recoleptonic_W_pt; leptonic_W_pt_{Gen} [GeV]; leptonic_W_pt_{Reco} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("GenM3_vs_RecoM3", "GenM3_vs_RecoM3; M3_{Gen} [GeV]; M3_{Reco} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("GenNu_vs_RecoMET", "GenMET_vs_RecoNu; MET_{Gen} [GeV]; MET_{Reco} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("GenParton_vs_RecoHT", "GenHT_vs_RecoPartonHT; HT_parton_{Gen} [GeV]; HT_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenJetHT_vs_GenParton", "GenJetHT_vs_RecoPartonHT; HT_parton_{Gen} [GeV]; HT_{Gen} [GeV]", 500,
						0, 2000, 500, 0, 2000);


	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/MuPlusJets/");
	histMan_->addH2D_BJetBinned("GenHTPlusMETPt_vs_RecoHTPlusMetPt", "GenHTPlusMETPt_vs_RecoHTPlusMETPt; HT+met_{Gen} [GeV]; HT+met_{Reco} [GeV]", 500,
			0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenLepPlusMETPt_vs_RecoLepPlusMetPt", "GenLepPlusMETPt_vs_RecoLepPlusMETPt; lepton+met pt_{Gen} [GeV]; lepton+met pt_{Reco} [GeV]", 500,
			0, 500, 500, 0, 500);
	histMan_->addH2D_BJetBinned("GenLepPt_vs_RecoLepPt", "GenLepPt_vs_RecoLepPt; Lep pt_{Gen} [GeV]; Lep pt_{Reco} [GeV]", 500,
			0, 500, 500, 0, 500);
	histMan_->addH2D_BJetBinned("GendPhiLepMet_vs_RecodPhiLepMetPt", "GendPhiLepMet_vs_RecodPhiLepMet; #Delta#Phi (lepton,MET)_{Gen}; #Delta#Phi (lepton,MET)_{Reco}", 400,
			-4., +4., 400, -4, +4);
	histMan_->addH2D_BJetBinned("GenHT_vs_RecoHT", "GenHT_vs_RecoHT; HT_{Gen} [GeV]; HT_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenHT_lep_vs_RecoHT_lep", "GenHT_lep_vs_RecoHT_lep; HT_lep_{Gen} [GeV]; HT_lep_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenHT_lep_met_vs_RecoHT_lep_met", "GenHT_lep_met_vs_RecoHT_lep_met; HT_lep_met_{Gen} [GeV]; HT_lep_met_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("Genleptonic_W_pt_vs_Recoleptonic_W_pt", "Genleptonic_W_pt_vs_Recoleptonic_W_pt; leptonic_W_pt_{Gen} [GeV]; leptonic_W_pt_{Reco} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("GenM3_vs_RecoM3", "GenM3_vs_RecoM3; M3_{Gen} [GeV]; M3_{Reco} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("GenNu_vs_RecoMET", "GenMET_vs_RecoNu; MET_{Gen} [GeV]; MET_{Reco} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("GenParton_vs_RecoHT", "GenHT_vs_RecoPartonHT; HT_parton_{Gen} [GeV]; HT_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenJetHT_vs_GenParton", "GenJetHT_vs_RecoPartonHT; HT_parton_{Gen} [GeV]; HT_{Gen} [GeV]", 500,
						0, 2000, 500, 0, 2000);

	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		histMan_->setCurrentHistogramFolder(histogramFolder_+ "/MuonMET/"+prefix);
		histMan_->addH2D_BJetBinned("GenMET_vs_RecoMET", "GenMET_vs_RecoMET; MET_{GEN} [GeV]; MET_{RECO} [GeV]", 300,
							0, 300, 300, 0, 300);

	}
	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		histMan_->setCurrentHistogramFolder(histogramFolder_+ "/ElectronMET/"+prefix);
		histMan_->addH2D_BJetBinned("GenMET_vs_RecoMET", "GenMET_vs_RecoMET; HT_{GEN} [GeV]; HT_{RECO} [GeV]", 300,
							0, 300, 300, 0, 300);


	}

}

BinningAnalyser::BinningAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		//signal selections
		topEplusJetsRefSelection_(new TopPairEPlusJetsReferenceSelection()), //
		topMuPlusJetsRefSelection_(new TopPairMuPlusJetsReferenceSelection()){

}


BinningAnalyser::~BinningAnalyser() {
}

} /* namespace BAT */
