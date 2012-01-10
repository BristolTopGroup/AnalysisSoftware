#include "../../interface/Printers/EventTablePrinter.h"
#include <iostream>
#include <iomanip>
#include "../../interface/TopPairEventCandidate.h"

namespace BAT {
namespace EventTablePrinter {
extern void printCutFlowLatexTable(const Counter& cutFlow, const std::string StringSteps[], const double lumi) {
    using namespace std;
    cout << "Printing cut flow table" << endl;

    //print header
    cout << "\\newpage\n" << endl;
    cout << "\n%------------------------------------------------------------------------\n";
    cout << "%       " << "cut flow table (weighted)";
    cout << "\n%------------------------------------------------------------------------\n";
    cout << "\\begin{tabular}{|l|rrrrrr|r|}" << endl;
    cout << "\\hline" << endl;
    cout << "\\multicolumn{8}{|l|}";
    cout << "{Expected number of events for " << lumi << "/pb}";
    cout << "\\\\\\hline" << endl;
    cout << " Cut " << setw(40) << left << right;
    //    cout << "Cut" << setw(40);

    for (unsigned int type = 0; type < cutFlow.getSizeOfFirstDimension(); ++type) {
        if (type == DataType::QCD_BCtoE_Pt20to30) {//first QCD data type, this needs to be changed
            cout << " &" << setw(13) << "QCD";
        }
        if (type == DataType::singleTop_And_W)
            cout << " &" << setw(13) << "singeTop";

        if (type >= DataType::singleTop_And_W && type <= DataType::singleAntiTopSChannel)
            continue;

        if (type >= DataType::QCD_BCtoE_Pt20to30 && type <= DataType::PhotonJets_Pt200toInf)
            continue;
        if (type == DataType::VQQ || type >= DataType::Zprime_M500GeV_W5GeV)//don't use them now
            continue;
        cout << " &" << setw(13) << DataType::names.at(type);
    }
    cout << " &" << setw(25) << "Total MC \\\\\n\\hline" << endl;
    for (unsigned int cut = 0; cut < cutFlow.getSizeOfSecondDimension(); ++cut) {
        unsigned long nQCD = 0;
        unsigned long nSingleTop = 0;
        unsigned long nTotalMC = 0;
        for (unsigned int type = 0; type < cutFlow.getSizeOfFirstDimension(); ++type) {
            if (type == 0) {
                cout << " Stage " << setw(2) << cut << " " << setw(40) << left
                        << StringSteps[cut] << right;
            }
            if (type >= DataType::QCD_BCtoE_Pt20to30 && type <= DataType::PhotonJets_Pt200toInf)
                nQCD += cutFlow.sumThirdDimension(type, cut);

            if (type >= DataType::singleTop_And_W && type <= DataType::singleAntiTopSChannel)
                nSingleTop += cutFlow.sumThirdDimension(type, cut);

            if (type == DataType::PhotonJets_Pt200toInf)
                cout << " & " << setw(12) << fixed << nQCD;

            if (type == DataType::singleTopSChannel)
                cout << " & " << setw(12) << fixed << nSingleTop;

            if (type != DataType::ElectronHad && type != DataType::MuHad)
                nTotalMC += cutFlow.sumThirdDimension(type, cut);

            if (type >= DataType::singleTop_And_W && type <= DataType::singleAntiTopSChannel)
                continue;
            if (type >= DataType::QCD_BCtoE_Pt20to30 && type <= DataType::PhotonJets_Pt200toInf)
                continue;
            if (type == DataType::VQQ || type >= DataType::Zprime_M500GeV_W5GeV)//don't use them now
                continue;

            cout << " & " << setw(12) << fixed << cutFlow.sumThirdDimension(type, cut);
        }
        cout << " & " << setw(12) << fixed << nTotalMC;
        cout << "  \\\\\n\\hline" << endl;
    }

    cout << "\\end{tabular}\\\\[5mm]" << endl;
    cout << endl;
}
extern void printUnweightedCutFlowLatexTable(const Counter& cutFlow, const std::string StringSteps[], const double lumi) {
    using namespace std;
    cout << "Printing cut flow table" << endl;

    //print header
    cout << "\\newpage\n" << endl;
    cout << "\n%------------------------------------------------------------------------\n";
    cout << "%       " << "cut flow table (unweighted)";
    cout << "\n%------------------------------------------------------------------------\n";
    cout << "\\begin{tabular}{|l|rrrrrrr|r|}" << endl;
    cout << "\\hline" << endl;
    cout << "\\multicolumn{8}{|l|}";
    cout << "{Expected number of events for " << lumi << "/pb}";
    cout << "\\\\\\hline" << endl;
    cout << " Cut " << setw(40) << left << right;
    //    cout << "Cut" << setw(40);

    for (unsigned int type = 0; type < cutFlow.getSizeOfFirstDimension(); ++type) {
        if (type == DataType::QCD_BCtoE_Pt20to30) {//first QCD data type, this needs to be changed
            cout << " &" << setw(13) << "QCD";
        }
        if (type == DataType::singleTop_And_W)
            cout << " &" << setw(13) << "singeTop";

        if (type >= DataType::singleTop_And_W && type <= DataType::singleAntiTopSChannel)
            continue;

        if (type >= DataType::QCD_BCtoE_Pt20to30 && type <= DataType::PhotonJets_Pt200toInf)
            continue;
        if (type == DataType::VQQ || type >= DataType::Zprime_M500GeV_W5GeV)//don't use them now
            continue;
        cout << " &" << setw(13) << DataType::names.at(type);
    }
    cout << " &" << setw(25) << "Total MC \\\\\n\\hline" << endl;
    for (unsigned int cut = 0; cut < cutFlow.getSizeOfSecondDimension(); ++cut) {
        unsigned long nQCD = 0;
        unsigned long nSingleTop = 0;
        unsigned long nTotalMC = 0;
        for (unsigned int type = 0; type < cutFlow.getSizeOfFirstDimension(); ++type) {
            if (type == 0) {
                cout << " Stage " << setw(2) << cut << " " << setw(40) << left
                        << StringSteps[cut] << right;
            }
            if (type >= DataType::QCD_BCtoE_Pt20to30 && type <= DataType::PhotonJets_Pt200toInf)
                nQCD += cutFlow.sumThirdDimensionUnweighted(type, cut);

            if (type >= DataType::singleTop_And_W && type <= DataType::singleAntiTopSChannel)
                nSingleTop += cutFlow.sumThirdDimensionUnweighted(type, cut);

            if (type == DataType::PhotonJets_Pt200toInf)
                cout << " & " << setw(12) << fixed << nQCD;

            if (type == DataType::singleTopSChannel)
                cout << " & " << setw(12) << fixed << nSingleTop;

            if (type != DataType::ElectronHad && type != DataType::MuHad)
                nTotalMC += cutFlow.sumThirdDimensionUnweighted(type, cut);

            if (type >= DataType::singleTop_And_W && type <= DataType::singleTopSChannel)
                continue;
            if (type >= DataType::QCD_BCtoE_Pt20to30 && type <= DataType::PhotonJets_Pt200toInf)
                continue;
            if (type == DataType::VQQ || type >= DataType::Zprime_M500GeV_W5GeV)//don't use them now
                continue;

            cout << " & " << setw(12) << fixed << cutFlow.sumThirdDimensionUnweighted(type, cut);
        }
        cout << " & " << setw(12) << fixed << nTotalMC;
        cout << "  \\\\\n\\hline" << endl;
    }

    cout << "\\end{tabular}\\\\[5mm]" << endl;
    cout << endl;
}

}
}
