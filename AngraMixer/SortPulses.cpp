#include <iostream>
#include <iomanip>
#include <stdio>
#include <string>
#include <climits>
#include <vector>
#include <fstream>
#include <map>

using namespace std;
using namespace TMath; 

void FindPulse(long &ev,int &pulse, long long index, long long *accumulated, long long maxPulses){
  
  if (ev>=maxPulses) ev = maxPulses-1;
  if (ev<0) ev = 0;
  
  if (accumulated[ev]==index){
    pulse=0;
    return;
  }
  
  if (accumulated[ev]<index){
    do{ev++; if (ev>=maxPulses) {ev=maxPulses-1; pulse=0; return;} }while(accumulated[ev]<index);
    if(accumulated[ev]!=index) ev--;
    pulse = index-accumulated[ev];
  } else{
    do{ev--; if (ev<0) {ev=0; pulse=0; return;}} while(accumulated[ev]>index);
    pulse = index-accumulated[ev];
  }
}


void SortPulses(
  TString  inFileName="/home/valdiviesso/Sandbox/Angra/v1/Data/MixedPulses/MIXED_1s_MCPULSE.root",
  TString outFileName="/home/valdiviesso/Sandbox/Angra/v1/Data/MixedPulses/SORTED.root") {

  const Int_t N = 20;
  const Int_t MAX_HITS = 2000;
  const Int_t MAX_PULSES = 200;
  const Int_t MAX_PPART = 20;
  const Int_t MAX_PMTN  = 48;  

//   gROOT->Reset();
  gSystem->Load("libPhysics");
  gROOT->ProcessLine(".L /home/drc01/kemp/angra/v1/AngraMCPulse/AngraMCPulse.cxx+");
    
  TStopwatch clock;
  clock.Start();

  TFile *inFile, *outFile;
  TTree *inTree, *outTree;

  AngraMCPulse *inPulse, *outPulse;

  TClonesArray *pulsesClones;
  Int_t    inPParticles;
  Int_t    inPDGPPart  [MAX_PPART];
  Double_t inPPart     [MAX_PPART];

  Int_t    outPParticles;
  Int_t    outPDGPPart  [MAX_PPART];
  Double_t outPPart     [MAX_PPART];

//   TString  inFileName("/home/valdiviesso/Sandbox/Angra/v1/Data/MixedPulses/MIXED_1s_MCPULSE.root");
//   TString outFileName("/home/valdiviesso/Sandbox/Angra/v1/Data/MixedPulses/SORTED.root");
  
  inFile = new TFile(inFileName,"READ");
  inTree = (TTree*)inFile->Get("PulseTree");
  pulsesClones = new TClonesArray("AngraMCPulse",MAX_PULSES);
  inTree->SetBranchAddress("Pulses",&pulsesClones);
  inTree->SetBranchAddress("nPP",   &inPParticles);
  inTree->SetBranchAddress("pdgPP", &inPDGPPart);
  inTree->SetBranchAddress("pPP",   &inPPart);

  outPulse = new AngraMCPulse();
  outFile = new TFile(outFileName,"RECREATE");
  outTree = new TTree("PulseTree","Tree of Mixed Pulses");
  outTree->Branch("Pulse",&outPulse,256000,1);
  outTree->Branch("nPP",&outPParticles,"nPP/I");
  outTree->Branch("pdgPP",&outPDGPPart,"pdgPP[nPP]/I");
  outTree->Branch("pPP",&outPPart,"pPP[nPP]/D");
  
  long inEntries = inTree->GetEntries();
  const long maxEntries = inEntries;
  int inPulses = 0;
  long i,j,k,l,verbose,completion;
  
  const long long MaxPulses = inEntries*MAX_PULSES;
  double timeVector[MaxPulses];
  long long timeSort[MaxPulses];
  long long howMany[maxEntries+1];
  long long iP=0;
  
  verbose = 0;
  howMany[0]=0;
  inTree->GetEntry(0);
  inPulses = pulsesClones->GetEntries();
  howMany[1]=inPulses;

  for (i=0; i<inEntries; i++){
//       completion=(int) ((double)i/(double)inEntries*100);
//       if (verbose!=completion) {cout << completion << "%" << endl; verbose=completion;}
    
      inTree->GetEntry(i);
      inPulses = pulsesClones->GetEntries();
      
      if(i) {howMany[i+1] = howMany[i] + inPulses;}
      for (j=0; j<inPulses; j++){
	inPulse = (AngraMCPulse*)pulsesClones->At(j);
	timeVector[iP] = inPulse->globalTime1*1.e9+inPulse->startTime;	
	iP++;
      }
  }

//   cout << "Partial time = " << clock.RealTime() << "s" << endl;
  cout << iP << "\t pulses read from file. Sorting...\t";
  
  TMath::Sort(iP,timeVector,timeSort,kFALSE);
 
//   cout << "done! (now "<< clock.RealTime() << "s)" << endl << endl;
  cout << "Building new Tree...\t"<< endl;
  
  long long maxP = iP;
  long eV=0;
  int  kP=0;
  int  lPP=0;
  int  iPMT;
  for(i=0;i<maxP;i++){

    FindPulse(eV,kP,timeSort[i],howMany,maxP);
    
    inTree->GetEntry(eV);    
    inPulse = (AngraMCPulse*) pulsesClones->At(kP);
        
    outPParticles = inPParticles;
    for(lPP=0;lPP<outPParticles;lPP++){
      outPDGPPart[lPP]=inPDGPPart[lPP];
      outPPart[lPP]=inPDGPPart[lPP];
    }

    outPulse->globalTime1 = inPulse->globalTime1;
    outPulse->startTime   = inPulse->startTime;
    outPulse->startPmt    = inPulse->startPmt;
    outPulse->run1	  = inPulse->run1;
    outPulse->event1      = inPulse->event1;

    for (iPMT=0; iPMT<MAX_PMTN; iPMT++){
      outPulse->hits[iPMT] = inPulse->hits[iPMT];
      outPulse->times[iPMT]= inPulse->times[iPMT];
    }
    
    outTree->Fill();
  }
  
  outTree->Write();
  outFile->Close();
  clock.Stop();
  
//   cout << "done!" << endl<< endl;
//   cout << "    Total time = " << clock.RealTime() << "s,   CPU time = " << clock.CpuTime() << "s." << endl;
  
}
