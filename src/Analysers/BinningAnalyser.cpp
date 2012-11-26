
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
				histMan_->H2D_BJetBinned("RecoMET_vs_GenMET")->Fill(event->GenMET()->et(), met->et(), weight_);
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
				histMan_->H2D_BJetBinned("RecoMET_vs_GenMET")->Fill(event->GenMET()->et(), met->et(), weight_);
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
		HT_lepton_MET = GenHT_lepton + met->pt();
		GenHT_lepton_MET = GenHT_lepton + event->GenMET()->et();

		histMan_->H2D_BJetBinned("RecoHT_vs_GenHT")->Fill(HT, GenHT, weight_);
		histMan_->H2D_BJetBinned("RecoM3_vs_GenM3")->Fill(RecoM3, GenM3, weight_);

		if(oneMuon){
		histMan_->H2D_BJetBinned("RecoHT_lep_vs_GenHT_lep")->Fill(HT_lepton,GenHT_lepton, weight_);
		histMan_->H2D_BJetBinned("RecoHT_lep_met_vs_GenHT_lep_met")->Fill(HT_lepton_MET, GenHT_lepton_MET, weight_);
		histMan_->H2D_BJetBinned("Recoleptonic_W_pt_vs_Genleptonic_W_pt")->Fill(leptonic_W->pt(), genWPt, weight_);
		histMan_->H2D_BJetBinned("RecoMET_vs_GenNu")->Fill(met->pt(), GenNuPt, weight_);
		histMan_->H2D_BJetBinned("RecoHT_vs_GenParton")->Fill(HT, partonPt, weight_);
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
		HT_lepton_MET = GenHT_lepton + met->pt();
		GenHT_lepton_MET = GenHT_lepton + event->GenMET()->et();

		histMan_->H2D_BJetBinned("RecoHT_vs_GenHT")->Fill(HT, GenHT, weight_);
		histMan_->H2D_BJetBinned("RecoM3_vs_GenM3")->Fill(RecoM3, GenM3, weight_);

		if(oneElectron){
		histMan_->H2D_BJetBinned("RecoHT_lep_vs_GenHT_lep")->Fill(HT_lepton,GenHT_lepton, weight_);
		histMan_->H2D_BJetBinned("RecoHT_lep_met_vs_GenHT_lep_met")->Fill(HT_lepton_MET, GenHT_lepton_MET, weight_);
		histMan_->H2D_BJetBinned("Recoleptonic_W_pt_vs_Genleptonic_W_pt")->Fill(leptonic_W->pt(), genWPt, weight_);
		histMan_->H2D_BJetBinned("RecoMET_vs_GenNu")->Fill(met->pt(), GenNuPt, weight_);
		histMan_->H2D_BJetBinned("RecoHT_vs_GenParton")->Fill(HT, partonPt, weight_);
		histMan_->H2D_BJetBinned("GenJetHT_vs_GenParton")->Fill(GenHT, partonPt, weight_);
		}

	}

}

void BinningAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/EPlusJets/");

	histMan_->addH2D_BJetBinned("RecoHT_vs_GenHT", "RecoHT_vs_GenHT; HT_{GEN} [GeV]; HT_{RECO} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("RecoHT_lep_vs_GenHT_lep", "RecoHT_lep_vs_GenHT_lep; HT_lep_{GEN} [GeV]; HT_lep_{RECO} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("RecoHT_lep_met_vs_GenHT_lep_met", "RecoHT_lep_met_vs_GenHT_lep_met; HT_lep_met_{GEN} [GeV]; HT_lep_met_{RECO} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("Recoleptonic_W_pt_vs_Genleptonic_W_pt", "Recoleptonic_W_pt_vs_Genleptonic_W_pt; leptonic_W_pt_{GEN} [GeV]; leptonic_W_pt_{RECO} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("RecoM3_vs_GenM3", "RecoM3_vs_GenM3; M3_{GEN} [GeV]; M3_{RECO} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("RecoMET_vs_GenNu", "RecoMET_vs_GenNu; MET_{GEN} [GeV]; MET_{RECO} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("RecoHT_vs_GenParton", "RecoHT_vs_GenPartonHT; HT_parton_{GEN} [GeV]; HT_{RECO} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenJetHT_vs_GenParton", "GenJetHT_vs_GenPartonHT; HT_parton_{GEN} [GeV]; HT_{RECO} [GeV]", 500,
						0, 2000, 500, 0, 2000);

	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/MuPlusJets/");

	histMan_->addH2D_BJetBinned("RecoHT_vs_GenHT", "RecoHT_vs_GenHT; HT_{GEN} [GeV]; HT_{RECO} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("RecoHT_lep_vs_GenHT_lep", "RecoHT_lep_vs_GenHT_lep; HT_lep_{GEN} [GeV]; HT_lep_{RECO} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("RecoHT_lep_met_vs_GenHT_lep_met", "RecoHT_lep_met_vs_GenHT_lep_met; HT_lep_met_{GEN} [GeV]; HT_lep_met_{RECO} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("Recoleptonic_W_pt_vs_Genleptonic_W_pt", "Recoleptonic_W_pt_vs_Genleptonic_W_pt; leptonic_W_pt_{GEN} [GeV]; leptonic_W_pt_{RECO} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("RecoM3_vs_GenM3", "RecoM3_vs_GenM3; M3_{GEN} [GeV]; M3_{RECO} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("RecoMET_vs_GenNu", "RecoMET_vs_GenNu; MET_{GEN} [GeV]; MET_{RECO} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("RecoHT_vs_GenParton", "RecoHT_vs_GenPartonHT; HT_parton_{GEN} [GeV]; HT_{RECO} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenJetHT_vs_GenParton", "GenJetHT_vs_GenPartonHT; HT_parton_{GEN} [GeV]; HT_{RECO} [GeV]", 500,
						0, 2000, 500, 0, 2000);

	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		histMan_->setCurrentHistogramFolder(histogramFolder_+ "/MuonMET/"+prefix);
		histMan_->addH2D_BJetBinned("RecoMET_vs_GenMET", "RecoMET_vs_GenMET; MET_{GEN} [GeV]; MET_{RECO} [GeV]", 300,
							0, 300, 300, 0, 300);

	}
	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		histMan_->setCurrentHistogramFolder(histogramFolder_+ "/ElectronMET/"+prefix);
		histMan_->addH2D_BJetBinned("RecoMET_vs_GenMET", "RecoMET_vs_GenMET; HT_{GEN} [GeV]; HT_{RECO} [GeV]", 300,
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
