#include "ana.C"

Int_t mainAna(){
  ana *t = new ana("../ex02.root");
  t->Loop();
  return 0;
}

//Int_t mainAna(){
//ana *t;
//gROOT->LoadMacro("ana.C");
//ana *t = new ana("../lhcBrich-build/lhcBrich.root");
//ana *t = new ana("../lhcBrich_K_20GeV.root");  //54.64
//ana *t = new ana("../lhcBrich_Pi_20GeV.root"); //61.25
//ana *t = new ana("../lhcBrich_K_50GeV.root");  //60.65
//ana *t = new ana("../lhcBrich_Pi_50GeV.root");   //61.64  
//ana *t = new ana("../lhcBrich_K_5GeV_aerogel.root");
//t->SetPMTpositionResolution(3);
//t->Loop();
//t->SetWavelengthMin(300);
//t->SetWavelengthMax(310.0);
//t->Loop();
//return 0;
//}
