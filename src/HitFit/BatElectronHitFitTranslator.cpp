 //
// $Id: BatElectronHitFitTranslator.cc,v 1.8 2010/08/06 22:02:52 haryo Exp $
//

/**
    @file BatElectronHitFitTranslator.cc

    @brief Specialization of template class LeptonTranslatorBase in the
    package HitFit for BAT::Electron.

    @author Greg Heath, after Haryo Sumowidagdo <Suharyo.Sumowidagdo@cern.ch>

    @par Created
    May 04, 2011

    @version $Id: BatElectronHitFitTranslator.cc,v 1.8 2010/08/06 22:02:52 haryo Exp $
 */


#include "TopQuarkAnalysis/TopHitFit/interface/LeptonTranslatorBase.h"
#include "../../interface/RecoObjects/Electron.h"

namespace hitfit {

template<>
LeptonTranslatorBase<BAT::Electron>::LeptonTranslatorBase()
{

//    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string resolution_filename = //CMSSW_BASE +
        std::string("TopQuarkAnalysis/PatHitFit/data/exampleElectronResolution.txt");
    resolution_ = EtaDepResolution(resolution_filename);

} // LeptonTranslatorBase<BAT::Electron>::LeptonTranslatorBase()


template<>
LeptonTranslatorBase<BAT::Electron>::LeptonTranslatorBase(const std::string& ifile)
{

//    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string resolution_filename;

    if (ifile.empty()) {
        resolution_filename = //CMSSW_BASE +
        std::string("TopQuarkAnalysis/PatHitFit/data/exampleElectronResolution.txt");
    } else {
        resolution_filename = ifile ;
    }

    resolution_ = EtaDepResolution(resolution_filename);

} // LeptonTranslatorBase<BAT::Electron>::LeptonTranslatorBase(const std::string& ifile)


template<>
LeptonTranslatorBase<BAT::Electron>::~LeptonTranslatorBase()
{
}


template<>
Lepjets_Event_Lep
LeptonTranslatorBase<BAT::Electron>::operator()(const BAT::Electron& lepton,
                                                int type /* = hitfit::lepton_label */,
                                                bool useObjEmbRes /* = false */)
{

    Fourvec p(lepton.px(),lepton.py(),lepton.pz(),lepton.energy());

    double            electron_eta        = lepton.eta();
    Vector_Resolution electron_resolution = resolution_.GetResolution(electron_eta);

    Lepjets_Event_Lep electron(p,
                               electron_label,
                               electron_resolution);
    return electron;

} // Lepjets_Event_Lep LeptonTranslatorBase<BAT::Electron>::operator()


template<>
const EtaDepResolution&
LeptonTranslatorBase<BAT::Electron>::resolution() const
{
    return resolution_;
}


template<>
bool
LeptonTranslatorBase<BAT::Electron>::CheckEta(const BAT::Electron& lepton) const
{
    double            electron_eta        = lepton.eta();
    return resolution_.CheckEta(electron_eta);
}


} // namespace hitfit
