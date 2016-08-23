'''
Run this over new ntuples to get the number of processed events from crab. 
This recreates DataSetInfo_13TeV_25ns.py using new numbers.
'''

import ROOT 
from ROOT import gROOT, TFile, TH1F
import math

import os
import glob

from analysis_info_2016 import datasets_13TeV
from shutil import move


if __name__ == '__main__':
	filename = 'DataSetInfo_13TeV_25ns.py' #Dataset file name
	filename_tmp = 'DataSetInfo_13TeV_25ns.py_temp' #Dataset file name

	user = 'phxlk'
	version = 'v0.0.8'
	regime = 'Spring16'

	temp_basepath = '/hdfs/TopQuarkGroup/{user}/ntuple/{version}/{regime}/'
	temp_ntuplepath = '{sample}/tmp/'
	temp_rootfile = 'ntuple_job_{job_number}/{sample}_ntuple_{job_number}.root'

	basepath = temp_basepath.format(
		user = user,
		version = version,
		regime = regime,
	)

	DatasetFile = open(filename,'r')
	content = DatasetFile.read().splitlines()
	DatasetFile.close()
	
	# out = open('data_ntuples_in_use.txt', 'wt')

	for dataset in datasets_13TeV.keys():
		
		# # What data ntuples are being used?
		# if dataset.startswith("Single"): 
		# 	for f in datasets_13TeV[dataset]:
		# 		tmp_datasamplename = f.split('/')
		# 		datasamplename = tmp_datasamplename[-3]
		# 		for ntuplejob in os.listdir(f):
		# 			if not os.path.isdir(f+ntuplejob): continue
		# 			tmp = ntuplejob.split('_')
		# 			filename = datasamplename+'_ntuple_'+tmp[2]+'.root'
		# 			print >> out, filename
	
		files = []
		ntuplepath = temp_ntuplepath.format(
			sample=dataset,
		)
		path = basepath+ntuplepath
		if not os.path.exists(path): continue
		for ntuplejob in os.listdir(path):
			if not os.path.isdir(path+ntuplejob): continue
			tmp = ntuplejob.split('_')
			rootfile = temp_rootfile.format(
				job_number = tmp[2],
				sample = dataset
			)
			full_path = path+rootfile
			files.append(full_path)
		total_processed_event = 0
		for file_path in files:
			rootfile = TFile(file_path)
			hist = rootfile.Get("topPairEPlusJetsSelectionAnalyser/individualCuts")
			if not hist : 
				print "Waaargh help. Can't find histogram for : ", f
				continue
			ntuple_processed_events = hist.GetBinContent(1)
			print file_path
			print ntuple_processed_events
			rootfile.Close()
			total_processed_event = total_processed_event + ntuple_processed_events

		print "Total number of processed events for ", dataset, " dataset is ", total_processed_event

		exists = False
		for i, line in enumerate(content):
			if not ( line.startswith("datasetInfo['"+dataset+"']") ): continue
			exists = True
			n_oldEvents = line.split()[-2]
			content[i] = line.replace(n_oldEvents, (str(int(total_processed_event))))
		if not (exists): print "This dataset does not have an entry in DatasetInfo file. For now, Go make one."

	datasetFile = open(filename_tmp,'w')
	print "-"*60
	print "New DataSetInfo_13TeV_25ns :\n"
	for line in content: 
		print line
		print >> datasetFile, line
  	datasetFile.close()
  	out.close()

  	move(filename_tmp,filename)









