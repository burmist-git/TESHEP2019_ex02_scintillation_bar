#define ana_cxx
#include "ana.h"
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TMath.h>

void ana::Loop()
{
  //   In a ROOT session, you can do:
  //      Root > .L ana.C
  //      Root > ana t
  //      Root > t.GetEntry(12); // Fill t data members with entry number 12
  //      Root > t.Show();       // Show values of entry 12
  //      Root > t.Show(16);     // Read and show values of entry 16
  //      Root > t.Loop();       // Loop on all entries
  //
  
  //     This is the loop skeleton where:
  //    jentry is the global entry number in the chain
  //    ientry is the entry number in the current Tree
  //  Note that the argument to GetEntry must be:
  //    jentry for TChain::GetEntry
  //    ientry for TTree::GetEntry and TBranch::GetEntry
  //
  //       To read only selected branches, Insert statements like:
  // METHOD1:
  //    fChain->SetBranchStatus("*",0);  // disable all branches
  //    fChain->SetBranchStatus("branchname",1);  // activate branchname
  // METHOD2: replace line
  //    fChain->GetEntry(jentry);       //read all branches
  //by  b_branchname->GetEntry(ientry); //read only this branch

  TH1D *h1_nPhot = new TH1D("h1_nPhot","nPhot",50,0.0,100); 
  TH1D *h1_PosX  = new TH1D("h1_PosX" ,"PosX" ,200,-2.0,2.0); 
  TH1D *h1_PosY  = new TH1D("h1_PosY" ,"PosY" ,200, 7.0,10.0); 
  TH1D *h1_PosZ  = new TH1D("h1_PosZ" ,"PosZ" ,1000,-1000,1000); 
  TH1D *h1_time  = new TH1D("h1_time" ,"time" ,1000, 0.0,100.0); 

  TH2D *h2_PosX_vs_PosY  = new TH2D("h2_PosX_vs_PosY" ,"PosX vs PosY" , 200,-2.0,2.0, 200,7.0,10.0); 
  //TH2D *h2_PosX_vs_PosZ_2  = new TH2D("h2_PosX_vs_PosZ_2" ,"PosX vs PosZ 2" ,1000,0,1700,1000,-1000,1000); 

  //TH1D *h1_R = new TH1D("h1_R","R",5000,0,500); 

  Int_t i;

  TRandom3 *rnd = new TRandom3(31232);

  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"nentries = "<<nentries<<endl;
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    h1_nPhot->Fill(nPhot);
    for(i = 0;i<nPhot;i++){
      h1_PosX->Fill(PosX[i]);
      h1_PosY->Fill(PosY[i]);
      h1_PosZ->Fill(PosZ[i]);
      h1_time->Fill(Time[i]);
      h2_PosX_vs_PosY->Fill(PosX[i],PosY[i]);
    }
  }

  TCanvas *c1 = new TCanvas("c1","canva",10,10,1500,1000);
  c1->Divide(3,2);
  c1->cd(1);
  h1_nPhot->Draw();
  c1->cd(2);
  h1_PosX->Draw();
  c1->cd(3);
  h1_PosY->Draw();
  c1->cd(4);
  h1_PosZ->Draw();
  c1->cd(5);
  h2_PosX_vs_PosY->Draw("ZCOLOR");
  c1->cd(6);
  h1_time->Draw();
  
  //TCanvas *c2 = new TCanvas("c2","canva",20,10,1800,600);
  //c2->Divide(3,1);
  //c2->cd(1);
  //h2_PosX_vs_PosZ->Draw();
  //c2->cd(2);
  //h2_PosX_vs_PosZ_2->Draw();
  //c2->cd(3);
  //h1_time->Draw();
  //h1_nPhot->Draw();
  //h1_PosX->Draw();
  //h1_R->Draw();

}
