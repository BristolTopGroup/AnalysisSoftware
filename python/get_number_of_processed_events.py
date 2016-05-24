'''
Run this over new ntuples to get the number of processed events from crab. 
This recreates DataSetInfo_13TeV_25ns.py using new numbers.
'''

import ROOT 
from ROOT import gROOT, TFile, TH1F
import math

import os
import glob

if __name__ == '__main__':
	filename = 'DataSetInfo_13TeV_25ns.py' #Dataset file name
	basepath = '/hdfs/TopQuarkGroup/run2/ntuples/25ns/'
	version = 'v12/'
	basepath = basepath + version

	DatasetFile = open(filename,'r')
	content = DatasetFile.read().splitlines()
	DatasetFile.close()
	# for line in range (0,len(content)): print content[line]

	datasets = []
	for dirs in os.listdir(basepath):
		datasets.append(dirs)

	for dataset in datasets:
		if dataset.startswith("Single"): continue
		path = basepath+dataset+'/'

		rootfiles = []
		for files in os.listdir(path):
			rootfiles.append(files)

		total_processed_event = 0
		for f in rootfiles :
			ntuple_processed_events = 0
			if not os.path.exists(path+f): print 'File Does Not exist'

			rootfile = TFile(path+f)

			hist = rootfile.Get("topPairEPlusJetsSelectionAnalyser/individualCuts")

			if not hist : 
				print "Waaargh help. Can't find histogram for : ", path+f
				continue

			ntuple_processed_events = hist.GetBinContent(1)
			rootfile.Close()
			total_processed_event = total_processed_event + ntuple_processed_events

		print "Total number of processed events for ", dataset, " dataset is ", total_processed_event

		exists = False
		for line in content:
			if not ( line.startswith("datasetInfo['"+dataset+"']") ): continue
			exists = True
			n_oldEvents = line.split()[-2]
			line.replace(n_oldEvents, (str(int(total_processed_event))))
		if not exists: print "This dataset does not have an entry in DatasetInfo file. For now, Go make one."

	DatasetFile = open(filename,'w')
	for line in content: print >> DatasetFile, line
  	DatasetFile.close()







