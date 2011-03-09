//
// 1 Jul 2010
//
// Helper code to add texts and legend to reliso fit plots.
// Used in:
//     fit_data_allj_linear.C
//     fit_data_allj_expo.C
//
//----------------------------------------------------------------

void add_cms_label(double intlumi) {
    add_cms_label(intlumi, "");
}

void add_cms_label(double intlumi, string njet) {

    //  TPaveText * mytext = new TPaveText(0.6,0.83,0.93,0.93,"NDC");
    TPaveText * mytext = new TPaveText(0.3, 0.8, 0.6, 0.93, "NDC");
    mytext->AddText("CMS Preliminary");
    mytext->AddText(Form("%.1f pb^{-1} at  #sqrt{s} = 7 TeV", intlumi));
    if (njet != "") {
        if (njet.find('mj') != string::npos) {
            string thisJet = njet.substr(njet.find('mj') - 1, 1);
            mytext->AddText(Form("e+jets, N_{jets}#geq %sj", thisJet.c_str()));
            cout << "thisjet " << thisJet << endl;
        }
        else if(njet.find('orMoreJets') != string::npos){
            string thisJet = njet.substr(njet.find('orMoreJets') - 1, 1);
            mytext->AddText(Form("e+jets, N_{jets}#geq %sj", thisJet.c_str()));
            cout << "thisjet " << thisJet << endl;
        }
        else {
            mytext->AddText(Form("e+jets, N_{jets} = %s", njet.c_str()));
        }
    }
    //  mytext->SetFillColor(0);
    mytext->SetFillStyle(0);
    mytext->SetBorderSize(0);
    mytext->SetTextFont(42);
    mytext->SetTextAlign(13);
    mytext->Draw();

}

void add_fit_res(double Fit_From, double chi2, double ndf) {

    //  TPaveText *mt2=new TPaveText(0.6,0.7,0.93,0.8,"NDC");
    TPaveText *mt2 = new TPaveText(0.64, 0.8, 0.93, 0.93, "NDC");
    mt2->AddText(Form("Fit Range: %0.1f-1.6", Fit_From));
    mt2->AddText(Form("#chi^{2}/N_{dof} = %.2f/%.0f", chi2, ndf));
    mt2->SetBorderSize(0);
    //  mt2->SetFillColor(0);
    mt2->SetFillStyle(0);
    mt2->SetTextFont(42);
    mt2->SetTextAlign(13);
    mt2->Draw();

}

void add_legend(string func = "pol1") {

    // legend
    //  TLegend *leg = new TLegend( 0.6,0.33,0.9,0.66 );
    //  TLegend *leg = new TLegend( 0.64,0.35,0.9,0.75 );
    TLegend *leg = new TLegend(0.64, 0.4, 0.9, 0.9);

    //  leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);

    // Here I define coloured lines for use in the legend
    TF1 *blue = new TF1("blue", "pol0", 0, 1);
    TF1 *red = new TF1("red", "pol0", 0, 1);

    blue->SetLineColor(kBlue);
    red->SetLineColor(kRed);

    red->SetLineWidth(2);
    blue->SetLineWidth(2);

    blue->SetLineStyle(kDashed);

    // Add entry to legend
    leg->AddEntry(data, "Data", "LP");
    if (func == "pol1")
        leg->AddEntry(red, "Linear Fit", "l");
    else if (func == "expo")
        leg->AddEntry(red, "Exponenetial Fit", "l");
    else if (func == "gaus")
        leg->AddEntry(red, "Gaussian Fit", "l");
    leg->AddEntry(blue, "Extrapolation", "l");
    leg->AddEntry(tt, "t#bar{t}", "F");
    ///  leg->AddEntry(stop,  "Single-Top", "F");
    leg->AddEntry(wj, "W#rightarrowl#nu", "F");
    leg->AddEntry(zj, "Z/#gamma*#rightarrowl^{+}l^{-}", "F");
    leg->AddEntry(QCD, "QCD & #gamma+jets", "F");
    leg->Draw();

}

void add_legend_nofit() {

    // legend
    TLegend *leg = new TLegend(0.6, 0.4 + 0.15, 0.9, 0.78 + 0.1);
    //  leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);

    // Add entry to legend
    leg->AddEntry(data, "Data", "LP");
    leg->AddEntry(tt, "t#bar{t}", "F");
    //  leg->AddEntry(stop,  "Single-top", "F");
    leg->AddEntry(wj, "W#rightarrowl#nu", "F");
    leg->AddEntry(zj, "Z/#gamma*#rightarrowl^{+}l^{-}", "F");
    leg->AddEntry(QCD, "QCD & #gamma+jets", "F");
    leg->Draw();

}

