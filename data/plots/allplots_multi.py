## This file is the same as allplots.py, except that it uses multiprocessing
## to make better use of machines with multiple cores

try:
  ## the normal way to import rootplot
  from rootplot import plot, plotmpl
  from rootplot.core import report_progress
except ImportError:
  ## special import for CMSSW installations of rootplot
  from PhysicsTools.PythonAnalysis.rootplot import plot, plotmpl
  from PhysicsTools.PythonAnalysis.rootplot.core import report_progress
import ROOT
import multiprocessing as multi
from Queue import Empty

import os
os.chdir('..')  # return to the directory with the ROOT files

calls = []

calls.append("""
canvas, objects = plot('PileUp_2011_truth_finebin_64600microbarn.root', 'PileUp_2011_truth_finebin_68000microbarn.root', 'PileUp_2011_truth_finebin_71400microbarn.root', 'pileup', ext='root', xlabel='Number of interactions per crossing', title='CMS Preliminary 5.1 fb^{-1} at #sqrt{s} = 7 TeV', legend_entries='inelastic cross-section = 64600 #mub,inelastic cross-section = 68000 #mub,inelastic cross-section = 71400 #mub')
canvas.SaveAs('plots/pileup.root')
""")


queue = multi.JoinableQueue()
qglobals = multi.Manager().Namespace()
qglobals.nfinished = 0
qglobals.ntotal = len(calls)
for call in calls:
    queue.put(call)

def qfunc(queue, qglobals):
    while True:
        try: mycall = queue.get(timeout=5)
        except (Empty, IOError): break
        exec(mycall)
        ROOT.gROOT.GetListOfCanvases().Clear()
        qglobals.nfinished += 1
        report_progress(qglobals.nfinished, qglobals.ntotal, 
                        'plots', 'root')
        queue.task_done()

for i in range(8):
    p = multi.Process(target=qfunc, args=(queue, qglobals))
    p.daemon = True
    p.start()
queue.join()
report_progress(len(calls), len(calls), 'plots', 'root')
print ''
