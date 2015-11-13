import ROOT 
from ROOT import gROOT, TFile, TH1F
import math


import os
import glob


if __name__ == '__main__':
	basepath = '/hdfs/TopQuarkGroup/run2/ntuples/25ns/'
	version = 'v1/'

	basepath = basepath + version

	datasets = []
	for dirs in os.listdir(basepath):
		datasets.append(dirs)

	for dataset in range( 0, len(datasets) ):
		datasetpath = basepath+datasets[dataset]+'/'

		rootfiles = []
		for files in os.listdir(datasetpath):
			rootfiles.append(files)

		total_processed_event = 0
		for f in range( 0, len(rootfiles) ):
			ntuple_processed_events = 0
			rootfile = datasetpath+rootfiles[f]
			if not os.path.exists(rootfile): print 'File Does Not exist'

			rootfile = TFile(rootfile)
			hist = rootfile.Get("topPairEPlusJetsSelectionAnalyser/individualCuts_unweighted")
			ntuple_processed_events = hist.GetBinContent(1)
			rootfile.Close()
			total_processed_event = total_processed_event + ntuple_processed_events

		print "Total number of processed events for ", datasets[dataset], " dataset is ", total_processed_event








