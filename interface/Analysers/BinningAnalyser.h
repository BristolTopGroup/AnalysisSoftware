 //BinningAnalyser.h

#ifndef BINNINGANALYSER_H_
#define BINNINGANALYSER_H_

#include "BasicAnalyser.h"
#include "../Selections/TopPairEPlusJetsReferenceSelection.h"
#include "../Selections/TopPairMuPlusJetsReferenceSelection.h"
#include "../TopPairMCEvent.h"

namespace BAT {

class BinningAnalyser: public BAT::BasicAnalyser {
public:
	BinningAnalyser(HistogramManagerPtr histMan, std::string histogramFolder = "Binning");
	virtual ~BinningAnalyser();
	virtual void analyse(const EventPtr);
	virtual void createHistograms();

private:
	SelectionPointer top_e_plus_jets_ref_selection_, top_mu_plus_jets_ref_selection_;
	TopPairMCEvent top_event_;

	void electron_plus_jets_analysis(const EventPtr event);
	void muon_plus_jets_analysis(const EventPtr event);
	void create_histograms();

	const ParticlePointer find_gen_lepton(unsigned int lepton_id, const MCParticleCollection particles);
	const ParticlePointer find_gen_neutrino(unsigned int neutrino_id, const MCParticleCollection particles);
};

} /* namespace BAT */
#endif /* BINNINGANALYSER_H_ */
