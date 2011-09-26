/**
    @file BatJetHitFitTranslator.cc

    @brief Specialization of template class JetTranslatorBase in the
    package HitFit

    @author Greg Heath, after Haryo Sumowidagdo <Suharyo.Sumowidagdo@cern.ch>

    @par Created
    May 04, 2011

    @version $Id: BatJetHitFitTranslator.cc,v 1.11 2011/01/26 16:07:59 haryo Exp $
 */


#include <TopQuarkAnalysis/TopHitFit/interface/JetTranslatorBase.h>
#include "../../interface/RecoObjects/Jet.h"

namespace hitfit {


template<>
JetTranslatorBase<BAT::Jet>::JetTranslatorBase()
{

//    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string resolution_filename = //CMSSW_BASE +
        std::string("TopQuarkAnalysis/PatHitFit/data/exampleJetResolution.txt");
    udscResolution_ = EtaDepResolution(resolution_filename);
    bResolution_    = EtaDepResolution(resolution_filename);

} // JetTranslatorBase<BAT::Jet>::JetTranslatorBase()


template<>
JetTranslatorBase<BAT::Jet>::JetTranslatorBase(const std::string& udscFile,
                                               const std::string& bFile)
{

//    std::string CMSSW_BASE(getenv("CMSSW_BASE"));
    std::string udscResolution_filename;
    std::string bResolution_filename;

    if (udscFile.empty()) {
        udscResolution_filename = //CMSSW_BASE +
        std::string("TopQuarkAnalysis/PatHitFit/data/exampleJetResolution.txt");
    } else {
        udscResolution_filename = udscFile;
    }

    if (bFile.empty()) {
        bResolution_filename = //CMSSW_BASE +
        std::string("TopQuarkAnalysis/PatHitFit/data/exampleJetResolution.txt");
    } else {
        bResolution_filename = bFile;
    }

    udscResolution_ = EtaDepResolution(udscResolution_filename);
    bResolution_    = EtaDepResolution(bResolution_filename);

} // JetTranslatorBase<BAT::Jet>::JetTranslatorBase(const std::string& ifile)


template<>
JetTranslatorBase<BAT::Jet>::~JetTranslatorBase()
{
} // JetTranslatorBase<BAT::Jet>::~JetTranslatorBase()


template<>
Lepjets_Event_Jet
JetTranslatorBase<BAT::Jet>::operator()(const BAT::Jet& jet,
                                        int type /*= hitfit::unknown_label */,
                                        bool useObjEmbRes /* = false */)
{

    Fourvec p;

    double            jet_eta        = jet.eta();

    Vector_Resolution jet_resolution;

//     if (type == hitfit::hadb_label || type == hitfit::lepb_label || type == hitfit::higgs_label) {
//         jet_resolution = bResolution_.GetResolution(jet_eta);
//         pat::Jet bPartonCorrJet(jet.correctedJet("L7Parton","BOTTOM"));
//         p = Fourvec(bPartonCorrJet.px(),bPartonCorrJet.py(),bPartonCorrJet.pz(),bPartonCorrJet.energy());

//     } else {
//         jet_resolution = udscResolution_.GetResolution(jet_eta);
//         pat::Jet udsPartonCorrJet(jet.correctedJet("L7Parton","UDS"));
//         p = Fourvec(udsPartonCorrJet.px(),udsPartonCorrJet.py(),udsPartonCorrJet.pz(),udsPartonCorrJet.energy());
//     }

// Need to understand how to implement flavour-dependent jet corrections!
// But for now, no corrections ...

    if (type == hitfit::hadb_label || type == hitfit::lepb_label || type == hitfit::higgs_label) {
        jet_resolution = bResolution_.GetResolution(jet_eta);
    } else {
        jet_resolution = udscResolution_.GetResolution(jet_eta);
    }
    p = Fourvec(jet.px(), jet.py(), jet.pz(), jet.energy());


    Lepjets_Event_Jet retjet(p,
                             type,
                             jet_resolution);
    return retjet;

} // Lepjets_Event_Jet JetTranslatorBase<BAT::Jet>::operator()(const BAT::Jet& j,int type)


template<>
const EtaDepResolution&
JetTranslatorBase<BAT::Jet>::udscResolution() const
{
    return udscResolution_;
}


template<>
const EtaDepResolution&
JetTranslatorBase<BAT::Jet>::bResolution() const
{
    return bResolution_;
}


template<>
bool
JetTranslatorBase<BAT::Jet>::CheckEta(const BAT::Jet& jet) const
{
    double            jet_eta        = jet.eta();

    return bResolution_.CheckEta(jet_eta) && udscResolution_.CheckEta(jet_eta);
}


    //

} // namespace hitfit
