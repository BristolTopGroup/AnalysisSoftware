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
extern void printCutFlowCSVTable(const Counter& cutFlow);
extern void printCutFlowLatexTable(const Counter& cutFlow);
extern void printUnweightedCutFlowLatexTable(const Counter& cutFlow);
}
}
#endif /* EVENTPRINTER_H_ */
