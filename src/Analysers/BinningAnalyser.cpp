/*
 * BinningAnalyser.cpp
 *
 * Since a lot of changes have been done compared to the last implementation,
 * here is the link to that version:
 * https://github.com/BristolTopGroup/AnalysisSoftware/blob/9681889f822f4d50ed2e89417d71146ad59e68d4/src/Analysers/BinningAnalyser.cpp
 */

#include "../../interface/Analysers/BinningAnalyser.h"

namespace BAT {

void BinningAnalyser::analyse(const EventPtr event) {
	//This analyser only works on MC
	if (event->isRealData())
		return;

	if (!have_histograms_been_created_)
		createHistograms();

	histMan_->setCurrentHistogramFolder(histogramFolder_);
	weight_ = event->weight() * prescale_ * scale_;

	const MCParticleCollection gen_particles(event->GenParticles());
	const JetCollection gen_jets(event->GenJets());

	top_event_ = TopPairMCEvent(gen_particles);

	JetCollection jets;
	LeptonPointer signal_lepton;

	if (top_event_.is_semi_leptonic()) {
		if (top_event_.get_top_pair_decay() == semi_leptonic_electron) {
			if (!top_e_plus_jets_ref_selection_->passesFullSelectionExceptLastTwoSteps(event))
				return;

			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/");
			jets = top_e_plus_jets_ref_selection_->cleanedJets(event);
			signal_lepton = top_e_plus_jets_ref_selection_->signalLepton(event);
		} else if (top_event_.get_top_pair_decay() == semi_leptonic_muon) {
			if (!top_mu_plus_jets_ref_selection_->passesFullSelectionExceptLastTwoSteps(event))
				return;

			histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/");
			jets = top_mu_plus_jets_ref_selection_->cleanedJets(event);
			signal_lepton = top_mu_plus_jets_ref_selection_->signalLepton(event);

		} else
			return;

		METAlgorithm::value met_type = Globals::metAlgorithm;
		const METPointer met(event->MET(met_type));
		const METPointer gen_met(event->GenMET());

		const MCParticlePointer lepton(top_event_.get_single_lepton());
		const MCParticlePointer neutrino(top_event_.get_single_neutrino());
		const METPointer gen_nu(new MET(neutrino->px(), neutrino->py()));

		// clean gen jets
		JetCollection clean_gen_jets;
		for (unsigned int i = 0; i < gen_jets.size(); ++i) {
			if (lepton->deltaR(gen_jets.at(i)) > 0.3)
				clean_gen_jets.push_back(gen_jets.at(i));
		}

		double HT = Event::HT(jets);
		double HT_gen = Event::HT(clean_gen_jets);
		//			//parton level Ht
		//			if (((abs(genPart.at(i)->pdgId()) > 0 && abs(genPart.at(i)->pdgId()) < 6)
		//					&& (abs(genPart.at(genPart.at(i)->motherIndex())->pdgId()) == (6)
		//							|| abs(genPart.at(genPart.at(i)->motherIndex())->pdgId()) == (24)))
		//					|| ((genPart.at(i)->pdgId() == 21
		//							|| (abs(genPart.at(i)->pdgId()) > 0 && abs(genPart.at(i)->pdgId()) < 6))
		//							&& genPart.at(i)->motherIndex() == 4)) {
		//				partonPt = partonPt + genPart.at(i)->pt();

		double ST = Event::ST(jets, signal_lepton, met);
		double ST_gen = Event::ST(clean_gen_jets, lepton, gen_met);
		double ST_gen_nu = Event::ST(clean_gen_jets, lepton, gen_nu);

		double M3 = Event::M3(jets);
		double M3_gen = Event::M3(gen_jets);

		double MT = Event::MT(signal_lepton, met);
		double MT_gen = Event::MT(lepton, gen_met);
		double MT_gen_nu = Event::MT(lepton, gen_nu);

		double WPT = Event::WPT(signal_lepton, met);
		double WPT_gen = Event::WPT(lepton, gen_met);
		double WPT_gen_nu = Event::WPT(lepton, gen_nu);
		// fill MET
		histMan_->H2D("MET_gen_vs_reco")->Fill(gen_met->et(), met->et(), weight_);
		histMan_->H2D("MET_gen_nu_vs_reco")->Fill(neutrino->pt(), met->et(), weight_);

		//fill HT
//		histMan_->H2D("HT_genParton_vs_reco");
		histMan_->H2D("HT_genJet_vs_reco")->Fill(HT_gen, HT, weight_);

		histMan_->H2D("ST_gen_vs_reco")->Fill(ST_gen, ST, weight_);
		histMan_->H2D("ST_gen_nu_vs_reco")->Fill(ST_gen_nu, ST, weight_);

		histMan_->H2D("M3_gen_vs_reco")->Fill(M3_gen, M3, weight_);

		histMan_->H2D("MT_gen_vs_reco")->Fill(MT_gen, MT, weight_);
		histMan_->H2D("MT_gen_nu_vs_reco")->Fill(MT_gen_nu, MT, weight_);

		histMan_->H2D("WPT_gen_vs_reco")->Fill(WPT_gen, WPT, weight_);
		histMan_->H2D("WPT_gen_nu_vs_reco")->Fill(WPT_gen_nu, WPT, weight_);
	}
}

void BinningAnalyser::createHistograms() {
	unsigned int number_of_histograms = histMan_->size();

	// electrons
	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/EPlusJets/");
	create_histograms();
	// muons
	histMan_->setCurrentHistogramFolder(histogramFolder_ + "/MuPlusJets/");
	create_histograms();

	have_histograms_been_created_ = true;
	if (Globals::verbose) {
		cout << "Number of histograms added by binningAnalyser: " << histMan_->size() - number_of_histograms << endl;
	}
}

void BinningAnalyser::create_histograms() {
	histMan_->addH2D("MET_gen_vs_reco", "MET_{Gen} [GeV]; MET_{Reco} [GeV]", 1000, 0, 1000, 1000, 0,
			1000);
	histMan_->addH2D("MET_gen_nu_vs_reco", "MET_gen_nu_vs_reco; MET_{Gen} [GeV]; MET_{Reco} [GeV]", 1000, 0, 1000, 1000,
			0, 1000);

//	histMan_->addH2D("HT_genParton_vs_reco", "HT_genParton_vs_reco; HT_parton_{Gen} [GeV]; HT_{Reco} [GeV]", 2000, 0,
//			2000, 2000, 0, 2000);
	histMan_->addH2D("HT_genJet_vs_reco", "HT_genJet_vs_reco; HT_jet_{Gen} [GeV]; HT_{Gen} [GeV]", 2000, 0, 2000, 2000,
			0, 2000);

	histMan_->addH2D("ST_gen_vs_reco", "ST_gen_vs_reco; ST{Gen} [GeV]; ST{Reco} [GeV]", 2500, 0, 2500, 2500, 0, 2500);
	histMan_->addH2D("ST_gen_nu_vs_reco", "ST_gen_nu_vs_reco; ST{Gen} [GeV]; ST{Reco} [GeV]", 2500, 0, 2500, 2500, 0,
			2500);

	histMan_->addH2D("MT_gen_vs_reco", "MT_gen_vs_reco; MT_{Gen} [GeV]; MT_{Reco} [GeV]", 1000, 0, 1000, 1000, 0, 1000);
	histMan_->addH2D("MT_gen_nu_vs_reco", "MT_gen_nu_vs_reco; MT_{Gen} [GeV]; MT_{Reco} [GeV]", 1000, 0, 1000, 1000, 0,
			1000);

	histMan_->addH2D("WPT_gen_vs_reco", "WPT_gen_vs_reco; WPT_{Gen} [GeV]; WPT_{Reco} [GeV]", 1000, 0, 1000, 1000, 0,
			1000);
	histMan_->addH2D("WPT_gen_nu_vs_reco", "WPT_gen_nu_vs_reco; WPT_{Gen} [GeV]; WPT_{Reco} [GeV]", 1000, 0, 1000, 1000,
			0, 1000);

	histMan_->addH2D("M3_gen_vs_reco", "M3_gen_vs_reco; M3_{Gen} [GeV]; M3_{Reco} [GeV]", 2000, 0, 2000, 2000, 0, 2000);
}

BinningAnalyser::BinningAnalyser(HistogramManagerPtr histMan, std::string histogramFolder) :
		BasicAnalyser(histMan, histogramFolder), //
		//signal selections
		top_e_plus_jets_ref_selection_(new TopPairEPlusJetsReferenceSelection()), //
		top_mu_plus_jets_ref_selection_(new TopPairMuPlusJetsReferenceSelection()), //
		top_event_() {

}

BinningAnalyser::~BinningAnalyser() {
}

} /* namespace BAT */
