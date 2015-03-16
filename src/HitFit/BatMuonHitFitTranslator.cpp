//
//     $Id: BatMuonHitFitTranslator.cc,v 1.8 2010/08/06 22:03:03 haryo Exp $
//
/**
    @file BatMuonHitFitTranslator.cc

    @brief Specialization of template class LeptonTranslatorBase in the
    package HitFit for pat::Muon.

    @author Greg Heath, after Haryo Sumowidagdo <Suharyo.Sumowidagdo@cern.ch>

    @par Created
    May 04, 2011

    @version $Id: BatMuonHitFitTranslator.cc,v 1.8 2010/08/06 22:03:03 haryo Exp $
 */


#include "TopQuarkAnalysis/TopHitFit/interface/LeptonTranslatorBase.h"
#include "../../interface/RecoObjects/Muon.h"

namespace hitfit {


template<>
LeptonTranslatorBase<BAT::Muon>::LeptonTranslatorBase()
{

//    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string resolution_filename = //CMSSW_BASE +
        std::string("TopQuarkAnalysis/PatHitFit/data/exampleMuonResolution.txt");
    resolution_ = EtaDepResolution(resolution_filename);

} // LeptonTranslatorBase<BAT::Muon>::LeptonTranslatorBase()


template<>
LeptonTranslatorBase<BAT::Muon>::LeptonTranslatorBase(const std::string& ifile)
{

//    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string resolution_filename;

    if (ifile.empty()) {
        resolution_filename = //CMSSW_BASE +
        std::string("TopQuarkAnalysis/PatHitFit/data/exampleMuonResolution.txt");
    } else {
        resolution_filename = ifile ;
    }

    resolution_ = EtaDepResolution(resolution_filename);

} // LeptonTranslatorBase<BAT::Muon>::LeptonTranslatorBase(const std::string& s)


template<>
LeptonTranslatorBase<BAT::Muon>::~LeptonTranslatorBase()
{
} // LeptonTranslatorBase<BAT::Muon>::~LeptonTranslatorBase()


template<>
Lepjets_Event_Lep
LeptonTranslatorBase<BAT::Muon>::operator()(const BAT::Muon& lepton,
                                            int type /*= hitfit::lepton_label */,
                                            bool useObjEmbRes /* = false */)
{

    Fourvec p(lepton.px(),lepton.py(),lepton.pz(),lepton.energy());

    double            muon_eta        = lepton.eta();
    Vector_Resolution muon_resolution = resolution_.GetResolution(muon_eta);

    Lepjets_Event_Lep muon(p,
                           muon_label,
                           muon_resolution);
    return muon;

} // Lepjets_Event_Lep LeptonTranslatorBase<BAT::Muon>::operator()


template<>
const EtaDepResolution&
LeptonTranslatorBase<BAT::Muon>::resolution() const
{
    return resolution_;
}


template<>
bool
LeptonTranslatorBase<BAT::Muon>::CheckEta(const BAT::Muon& lepton) const
{
    return resolution_.CheckEta(lepton.eta());
}


} // namespace hitfit
