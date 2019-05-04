#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <TFile.h>
#include <TGraph.h>
#include <TF1.h>
#include <TH1.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <TROOT.h>
#include <TImage.h>
#include <TGraphErrors.h>
#include <TKey.h>
#include <TDirectory.h>
#include <sstream>
#include <TPaveText.h>
#include <TPaveStats.h>


using namespace std;


int MaterialTruthLRPlots()
{
  // gROOT->SetBatch(kTRUE);

  // gStyle->SetOptStat(111111);
  // gStyle->SetOptStat(1100);
  // gStyle->SetOptStat(2201);
  gStyle->SetOptStat(0);

  gStyle->SetOptTitle(1);
  gStyle->SetOptFit(0);
  gStyle->SetMarkerStyle(20);
  gStyle->SetMarkerColor(1);
  gStyle->SetMarkerSize(1);
  gStyle->SetLineColor(1);
  gStyle->SetPadRightMargin(.08);


  TFile *f = TFile::Open("gm2GEANE_ana_19831354_1504040981.139.root");


  TH1F* pValues = (TH1F*) f->Get("0nCut/pValues"); 
  pValues->GetXaxis()->SetTitle("P value");
  pValues->GetYaxis()->SetTitle("Tracks");
  pValues->GetYaxis()->SetRangeUser(0, 2000);

  auto canv_pVal = new TCanvas("canv_pVal","canv_pVal",200,10,500,400);
  pValues->Draw();

  TPaveText* tB_pVal = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_pVal->AddText(Form("Mean  %0.3f", pValues->GetMean(1)));
  tB_pVal->AddText(Form("RMS  %0.3f", pValues->GetRMS(1)));
  tB_pVal->SetBorderSize(1);
  tB_pVal->SetFillStyle(1001);
  tB_pVal->Draw("SAME");


  canv_pVal->SaveAs("Images/MaterialTruthLR_Pvalue.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* chi2_10planes = (TH1F*) f->Get("0nCut/chiSquaredsPerPlanesHit/ChiSquareds Planes Hit 10"); 
  chi2_10planes->GetXaxis()->SetTitle("#chi^{2}");
  chi2_10planes->GetYaxis()->SetTitle("Tracks with 10 planes hit");

  double xMax = 40;
  chi2_10planes->GetXaxis()->SetRangeUser(0, xMax);

  auto canv_chi2 = new TCanvas("canv_chi2","canv_chi2",200,10,500,400);
  
  chi2_10planes->Draw();

  TF1* chi2pdf = new TF1("chi2pdf","[2]*ROOT::Math::chisquared_pdf(x,[0],[1])",0,xMax);
  chi2pdf->SetParameters(10-5, 0., chi2_10planes->Integral("WIDTH")); 
  chi2pdf->SetLineColor(2);
  chi2pdf->Draw("SAME");

  TPaveText* tB_chi2 = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_chi2->AddText(Form("Mean  %0.3f", chi2_10planes->GetMean(1)));
  tB_chi2->AddText(Form("RMS  %0.3f", chi2_10planes->GetRMS(1)));
  tB_chi2->SetBorderSize(1);
  tB_chi2->SetFillStyle(1001);
  tB_chi2->Draw("SAME");

  canv_chi2->SaveAs("Images/MaterialTruthLR_Chi2.png");

/////////////////////////////////////////////////////////////////////////////////////

  f->cd("0nCut/PlanePlots/Plane0/Truth Pulls/Planes Hit 0/");

  TKey* key = gDirectory->GetKey("1P Truth Pull Planes Hit 0"); 
    
    TH1F* truthPull_1P = (TH1F*) key->ReadObj(); 
    // truthPull_1P->SetTitle("(1/P) Truth Pull");
    truthPull_1P->GetXaxis()->SetTitle("#Delta(1/P)/#sigma");
    truthPull_1P->GetYaxis()->SetTitle("Tracks");

    auto canv_truthPull_1P = new TCanvas("canv_truthPull_1P","canv_truthPull_1P",200,10,500,400);

    truthPull_1P->Draw();

    canv_truthPull_1P->SetLogy();
    canv_truthPull_1P->Update();

    TF1* unitGauss = new TF1("unitGauss","gaus(0)",-10,10);
    unitGauss->SetLineColor(2);
    unitGauss->SetParameters(truthPull_1P->Integral("WIDTH"), 0, 1);

    truthPull_1P->Fit(unitGauss, "Q");

    unitGauss->Draw("SAME");

      TPaveText* tB_1P = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
      tB_1P->AddText(Form("Mean  %0.3f", unitGauss->GetParameter(1)));
      tB_1P->AddText(Form("RMS  %0.3f", unitGauss->GetParameter(2)));
      tB_1P->SetBorderSize(1);
      tB_1P->SetFillStyle(1001);
      tB_1P->Draw("SAME");

  canv_truthPull_1P->SaveAs("Images/MaterialTruthLR_TruthPull_1P.png");

/////////////////////////////////////////////////////////////////////////////////////

   key = gDirectory->GetKey("PuPx Truth Pull Planes Hit 0"); 
    
    TH1F* truthPull_PuPz = (TH1F*) key->ReadObj(); 
    truthPull_PuPz->GetXaxis()->SetTitle("#Delta(P_{u} / P_{z})/#sigma");
    truthPull_PuPz->GetYaxis()->SetTitle("Tracks");

    auto canv_truthPull_PuPz = new TCanvas("canv_truthPull_PuPz","canv_truthPull_PuPz",200,10,500,400);

    truthPull_PuPz->Draw();

    canv_truthPull_PuPz->SetLogy();
    canv_truthPull_PuPz->Update();

    truthPull_PuPz->Fit(unitGauss, "Q");

    unitGauss->Draw("SAME");

      TPaveText* tB_PuPx = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
      tB_PuPx->AddText(Form("Mean  %0.3f", unitGauss->GetParameter(1)));
      tB_PuPx->AddText(Form("RMS  %0.3f", unitGauss->GetParameter(2)));
      tB_PuPx->SetBorderSize(1);
      tB_PuPx->SetFillStyle(1001);
      tB_PuPx->Draw("SAME");

  canv_truthPull_PuPz->SaveAs("Images/MaterialTruthLR_TruthPull_PuPz.png");

/////////////////////////////////////////////////////////////////////////////////////

   key = gDirectory->GetKey("PvPx Truth Pull Planes Hit 0"); 
    
    TH1F* truthPull_PvPz = (TH1F*) key->ReadObj(); 
    truthPull_PvPz->GetXaxis()->SetTitle("#Delta(P_{v} / P_{z})/#sigma");
    truthPull_PvPz->GetYaxis()->SetTitle("Tracks");

    auto canv_truthPull_PvPz = new TCanvas("canv_truthPull_PvPz","canv_truthPull_PvPz",200,10,500,400);

    truthPull_PvPz->Draw();

    canv_truthPull_PvPz->SetLogy();
    canv_truthPull_PvPz->Update();

    truthPull_PvPz->Fit(unitGauss, "Q");

    unitGauss->Draw("SAME");

      TPaveText* tB_PvPx = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
      tB_PvPx->AddText(Form("Mean  %0.3f", unitGauss->GetParameter(1)));
      tB_PvPx->AddText(Form("RMS  %0.3f", unitGauss->GetParameter(2)));
      tB_PvPx->SetBorderSize(1);
      tB_PvPx->SetFillStyle(1001);
      tB_PvPx->Draw("SAME");

  canv_truthPull_PvPz->SaveAs("Images/MaterialTruthLR_TruthPull_PvPz.png");

/////////////////////////////////////////////////////////////////////////////////////

   key = gDirectory->GetKey("U Truth Pull Planes Hit 0"); 
    
    TH1F* truthPull_U = (TH1F*) key->ReadObj(); 
    truthPull_U->GetXaxis()->SetTitle("#DeltaU/#sigma");
    truthPull_U->GetYaxis()->SetTitle("Tracks");

    auto canv_truthPull_U = new TCanvas("canv_truthPull_U","canv_truthPull_U",200,10,500,400);

    truthPull_U->Draw();

    canv_truthPull_U->SetLogy();
    canv_truthPull_U->Update();

    truthPull_U->Fit(unitGauss, "Q");

    unitGauss->Draw("SAME");

      TPaveText* tB_U = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
      tB_U->AddText(Form("Mean  %0.3f", unitGauss->GetParameter(1)));
      tB_U->AddText(Form("RMS  %0.3f", unitGauss->GetParameter(2)));
      tB_U->SetBorderSize(1);
      tB_U->SetFillStyle(1001);
      tB_U->Draw("SAME");

  canv_truthPull_U->SaveAs("Images/MaterialTruthLR_TruthPull_U.png");

/////////////////////////////////////////////////////////////////////////////////////

   key = gDirectory->GetKey("V Truth Pull Planes Hit 0"); 
    
    TH1F* truthPull_V = (TH1F*) key->ReadObj(); 
    truthPull_V->GetXaxis()->SetTitle("#DeltaV/#sigma");
    truthPull_V->GetYaxis()->SetTitle("Tracks");

    auto canv_truthPull_V = new TCanvas("canv_truthPull_V","canv_truthPull_V",200,10,500,400);

    truthPull_V->Draw();

    canv_truthPull_V->SetLogy();
    canv_truthPull_V->Update();

    truthPull_V->Fit(unitGauss, "Q");

    unitGauss->Draw("SAME");

      TPaveText* tB_V = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
      tB_V->AddText(Form("Mean  %0.3f", unitGauss->GetParameter(1)));
      tB_V->AddText(Form("RMS  %0.3f", unitGauss->GetParameter(2)));
      tB_V->SetBorderSize(1);
      tB_V->SetFillStyle(1001);
      tB_V->Draw("SAME");

  canv_truthPull_V->SaveAs("Images/MaterialTruthLR_TruthPull_V.png");

/////////////////////////////////////////////////////////////////////////////////////

  gStyle->SetOptStat(1101);

  TH1F* energyLoss_true = (TH1F*) f->Get("0nCut/energyLoss/energyLoss2MeVBound"); 
  energyLoss_true->GetXaxis()->SetTitle("Particle energy lost (MeV)");
  energyLoss_true->GetYaxis()->SetTitle("Tracks");

  TH1F* energyLoss_pred = (TH1F*) f->Get("0nCut/energyLoss/predEnergyLoss2MeVBound"); 

  TCanvas* canv_energyLoss = new TCanvas("canv_energyLoss","canv_energyLoss",200,10,500,400);

  // energyLoss_true->SetTitle("True Energy Loss");
  energyLoss_true->SetName("True energy loss");
  energyLoss_true->Draw("hist"); 

  energyLoss_pred->SetName("Predicted energy loss");
  energyLoss_pred->SetLineColor(2);
  energyLoss_pred->Draw("histsames");

  canv_energyLoss->Update();

  TPaveStats* energyLoss_true_stats = (TPaveStats*) energyLoss_true->GetListOfFunctions()->FindObject("stats");
  energyLoss_true_stats->SetX1NDC(.7);
  energyLoss_true_stats->SetX2NDC(.95);
  energyLoss_true_stats->SetY1NDC(.66);
  energyLoss_true_stats->SetY2NDC(.9);
      energyLoss_true_stats->SetBorderSize(1);
      energyLoss_true_stats->SetFillStyle(1001);

  canv_energyLoss->Modified();

  TPaveStats* energyLoss_pred_stats = (TPaveStats*) energyLoss_pred->GetListOfFunctions()->FindObject("stats");
  energyLoss_pred_stats->SetX1NDC(.7);
  energyLoss_pred_stats->SetX2NDC(.95);
  energyLoss_pred_stats->SetY1NDC(.42);
  energyLoss_pred_stats->SetY2NDC(.66);
  energyLoss_pred_stats->SetTextColor(2);
  energyLoss_pred_stats->SetLineColor(2);
      energyLoss_pred_stats->SetBorderSize(1);
      energyLoss_pred_stats->SetFillStyle(1001);

  canv_energyLoss->Modified();
  canv_energyLoss->SaveAs("Images/MaterialTruthLR_EnergyLost.png");

/////////////////////////////////////////////////////////////////////////////////////

  gStyle->SetOptStat(0);

  TH1F* numberPlanesHit = (TH1F*) f->Get("0nCut/numPlanesHit/numPlanesHit"); 
  numberPlanesHit->GetXaxis()->SetTitle("Number of planes hit");
  numberPlanesHit->GetYaxis()->SetTitle("Tracks");

  auto canv_numPlanesHit = new TCanvas("canv_numPlanesHit","canv_numPlanesHit",200,10,500,400);

  numberPlanesHit->Draw();
  canv_numPlanesHit->SaveAs("Images/MaterialTruthLR_NumberPlanesHit.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* numberPlanesHitVsP = (TH1F*) f->Get("0nCut/numPlanesHit/numPlanesHit vs P"); 
  numberPlanesHitVsP->GetXaxis()->SetTitle("Track Momentum (GeV)");
  numberPlanesHitVsP->GetYaxis()->SetTitle("Number of planes hit");

  auto canv_numPlanesHitVsP = new TCanvas("canv_numPlanesHitVsP","canv_numPlanesHitVsP",200,10,500,400);

  numberPlanesHitVsP->Draw("colz");
  canv_numPlanesHitVsP->SaveAs("Images/MaterialTruthLR_NumberPlanesHitVsP.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* numberIterations = (TH1F*) f->Get("0nCut/Iterations/numIterations"); 
  numberIterations->GetXaxis()->SetTitle("Number of fit iterations");
  numberIterations->GetYaxis()->SetTitle("Tracks");

  auto canv_numIterations = new TCanvas("canv_numIterations","canv_numIterations",200,10,500,400);

  numberIterations->Draw();

  canv_numIterations->SetLogy();
  canv_numIterations->Update();
  canv_numIterations->SaveAs("Images/MaterialTruthLR_NumberIterations.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_PTotal = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Fit Parameters/P Fit Plane 0"); 
  fitted_PTotal->GetXaxis()->SetTitle("Total Momentum (GeV)");
  fitted_PTotal->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_PTotal = new TCanvas("canv_fitted_PTotal","canv_fitted_PTotal",200,10,500,400);
  
  fitted_PTotal->Draw();

  TPaveText* tB_fitted_PTotal = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_PTotal->AddText(Form("Mean  %0.1f", fitted_PTotal->GetMean(1)));
  tB_fitted_PTotal->AddText(Form("RMS  %0.1f", fitted_PTotal->GetRMS(1)));
  tB_fitted_PTotal->SetBorderSize(1);
  tB_fitted_PTotal->SetFillStyle(1001);
  tB_fitted_PTotal->Draw("SAME");

  canv_fitted_PTotal->SaveAs("Images/MaterialTruthLR_Fitted_PTotal.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_Py = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Fit Parameters/Py Fit Plane 0"); 
  fitted_Py->GetXaxis()->SetTitle("P_{y} (MeV)");
  fitted_Py->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_Py = new TCanvas("canv_fitted_Py","canv_fitted_Py",200,10,500,400);
  
  fitted_Py->Draw();

  TPaveText* tB_fitted_Py = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_Py->AddText(Form("Mean  %0.2f", fitted_Py->GetMean(1)));
  tB_fitted_Py->AddText(Form("RMS  %0.2f", fitted_Py->GetRMS(1)));
  tB_fitted_Py->SetBorderSize(1);
  tB_fitted_Py->SetFillStyle(1001);
  tB_fitted_Py->Draw("SAME");

  canv_fitted_Py->SaveAs("Images/MaterialTruthLR_Fitted_Py.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_Px = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Fit Parameters/Pz Fit Plane 0"); 
  fitted_Px->GetXaxis()->SetTitle("P_{x} (MeV)");
  fitted_Px->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_Px = new TCanvas("canv_fitted_Px","canv_fitted_Px",200,10,500,400);
  
  fitted_Px->Draw();

  TPaveText* tB_fitted_Px = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_Px->AddText(Form("Mean  %0.2f", fitted_Px->GetMean(1)));
  tB_fitted_Px->AddText(Form("RMS  %0.2f", fitted_Px->GetRMS(1)));
  tB_fitted_Px->SetBorderSize(1);
  tB_fitted_Px->SetFillStyle(1001);
  tB_fitted_Px->Draw("SAME");

  canv_fitted_Px->SaveAs("Images/MaterialTruthLR_Fitted_Px.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_Y = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Fit Parameters/Y Fit Plane 0"); 
  fitted_Y->GetXaxis()->SetTitle("Y (mm)");
  fitted_Y->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_Y = new TCanvas("canv_fitted_Y","canv_fitted_Y",200,10,500,400);
  
  fitted_Y->Draw();

  TPaveText* tB_fitted_Y = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_Y->AddText(Form("Mean  %0.2f", fitted_Y->GetMean(1)));
  tB_fitted_Y->AddText(Form("RMS  %0.2f", fitted_Y->GetRMS(1)));
  tB_fitted_Y->SetBorderSize(1);
  tB_fitted_Y->SetFillStyle(1001);
  tB_fitted_Y->Draw("SAME");

  canv_fitted_Y->SaveAs("Images/MaterialTruthLR_Fitted_Y.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_X = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Fit Parameters/Z Fit Plane 0"); 
  fitted_X->GetXaxis()->SetTitle("X (mm)");
  fitted_X->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_X = new TCanvas("canv_fitted_X","canv_fitted_X",200,10,500,400);
  
  fitted_X->Draw();

  TPaveText* tB_fitted_X = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_X->AddText(Form("Mean  %0.2f", fitted_X->GetMean(1)));
  tB_fitted_X->AddText(Form("RMS  %0.2f", fitted_X->GetRMS(1)));
  tB_fitted_X->SetBorderSize(1);
  tB_fitted_X->SetFillStyle(1001);
  tB_fitted_X->Draw("SAME");

  canv_fitted_X->SaveAs("Images/MaterialTruthLR_Fitted_X.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_PTotal_AbsResidual = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Truth Residuals/Absolute/P Truth Residual Abs Plane 0"); 
  fitted_PTotal_AbsResidual->GetXaxis()->SetTitle("#Delta P (MeV)");
  fitted_PTotal_AbsResidual->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_PTotal_AbsResidual = new TCanvas("canv_fitted_PTotal_AbsResidual","canv_fitted_PTotal_AbsResidual",200,10,500,400);
  
  fitted_PTotal_AbsResidual->Draw();

  TPaveText* tB_fitted_PTotal_AbsResidual = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_PTotal_AbsResidual->AddText(Form("Mean  %0.2f", fitted_PTotal_AbsResidual->GetMean(1)));
  tB_fitted_PTotal_AbsResidual->AddText(Form("RMS  %0.2f", fitted_PTotal_AbsResidual->GetRMS(1)));
  tB_fitted_PTotal_AbsResidual->SetBorderSize(1);
  tB_fitted_PTotal_AbsResidual->SetFillStyle(1001);
  tB_fitted_PTotal_AbsResidual->Draw("SAME");

  canv_fitted_PTotal_AbsResidual->SaveAs("Images/MaterialTruthLR_Fitted_PTotal_AbsResidual.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_PTotal_RelResidual = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Truth Residuals/Relative/P Truth Residual Rel Plane 0"); 
  fitted_PTotal_RelResidual->GetXaxis()->SetTitle("#Delta P / P");
  fitted_PTotal_RelResidual->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_PTotal_RelResidual = new TCanvas("canv_fitted_PTotal_RelResidual","canv_fitted_PTotal_RelResidual",200,10,500,400);
  
  fitted_PTotal_RelResidual->Draw();

  TPaveText* tB_fitted_PTotal_RelResidual = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_PTotal_RelResidual->AddText(Form("Mean  %0.3f", fitted_PTotal_RelResidual->GetMean(1)));
  tB_fitted_PTotal_RelResidual->AddText(Form("RMS  %0.3f", fitted_PTotal_RelResidual->GetRMS(1)));
  tB_fitted_PTotal_RelResidual->SetBorderSize(1);
  tB_fitted_PTotal_RelResidual->SetFillStyle(1001);
  tB_fitted_PTotal_RelResidual->Draw("SAME");

  canv_fitted_PTotal_RelResidual->SaveAs("Images/MaterialTruthLR_Fitted_PTotal_RelResidual.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_Py_AbsResidual = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Truth Residuals/Absolute/Py Truth Residual Abs Plane 0"); 
  fitted_Py_AbsResidual->GetXaxis()->SetTitle("#Delta P_{y} (MeV)");
  fitted_Py_AbsResidual->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_Py_AbsResidual = new TCanvas("canv_fitted_Py_AbsResidual","canv_fitted_Py_AbsResidual",200,10,500,400);
  
  fitted_Py_AbsResidual->Draw();

  TPaveText* tB_fitted_Py_AbsResidual = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_Py_AbsResidual->AddText(Form("Mean  %0.3f", fitted_Py_AbsResidual->GetMean(1)));
  tB_fitted_Py_AbsResidual->AddText(Form("RMS  %0.3f", fitted_Py_AbsResidual->GetRMS(1)));
  tB_fitted_Py_AbsResidual->SetBorderSize(1);
  tB_fitted_Py_AbsResidual->SetFillStyle(1001);
  tB_fitted_Py_AbsResidual->Draw("SAME");

  canv_fitted_Py_AbsResidual->SaveAs("Images/MaterialTruthLR_Fitted_Py_AbsResidual.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_Px_AbsResidual = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Truth Residuals/Absolute/Pz Truth Residual Abs Plane 0"); 
  fitted_Px_AbsResidual->GetXaxis()->SetTitle("#Delta P_{x} (MeV)");
  fitted_Px_AbsResidual->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_Px_AbsResidual = new TCanvas("canv_fitted_Px_AbsResidual","canv_fitted_Px_AbsResidual",200,10,500,400);
  
  fitted_Px_AbsResidual->Draw();

  TPaveText* tB_fitted_Px_AbsResidual = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_Px_AbsResidual->AddText(Form("Mean  %0.3f", fitted_Px_AbsResidual->GetMean(1)));
  tB_fitted_Px_AbsResidual->AddText(Form("RMS  %0.3f", fitted_Px_AbsResidual->GetRMS(1)));
  tB_fitted_Px_AbsResidual->SetBorderSize(1);
  tB_fitted_Px_AbsResidual->SetFillStyle(1001);
  tB_fitted_Px_AbsResidual->Draw("SAME");

  canv_fitted_Px_AbsResidual->SaveAs("Images/MaterialTruthLR_Fitted_Px_AbsResidual.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_Y_AbsResidual = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Truth Residuals/Absolute/Y Truth Residual Abs Plane 0"); 
  fitted_Y_AbsResidual->GetXaxis()->SetTitle("#Delta Y (mm)");
  fitted_Y_AbsResidual->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_Y_AbsResidual = new TCanvas("canv_fitted_Y_AbsResidual","canv_fitted_Y_AbsResidual",200,10,500,400);
  
  fitted_Y_AbsResidual->Draw();

  TPaveText* tB_fitted_Y_AbsResidual = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_Y_AbsResidual->AddText(Form("Mean  %0.3f", fitted_Y_AbsResidual->GetMean(1)));
  tB_fitted_Y_AbsResidual->AddText(Form("RMS  %0.3f", fitted_Y_AbsResidual->GetRMS(1)));
  tB_fitted_Y_AbsResidual->SetBorderSize(1);
  tB_fitted_Y_AbsResidual->SetFillStyle(1001);
  tB_fitted_Y_AbsResidual->Draw("SAME");

  canv_fitted_Y_AbsResidual->SaveAs("Images/MaterialTruthLR_Fitted_Y_AbsResidual.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* fitted_X_AbsResidual = (TH1F*) f->Get("0nCut/PlanePlots/Plane0/Truth Residuals/Absolute/Z Truth Residual Abs Plane 0"); 
  fitted_X_AbsResidual->GetXaxis()->SetTitle("#Delta X (mm)");
  fitted_X_AbsResidual->GetYaxis()->SetTitle("Tracks");

  auto canv_fitted_X_AbsResidual = new TCanvas("canv_fitted_X_AbsResidual","canv_fitted_X_AbsResidual",200,10,500,400);
  
  fitted_X_AbsResidual->Draw();

  TPaveText* tB_fitted_X_AbsResidual = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_fitted_X_AbsResidual->AddText(Form("Mean  %0.3f", fitted_X_AbsResidual->GetMean(1)));
  tB_fitted_X_AbsResidual->AddText(Form("RMS  %0.3f", fitted_X_AbsResidual->GetRMS(1)));
  tB_fitted_X_AbsResidual->SetBorderSize(1);
  tB_fitted_X_AbsResidual->SetFillStyle(1001);
  tB_fitted_X_AbsResidual->Draw("SAME");

  canv_fitted_X_AbsResidual->SaveAs("Images/MaterialTruthLR_Fitted_X_AbsResidual.png");

/////////////////////////////////////////////////////////////////////////////////////

  return 1;
}
