// ===========================================
//
// Checks how many events have hits NH
// Checks total number of events per file NE
// SRC_RATIO = NH/NE
// version 1.0
// 20/10/2014
//
// Author: G. A. Valdiviesso
//
// ===========================================



void CalcRatio( TString inFile="")
{
//   if (inFile=="") return;
  
  TString path = "/home/drc01/kemp/angra/data/G4_Root/noTracks/";
//  inFile="000011.muons_v1_250k_20131001.root";
  inFile="000012.muons_v1_250k_20131002.root";
//  inFile="000013.muons_v1_250k_20131003.root";
//  inFile="000014.muons_v1_250k_20131004.root";
//   inFile="electrons_1M_20131000_NT_MCEVENT.root";
//   inFile="positrons_620k_20131000_NT_MCEVENT.root";
//   inFile="muons_140k_20131000_NT_MCEVENT.root";
//   inFile="protons_20k_20131000_NT_MCEVENT.root";
//   inFile="pions_10k_20131000_NT_MCEVENT.root";
//   inFile="neutrons_50k_20131000_NT_MCEVENT.root";
//   inFile="neutrinos_70k_20131000_NT_MCEVENT.root";
  
  inFile = path + inFile;
  
  gROOT->Reset();
  gSystem->Load("libPhysics");
  gROOT->ProcessLine(".L ../AngraMCEvent/AngraMCEvent.cxx+");
  gROOT->ProcessLine(".L ../AngraMCPulse/AngraMCPulse.cxx+");

  //open file
  TFile *fileIn = new TFile(inFile,"READ");
  TTree *treeIn = (TTree*)fileIn-> Get("AngraMC");
  AngraMCEvent *event = 0;
  treeIn -> SetBranchAddress("Event", &event);
  Long64_t nEntries = treeIn->GetEntries();
  
  Long64_t noHits=0;
  
  for(int i=0; i<nEntries; i++){
    treeIn->GetEntry(i);
    if (event->GetNhit()) noHits++;
  }
  
  double ratio = (double) noHits/(double) nEntries;
  
  cout << nEntries << "\t" << noHits << "\t" << ratio << endl;
  
  
}
