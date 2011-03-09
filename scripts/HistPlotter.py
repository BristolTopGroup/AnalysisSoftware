from tdrStyle import *
from ROOT import *


def setStyle():
        tdrStyle = setTDRStyle();

        #slight adaptation
        tdrStyle.SetPadRightMargin( 0.05 ); #originally was 0.02, too narrow!
        tdrStyle.SetStatH( 0.2 );
        #tdrStyle.SetOptStat(1110);//off title
        tdrStyle.SetOptStat( 0 );#off title
        tdrStyle.SetOptFit( 0 );#off title
        tdrStyle.cd();
        gROOT.ForceStyle();


def rebin( hists, nbins, histname ):
    for sample in hists.keys():
        if len( hists[sample].keys() ) == 0:
            continue
        if '*' in histname:
            nameToken = histname.replace('*', '')
            histlist = hists[sample]
            for name in histlist.keys():
                if nameToken in name:
                    hists[sample][name].Rebin( nbins )
        elif hists[sample].has_key( histname ):
            hists[sample][histname].Rebin( nbins )
    return hists

def setXRange( hists, limits = ( 0, 5000 ), histname = '' ):
    for sample in hists.keys():
        if len( hists[sample].keys() ) == 0:
            continue
        if '*' in histname:
            nameToken = histname.replace('*', '')
            histlist = hists[sample]
            for name in histlist.keys():
                if nameToken in name:
                    hists[sample][name].GetXaxis().SetRangeUser( limits[0], limits[1] )
        elif hists[sample].has_key( histname ):
            hists[sample][histname].GetXaxis().SetRangeUser( limits[0], limits[1] );
    return hists


def setXTitle( hists, title, histname = '' ):
    for sample in hists.keys():
        if len( hists[sample].keys() ) == 0:
            continue
        if '*' in histname:
            nameToken = histname.replace('*', '')
            histlist = hists[sample]
            for name in histlist.keys():
                if nameToken in name:
                    hists[sample][name].SetXTitle( title )
        elif hists[sample].has_key( histname ):
            hists[sample][histname].SetXTitle( title );
    return hists

def setYTitle( hists, title, histname = '' ):
    for sample in hists.keys():
        if len( hists[sample].keys() ) == 0:
            continue
        if '*' in histname:
            nameToken = histname.replace('*', '')
            histlist = hists[sample]
            for name in histlist.keys():
                if nameToken in name:
                    hists[sample][name].SetYTitle( title )
        elif hists[sample].has_key( histname ):
            hists[sample][histname].SetYTitle( title );
    return hists

#    for sample in hists.keys():
#        if len( hists[sample].keys() ) == 0 or not hists[sample].has_key( histname ) or not len( limits ) == 2:
#            continue
#        hists[sample][histname].GetXaxis().SetRangeUser( limits[0], limits[1] );
#    return hists

def applyDefaultStylesAndColors( hists ):
    defaultColors = {'data':0,
    'ttbar' : kRed + 1,
    'wjets' : kGreen - 3,
    'zjets' : kAzure - 2,
    'bce1' : kRed-7,
    'bce2' : kRed-8,
    'bce3' : kRed-9,
    'enri1' : kBlue-7,
    'enri2' : kBlue-8,
    'enri3' : kBlue-9,
    'pj1' : kYellow-7,
    'pj2' : kYellow-8,
    'pj3' : kYellow-9,
    'qcd': kYellow,
    'singleTop': kMagenta,
    'Zprime500': kTeal - 9,
    'Zprime750': kBlue - 6,
    'Zprime1000': 28,
    'Zprime1250': kCyan - 5,
    'Zprime1500': kOrange + 1, }

    for sample, histlist in hists.iteritems():
        if not sample in defaultColors.keys():
            continue
        for histname, hist in histlist.iteritems():
            hists[sample][histname].SetFillColor( defaultColors[sample] )
            if histname == 'data':
                hists[sample][histname].SetMarkerStyle( 8 )
            elif 'Zprime' in histname:
                hists[sample][histname].SetFillStyle( 0 )
                hists[sample][histname].SetLineColor( defaultColors[sample] )
            else:
                hists[sample][histname].SetFillStyle( 1001 );

    return hists

