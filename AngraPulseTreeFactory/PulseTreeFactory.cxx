#include "math.h"
#include "TString.h"

#define MAX_HITS 20000
#define MAX_PULSES 200
#define MAX_PPART 20;

class PulseTreeFactory{

public:
  PulseTreeFactory();
  void Welcome();
  void SetInTree(TTree *aTree);
  void SetPulseTree(TTree *pTree);
  void StoreADummyPulse();
  void StoreAnEvent(int iEvent);

  void Loop(int start, int end);
  void Loop();
  void Finalize();


  Double_t maxTInterval;
  Int_t runNum;

private:
  TTree *inTree;
  TTree *pulseTree;
  TH1F *timeHistos[48];
  AngraMCEvent *event;
  TClonesArray *pulsesClones;
  //AngraMCPulse pulses[MAX_PULSES];
  Int_t pParticles;
  Int_t pdgPPart[MAX_PPART];
  Double_t pPart[MAX_PPART];
  Double_t lastTime;
  Double_t timeVector[MAX_HITS];
  Int_t   timeSort[MAX_HITS];
  

  //  void ResetPulses(){ for(int i=0; i<MAX_PULSES; i++) pulses[i].Reset();};
  void ResetPulses(){ if(pulsesClones) pulsesClones->Clear();};
  void ResetPPart(){ pParticles=0; for(int i=0; i<MAX_PPART; i++){pdgPPart[i]=0;pPart[i]=0.;}};
  void LoadPrimaryInfos();

};


PulseTreeFactory::PulseTreeFactory(){
  inTree=0;
  pulseTree=0;
  event=0;
  pulsesClones=0;
  for(int i=0; i<48; i++){
    TString nameTitle = "timeHisto"+TString::Itoa(i,10);
    timeHistos[i]=new TH1F(nameTitle.Data(),nameTitle.Data(),10000,0.,1000.);
  }
  lastTime= -HUGE_VAL;
  runNum=0;
  maxTInterval=100;//nanosecondss
  ResetPPart();
  Welcome();

}

PulseTreeFactory::Finalize(){
  for(int i=0; i<48; i++){
    timeHistos[i]->Write();
  }
  cout << "PTF: bye bye!"<< endl;
}

PulseTreeFactory::SetInTree(TTree *aTree){
  inTree=aTree;
  inTree->SetBranchAddress("Event", &event);
  Long64_t nentries = inTree->GetEntries();
  cout << "PTF found " << nentries << " entries." << endl;
}

PulseTreeFactory::SetPulseTree(TTree *pTree){
  pulseTree=pTree;
  pulsesClones=new TClonesArray("AngraMCPulse",MAX_PULSES);
  pulseTree->Branch("Pulses",&pulsesClones,256000,2);
  pulseTree->Branch("nPP",&pParticles,"nPP/I");
  pulseTree->Branch("pdgPP",&pdgPPart,"pdgPP[nPP]/I");
  pulseTree->Branch("pPP",&pPart,"pPP[nPP]/D");
  cout << "PTF set branches " << endl;  
}

PulseTreeFactory::StoreADummyPulse(){
  if(pulsesClones){
    pulsesClones->Clear();
    AngraMCPulse *aPulse = new((*pulsesClones)[0]) AngraMCPulse();
    aPulse->hits[1]=1;
    aPulse->times[1]=0;
    pulseTree->Fill();
    cout << "PTF dummy pulse" << endl;
  }
  else cout << "PTF dummy failure" << endl;
}

PulseTreeFactory::StoreAnEvent(int iEvent){
  if(pulsesClones&&inTree){
    if(inTree->GetEntry(iEvent)){
      Int_t nHits = event->GetNhit();
      LoadPrimaryInfos();
      Int_t iPulse=0;
      if( nHits>0 ){
	Int_t Hlimit = ( nHits < MAX_HITS ? nHits : MAX_HITS ); // total hits?
	// sort hits!
	for(Long64_t iH=0; iH<Hlimit; iH++){
	  timeVector[iH]=event->GetHit(iH)->GetLocalTime();
	}
	TMath::Sort(Hlimit,timeVector,timeSort,kFALSE );
	
	// get hit infos
	Double_t hTime = event->GetHit(timeSort[0])->GetLocalTime();
	Int_t hLoc = event->GetHit(timeSort[0])->GetPMTLocation();
	Int_t hLev = event->GetHit(timeSort[0])->GetPMTLevel();
	Int_t pmtN = event->GetHit(timeSort[0])->GetPMTNumber();
	if(pmtN==0)return; // why == 0? a bug?
	Int_t pmtTotalNum=pmtNum(hLoc,hLev,pmtN);
	lastTime=hTime;
	
	// init first pulse
	AngraMCPulse *aPulse = new((*pulsesClones)[iPulse]) AngraMCPulse();
	aPulse->hits[pmtTotalNum]=1;
	aPulse->times[pmtTotalNum]=hTime;
	aPulse->startPmt=pmtTotalNum;
	aPulse->startTime=hTime;
	aPulse->run1=runNum;
	aPulse->event1=iEvent;
	iPulse++;
	
	// loop over hits
	for(Long64_t iH=1; iH<Hlimit; iH++){
	  
	  // get infos of hit iH
	  hTime = event->GetHit(timeSort[iH])->GetLocalTime();
	  hLoc = event->GetHit(timeSort[iH])->GetPMTLocation();
	  hLev = event->GetHit(timeSort[iH])->GetPMTLevel();
	  pmtN = event->GetHit(timeSort[iH])->GetPMTNumber();
	  if(pmtN==0)return; // why 0??
	  pmtTotalNum=pmtNum(hLoc,hLev,pmtN);
	  
	  // test if new signal
	  if((hTime-lastTime)>maxTInterval){
	    aPulse = new((*pulsesClones)[iPulse]) AngraMCPulse();
	    aPulse->hits[pmtTotalNum]=1;
	    aPulse->times[pmtTotalNum]=hTime;
	    aPulse->startPmt=pmtTotalNum;
	    aPulse->startTime=hTime;
	    aPulse->run1=runNum;
	    aPulse->event1=iEvent;
	    iPulse++;
	  }
	  else{
	    aPulse->hits[pmtTotalNum]+=1;
	    if(aPulse->hits[pmtTotalNum]==1)aPulse->times[pmtTotalNum]=hTime;
	    else timeHistos[pmtTotalNum]->Fill(hTime-aPulse->times[pmtTotalNum]);	    
	  }
	  lastTime=hTime;
	}
	pulseTree->Fill();
	pulsesClones->Clear();
      }
      //    cout << "PTF pulses done for event " << iEvent << " pulses " << iPulse <<  endl;
    }
    //  else cout << "PTF store event failure" << endl;
    
  }
}

PulseTreeFactory::LoadPrimaryInfos(){
  pParticles=event->GetNparticle();
  for(int ip=0; ip<pParticles;ip++){
    pdgPPart[ip]=event->GetPrimary(ip)->GetPDG();
    Double_t px=event->GetPrimary(ip)->GetPx();
    Double_t py=event->GetPrimary(ip)->GetPy();
    Double_t pz=event->GetPrimary(ip)->GetPz();
    pPart[ip]=sqrt(px*px+py*py+pz*pz);
  }
}

PulseTreeFactory::Loop(int start, int end){
  for(int i = start; i<end; i++){
    if(i % 100 ==0) cout << "Event " <<  i << endl;
    StoreAnEvent(i);
  }
}

PulseTreeFactory::Loop(){
  Loop( 0, inTree->GetEntries());
}

PulseTreeFactory::Welcome(){
  cout << "Welcome to Pulse Tree Factory!" << endl;
}
