//
// Try fit ranges of:
//
//  0.2  to  1.0, 1.2, 1.4
//  0.3  to  1.1, 1.3, 1.5
//  0.4  to  1.2, 1.4, 1.6
//
void run_fit(bool mc=true, bool data=false){


  gROOT->LoadMacro("fit_new.C");

  SetDrawMC(mc);
  SetDrawData(data);



  gROOT->ProcessLine(".!rm -f est_*gaus.txt");
  gROOT->ProcessLine(".!rm -f est_*pol3.txt");

  double begin[9] = { 0.1, 0.1, 0.1,
		      0.2, 0.2, 0.2,
		      0.3, 0.3, 0.3 };
  double end[9] = {  1.0, 1.2, 1.4,
		     1.1, 1.3, 1.5,
		     1.2, 1.4, 1.6 };


  
  cout << "\n Free Fit"<< endl;
  cout << "==========\n"<< endl;
  // no zoom
  for(short i=0; i<9; ++i)  fit("gaus", begin[i], end[i], false);
  //for(short i=0; i<9; ++i)  fit("landau", begin[i], end[i], false);
  //for(short i=0; i<9; ++i)  fit("pol3", begin[i], end[i], false);
  // zoom in on y-axis
  for(short i=0; i<9; ++i)  fit("gaus", begin[i], end[i], true);
  //for(short i=0; i<9; ++i)  fit("landau", begin[i], end[i], true);
  //for(short i=0; i<9; ++i)  fit("pol3", begin[i], end[i], true);
  

  cout << "\n Constrained Fit"<< endl;
  cout << "=================\n"<< endl;

  /*
  // no zoom
  for(short i=0; i<9; ++i)  fit_fixPara("gaus", begin[i], end[i], false);
  //for(short i=0; i<9; ++i)  fit_fixPara("landau", begin[i], end[i], false);
  //for(short i=0; i<9; ++i)  fit_fixPara("pol3", begin[i], end[i], false);
  // zoom in on y axis
  for(short i=0; i<9; ++i)  fit_fixPara("gaus", begin[i], end[i], true);
  //for(short i=0; i<9; ++i)  fit_fixPara("landau", begin[i], end[i], true);
  //for(short i=0; i<9; ++i)  fit_fixPara("pol3", begin[i], end[i], true);
  */

  gROOT->LoadMacro("plot_qcd_estimate.C");
  plot_qcd_estimate("gaus");

}
