from ROOT import *
from math import fsum


def getHistsFromFiles( histnames, files ):
        TFileOpen = TFile.Open
        allHists = {}
        gcd = gROOT.cd
        for sample, file in files.iteritems():
            file = TFileOpen( file )
            allHists[sample] = {}
            fg = file.Get
            gcd()
            for hist in histnames:
                allHists[sample][hist] = fg( hist ).Clone()
        return allHists

def addSampleSum( hists = {} ):
        qcdList = {}
        mc_all_list = {}
        singleTopList = {}

        qcdSamples = ['bce1', 'bce2', 'bce3', 'enri1', 'enri2', 'enri3', 'pj1', 'pj2', 'pj3']
#        allMCSamples = ['ttbar', 'wjets', 'zjets', 'tW', 'tchan', 'bce1', 'bce2', 'bce3', 'enri1',
#                        'enri2', 'enri3', 'pj1', 'pj2', 'pj3']
        singleTopSamples = ['tW', 'tchan']


        for sample, histlist in hists.iteritems():
            for histname, hist in histlist.iteritems():
                if sample in qcdSamples:
                    if not qcdList.has_key( histname ):
                        qcdList[histname] = hist.Clone( 'qcd' )
                    else:
                        qcdList[histname].Add( hist )

#                if sample in allMCSamples:
#                    if not mc_all_list.has_key( histname ):
#                        mc_all_list[histname] = hist.Clone( 'all_mc' )
#                    else:
#                        mc_all_list[histname].Add( hist )

                if sample in singleTopSamples:
                    if not singleTopList.has_key( histname ):
                        singleTopList[histname] = hist.Clone( 'singleTop' )
                    else:
                        singleTopList[histname].Add( hist )

        hists['qcd'] = qcdList
#        hists['allMC'] = mc_all_list
        hists['singleTop'] = singleTopList

        return hists

def makeDetailedMCStack( hists ):

    allMCSamples = ['bce1', 'bce2', 'bce3', 'enri1',
                        'enri2', 'enri3', 'pj1', 'pj2', 'pj3', 'zjets', 'wjets', 'tW', 'tchan', 'ttbar' ]

    hists['allMCDetailed'] = makeStack( hists, allMCSamples )
    return hists
#    mcStack = {}
#    
#    for sample in allMCSamples:
#        if hists.has_key(sample):
#            histlist = hists[sample]
#            for histname, hist in histlist.iteritems():
#                if sample in allMCSamples:
#                    if not mcStack.has_key( histname ):
#                        mcStack[histname] = THStack("MC", "MC")
#                    mcStack[histname].Add( hist )
#    hists['allMCDetailed'] = mcStack
#    return hists

def makeMCStack( hists ):

    allMCSamples = ['qcd', 'zjets', 'wjets', 'singleTop', 'ttbar']
    hists['allMC'] = makeStack( hists, allMCSamples )
    return hists

def makeStack( hists, samples ):
    mcStack = {}

    for sample in samples:
        if hists.has_key( sample ):
            histlist = hists[sample]
            for histname, hist in histlist.iteritems():
                if not mcStack.has_key( histname ):
                    mcStack[histname] = THStack( "MC", "MC" )
                mcStack[histname].Add( hist )

    return mcStack

def addJetSum( hists ):
        allhists = ['QCDest_CombRelIso_0jet', 'QCDest_CombRelIso_1jet', 'QCDest_CombRelIso_2jets',
                        'QCDest_CombRelIso_3jets', 'QCDest_CombRelIso_4orMoreJets']
        oneOrMore = ['QCDest_CombRelIso_1jet', 'QCDest_CombRelIso_2jets',
                        'QCDest_CombRelIso_3jets', 'QCDest_CombRelIso_4orMoreJets']
        twoOrMore = ['QCDest_CombRelIso_2jets',
                        'QCDest_CombRelIso_3jets', 'QCDest_CombRelIso_4orMoreJets']
        threeOrMore = ['QCDest_CombRelIso_3jets', 'QCDest_CombRelIso_4orMoreJets']

        addUp = addUpHistograms
        for sample, histlist in hists.iteritems():
            if( len( hists[sample].keys() ) == 0 ):
                continue
            hists[sample]['QCDest_CombRelIso_0orMoreJets'] = addUp( hists[sample], allhists )
            hists[sample]['QCDest_CombRelIso_1orMoreJets'] = addUp( hists[sample], oneOrMore )
            hists[sample]['QCDest_CombRelIso_2orMoreJets'] = addUp( hists[sample], twoOrMore )
            hists[sample]['QCDest_CombRelIso_3orMoreJets'] = addUp( hists[sample], threeOrMore )
        return hists

def addUpHistograms( dictOfHists, histsToAdd ):
        hist = dictOfHists[histsToAdd[0]].Clone()
        hadd = hist.Add
        [hadd( h ) for name, h in dictOfHists.iteritems() if name in histsToAdd[1:]]
        return hist



if __name__ == "__main__":
    histnames = ['QCDest_CombRelIso_0jet', 'QCDest_CombRelIso_1jet', 'QCDest_CombRelIso_2jets',
                        'QCDest_CombRelIso_3jets', 'QCDest_CombRelIso_4orMoreJets']
    files = {'data':"/storage/workspace/BristolAnalysisTools/outputfiles/new/data_35pb_PFElectron_PF2PATJets_PFMET.root",
    'ttbar' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/ttjet_35pb_PFElectron_PF2PATJets_PFMET.root",
    'wjets' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/wj_35pb_PFElectron_PF2PATJets_PFMET.root",
    'zjets' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/zj_35pb_PFElectron_PF2PATJets_PFMET.root",
#    'bce1' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/bce1_35pb.root",
    'bce2' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/bce2_35pb_PFElectron_PF2PATJets_PFMET.root",
    'bce3' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/bce3_35pb_PFElectron_PF2PATJets_PFMET.root",
    'enri1' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/enri1_35pb_PFElectron_PF2PATJets_PFMET.root",
    'enri2' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/enri2_35pb_PFElectron_PF2PATJets_PFMET.root",
    'enri3' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/enri3_35pb_PFElectron_PF2PATJets_PFMET.root",
    'pj1' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/pj1_35pb_PFElectron_PF2PATJets_PFMET.root",
    'pj2' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/pj2_35pb_PFElectron_PF2PATJets_PFMET.root",
    'pj3' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/pj3_35pb_PFElectron_PF2PATJets_PFMET.root"}
#    'tW' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/tW_35pb_PFElectron_PF2PATJets_PFMET.root",
#    'tchan' : "/storage/workspace/BristolAnalysisTools/outputfiles/new/tchan_35pb_PFElectron_PF2PATJets_PFMET.root"}
#    HistGetter.samplefiles = files
#    HG = HistGetter()
#    HG.setStyle()
    hists = getHistsFromFiles( histnames, files )
    hists = addSampleSum( hists )
    hists = addJetSum( hists )
    qcdSamples = ['bce1', 'bce2', 'bce3', 'enri1', 'enri2', 'enri3', 'pj1', 'pj2', 'pj3']
    allMCSamples = ['ttbar', 'wjets', 'zjets', 'tW', 'tchan', 'bce1', 'bce2', 'bce3', 'enri1',
                        'enri2', 'enri3', 'pj1', 'pj2', 'pj3']
    singleTopSamples = ['tW', 'tchan']

    nqcd = 0
    nstop = 0
    nmc = 0
    nqcd = fsum( [hists[sample]['QCDest_CombRelIso_0jet'].Integral() for sample in qcdSamples if hists.has_key( sample )] )
#    nstop = sum([hists[sample][0].Integral() for sample in singleTopSamples if hists.has_key(sample)])
    nmc = fsum( [hists[sample]['QCDest_CombRelIso_0jet'].Integral() for sample in allMCSamples if hists.has_key( sample )] )
    print hists['qcd']['QCDest_CombRelIso_0jet'].Integral(), nqcd
#    print hists['singleTop'][0].Integral(), nstop
    print hists['allMC']['QCDest_CombRelIso_0jet'].Integral(), nmc
    print
    print hists['allMC']['QCDest_CombRelIso_0orMoreJets'].Integral(), hists['allMC']['QCDest_CombRelIso_0jet'].Integral()
    print hists['allMC']['QCDest_CombRelIso_1orMoreJets'].Integral(), hists['allMC']['QCDest_CombRelIso_1jet'].Integral()
    print hists['allMC']['QCDest_CombRelIso_2orMoreJets'].Integral(), hists['allMC']['QCDest_CombRelIso_2jets'].Integral()
    print hists['allMC']['QCDest_CombRelIso_3orMoreJets'].Integral(), hists['allMC']['QCDest_CombRelIso_3jets'].Integral()
    print hists['allMC']['QCDest_CombRelIso_4orMoreJets'].Integral()
    print hists['allMC'].keys()
    c = []
    for histname, hist in hists['allMC'].iteritems():
        c.append( TCanvas( "cname" + histname, histname, 800, 600 ) )
        c[-1].cd()
        hist.Draw()

    a = raw_input()
