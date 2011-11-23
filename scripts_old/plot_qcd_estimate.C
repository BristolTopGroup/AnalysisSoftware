//
// 31 Mar 2010
// Plots (signed) Delta = (est-true)/true, for QCD estimate
//
//---------------------------------------------------------------

const int n = 4;
const int nrange = 9;
const bool plot_average_est = true;

// free in 12j, fix in 34j
bool constrain_gaus_mean_12j = false;
bool show_free_fit_res_34j = true;

// landau free
//bool show_free_fit_res_34j = true;
//const bool constrain_gaus_mean_12j = true;


// gaus mean-constrained in 12j, fix in 34j
void plot_qcd_estimate_gaus_mean12j() {
    constrain_gaus_mean_12j = true;
    show_free_fits_res_34j = false;
    plot_qcd_estimate("gaus");
}

void setStyle() {

    gROOT->SetStyle("Plain");
    gStyle->SetTextFont(42);
    gStyle->SetLabelFont(42, "xy");
    gStyle->SetFrameFillColor(0);
    gStyle->SetTitleBorderSize(0);

    gStyle->SetTitleH(0.06);
    gStyle->SetPadTopMargin(0.15);

    gStyle->SetOptFit(1);
    gStyle->SetPalette(1);

    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
}

void plot_qcd_estimate(string func = "gaus") {

    setStyle();

    if (constrain_gaus_mean_12j)
        show_free_fit_res_34j = false;

    double x[4] = { 1, 2, 3, 4 };
    double xx[2] = { 3, 4 };
    double x12j[2] = { 1, 2 };
    /*
     string func = "gaus";
     cout << "enter function: ";
     cin >> func;
     //   string sfunc = "pol3";
     bool overview = false;
     cout << "Overview to see all points? (yes=1; no=0) ";
     cin >> overview;
     */
    // Gaus (Free-fits)
    ifstream result; //eg. est_free_gaus.txt

    result.open(Form("est_free_%s.txt", func.c_str()));
    //result.open(Form("gaus_mean_0.3to0.6_12j__rb5/est_free_%s.txt",func.c_str()));

    int m = nrange * (4 + 2);
    double est[500];
    double estFix[500];
    double max_dev = 0;
    double max_dev_12j = 0;
    double max_dev_34j = 0;
    double max_dev_3j = 0;
    double max_dev_4j = 0;
    double max_dev_34j_fix = 0;
    double max_dev_3j_fix = 0;
    double max_dev_4j_fix = 0;

    // Free fits
    for (int i = 0; i < 4 * nrange; ++i) {
        result >> est[i];
        if (fabs(est[i]) > max_dev)
            max_dev = fabs(est[i]);

        if (i % 4 <= 1) {//1,2j
            if (fabs(est[i]) > max_dev_12j)
                max_dev_12j = fabs(est[i]);
        } else if (i % 4 == 2) { //3j
            if (fabs(est[i]) > max_dev_3j)
                max_dev_3j = fabs(est[i]);
        } else if (i % 4 == 3) { //4mj
            if (fabs(est[i]) > max_dev_4j)
                max_dev_4j = fabs(est[i]);
        }
        if (i % 4 == 0)
            cout << endl; //first of 4
        cout << i % 4 + 1 << "j   est[" << i << "] = " << est[i] << "   ";
        if (i % 4 <= 1)
            cout << "max_dev_12j = " << max_dev_12j; //12j
        cout << endl;
    }
    result.close();

    // Gaus (Constrained-fits in 3,4j)
    result.open(Form("est_fix_%s.txt", func.c_str()));
    //result.open(Form("gaus_mean_0.3to0.6_12j__rb5/est_fix_%s.txt",func.c_str()));

    vector<double> estFix2;
    double maxDevFix = 0;

    for (int i = 0; i < 4 * nrange; ++i) {

        // read in 3,4j only
        //cout << "amtb: i=" << i << endl;
        if (!constrain_gaus_mean_12j && (i % 4 < 2))
            continue;

        result >> estFix[i];
        estFix2.push_back(estFix[i]);

        cout << i % 4 + 1 << "j   estFix[" << i << "] = " << estFix[i] << "   " << endl;

        //cout << "amtb: i=" << i << endl;


        if (fabs(estFix[i]) > maxDevFix)
            maxDevFix = fabs(estFix[i]);
        if (fabs(estFix[i]) > max_dev_34j_fix)
            max_dev_34j_fix = fabs(estFix[i]);
        if (i % 4 == 2) {//3j
            if (fabs(estFix[i]) > max_dev_3j_fix)
                max_dev_3j_fix = fabs(estFix[i]);
        } else if (i % 4 == 3) { //>=4j
            if (fabs(estFix[i]) > max_dev_4j_fix)
                max_dev_4j_fix = fabs(estFix[i]);
        }
    }
    for (short k = 0; k < estFix2.size(); k++) {
        cout << "amtb: estFix2[" << k << "]: " << estFix2[k] << endl;
    }
    result.close();
    max_dev_34j_fix = TMath::Max(max_dev_3j_fix, max_dev_4j_fix);
    max_dev_34j = TMath::Max(max_dev_3j, max_dev_4j);

    cout << "maxDevFix: " << maxDevFix << endl;

    cout << "\nFor all ranges, |max| deviation is " << max_dev << endl;
    cout << "For 1,2j,   |max| deviation is " << max_dev_12j << endl;
    cout << "For free fit (3j)      |max| deviation is " << max_dev_3j << endl;
    cout << "For free fit (>=4j)    |max| deviation is " << max_dev_4j << endl;
    cout << "For free fit (3,>=4j), |max| deviation is " << max_dev_34j << endl;
    cout << "For constrained fit (3j),     |max| deviation is " << max_dev_3j_fix << endl;
    cout << "For constrained fit (>=4j),   |max| deviation is " << max_dev_4j_fix << endl;
    cout << "For constrained fit (3,>=4j), |max| deviation is " << max_dev_34j_fix << endl;

    TCanvas c2("c2", "QCD estimates", 600, 600);

    double y[nrange][4];

    int index = 0;

    for (int i = 0; i < nrange; ++i) {
        for (int j = 0; j < 4; ++j) { //4 values

            y[i][j] = est[index]; //read in
            index++;
            //cout << "index="<<index<< endl;
            //cout << y[i][j]<<endl;;
        }
    }

    double yy12j[nrange][2];
    double yy34j[nrange][2];
    // double ix=0;
    int jj = 0;
    for (int i = 0; i < nrange; ++i) {
        yy12j[i][0] = estFix[jj++];//1j
        yy12j[i][1] = estFix[jj++];//2j
        yy34j[i][0] = estFix[jj++];//3j
        yy34j[i][1] = estFix[jj++];//4mj

        //cout << "index="<<index<< endl;
    }

    gStyle->SetMarkerSize(1.7);
    gStyle->SetMarkerStyle(20);
    c2.SetTopMargin(0.1);
    c2.SetLeftMargin(0.12);
    c2.SetRightMargin(0.35);

    TGraph *gr1;
    TGraph *gr2;
    TGraph *gr3;
    TGraph *gr4;
    TGraph *gr5;
    TGraph *gr6;
    TGraph *gr7;
    TGraph *gr8;
    TGraph *gr9;
    if (constrain_gaus_mean_12j == false) {
        if (show_free_fit_res_34j) {
            //draw free-fit results for 1-4j
            gr1 = new TGraph(n, x, y[1 - 1]);
            gr2 = new TGraph(n, x, y[2 - 1]);
            gr3 = new TGraph(n, x, y[3 - 1]);
            gr4 = new TGraph(n, x, y[4 - 1]);
            gr5 = new TGraph(n, x, y[5 - 1]);
            gr6 = new TGraph(n, x, y[6 - 1]);
            gr7 = new TGraph(n, x, y[7 - 1]);
            gr8 = new TGraph(n, x, y[8 - 1]);
            gr9 = new TGraph(n, x, y[9 - 1]);
        } else {
            //draw: free-fit results for 1-2j, OR
            //      gaus-mean-limited in 1,2j
            gr1 = new TGraph(2, x12j, y[1 - 1]);
            gr2 = new TGraph(2, x12j, y[2 - 1]);
            gr3 = new TGraph(2, x12j, y[3 - 1]);
            gr4 = new TGraph(2, x12j, y[4 - 1]);
            gr5 = new TGraph(2, x12j, y[5 - 1]);
            gr6 = new TGraph(2, x12j, y[6 - 1]);
            gr7 = new TGraph(2, x12j, y[7 - 1]);
            gr8 = new TGraph(2, x12j, y[8 - 1]);
            gr9 = new TGraph(2, x12j, y[9 - 1]);
        }
    } else {
        gr1 = new TGraph(2, x12j, yy12j[1 - 1]);
        gr2 = new TGraph(2, x12j, yy12j[2 - 1]);
        gr3 = new TGraph(2, x12j, yy12j[3 - 1]);
        gr4 = new TGraph(2, x12j, yy12j[4 - 1]);
        gr5 = new TGraph(2, x12j, yy12j[5 - 1]);
        gr6 = new TGraph(2, x12j, yy12j[6 - 1]);
        gr7 = new TGraph(2, x12j, yy12j[7 - 1]);
        gr8 = new TGraph(2, x12j, yy12j[8 - 1]);
        gr9 = new TGraph(2, x12j, yy12j[9 - 1]);
    }

    // constrained
    TGraph *gr_1 = new TGraph(2, xx, yy34j[1 - 1]);
    TGraph *gr_2 = new TGraph(2, xx, yy34j[2 - 1]);
    TGraph *gr_3 = new TGraph(2, xx, yy34j[3 - 1]);
    TGraph *gr_4 = new TGraph(2, xx, yy34j[4 - 1]);
    TGraph *gr_5 = new TGraph(2, xx, yy34j[5 - 1]);
    TGraph *gr_6 = new TGraph(2, xx, yy34j[6 - 1]);
    TGraph *gr_7 = new TGraph(2, xx, yy34j[7 - 1]);
    TGraph *gr_8 = new TGraph(2, xx, yy34j[8 - 1]);
    TGraph *gr_9 = new TGraph(2, xx, yy34j[9 - 1]);

    gr1->SetMarkerColor(kGreen + 1);
    gr2->SetMarkerColor(kGreen + 2);
    gr3->SetMarkerColor(kGreen + 3);
    gr4->SetMarkerColor(kAzure + 7);
    gr5->SetMarkerColor(kAzure - 3);
    gr6->SetMarkerColor(kBlue);
    gr7->SetMarkerColor(kOrange);
    gr8->SetMarkerColor(kOrange - 1);
    gr9->SetMarkerColor(kOrange - 6);

    gr_1->SetMarkerColor(kGreen + 1);
    gr_2->SetMarkerColor(kGreen + 2);
    gr_3->SetMarkerColor(kGreen + 3);
    gr_4->SetMarkerColor(kAzure + 7);
    gr_5->SetMarkerColor(kAzure - 3);
    gr_6->SetMarkerColor(kBlue);
    gr_7->SetMarkerColor(kOrange);
    gr_8->SetMarkerColor(kOrange - 1);
    gr_9->SetMarkerColor(kOrange - 6);

    /*
     gr_1->SetMarkerColor(kRed);
     gr_2->SetMarkerColor(kRed+1);
     gr_3->SetMarkerColor(kRed+2);
     gr_4->SetMarkerColor(kViolet-4);
     gr_5->SetMarkerColor(kViolet);
     gr_6->SetMarkerColor(kViolet-1);
     gr_7->SetMarkerColor(kGray+1);
     gr_8->SetMarkerColor(kGray+2);
     gr_9->SetMarkerColor(kGray+3);
     */

    /*
     gr4->SetMarkerStyle(21);
     gr5->SetMarkerStyle(21);
     gr6->SetMarkerStyle(21);
     gr7->SetMarkerStyle(22);
     gr8->SetMarkerStyle(22);
     gr9->SetMarkerStyle(22);
     */
    gr_1->SetMarkerStyle(22);
    gr_2->SetMarkerStyle(22);
    gr_3->SetMarkerStyle(22);
    gr_4->SetMarkerStyle(22);
    gr_5->SetMarkerStyle(22);
    gr_6->SetMarkerStyle(22);
    gr_7->SetMarkerStyle(22);
    gr_8->SetMarkerStyle(22);
    gr_9->SetMarkerStyle(22);

    // To get desired x range, draw blank histo
    gStyle->SetTitleW(0.9);
    gStyle->SetTitleH(0.05);//?
    if (func == "gaus")
        h = new TH1D("h", "Variation of QCD estimates with fit range (Gaussian)", 4, 0.5, 4.5);
    else if (func == "pol3")
        h = new TH1D("h", "Variation of QCD estimates with fit range (Pol3)", 4, 0.5, 4.5);
    else if (func == "landau")
        h = new TH1D("h", "Variation of QCD estimates with fit range (Landau)", 4, 0.5, 4.5);

    h->SetStats(kFALSE); // no statistics
    h->Draw();
    h->SetYTitle("Deviation = (Est-True)/True");

    double show_range = int(max_dev) + 1;
    if (constrain_gaus_mean_12j)
        show_range = int(max_dev) + 1;

    /*
     double show_range = 3.0;
     if(overview==true) {
     if( !constrain_gaus_mean_12j ) {
     if(max_dev>3.0) show_range = max_dev*1.1;
     }
     else {
     show_range = int(maxDevFix)+1;
     }
     } else {
     //show_range = 0.5; //zoom in 0.5
     show_range = 0.8; //zoom in 0.8
     //show_range = 1.0; //zoom in 1.0
     }
     */
    cout << "show_range: " << show_range << endl;

    //if(oveview==true) h->GetYaxis()->SetRangeUser( 0-max_dev*1.1, max_dev*1.1 ); //<-- large scale
    //  else               h->GetYaxis()->SetRangeUser( -0.5, 0.5 ); //<-- zoom in (0.5)
    //  else               h->GetYaxis()->SetRangeUser( -0.8, 0.8 ); //<-- zoom in (0.8)
    //  else               h->GetYaxis()->SetRangeUser( -1, 1 ); //<-- zoom in (1.0)
    h->GetYaxis()->SetRangeUser(0 - show_range, show_range);
    h->GetXaxis()->SetRangeUser(0.5, 5.5);
    h->GetXaxis()->SetBinLabel(1., "1j");
    h->GetXaxis()->SetBinLabel(2., "2j");
    h->GetXaxis()->SetBinLabel(3., "3j");
    h->GetXaxis()->SetBinLabel(4., "#geq4j");
    h->GetXaxis()->SetLabelSize(0.07);
    h->GetYaxis()->SetTitleOffset(1.3);

    //  gr1->GetHistogram()->GetYaxis()->SetRangeUser(-50,50);
    //  gr1->GetXaxis()->SetRangeUser(0,5);
    //  gr1->GetHistogram()->SetYTitle("Deviation (%)");

    // Free-fits
    gr1->Draw("P");
    gr2->Draw("P"); //to superimpose graphs, do not re-draw axis
    gr3->Draw("P");
    gr4->Draw("P");
    gr5->Draw("P");
    gr6->Draw("P");
    gr7->Draw("P");
    gr8->Draw("P");
    gr9->Draw("P");

    if (!show_free_fit_res_34j) {
        // Constrained fits
        gr_1->Draw("P");
        gr_2->Draw("P");
        gr_3->Draw("P");
        gr_4->Draw("P");
        gr_5->Draw("P");
        gr_6->Draw("P");
        gr_7->Draw("P");
        gr_8->Draw("P");
        gr_9->Draw("P");
    }
    c2.SetGrid(1, 1);

    TLegend leg(0.65, 0.1, 0.98, 0.9);
    leg.SetFillColor(0);
    //  leg.SetTextFont(62);
    if (func != "gaus" || !constrain_gaus_mean_12j) {
        leg.AddEntry(gr1, "Free: 0.1-1.0", "p");
        leg.AddEntry(gr2, "Free: 0.1-1.2", "p");
        leg.AddEntry(gr3, "Free: 0.1-1.4", "p");
        leg.AddEntry(gr4, "Free: 0.2-1.1", "p");
        leg.AddEntry(gr5, "Free: 0.2-1.3", "p");
        leg.AddEntry(gr6, "Free: 0.2-1.5", "p");
        leg.AddEntry(gr7, "Free: 0.3-1.2", "p");
        leg.AddEntry(gr8, "Free: 0.3-1.4", "p");
        leg.AddEntry(gr9, "Free: 0.3-1.6", "p");
    }
    if (func == "gaus" && constrain_gaus_mean_12j) {
        leg.AddEntry(gr1, "#mu-constr.: 0.1-1.0", "p");
        leg.AddEntry(gr2, "#mu-constr.: 0.1-1.2", "p");
        leg.AddEntry(gr3, "#mu-constr.: 0.1-1.4", "p");
        leg.AddEntry(gr4, "#mu-constr.: 0.2-1.1", "p");
        leg.AddEntry(gr5, "#mu-constr.: 0.2-1.3", "p");
        leg.AddEntry(gr6, "#mu-constr.: 0.2-1.5", "p");
        leg.AddEntry(gr7, "#mu-constr.: 0.3-1.2", "p");
        leg.AddEntry(gr8, "#mu-constr.: 0.3-1.4", "p");
        leg.AddEntry(gr9, "#mu-constr.: 0.3-1.6", "p");
    }
    if (!show_free_fit_res_34j) {
        leg.AddEntry(gr_1, "Constrained: 0.1-1.0", "p");
        leg.AddEntry(gr_2, "Constrained: 0.1-1.2", "p");
        leg.AddEntry(gr_3, "Constrained: 0.1-1.4", "p");
        leg.AddEntry(gr_4, "Constrained: 0.2-1.1", "p");
        leg.AddEntry(gr_5, "Constrained: 0.2-1.3", "p");
        leg.AddEntry(gr_6, "Constrained: 0.2-1.5", "p");
        leg.AddEntry(gr_7, "Constrained: 0.3-1.2", "p");
        leg.AddEntry(gr_8, "Constrained: 0.3-1.4", "p");
        leg.AddEntry(gr_9, "Constrained: 0.3-1.6", "p");
    }
    leg.Draw();
    if (func == "gaus")
        c2.SaveAs("qcd_estimate_gaus.gif");
    else if (func == "pol3")
        c2.SaveAs("qcd_estimate_pol3.gif");
    else if (func == "landau")
        c2.SaveAs("qcd_estimate_landau.gif");

    h->GetYaxis()->SetRangeUser(-1, 1);
    c2.SaveAs(Form("qcd_estimate_%s_zoom_11.gif", func.c_str()));

    h->GetYaxis()->SetRangeUser(-2, 2);
    c2.SaveAs(Form("qcd_estimate_%s_zoom_22.gif", func.c_str()));

    h->GetYaxis()->SetRangeUser(-3, 3);
    c2.SaveAs(Form("qcd_estimate_%s_zoom_33.gif", func.c_str()));

    h->GetYaxis()->SetRangeUser(-6, 6);
    c2.SaveAs(Form("qcd_estimate_%s_zoom_66.gif", func.c_str()));

    h->GetYaxis()->SetRangeUser(-8, 8);
    c2.SaveAs(Form("qcd_estimate_%s_zoom_88.gif", func.c_str()));

    //------------------------------
    // Plot 2: Average of 9 ranges
    //------------------------------

    if (plot_average_est) {

        double average_1j;
        double average_2j;
        double average_3j;
        double average_4mj;
        double average_3j_fix;
        double average_4mj_fix;

        for (int i = 0; i < nrange; ++i) {
            average_1j += y[i][0];
            average_2j += y[i][1];
            average_3j += y[i][2];
            average_4mj += y[i][3];
            average_3j_fix += yy34j[i][0];
            average_4mj_fix += yy34j[i][1];
        }
        average_1j = average_1j / nrange;
        average_2j = average_2j / nrange;
        average_3j = average_3j / nrange;
        average_4mj = average_4mj / nrange;
        average_3j_fix = average_3j_fix / nrange;
        average_4mj_fix = average_4mj_fix / nrange;
        cout << "------------------------------------------" << endl;
        cout << "average of 9 ranges, 1j:  " << average_1j << endl;
        cout << "average of 9 ranges, 2j:  " << average_2j << endl;
        cout << "average of 9 ranges, 3j:  " << average_3j_fix << endl;
        cout << "average of 9 ranges, 4mj: " << average_4mj_fix << endl;
        cout << "average of 9 ranges, 3j (free):  " << average_3j << endl;
        cout << "average of 9 ranges, 4mj (free): " << average_4mj << endl;
        cout << "------------------------------------------" << endl;

        double x12[2] = { 1, 2 };
        double av12[2] = { average_1j, average_2j };
        double av34[2] = { average_3j, average_4mj };
        double av34fix[2] = { average_3j_fix, average_4mj_fix };
        TGraph *gra12 = new TGraph(2, x12, av12);
        TGraph *gra34 = new TGraph(2, xx, av34);
        TGraph *gra34fix = new TGraph(2, xx, av34fix);

        TCanvas c3("c3", "Average QCD estimate", 610, 10, 600, 600);
        c3.cd();
        c3.SetGrid(1, 1);
        c3.SetLeftMargin(0.12);
        c3.SetRightMargin(0.35);

        TH1D *hh = h->Clone();
        hh->GetYaxis()->SetRangeUser(-1, 1);
        hh->SetTitle(Form("Average QCD estimate (%s)", func.c_str()));
        hh->Draw();

        gra12->SetMarkerColor(kRed);
        gra34->SetMarkerColor(kRed);
        gra34fix->SetMarkerColor(kGreen + 3);

        gra12->SetMarkerStyle(29);
        gra34->SetMarkerStyle(30);
        gra34fix->SetMarkerStyle(29);

        gra12->SetMarkerSize(2.5);
        gra34->SetMarkerSize(2.5);
        gra34fix->SetMarkerSize(2.5);

        gra12->Draw("P");
        gra34->Draw("P");
        gra34fix->Draw("P");

        TLegend leg2(0.65, 0.7, 0.98, 0.9);
        leg2.SetFillColor(0);
        leg2.AddEntry(gra12, "Free fits (1,2j)", "p");
        leg2.AddEntry(gra34, "Free fits (3,#geq4j)", "p");
        leg2.AddEntry(gra34fix, "Constrained fits (3,#geq4j)", "p");
        leg2.Draw();

        c3.SaveAs(Form("qcd_estimate_%s_average.gif", func.c_str()));
    }

}
