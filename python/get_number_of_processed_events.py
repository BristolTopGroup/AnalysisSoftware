'''
Run this over new ntuples to get the number of processed events from crab. 
This recreates DataSetInfo_13TeV_25ns.py using new numbers.
'''

import ROOT 
from ROOT import gROOT, TFile, TH1F
import math

import os
import glob

from analysis_info import datasets_13TeV
from shutil import move

if __name__ == '__main__':
	filename = 'DataSetInfo_13TeV_25ns.py' #Dataset file name
	filename_tmp = 'DataSetInfo_13TeV_25ns.py_temp' #Dataset file name
	version = 'v0.0.1'
	campaign = 'Spring16'
	# basepath = '/hdfs/TopQuarkGroup/phxlk/ntuple/{version}/{campaign}/'.format(
	# 	version=version,
	# 	campaign=campaign,
	# )
	basepath = '/hdfs/TopQuarkGroup/run2/ntuples/25ns/v0.0.3/'

	DatasetFile = open(filename,'r')
	content = DatasetFile.read().splitlines()
	newcontent = []
	DatasetFile.close()
	# for line in range (0,len(content)): print content[line]

	datasets = []
	for dirs in os.listdir(basepath):
		datasets.append(dirs)

	for dataset in datasets:
		file_paths = []
		if dataset not in datasets_13TeV: continue
		if dataset.startswith("Single"): continue
		# path = basepath+dataset+'/merge/'
		path = basepath+dataset+'/'
		if os.path.exists(path):
			file_paths.append(path)
		else:
			path = path.replace('merge/', 'tmp/')
			for dirs in os.listdir(path):
				if 'job' not in dirs : continue
				file_paths.append(path+dirs+'/')

		rootfiles = []
		total_processed_event = 0
		for p in file_paths :
			ntuple_processed_events = 0
			for files in os.listdir(p):
				if '.root' not in files : continue
				f = p+files
				rootfile = TFile(f)
				hist = rootfile.Get("topPairEPlusJetsSelectionAnalyser/individualCuts")
				if not hist : 
					print "Waaargh help. Can't find histogram for : ", f
					continue
				ntuple_processed_events = hist.GetBinContent(1)
				rootfile.Close()
				total_processed_event = total_processed_event + ntuple_processed_events
		print "Total number of processed events for ", dataset, " dataset is ", total_processed_event

		exists = False
		for i, line in enumerate(content):
			if not ( line.startswith("datasetInfo['"+dataset+"']") ): continue
			exists = True
			n_oldEvents = line.split()[-2]
			content[i] = line.replace(n_oldEvents, (str(int(total_processed_event))))
			# newcontent.append(newline)
		if not (exists): print "This dataset does not have an entry in DatasetInfo file. For now, Go make one."

	datasetFile = open(filename_tmp,'w')
	print "-"*60
	print "New DataSetInfo_13TeV_25ns :\n"
	for line in content: 
		print line
		print >> datasetFile, line
  	datasetFile.close()

  	move(filename_tmp,filename)








