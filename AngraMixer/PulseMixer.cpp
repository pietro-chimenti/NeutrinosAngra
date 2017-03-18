#include <iostream>
#include <iomanip>
#include <stdio>
#include <string>
#include <climits>
#include <vector>
#include <fstream>
#include <map>
#include "TRandom3.h"

using namespace std;
using namespace TMath; 

#include "ReadMacro.cpp"
#include "WriteTimes.cpp"

void PulseMixer(TString macroName="macros/000001.450s.mix") {

  gROOT->Reset();
  gSystem->Load("libPhysics");
  gROOT->ProcessLine(".L /home/drc01/kemp/angra/v1/AngraMCPulse/AngraMCPulse.cxx+");
  
  const Int_t N = 20;
  const Int_t MAX_HITS = 2000;
  const Int_t MAX_PULSES = 200;
  const Int_t MAX_PPART = 20;
  const Int_t MAX_PMTN  = 48;  
  
  TStopwatch clock;
  clock.Start();

  Int_t seed;
  Int_t compression = 1;
  Int_t i, ii, iM, iiM;

  Double_t parD[3*N];
  Int_t    parI[3*N];
  TString  parS[3*N];


  ReadMacro(macroName,parI,parD,parS,N);
  Int_t  runID   = parI[0];
  const Int_t  nFiles  = parI[1];
  Bool_t fFilter = parI[2];
  float fracJump = parD[2];  
  Int_t usePOISSON = parI[3];
  Int_t useTimeFile = 1;
  
  // if times will be generated on-the-fly
  if (parS[3]=="") useTimeFile = 0;
  Double_t tau[nFiles+1];
  Double_t next[nFiles+1];
  
  seed = runID;
  TRandom3 *rnd = new TRandom3(seed);
  

  cout << "Step 1 :: Time distribution" << endl;
  cout << "\t" << nFiles << " source files to read from.";
  cout << endl;


  Double_t runTime  = parD[0]*1.e9;
  Double_t darkRate = parD[1];

  cout << "\t" << runTime*1.e-9 << " seconds of physics."<< endl;
  cout << "\t" << seed << " is the global random seed."<< endl;
  
  ifstream timeFile;
  
  if (useTimeFile){
      // Working with TimeFile (generating or reading). See macro WriteTimes.cpp for reference
      cout << "\t Using TimeFile system:" << endl;
      TString timeFileName = parS[2]+parS[3];
      ifstream timeFile(timeFileName);
      if (!timeFile.is_open()) { 
	  timeFile.close();
	  cout << "\t\t Time file " << parS[3] << " not found at " << parS[2] << ". Let us make a new one! " << endl;
	  WriteTimes(nFiles,parI,parD,parS,N);
      } else { 
	  cout << "\t\t Time file " << parS[3] << " found at " << parS[2]  << endl;
	  timeFile.close(); }
  } else {
      // Generating times on-the-fly (is it faster? less I/O)
      // Preparing next[] array, which stores when each source will be used next
      cout << "\t" << "Will generate time distribution on-the-fly:" << endl;

      tau[0]  = 1./darkRate * 1.E+09;
      cout << "\t\t DC " << tau[0] << "ns" << endl;
      next[0] = rnd->Exp(tau[0]);
      for (Int_t i = 1; i<=nFiles; i++) {
	  tau[i]  = 1./(parD[1*N+i]*parD[2*N+i]) * 1.E+09;
	  next[i] = rnd->Exp(tau[i]);
      }
  }
  
  TString    inFileName[N];
  TString    outFileName = parS[2] + parS[0];
  TFile      *inFile[N];
  TTree      *inTree[N];
  Long_t     inEntries[N];
  Long_t     inEvent[N];
  Long_t     inJumps[N];
  Double_t   frac[N];

  TClonesArray *pulsesClones[N];
  Int_t    pParticles[N];
  Int_t    pdgPPart  [N][MAX_PPART];
  Double_t pPart     [N][MAX_PPART];

  cout << endl;
  cout << "Step 2 :: Reading source files." << endl;
  cout << endl;

  for (i=1; i<=nFiles; i++) {
    inFileName[i] = parS[1] + parS[2*N+i];
    inFile[i] = new TFile(inFileName[i],"READ");

    if (!inFile[i]->IsZombie()){    
      inTree[i] = (TTree*)inFile[i]->Get("PulseTree");
      pulsesClones[i] = new TClonesArray("AngraMCPulse",MAX_PULSES);
      inTree[i]->SetBranchAddress("Pulses",&pulsesClones[i]);
      inTree[i]->SetBranchAddress("nPP",   &pParticles[i]);
      inTree[i]->SetBranchAddress("pdgPP", &pdgPPart[i]);
      inTree[i]->SetBranchAddress("pPP",   &pPart[i]);
      inEntries[i] = inTree[i]->GetEntries();
      inJumps[i]   = inEntries[i]*fracJump;
      if (inJumps[i]<5) inJumps[i]=5;
      inEvent[i]   = 0.;
      cout << i << "  " << inJumps[i] << " " << inFileName[i] << endl;
    } else {cout << "Error :: Source file " << parS[2*N+i] << " broken or not found at" << parS[1] << " ... aborting." << endl; return;}
  }

  TFile *outFile = new TFile(outFileName.Data(),"RECREATE","File of mixed Pulses,compression");
  TTree *outTree = new TTree("PulseTree","Tree of mixed Pulses");
  TClonesArray *outPulsesClones = new TClonesArray("AngraMCPulse",MAX_PULSES);
  Int_t outpParticles;
  Int_t outpdgPPart[MAX_PPART];
  Double_t outpPart[MAX_PPART];
    
  cout << endl;
  cout << "Step 3 :: Mixing! (this will take a while)" << endl;
  cout << endl;
  
  if (useTimeFile){
    ifstream timeFile(timeFileName);
    if (!timeFile.is_open()) 
    { cout << " Error :: Time file " << parS[3] << " not found at " << parS[2] << " ... aborting." << endl; return;}
  }
  
  Int_t iSource, eventID;
  Double_t globalTime = 0.;
  Double_t completion = 0.;
  eventID = 0;

  outTree->Branch("Pulses",&outPulsesClones,256000,2);
  outTree->Branch("nPP",   &outpParticles,"nPP/I");
  outTree->Branch("pdgPP", &outpdgPPart,"pdgPP[nPP]/I");
  outTree->Branch("pPP",   &outpPart,"pPP[nPP]/D");

  AngraMCPulse *aPulse, *inPulse;
  
  UInt_t oneJump = 0;
  Int_t  iPulse = 0;
  Int_t  iPMT = 0;
  Int_t  iPP=0;
  Int_t  iDC=0;
  
  Int_t keepLooping = 1;
  Int_t verbose = 1;
  
  do{
    if (useTimeFile) { timeFile >> iSource >> globalTime;}
    else{ 
	  iSource=Smaller(nFiles,next);
	  globalTime = next[iSource];
	  next[iSource] += rnd->Exp(tau[iSource]);
    }
    completion = globalTime/runTime*100;
    if (((int)completion)%10==0 && verbose) {cout << (int) completion << "% " <<  globalTime << "ns" << endl; verbose=0;}
    if ( (int)completion %10!=0)  {verbose=1;}
    
    if (iSource) {
      
      if (usePOISSON) {oneJump = rnd -> Poisson(inJumps[iSource]);}
      else {oneJump = rnd -> Integer(2*inJumps[iSource]);}
      
      inEvent[iSource] += oneJump;
      if (inEvent[iSource]>inEntries[iSource]) inEvent[iSource] = oneJump;     
      
      inTree[iSource]->GetEntry(inEvent[iSource]);
	
	outPulsesClones->Clear();
 	for (iPulse=0; iPulse<pulsesClones[iSource]->GetEntries(); iPulse++){
	  aPulse = new((*outPulsesClones)[iPulse]) AngraMCPulse();
	  inPulse = (AngraMCPulse*) pulsesClones[iSource]->At(iPulse);
	  for (iPMT=0; iPMT<MAX_PMTN; iPMT++){
	      aPulse->hits[iPMT] = inPulse->hits[iPMT];
	      aPulse->times[iPMT]= inPulse->times[iPMT];
	  }
	  aPulse->globalTime1 = globalTime*1.e-9;
          aPulse->startPmt  = inPulse->startPmt;
	  aPulse->startTime = inPulse->startTime;
	  aPulse->run1      = inPulse->run1;
	  aPulse->event1    = inPulse->event1;
	}

	outpParticles = pParticles[iSource];
	
	for (iPP=0; iPP<outpParticles; iPP++){
	  outpdgPPart[iPP]   = pdgPPart[iSource][iPP];
	  outpPart[iPP]      = pPart[iSource][iPP];
	}
	
    } else {
      // dark_Current
      outPulsesClones->Clear();      
      aPulse = new((*outPulsesClones)[0]) AngraMCPulse();

	  for (iPMT=0; iPMT<MAX_PMTN; iPMT++){
	      aPulse->hits[iPMT] = 0;
	      aPulse->times[iPMT]= 0.;
	  }
      iDC = rnd->Integer(MAX_PMTN);
      aPulse->hits[iDC]  = 1;
      aPulse->times[iDC] = 0.;
      aPulse->globalTime1 = globalTime*1.e-9;
      aPulse->startPmt  = iDC;
      aPulse->startTime = 0.;
      aPulse->run1      = 0;
      aPulse->event1    = 0;
      
      outpParticles = 0;
      outpdgPPart[0]= 0;
      outpPart[0]   = 0.;
      
    }
    outTree->Fill(); 
    eventID++;
    if ( useTimeFile && timeFile.eof()) keepLooping = 0;
    if (!useTimeFile && globalTime>runTime ) keepLooping = 0;
  }while(keepLooping);
  
  outTree->Write();
  outFile->Close();
  
  for (int i=1; i<=nFiles; i++) {
    inFile[i]->Close();
  }
  clock.Stop();
  
  cout << "    Total time = " << clock.RealTime() << "s,   CPU time = " << clock.CpuTime() << "s." << endl;
  
}
