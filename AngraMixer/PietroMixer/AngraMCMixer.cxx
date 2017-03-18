#include "AngraMCMixer.h"

AngraMCMixer::AngraMCMixer() :  currentPrimary(0), currentEvent(0), currentTime(0), outFile(), theTree(), pulseDB(), rates(),  pulseBuffer(), bufferSize(0)
{
}

void AngraMCMixer::AddPrimary(PulseTree * aPulseTree, double rate)
{

  std::cout << " AngraMCMixer: Adding a generator" << std::endl;
  rates.push_back(rate);

  std::vector<std::vector< AngraMCPulse  > > aTree;
  pulseDB.push_back(aTree);
  int lastGen = pulseDB.size();

  //loop over  tree and add pulses
  std::vector< AngraMCPulse >  theEvent;
  AngraMCPulse aPulse;
  Long64_t pt1_ne = aPulseTree->fChain->GetEntriesFast();
  for (Long64_t pt1_j=0; pt1_j<pt1_ne;pt1_j++) {
    theEvent.clear();
    Long64_t ientry = aPulseTree->LoadTree(pt1_j);
    if (ientry < 0) break;
    aPulseTree->fChain->GetEntry(pt1_j); 
    for(int ip=0; ip<aPulseTree->Pulses_; ip++) {
      LoadPulse(ip, aPulse, aPulseTree);
      theEvent.push_back(aPulse);
    }
    pulseDB[lastGen-1].push_back(theEvent);
  }
  std::cout << " AngraMCMixer: added " << pulseDB[lastGen-1].size() << " events  to generator " << (lastGen-1) << std::endl;
}

void AngraMCMixer::LoadPulse(int ip, AngraMCPulse &aPulse, PulseTree * aPulseTree){
  for(int i=0; i<TOT_PMTS; i++){
    aPulse.hits[i]=aPulseTree->Pulses_hits[ip][i];
    aPulse.times[i]=aPulseTree->Pulses_times[ip][i];
  }
  aPulse.startPmt=aPulseTree->Pulses_startPmt[ip];
  aPulse.startTime=aPulseTree->Pulses_startTime[ip];
  aPulse.globalTime1=aPulseTree->Pulses_globalTime1[ip];
  aPulse.globalTime2=aPulseTree->Pulses_globalTime2[ip];
  aPulse.run1=aPulseTree->Pulses_run1[ip];
  aPulse.run2=aPulseTree->Pulses_run2[ip];
  aPulse.event1=aPulseTree->Pulses_event1[ip];
  aPulse.event2=aPulseTree->Pulses_event2[ip];
}

int AngraMCMixer::GetNPrimaries(){
  return pulseDB.size();
}

int AngraMCMixer::GetNEvents(int primary){
  return pulseDB[primary].size();
}

int AngraMCMixer::GetNPulses(int primary, int event){
  return pulseDB[primary][event].size();
}

AngraMCPulse AngraMCMixer::GetAPulse(int primary, int event, int pulse){
  return pulseDB[primary][event][pulse];
}

void AngraMCMixer::Init(std::string fileName, int seed){
  std::cout << " AngraMCMixer: Init " << std::endl;
  outFile.Open(fileName.c_str(),"recreate");
  theTree = new TTree("MixerTree", "Mixed Pulses");
  thePulseP=new AngraMCPulse();
  theTree->Branch("Pulse","AngraMCPulse",&thePulseP,32000,99);
  theTimer=new AngraMixerTimer(seed);
  std::vector< double >::iterator ratesIt;
  for(ratesIt=rates.begin();ratesIt!=rates.end();++ratesIt){
    theTimer->PushBack(*ratesIt);
  }
  theTimer->Init( );
  theRGenerator = new TRandom2(seed);
}

void AngraMCMixer::Finalize(){
  std::cout << " AngraMCMixer: Finalize " << std::endl;
  theTree->Write();
  outFile.Close();
  
  thePulseP->Delete();
  theTree->Delete();
  theRGenerator->Delete();
  delete theTimer; 
  
}

void AngraMCMixer::FillSequentially(){
  std::cout << " AngraMCMixer: Filling sequentially " << std::endl;
  std::vector<std::vector< std::vector< AngraMCPulse > > >::iterator it1;
  std::vector< std::vector< AngraMCPulse > >::iterator it2;
  std::vector< AngraMCPulse >::iterator it3;
  int cycle = 0;
  for(it1=pulseDB.begin();it1!=pulseDB.end();++it1){
    std::cout << " primary " << cycle << std::endl;
    for(it2=it1->begin();it2!=it1->end();++it2){
      for(it3=it2->begin();it3!=it2->end();++it3){
	(*thePulseP)=(*it3);
	theTree->Fill();
      } 
    }
    cycle++;
  }


}

void AngraMCMixer::Fill(double time){
  std::cout << " AngraMCMixer: Filling for " << time << " seconds " << std::endl;
  currentPrimary=theTimer->GetCurrentGenerator();
  currentEvent=theRGenerator->Integer(pulseDB[currentPrimary].size());
  currentTime=theTimer->GetCurrentTime();
  while(theTimer->Next()<time){
    StoreEvent();
    if( pulseBuffer.size() >  buffersize) Flush();
    currentPrimary=theTimer->GetCurrentGenerator();
    currentEvent=theRGenerator->Integer(pulseDB[currentPrimary].size());
    currentTime=theTimer->GetCurrentTime();
  }
  FlushAll();
  for(unsigned int i=0; i<rates.size();++i)
    std::cout << " timer: total events generator " << i << " " << theTimer->GetEvents(i) << std::endl;

}

void AngraMCMixer::StoreEvent(){
  AngraMCPulse aPulse;
  std::vector< AngraMCPulse >::iterator itPulse;
  for(itPulse=pulseDB[currentPrimary][currentEvent].begin();itPulse!=pulseDB[currentPrimary][currentEvent].end();++itPulse){
    aPulse=(*itPulse);
    aPulse.globalTime1=currentTime;
    pulseBuffer.push_back(aPulse);
    bufferSize++;
  } 
}

void AngraMCMixer::Flush(){
  pulseBuffer.sort();
  while( bufferSize >  minbuffersize)
    {
      (*thePulseP)=pulseBuffer.front();
      theTree->Fill();
      pulseBuffer.pop_front();
      bufferSize--; 
    }
}

void AngraMCMixer::FlushAll(){
  pulseBuffer.sort();
  while( bufferSize > 0)
    {
      (*thePulseP)=pulseBuffer.front();
      theTree->Fill();
      pulseBuffer.pop_front();
      bufferSize--; 
    }
  
}
