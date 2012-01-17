#!/env

import os
import sys
from fileInfo import *

skipGroupsUntil = 0
startWithGroup = 1

def groupFilesToSize(files, finalSize=1024 * 1024 * 1024 * 2):# < 3 GB
    getsize = os.path.getsize
    groupSize = 0
    groups = [[]]
    groupIndex = 0
    for file in sorted(files):
        size = getsize(file)
        if (groupSize + size) > finalSize:#start new group
            groupIndex += 1
            groups.append([])
            groupSize = 0
        groupSize += size    
        groups[groupIndex].append(file)
    return groups

def fuseFiles(groupedFiles):
    group = startWithGroup
    
    for files in groupedFiles:
        outputFile = getProcess(files[0]) + '_merged_%03d' % group
        outputFile += '.root'
        command = 'hadd -f7 %s ' % outputFile
        for file in files:
            command += file + ' '
        command.rstrip(' ')
        print '=' * 100
        print '*' * 100
        if group > skipGroupsUntil:
            print 'Executing:'
        else:
            print 'Skipping:'
        print command
        print '*' * 100
        print '=' * 100
        if group > skipGroupsUntil:
            os.system(command)
        group += 1

def getProcess(filepath):
    file = filepath.split('/')[-1]
    a = file.split('_')
    process = 'default'
    if len(a) <= 5:
        process = a[0] + '_' + a[1]
    else:
        process = a[0]
        for token in a[1:-3]:
            process += '_' + token
    return process
    

def removeUsedFiles(allFiles, usedFiles):
    allFiles = set(allFiles)
    usedFiles = set(usedFiles)
    return allFiles.difference(usedFiles)

def readMergeLog(mergeLog):
    usedFiles = []
    mergeLog = file(mergeLog)
    lastOutputFileNumber = 0
    for line in mergeLog.readlines():
        if line.startswith('Source file'):
            input = line.split(' ')
            inputfile = input[3].replace('\n', '')
            usedFiles.append(inputfile)
            
        if line.startswith('hadd'):
            input = line.split(' ')
            outputfile = input[2]
            number = outputfile.replace('.root', '')
            number = number.split('_')[-1]
            lastOutputFileNumber = int(number)
    print lastOutputFileNumber
    global startWithGroup
    startWithGroup = lastOutputFileNumber + 1
    return usedFiles
    
if __name__ == "__main__":
    continueLastMerge = False
    allButUsedFiles = []
    groupedFiles = []
    
    args = sys.argv
    if not len(args) >= 2:
        print "Please specify a folder to merge files in."
        sys.exit()
    
    path = sys.argv[1]
    files = getROOTFiles(path)
    uniqueFiles = getUniqueFiles(files)
    
    if len(args) == 3:
        continueLastMerge = True
        mergeLog = sys.argv[2]
        usedFiles = readMergeLog(mergeLog)
        allButUsedFiles = removeUsedFiles(uniqueFiles, usedFiles)
        
    if not continueLastMerge:
        groupedFiles = groupFilesToSize(uniqueFiles)
    else:
        groupedFiles = groupFilesToSize(allButUsedFiles)
    
    print 'Total number of files:', len(files)
    print 'Total number of unique files:', len(uniqueFiles)
    if continueLastMerge:
        print 'Total number of remaining files:', len(allButUsedFiles)
    print 'Process recognised:', getProcess(files[0])
    print 'Number of merged files to be produced:', len(groupedFiles)
    
    fuseFiles(groupedFiles)
