//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul 16 04:13:07 2014 by ROOT version 5.32/04
// from TTree T/UA9 Data Tree
// found on file: lhcBrich.root
//////////////////////////////////////////////////////////

#ifndef ana_h
#define ana_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class ana {
public :

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           EventID;
   Int_t           BunchXID;
   Int_t           NTotPhot;
   Int_t           Nhits;
   Int_t           primType;
   Double_t        primMomX;
   Double_t        primMomY;
   Double_t        primMomZ;
   Double_t        primPosX;
   Double_t        primPosY;
   Double_t        primPosZ;
   Double_t        primTime;
   Double_t        trigTopL;
   Double_t        trigTopEdep;
   Double_t        trigBotL;
   Double_t        trigBotEdep;
   Int_t           nPhot;
   static const Int_t nMaxPhotons = 2000;
   Int_t           TrackID[nMaxPhotons];   //[nPhot]
   Int_t           ParentID[nMaxPhotons];   //[nPhot]
   Double_t        Energy[nMaxPhotons];   //[nPhot]
   Double_t        Wavelength[nMaxPhotons];   //[nPhot]
   Double_t        Time[nMaxPhotons];   //[nPhot]
   Double_t        photPathLen[nMaxPhotons];   //[nPhot]
   Int_t           SecID[nMaxPhotons];   //[nPhot]
   Int_t           chID[nMaxPhotons];   //[nPhot]
   Double_t        PosX[nMaxPhotons];   //[nPhot]
   Double_t        PosY[nMaxPhotons];   //[nPhot]
   Double_t        PosZ[nMaxPhotons];   //[nPhot]
   Double_t        MomX[nMaxPhotons];   //[nPhot]
   Double_t        MomY[nMaxPhotons];   //[nPhot]
   Double_t        MomZ[nMaxPhotons];   //[nPhot]
   Double_t        trkMomX[nMaxPhotons];   //[nPhot]
   Double_t        trkMomY[nMaxPhotons];   //[nPhot]
   Double_t        trkMomZ[nMaxPhotons];   //[nPhot]
   Double_t        trkPosX[nMaxPhotons];   //[nPhot]
   Double_t        trkPosY[nMaxPhotons];   //[nPhot]
   Double_t        trkPosZ[nMaxPhotons];   //[nPhot]
   Double_t        trkT[nMaxPhotons];   //[nPhot]
   Double_t        trkLength[nMaxPhotons];   //[nPhot]

   // List of branches
   TBranch        *b_Event;   //!
   TBranch        *b_BunchXID;   //!
   TBranch        *b_NTotPhot;   //!
   TBranch        *b_Nhits;   //!
   TBranch        *b_primType;   //!
   TBranch        *b_primMomX;   //!
   TBranch        *b_primMomY;   //!
   TBranch        *b_primMomZ;   //!
   TBranch        *b_primPosX;   //!
   TBranch        *b_primPosY;   //!
   TBranch        *b_primPosZ;   //!
   TBranch        *b_primTime;   //!
   TBranch        *b_trigTopL;   //!
   TBranch        *b_trigTopEdep;   //!
   TBranch        *b_trigBotL;   //!
   TBranch        *b_trigBotEdep;   //!
   TBranch        *b_nPhot;   //!
   TBranch        *b_TrackID;   //!
   TBranch        *b_ParentID;   //!
   TBranch        *b_Energy;   //!
   TBranch        *b_Wavelength;   //!
   TBranch        *b_Time;   //!
   TBranch        *b_photPathLen;   //!
   TBranch        *b_SecID;   //!
   TBranch        *b_chID;   //!
   TBranch        *b_PosX;   //!
   TBranch        *b_PosY;   //!
   TBranch        *b_PosZ;   //!
   TBranch        *b_MomX;   //!
   TBranch        *b_MomY;   //!
   TBranch        *b_MomZ;   //!
   TBranch        *b_trkMomX;   //!
   TBranch        *b_trkMomY;   //!
   TBranch        *b_trkMomZ;   //!
   TBranch        *b_trkPosX;   //!
   TBranch        *b_trkPosY;   //!
   TBranch        *b_trkPosZ;   //!
   TBranch        *b_trkT;   //!
   TBranch        *b_trkLength;   //!

   ana(TString name);
   virtual ~ana();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef ana_cxx
ana::ana(TString name) : fChain(0) 
{
  TTree *tree=0;
  // if parameter tree is not specified (or zero), connect the file
  // used to generate this class and read the Tree.
  if (tree == 0) {
    TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(name.Data());
    if (!f || !f->IsOpen()) {
      f = new TFile(name.Data());
    }
    f->GetObject("T",tree);    
  }
  Init(tree);
}

ana::~ana()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t ana::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t ana::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void ana::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EventID", &EventID, &b_Event);
   fChain->SetBranchAddress("BunchXID", &BunchXID, &b_BunchXID);
   fChain->SetBranchAddress("NTotPhot", &NTotPhot, &b_NTotPhot);
   fChain->SetBranchAddress("Nhits", &Nhits, &b_Nhits);
   fChain->SetBranchAddress("primType", &primType, &b_primType);
   fChain->SetBranchAddress("primMomX", &primMomX, &b_primMomX);
   fChain->SetBranchAddress("primMomY", &primMomY, &b_primMomY);
   fChain->SetBranchAddress("primMomZ", &primMomZ, &b_primMomZ);
   fChain->SetBranchAddress("primPosX", &primPosX, &b_primPosX);
   fChain->SetBranchAddress("primPosY", &primPosY, &b_primPosY);
   fChain->SetBranchAddress("primPosZ", &primPosZ, &b_primPosZ);
   fChain->SetBranchAddress("primTime", &primTime, &b_primTime);
   fChain->SetBranchAddress("trigTopL", &trigTopL, &b_trigTopL);
   fChain->SetBranchAddress("trigTopEdep", &trigTopEdep, &b_trigTopEdep);
   fChain->SetBranchAddress("trigBotL", &trigBotL, &b_trigBotL);
   fChain->SetBranchAddress("trigBotEdep", &trigBotEdep, &b_trigBotEdep);
   fChain->SetBranchAddress("nPhot", &nPhot, &b_nPhot);
   fChain->SetBranchAddress("TrackID", TrackID, &b_TrackID);
   fChain->SetBranchAddress("ParentID", ParentID, &b_ParentID);
   fChain->SetBranchAddress("Energy", Energy, &b_Energy);
   fChain->SetBranchAddress("Wavelength", Wavelength, &b_Wavelength);
   fChain->SetBranchAddress("Time", Time, &b_Time);
   fChain->SetBranchAddress("photPathLen", photPathLen, &b_photPathLen);
   fChain->SetBranchAddress("SecID", SecID, &b_SecID);
   fChain->SetBranchAddress("chID", chID, &b_chID);
   fChain->SetBranchAddress("PosX", PosX, &b_PosX);
   fChain->SetBranchAddress("PosY", PosY, &b_PosY);
   fChain->SetBranchAddress("PosZ", PosZ, &b_PosZ);
   fChain->SetBranchAddress("MomX", MomX, &b_MomX);
   fChain->SetBranchAddress("MomY", MomY, &b_MomY);
   fChain->SetBranchAddress("MomZ", MomZ, &b_MomZ);
   fChain->SetBranchAddress("trkMomX", trkMomX, &b_trkMomX);
   fChain->SetBranchAddress("trkMomY", trkMomY, &b_trkMomY);
   fChain->SetBranchAddress("trkMomZ", trkMomZ, &b_trkMomZ);
   fChain->SetBranchAddress("trkPosX", trkPosX, &b_trkPosX);
   fChain->SetBranchAddress("trkPosY", trkPosY, &b_trkPosY);
   fChain->SetBranchAddress("trkPosZ", trkPosZ, &b_trkPosZ);
   fChain->SetBranchAddress("trkT", trkT, &b_trkT);
   fChain->SetBranchAddress("trkLength", trkLength, &b_trkLength);
   Notify();
}

Bool_t ana::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void ana::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t ana::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef ana_cxx
