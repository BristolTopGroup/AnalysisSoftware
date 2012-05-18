'''
Created on Nov 23, 2011

@author: Lukasz Kreczko

Email: Lukasz.Kreczko@cern.ch
'''
class Plot():
    '''
    Plot unites the tuning parameters for plot style
    '''


    def __init__(self):
        self.rebin = 1
        self.scale = 1
        self.UserRange = (0,0)
        self.name = ""
        self.location = ""
        self.jetBins = []
        self.bJetBins = []
        self.qcdShapeFrom = ""
        self.qcdEstimate = 0 
        
        
