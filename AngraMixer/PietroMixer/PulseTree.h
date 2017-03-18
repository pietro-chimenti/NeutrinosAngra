//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Oct 17 16:10:02 2014 by ROOT version 5.34/18
// from TTree PulseTree/Tree of unmixed Pulses
// found on file: 000001.photons_v1_1M_20131000_PULSES.root
//////////////////////////////////////////////////////////

#ifndef PulseTree_h
#define PulseTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include <TClonesArray.h>
#include <TObject.h>
#include "AngraMCPulse.h"

// Fixed size dimensions of array or collections stored in the TTree if any.
const Int_t kMaxPulses = 100;

class PulseTree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           Pulses_;
   UInt_t          Pulses_fUniqueID[kMaxPulses];   //[Pulses_]
   UInt_t          Pulses_fBits[kMaxPulses];   //[Pulses_]
   Int_t           Pulses_hits[kMaxPulses][48];   //[Pulses_]
   Double_t        Pulses_times[kMaxPulses][48];   //[Pulses_]
   Int_t           Pulses_startPmt[kMaxPulses];   //[Pulses_]
   Double_t        Pulses_startTime[kMaxPulses];   //[Pulses_]
   Double_t        Pulses_globalTime1[kMaxPulses];   //[Pulses_]
   Double_t        Pulses_globalTime2[kMaxPulses];   //[Pulses_]
   Int_t           Pulses_run1[kMaxPulses];   //[Pulses_]
   Int_t           Pulses_event1[kMaxPulses];   //[Pulses_]
   Int_t           Pulses_run2[kMaxPulses];   //[Pulses_]
   Int_t           Pulses_event2[kMaxPulses];   //[Pulses_]
   Int_t           nPP;
   Int_t           pdgPP[3];   //[nPP]
   Double_t        pPP[3];   //[nPP]

   // List of branches
   TBranch        *b_Pulses_;   //!
   TBranch        *b_Pulses_fUniqueID;   //!
   TBranch        *b_Pulses_fBits;   //!
   TBranch        *b_Pulses_hits;   //!
   TBranch        *b_Pulses_times;   //!
   TBranch        *b_Pulses_startPmt;   //!
   TBranch        *b_Pulses_startTime;   //!
   TBranch        *b_Pulses_globalTime1;   //!
   TBranch        *b_Pulses_globalTime2;   //!
   TBranch        *b_Pulses_run1;   //!
   TBranch        *b_Pulses_event1;   //!
   TBranch        *b_Pulses_run2;   //!
   TBranch        *b_Pulses_event2;   //!
   TBranch        *b_nPP;   //!
   TBranch        *b_pdgPP;   //!
   TBranch        *b_pPP;   //!

   PulseTree(TTree *tree=0);
   virtual ~PulseTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef PulseTree_cxx
PulseTree::PulseTree(TTree *tree) : fChain(0) 
{
   Init(tree);
}

PulseTree::~PulseTree()
{
   if (!fChain) return;
}

Int_t PulseTree::GetEntry(Long64_t entry)
{
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t PulseTree::LoadTree(Long64_t entry)
{
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void PulseTree::Init(TTree *tree)
{
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Pulses", &Pulses_, &b_Pulses_);
   fChain->SetBranchAddress("Pulses.fUniqueID", Pulses_fUniqueID, &b_Pulses_fUniqueID);
   fChain->SetBranchAddress("Pulses.fBits", Pulses_fBits, &b_Pulses_fBits);
   fChain->SetBranchAddress("Pulses.hits[48]", Pulses_hits, &b_Pulses_hits);
   fChain->SetBranchAddress("Pulses.times[48]", Pulses_times, &b_Pulses_times);
   fChain->SetBranchAddress("Pulses.startPmt", Pulses_startPmt, &b_Pulses_startPmt);
   fChain->SetBranchAddress("Pulses.startTime", Pulses_startTime, &b_Pulses_startTime);
   fChain->SetBranchAddress("Pulses.globalTime1", Pulses_globalTime1, &b_Pulses_globalTime1);
   fChain->SetBranchAddress("Pulses.globalTime2", Pulses_globalTime2, &b_Pulses_globalTime2);
   fChain->SetBranchAddress("Pulses.run1", Pulses_run1, &b_Pulses_run1);
   fChain->SetBranchAddress("Pulses.event1", Pulses_event1, &b_Pulses_event1);
   fChain->SetBranchAddress("Pulses.run2", Pulses_run2, &b_Pulses_run2);
   fChain->SetBranchAddress("Pulses.event2", Pulses_event2, &b_Pulses_event2);
   fChain->SetBranchAddress("nPP", &nPP, &b_nPP);
   fChain->SetBranchAddress("pdgPP", pdgPP, &b_pdgPP);
   fChain->SetBranchAddress("pPP", pPP, &b_pPP);
   Notify();
}

Bool_t PulseTree::Notify()
{
   return kTRUE;
}

void PulseTree::Show(Long64_t entry)
{
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t PulseTree::Cut(Long64_t entry)
{
   return 1;
}
#endif // #ifdef PulseTree_cxx
