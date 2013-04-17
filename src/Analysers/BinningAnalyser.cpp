
/*
 * BinningAnalyser.cpp
 */

#include "../../interface/Analysers/BinningAnalyser.h"

namespace BAT {

void BinningAnalyser::analyse(const EventPtr event) {

	if(!event->isRealData()){
	ePlusJetsSignalAnalysis(event);
	//ePlusJetsHTAnalysis(event);
	//ePlusJetsHTskimAnalysis(event);
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

			const MCParticleCollection genPart(event->GenParticles());
			unsigned int numberOfGenParts(genPart.size());
			bool oneMuon = false;
			double muonID = 0;
			double GenNuPt = 0;
			for (unsigned int i = 0; i < numberOfGenParts; i++) {

				// find the muon
				if(abs(genPart.at(i)->pdgId())==(13) && abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24)){
					oneMuon = true;
				}

				// find neutrino
				if(abs(genPart.at(i)->pdgId())==(14) && abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24)){
					GenNuPt = genPart.at(i)->pt();
				}

				// find additional lepton for veto
				if((muonID>1 && (genPart.at(i)->pdgId()==(-11) || genPart.at(i)->pdgId()==(-13) || genPart.at(i)->pdgId()==(-17))) || (muonID<-1 && (genPart.at(i)->pdgId()==(11) || genPart.at(i)->pdgId()==(13) || genPart.at(i)->pdgId()==(17)))){
					oneMuon = false;
			    }
			}

			if(oneMuon == true){

			if (isMCOnlyMET && event->isRealData()) //these METs are MC only (Jet resolution systematics)
				continue;
			const METPointer met(event->MET(metType));


			histMan_->setCurrentHistogramFolder(histogramFolder_+"/MuonMET/"+prefix);
			if (index != METAlgorithm::GenMET && !event->isRealData()) {
				histMan_->H2D_BJetBinned("GenMET_vs_RecoMET")->Fill(event->GenMET()->et(), met->et(), weight_);
				histMan_->H2D_BJetBinned("GenNu_vs_RecoMET")->Fill(GenNuPt, met->et(), weight_);
			}

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

			const MCParticleCollection genPart(event->GenParticles());
			unsigned int numberOfGenParts(genPart.size());
			bool oneElectron = false;
			double electronID = 0;
			double GenNuPt = 0;
			for (unsigned int i = 0; i < numberOfGenParts; i++) {

				// find the electron
				if(abs(genPart.at(i)->pdgId())==(11) && abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24)){
					oneElectron = true;
				}

				// find neutrino
				if(abs(genPart.at(i)->pdgId())==(12) && abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24)){
					GenNuPt = genPart.at(i)->pt();
				}

				// find additional lepton for veto
				if((electronID>1 && (genPart.at(i)->pdgId()==(-11) || genPart.at(i)->pdgId()==(-13) || genPart.at(i)->pdgId()==(-17))) || (electronID<-1 && (genPart.at(i)->pdgId()==(11) || genPart.at(i)->pdgId()==(13) || genPart.at(i)->pdgId()==(17)))){
					oneElectron = false;
			    }

			}

			if(oneElectron == true){

			if (isMCOnlyMET && event->isRealData()) //these METs are MC only (Jet resolution systematics)
				continue;
			const METPointer met(event->MET(metType));
			histMan_->setCurrentHistogramFolder(histogramFolder_+"/ElectronMET/"+prefix);
			if (index != METAlgorithm::GenMET && !event->isRealData()) {
				histMan_->H2D_BJetBinned("GenMET_vs_RecoMET")->Fill(event->GenMET()->et(), met->et(), weight_);
				histMan_->H2D_BJetBinned("GenNu_vs_RecoMET")->Fill(GenNuPt, met->et(), weight_);
			}

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
		const METPointer genMet(event->GenMET());

		//added for gen jet cleaning
		MCParticleCollection genMuons;
		//added for gen jet cleaning
		MCParticleCollection allGenMuons;

		ParticlePointer W_boson;
		W_boson = ParticlePointer(new Particle(*met + *signalLepton));

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
		double genWPt = 0;  //from W boson
		bool oneMuon = false;
		double muonID = 0;
		double partonPt = 0;
		double Npartons = 0;
		for (unsigned int i = 0; i < numberOfGenParts; i++) {


			//store all mouns
			if(abs(genPart.at(i)->pdgId())==(13)){
			MCParticlePointer muon(genPart.at(i));
			allGenMuons.push_back(muon);
			}

			// find the muon
			if(abs(genPart.at(i)->pdgId())==(13) && abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24)){
				oneMuon = true;
				muonID = genPart.at(i)->pdgId();
				genLeptPt = genPart.at(i)->pt();
				gendPhi = genPart.at(i)->deltaPhi(event->GenMET());
				genWPt = genPart.at(genPart.at(i)->motherIndex())->pt();
				MCParticlePointer genMuon(genPart.at(i));
				genMuons.push_back(genMuon);
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
				Npartons++;
			}




		}


		double minDR = 99999999.;
		if (oneMuon == true){

			for(unsigned int i = 0; i < genJets.size(); i++) {
				JetPointer generatorJet(genJets.at(i));

				double deltaRele = generatorJet->deltaR(genMuons.at(0));
				if(deltaRele < minDR)
					minDR = deltaRele;

			histMan_->H1D_BJetBinned("deltaR Gen Mu")->Fill(minDR,weight_);
			}
		}

		HT = Event::HT(jets);
		GenHT = Event::HT(genJets);
		double MT = Event::MT(signalMuon,met);

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

		if(oneMuon){

			HT_lepton = HT + signalLepton->pt();
			GenHT_lepton = GenHT + allGenMuons.at(1)->pt();
			HT_lepton_MET = Event::ST(jets, signalMuon, met);
			GenHT_lepton_MET = GenHT_lepton + event->GenMET()->et();

	        double MT_gen = -1;
			double Esq= pow(allGenMuons.at(1)->et()+genMet->et(),2);
			double Psq= pow(allGenMuons.at(1)->px()+genMet->px(),2)+pow(allGenMuons.at(1)->py()+genMet->py(),2);
			if(Esq-Psq >0)
			MT_gen = sqrt(Esq-Psq);


			histMan_->H2D_BJetBinned("GenHTPlusMETPt_vs_RecoHTPlusMetPt")->Fill(GenHT+event->GenMET()->pt(),HT+met->pt(), weight_);
			histMan_->H2D_BJetBinned("GenHT_vs_RecoHT")->Fill(GenHT,HT,  weight_);
			histMan_->H2D_BJetBinned("GenM3_vs_RecoM3")->Fill(GenM3,RecoM3,  weight_);

			histMan_->H2D_BJetBinned("GenNJets_vs_RecoNJets")->Fill(numberOfGenJets,numberOfJets, weight_);


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
		histMan_->H2D_BJetBinned("GenLepPlusMETPt_vs_RecoLepPlusMetPt")->Fill(genLeptPt+event->GenMET()->pt(),signalLepton->pt()+met->pt(), weight_);
	    histMan_->H2D_BJetBinned("GendPhiLepMet_vs_RecodPhiLepMetPt")->Fill(gendPhi,signalLepton->deltaPhi(met), weight_);
		histMan_->H2D_BJetBinned("GenLepPt_vs_RecoLepPt")->Fill(genLeptPt, signalLepton->pt(),weight_);
		histMan_->H2D_BJetBinned("GenHT_lep_vs_RecoHT_lep")->Fill(GenHT_lepton,HT_lepton, weight_);
		histMan_->H2D_BJetBinned("GenHT_lep_met_vs_RecoHT_lep_met")->Fill(GenHT_lepton_MET,HT_lepton_MET,  weight_);
		histMan_->H2D_BJetBinned("Gen_WPT_vs_Reco_WPT")->Fill(genWPt,W_boson->pt(),  weight_);
		histMan_->H2D_BJetBinned("Gen_MT_vs_Reco_MT")->Fill(MT_gen,MT,  weight_);
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
		const METPointer genMet(event->GenMET());

		//added for gen jet cleaning
		JetCollection cleanedGenJets;
		MCParticleCollection genElectrons;
		MCParticleCollection allGenElectrons;

		ParticlePointer W_boson;
		W_boson = ParticlePointer(new Particle(*met + *signalLepton));

		ParticlePointer M3;
		ParticlePointer GeneratorM3;

//		unsigned int numberOfGenJets(genJets.size());
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

			if(abs(genPart.at(i)->pdgId())==(11)){
				MCParticlePointer ele(genPart.at(i));
				allGenElectrons.push_back(ele);
			}

			// find the electron
			if(abs(genPart.at(i)->pdgId())==(11) && abs(genPart.at(genPart.at(i)->motherIndex())->pdgId())==(24)){
				oneElectron = true;
				electronID = genPart.at(i)->pdgId();
				genLeptPt = genPart.at(i)->pt();
				gendPhi = genPart.at(i)->deltaPhi(event->GenMET());
				genWPt = genPart.at(genPart.at(i)->motherIndex())->pt();
				MCParticlePointer genElectron(genPart.at(i));
				genElectrons.push_back(genElectron);

				ParticlePointer genW_check(new Particle(*genMet + *genElectron));

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

		double minDR = 99999999.;
		double minDR2 = 99999999.;

		if (oneElectron == true){

			for(unsigned int i = 0; i < genJets.size(); i++) {
				JetPointer generatorJet(genJets.at(i));

				double deltaRele = generatorJet->deltaR(genElectrons.at(0));
				if(deltaRele < minDR)
					minDR = deltaRele;

				if(!generatorJet->isWithinDeltaR(0.1, genElectrons.at(0)))
				cleanedGenJets.push_back(generatorJet);
			}

			for(unsigned int i = 0; i < genJets.size(); i++) {
				JetPointer generatorJet(genJets.at(i));
				double deltaRele = generatorJet->deltaR(genElectrons.at(0));
				if(deltaRele > minDR && deltaRele < minDR2)
						minDR2 = deltaRele;
			}
		}

		if (oneElectron == true){
			histMan_->H1D_BJetBinned("deltaR Gen Ele bin")->Fill(minDR,weight_);
			histMan_->H1D_BJetBinned("deltaR Gen Ele")->Fill(minDR,weight_);
			histMan_->H1D_BJetBinned("deltaR Gen Ele2")->Fill(minDR2,weight_);
		}

		unsigned int numberOfCleanedGenJets(cleanedGenJets.size());

		HT = Event::HT(jets);
		GenHT = Event::HT(cleanedGenJets);

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
		for (unsigned int a = 0; a < numberOfCleanedGenJets; ++a) {
			for (unsigned int b = 1; b < numberOfCleanedGenJets; ++b) {
				for (unsigned int c = 2; c < numberOfCleanedGenJets; ++c) {
					if(a!=b && a!=c){
						GeneratorM3 = ParticlePointer(new Particle(*cleanedGenJets.at(a) + *cleanedGenJets.at(b)+ *cleanedGenJets.at(c)));
					    double pt =  GeneratorM3->pt();

					    if(pt>ptGenMax){
					    	ptGenMax = pt;
					        GenM3 = GeneratorM3->mass();
					    }
					}
				}
			}
		}


		if(oneElectron){

			HT_lepton = HT + signalLepton->pt();
			GenHT_lepton = GenHT + allGenElectrons.at(1)->pt();
			HT_lepton_MET = Event::ST(jets, signalElectron, met);
			GenHT_lepton_MET = GenHT_lepton + event->GenMET()->et();
			double MT = Event::MT(signalElectron,met);

	        double MT_gen = -1;
			double Esq= pow(allGenElectrons.at(1)->et()+genMet->et(),2);
			double Psq= pow(allGenElectrons.at(1)->px()+genMet->px(),2)+pow(allGenElectrons.at(1)->py()+genMet->py(),2);
			if(Esq-Psq >0)
			MT_gen = sqrt(Esq-Psq);

			histMan_->H2D_BJetBinned("GenHTPlusMETPt_vs_RecoHTPlusMetPt")->Fill(GenHT+event->GenMET()->pt(),HT+met->pt(), weight_);
			histMan_->H2D_BJetBinned("GenHT_vs_RecoHT")->Fill(GenHT,HT,  weight_);
			histMan_->H2D_BJetBinned("GenM3_vs_RecoM3")->Fill(GenM3,RecoM3,  weight_);

			histMan_->H2D_BJetBinned("GenNJets_vs_RecoNJets")->Fill(numberOfCleanedGenJets,numberOfJets, weight_);


			if(numberOfJets>0 && numberOfCleanedGenJets>0){
	        histMan_->H2D_BJetBinned("GenJet1Pt_vs_RecoJet1Pt")->Fill(cleanedGenJets.at(0)->pt(),jets.at(0)->pt(), weight_);}
	        if(numberOfJets>1 && numberOfCleanedGenJets>1){
	        histMan_->H2D_BJetBinned("GenJet2Pt_vs_RecoJet2Pt")->Fill(cleanedGenJets.at(1)->pt(),jets.at(1)->pt(), weight_);}
	        if(numberOfJets>2 && numberOfCleanedGenJets>2){
	        histMan_->H2D_BJetBinned("GenJet3Pt_vs_RecoJet3Pt")->Fill(cleanedGenJets.at(2)->pt(),jets.at(2)->pt(), weight_);}
	        if(numberOfJets>3 && numberOfCleanedGenJets>3){
	        histMan_->H2D_BJetBinned("GenJet4Pt_vs_RecoJet4Pt")->Fill(cleanedGenJets.at(3)->pt(),jets.at(3)->pt(), weight_);}
	        if(numberOfJets>4 && numberOfCleanedGenJets>4){
	        histMan_->H2D_BJetBinned("GenJet5Pt_vs_RecoJet5Pt")->Fill(cleanedGenJets.at(4)->pt(),jets.at(4)->pt(), weight_);}

			histMan_->H2D_BJetBinned("GenLepPlusMETPt_vs_RecoLepPlusMetPt")->Fill(genLeptPt+event->GenMET()->pt(),signalLepton->pt()+met->pt(), weight_);
		    histMan_->H2D_BJetBinned("GendPhiLepMet_vs_RecodPhiLepMetPt")->Fill(gendPhi,signalLepton->deltaPhi(met), weight_);
			histMan_->H2D_BJetBinned("GenLepPt_vs_RecoLepPt")->Fill(genLeptPt, signalLepton->pt(),weight_);
			histMan_->H2D_BJetBinned("GenHT_lep_vs_RecoHT_lep")->Fill(GenHT_lepton,HT_lepton, weight_);
			histMan_->H2D_BJetBinned("GenHT_lep_met_vs_RecoHT_lep_met")->Fill(GenHT_lepton_MET,HT_lepton_MET,  weight_);
			histMan_->H2D_BJetBinned("Gen_WPT_vs_Reco_WPT")->Fill(genWPt,W_boson->pt(),  weight_);
			histMan_->H2D_BJetBinned("Gen_MT_vs_Reco_MT")->Fill(MT_gen, MT,  weight_);
			histMan_->H2D_BJetBinned("GenNu_vs_RecoMET")->Fill(GenNuPt, met->pt(), weight_);
			histMan_->H2D_BJetBinned("GenParton_vs_RecoHT")->Fill(partonPt, HT, weight_);
			histMan_->H2D_BJetBinned("GenJetHT_vs_GenParton")->Fill(GenHT, partonPt, weight_);
		}

	}

}

void BinningAnalyser::ePlusJetsHTAnalysis(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/EPlusJetsHTstep0/");
	weight_ = event->weight() * prescale_ * scale_;

	if (topEplusJetsRefSelection_->passesSelectionUpToStep(event,0)) {

		const JetCollection jets(event->Jets());
		const JetCollection genJets(event->GenJets());

		unsigned int numberOfGenJets(genJets.size());
		unsigned int numberOfJets(jets.size());

		//		unsigned int numberOfBJets(bJets.size());
		double HT = 0;
		double GenHT = 0;
		double dRJets = 0;

		for (unsigned int i = 0; i < numberOfJets; ++i) {
			HT = HT + jets.at(i)->pt();
		}
		for (unsigned int i = 0; i < numberOfGenJets; ++i) {
			GenHT = GenHT + genJets.at(i)->pt();
		}

		histMan_->H2D_BJetBinned("GenHT_vs_RecoHT")->Fill(GenHT,HT,  weight_);

		histMan_->H2D_BJetBinned("GenNJets_vs_RecoNJets")->Fill(numberOfGenJets,numberOfJets, weight_);


		if(numberOfJets>0 && numberOfGenJets>0){
			dRJets = jets.at(0)->deltaR(genJets.at(0));
        histMan_->H2D_BJetBinned("GenJet1Pt_vs_RecoJet1Pt")->Fill(genJets.at(0)->pt(),jets.at(0)->pt(), weight_);}
		histMan_->H1D_BJetBinned("deltaR GenReco1")->Fill(dRJets,weight_);
		if(numberOfJets>1 && numberOfGenJets>1){
			dRJets = jets.at(1)->deltaR(genJets.at(1));
			histMan_->H1D_BJetBinned("deltaR GenReco2")->Fill(dRJets,weight_);
        histMan_->H2D_BJetBinned("GenJet2Pt_vs_RecoJet2Pt")->Fill(genJets.at(1)->pt(),jets.at(1)->pt(), weight_);}
        if(numberOfJets>2 && numberOfGenJets>2){
        	dRJets = jets.at(1)->deltaR(genJets.at(1));
        	histMan_->H1D_BJetBinned("deltaR GenReco3")->Fill(dRJets,weight_);
        histMan_->H2D_BJetBinned("GenJet3Pt_vs_RecoJet3Pt")->Fill(genJets.at(2)->pt(),jets.at(2)->pt(), weight_);}
        if(numberOfJets>3 && numberOfGenJets>3){
        	dRJets = jets.at(1)->deltaR(genJets.at(1));
        	histMan_->H1D_BJetBinned("deltaR GenReco4")->Fill(dRJets,weight_);
        histMan_->H2D_BJetBinned("GenJet4Pt_vs_RecoJet4Pt")->Fill(genJets.at(3)->pt(),jets.at(3)->pt(), weight_);}
        if(numberOfJets>4 && numberOfGenJets>4){
        	dRJets = jets.at(1)->deltaR(genJets.at(1));
        	histMan_->H1D_BJetBinned("deltaR GenReco5")->Fill(dRJets,weight_);
        histMan_->H2D_BJetBinned("GenJet5Pt_vs_RecoJet5Pt")->Fill(genJets.at(4)->pt(),jets.at(4)->pt(), weight_);}


	}

}

void BinningAnalyser::ePlusJetsHTskimAnalysis(const EventPtr event) {
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/EPlusJetsHTclean/");
	weight_ = event->weight() * prescale_ * scale_;

	if (topEplusJetsRefSelection_->passesSelectionUpToStep(event,1)) {

		const JetCollection jets(event->Jets());
		const JetCollection genJets(event->GenJets());

		unsigned int numberOfGenJets(genJets.size());
		unsigned int numberOfJets(jets.size());

		//		unsigned int numberOfBJets(bJets.size());
		double HT = 0;
		double GenHT = 0;


		for (unsigned int i = 0; i < numberOfJets; ++i) {
			HT = HT + jets.at(i)->pt();
		}
		for (unsigned int i = 0; i < numberOfGenJets; ++i) {
			GenHT = GenHT + genJets.at(i)->pt();
		}


		histMan_->H2D_BJetBinned("GenHT_vs_RecoHT")->Fill(GenHT,HT,  weight_);

		histMan_->H2D_BJetBinned("GenNJets_vs_RecoNJets")->Fill(numberOfGenJets,numberOfJets, weight_);


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


	}

}
void BinningAnalyser::createHistograms() {
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/EPlusJets/");

	histMan_->addH2D_BJetBinned("GenNJets_vs_RecoNJets", "GenNJets_vs_RecoNJets; N Jets_{Gen}; N Jets_{Reco}", 20,
			0, 20, 20, 0, 20);
	histMan_->addH1D_BJetBinned("deltaR Gen Ele", "deltaR Ele; Delta R", 100,
				0, 0.5);
	histMan_->addH1D_BJetBinned("deltaR Gen Ele2", "deltaR Ele 2; Delta R", 500,
					0, 4);
	histMan_->addH1D_BJetBinned("deltaR Gen Ele bin", "deltaR Ele; Delta R", 500,
					0, 4);

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
	histMan_->addH2D_BJetBinned("Gen_WPT_vs_Reco_WPT", "Gen_WPT_vs_Reco_WPT; leptonic_W_pt_{Gen} [GeV]; leptonic_W_pt_{Reco} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("Gen_MT_vs_Reco_MT", "Gen_MT_vs_Reco_MT; MT_{Gen} [GeV]; MT_{Reco} [GeV]", 300,
								0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("GenM3_vs_RecoM3", "GenM3_vs_RecoM3; M3_{Gen} [GeV]; M3_{Reco} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("GenNu_vs_RecoMET", "GenMET_vs_RecoNu; MET_{Gen} [GeV]; MET_{Reco} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("GenParton_vs_RecoHT", "GenHT_vs_RecoPartonHT; HT_parton_{Gen} [GeV]; HT_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
	histMan_->addH2D_BJetBinned("GenJetHT_vs_GenParton", "GenJetHT_vs_RecoPartonHT; HT_parton_{Gen} [GeV]; HT_{Gen} [GeV]", 500,
						0, 2000, 500, 0, 2000);

	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/EPlusJetsHTstep0/");

	histMan_->addH1D_BJetBinned("deltaR GenReco1", "deltaR (GenReco); Events", 500,
			0, 5);
	histMan_->addH1D_BJetBinned("deltaR GenReco2", "deltaR (GenReco); Events", 500,
			0, 5);
	histMan_->addH1D_BJetBinned("deltaR GenReco3", "deltaR (GenReco); Events", 500,
			0, 5);
	histMan_->addH1D_BJetBinned("deltaR GenReco4", "deltaR (GenReco); Events", 500,
			0, 5);
	histMan_->addH1D_BJetBinned("deltaR GenReco5", "deltaR (GenReco); Events", 500,
			0, 5);

	histMan_->addH2D_BJetBinned("GenNJets_vs_RecoNJets", "GenNJets_vs_RecoNJets; N Jets_{Gen}; N Jets_{Reco}", 20,
			0, 20, 20, 0, 20);
	histMan_->addH2D_BJetBinned("GenHT_vs_RecoHT", "GenHT_vs_RecoHT; HT_{Gen} [GeV]; HT_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
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

	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/EPlusJetsHTclean/");

	histMan_->addH2D_BJetBinned("GenNJets_vs_RecoNJets", "GenNJets_vs_RecoNJets; N Jets_{Gen}; N Jets_{Reco}", 20,
			0, 20, 20, 0, 20);
	histMan_->addH2D_BJetBinned("GenHT_vs_RecoHT", "GenHT_vs_RecoHT; HT_{Gen} [GeV]; HT_{Reco} [GeV]", 500,
						0, 2000, 500, 0, 2000);
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
	histMan_->setCurrentHistogramFolder(histogramFolder_+ "/MuPlusJets/");

	histMan_->addH2D_BJetBinned("GenNJets_vs_RecoNJets", "GenNJets_vs_RecoNJets; N Jets_{Gen}; N Jets_{Reco}", 20,
			0, 20, 20, 0, 20);
	histMan_->addH1D_BJetBinned("deltaR Gen Mu", "deltaR Mu; Delta R", 500,
					0, 4);

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
	histMan_->addH2D_BJetBinned("Gen_WPT_vs_Reco_WPT", "Gen_WPT_vs_Reco_WPT; WPT_{Gen} [GeV]; WPT_{Reco} [GeV]", 300,
						0, 300, 300, 0, 300);
	histMan_->addH2D_BJetBinned("Gen_MT_vs_Reco_MT", "Gen_MT_vs_Reco_MT; MT_{Gen} [GeV]; MT_{Reco} [GeV]", 300,
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
		histMan_->addH2D_BJetBinned("GenNu_vs_RecoMET", "GenNu_vs_RecoMET; Nu_pt_{GEN} [GeV]; MET_{RECO} [GeV]", 300,
									0, 300, 300, 0, 300);
	}
	for (unsigned index = 0; index < METAlgorithm::NUMBER_OF_METALGORITHMS; ++index) {
		std::string prefix = METAlgorithm::prefixes.at(index);
		histMan_->setCurrentHistogramFolder(histogramFolder_+ "/ElectronMET/"+prefix);
		histMan_->addH2D_BJetBinned("GenMET_vs_RecoMET", "GenMET_vs_RecoMET; HT_{GEN} [GeV]; HT_{RECO} [GeV]", 300,
							0, 300, 300, 0, 300);
		histMan_->addH2D_BJetBinned("GenNu_vs_RecoMET", "GenNu_vs_RecoMET; Nu_pt_{GEN} [GeV]; MET_{RECO} [GeV]", 300,
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
