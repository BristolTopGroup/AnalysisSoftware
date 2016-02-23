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
	version = 'v9/'
	basepath = basepath + version

	DatasetFile = open(filename,'r')
	content = DatasetFile.read().splitlines()
	DatasetFile.close()
	# for line in range (0,len(content)): print content[line]

	datasets = []
	for dirs in os.listdir(basepath):
		datasets.append(dirs)

	for dataset in range( 0, len(datasets) ):
		if datasets[dataset].startswith("Single"): continue
		datasetpath = basepath+datasets[dataset]+'/'

		rootfiles = []
		for files in os.listdir(datasetpath):
			rootfiles.append(files)


		total_processed_event = 0
		for f in rootfiles :
			ntuple_processed_events = 0
			if not os.path.exists(datasetpath+f): print 'File Does Not exist'

			rootfile = TFile(datasetpath+f)

			hist = rootfile.Get("topPairEPlusJetsSelectionAnalyser/individualCuts")

			if not hist : 
				print "Waaargh help. Can't find histogram for : ", datasetpath+f
				continue

			ntuple_processed_events = hist.GetBinContent(1)
			rootfile.Close()
			total_processed_event = total_processed_event + ntuple_processed_events

		print "Total number of processed events for ", datasets[dataset], " dataset is ", total_processed_event

		exists = False
		for line in range (0,len(content)):
			if not ( content[line].startswith("datasetInfo['"+datasets[dataset]+"']") ): continue
			exists = True
			oldline=content[line]
			newline=''
			linecontents = oldline.split()
			i=0
			for x in linecontents:
				i = i-1
				if (i==0) : 
					newline += (str(int(total_processed_event))+' ')
				else: newline += (x+' ')
				if x == '"NumberOfProcessedEvents"': 
					i=2
			content[line] = newline

		if not exists: print "This dataset does not have an entry in DatasetInfo file. For now, Go make one."

	DatasetFile = open(filename,'w')
	for line in range (0,len(content)):
	  	print>>DatasetFile, content[line]
  	DatasetFile.close()







