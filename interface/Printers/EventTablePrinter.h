/*
 * EventPrinter.h
 *
 *  Created on: 22 Aug 2010
 *      Author: kreczko
 */

#ifndef EVENTTABLEPRINTER_H_
#define EVENTTABLEPRINTER_H_
#include "../EventCounter.h"

namespace BAT {

namespace EventTablePrinter {
extern void printCutFlowCSVTable(const Counter& cutFlow, const double lumi);
extern void printCutFlowTwikiTable(const Counter& cutFlow, const double lumi);
extern void printCutFlowLatexTable(const Counter& cutFlow, const std::string StringSteps[], const double lumi);
extern void printUnweightedCutFlowLatexTable(const Counter& cutFlow, const std::string StringSteps[], const double lumi);
}
}
#endif /* EVENTPRINTER_H_ */
