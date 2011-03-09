#ifndef SELECTION_H_
#define SELECTION_H_
#include <string>

namespace BAT {
namespace TTbarEPlusJetsSelection {
enum Step {
    FilterOutScraping,
    HighLevelTrigger,
    GoodPrimaryvertex,
    OneIsolatedElectron,
    LooseMuonVeto,
    Zveto,
    ConversionRejection,
    ConversionFinder,
    AtLeastOneGoodJets,
    AtLeastTwoGoodJets,
    AtLeastThreeGoodJets,
    AtLeastFourGoodJets,
    NUMBER_OF_SELECTION_STEPS
};

const std::string StringSteps[NUMBER_OF_SELECTION_STEPS] = {
        "Scraping Filter",
        "High Level Trigger",
        "good primary vertex",
        "exactly one isolated electron",
        "loose muon veto",
        "Z veto",
        "Conversion veto (missing hits)",
        "Conversion finder (partner track)",
        ">= 1 jets",
        ">= 2 jets",
        ">= 3 jets",
        ">= 4 jets" };
}
}
#endif //SELECTION_H_
