//
//     $Id: BatMETHitFitTranslator.cc,v 1.7 2011/01/29 22:21:38 haryo Exp $
//

/**
    @file BatMETHitFitTranslator.cc

    @brief Specialization of template class METTranslatorBase in the
    package HitFit for BAT::MET

    @author Greg Heath, after Haryo Sumowidagdo <Suharyo.Sumowidagdo@cern.ch>

    @par Created
    May 04, 2011

    @version $Id: BatMETHitFitTranslator.cc,v 1.7 2011/01/29 22:21:38 haryo Exp $
 */


#include "TopQuarkAnalysis/TopHitFit/interface/METTranslatorBase.h"
#include "../../interface/RecoObjects/MET.h"

#include <cmath>

namespace hitfit {


template<>
METTranslatorBase<BAT::MET>::METTranslatorBase()
{
    resolution_ = Resolution(std::string("0,0,12"));
} // METTranslatorBase<BAT::MET>::METTranslatorBase()


template<>
METTranslatorBase<BAT::MET>::METTranslatorBase(const std::string& ifile)
{
    const Defaults_Text defs(ifile);
    std::string resolution_string(defs.get_string("met_resolution"));
    resolution_ = Resolution(resolution_string);

} // METTranslatorBase<BAT::MET>::METTranslatorBase(const std::string& ifile)


template<>
METTranslatorBase<BAT::MET>::~METTranslatorBase()
{
} // METTranslatorBase<BAT::MET>::~METTranslatorBase()


template<>
Fourvec
METTranslatorBase<BAT::MET>::operator()(const BAT::MET& m,
                                        bool useObjEmbRes /* = false */)
{
    double px = m.px();
    double py = m.py();

    return Fourvec (px,py,0.0,sqrt(px*px + py*py));

} // Fourvec METTranslatorBase<BAT::MET>::operator()(const BAT::MET& m)



template<>
Resolution
METTranslatorBase<BAT::MET>::KtResolution(const BAT::MET& m,
                                          bool useObjEmbRes /* = false */) const
{
    return resolution_;
} // Resolution METTranslatorBase<BAT::MET>::KtResolution(const BAT::MET& m)



template<>
Resolution
METTranslatorBase<BAT::MET>::METResolution(const BAT::MET& m,
                                           bool useObjEmbRes /* = false */) const
{
    return KtResolution(m,useObjEmbRes);
} // Resolution METTranslatorBase<BAT::MET>::METResolution(const BAT::MET& m)


} // namespace hitfit
