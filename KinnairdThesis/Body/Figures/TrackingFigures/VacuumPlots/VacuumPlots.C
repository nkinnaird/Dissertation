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


using namespace std;


int VacuumPlots()
{
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


  TFile *f = TFile::Open("total-vacuumtracks-plots.root");


  TH1F* pValues = (TH1F*) f->Get("9nCut/pValues"); 
  pValues->GetXaxis()->SetTitle("P value");
  pValues->GetYaxis()->SetTitle("Tracks");
  pValues->GetYaxis()->SetRangeUser(0, 3000);

  auto canv_pVal = new TCanvas("canv_pVal","canv_pVal",200,10,500,400);
  pValues->Draw();

  TPaveText* tB_pVal = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_pVal->AddText(Form("Mean  %0.3f", pValues->GetMean(1)));
  tB_pVal->AddText(Form("RMS  %0.3f", pValues->GetRMS(1)));
  tB_pVal->SetBorderSize(1);
  tB_pVal->SetFillStyle(1001);
  tB_pVal->Draw("SAME");


  canv_pVal->SaveAs("Images/Vacuum_Pvalue.png");

/////////////////////////////////////////////////////////////////////////////////////

  TH1F* chi2_24planes = (TH1F*) f->Get("9nCut/chiSquaredsPerPlanesHit/ChiSquareds Planes Hit 24"); 
  chi2_24planes->GetXaxis()->SetTitle("#chi^{2}");
  chi2_24planes->GetYaxis()->SetTitle("Tracks with 24 planes hit");

  double xMax = 60;
  chi2_24planes->GetXaxis()->SetRangeUser(0, xMax);

  auto canv_chi2 = new TCanvas("canv_chi2","canv_chi2",200,10,500,400);
  
  chi2_24planes->Draw();

  TF1* chi2pdf = new TF1("chi2pdf","[2]*ROOT::Math::chisquared_pdf(x,[0],[1])",0,xMax);
  chi2pdf->SetParameters(24-5, 0., chi2_24planes->Integral("WIDTH")); 
  chi2pdf->SetLineColor(2);
  chi2pdf->Draw("SAME");

  TPaveText* tB_chi2 = new TPaveText(0.8,0.78,0.97,0.95, "NDC");
  tB_chi2->AddText(Form("Mean  %0.3f", chi2_24planes->GetMean(1)));
  tB_chi2->AddText(Form("RMS  %0.3f", chi2_24planes->GetRMS(1)));
  tB_chi2->SetBorderSize(1);
  tB_chi2->SetFillStyle(1001);
  tB_chi2->Draw("SAME");

  canv_chi2->SaveAs("Images/Vacuum_Chi2.png");

/////////////////////////////////////////////////////////////////////////////////////

  f->cd("9nCut/PlanePlots/Plane0/Truth Pulls/Planes Hit 0/");

  TKey* key = gDirectory->GetKey("1/P Truth Pull Planes Hit 0"); 
    
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

  canv_truthPull_1P->SaveAs("Images/Vacuum_TruthPull_1P.png");

/////////////////////////////////////////////////////////////////////////////////////

   key = gDirectory->GetKey("Pu/Px Truth Pull Planes Hit 0"); 
    
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

  canv_truthPull_PuPz->SaveAs("Images/Vacuum_TruthPull_PuPz.png");

/////////////////////////////////////////////////////////////////////////////////////

   key = gDirectory->GetKey("Pv/Px Truth Pull Planes Hit 0"); 
    
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

  canv_truthPull_PvPz->SaveAs("Images/Vacuum_TruthPull_PvPz.png");

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

  canv_truthPull_U->SaveAs("Images/Vacuum_TruthPull_U.png");

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

  canv_truthPull_V->SaveAs("Images/Vacuum_TruthPull_V.png");

/////////////////////////////////////////////////////////////////////////////////////

  return 1;
}
