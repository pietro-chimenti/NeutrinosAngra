#include <stdlib.h>
#include <iostream>
#include <list>

#include "TFile.h"
#include "TTree.h"

#include "AngraMCPulse.h"

int main(int argc, char **argv)
{
  std::cout << "Hello Mixer!" << std::endl;

  AngraMCPulse aPulse;
  AngraMCPulse *aPulsePointer=&aPulse;
  std::list<AngraMCPulse> pulseList;
  TFile f("TestListTree.root","recreate");
  TTree tlTree("tlt","TestList Tree");
  tlTree.Branch("PulseSplit","AngraMCPulse",&aPulsePointer,32000,99);



  // fill list...
  // first pulse
  aPulse.startTime=0;
  aPulse.globalTime1=0;
  std::cout << "aPulse startTime:" << aPulse.startTime << " globalTime1 " << aPulse.globalTime1 << std::endl; 
  pulseList.push_back(aPulse);

  // second pulse (will be third)
  aPulse.startTime=3;
  aPulse.globalTime1=3;
  std::cout << "aPulse startTime:" << aPulse.startTime << " globalTime1 " << aPulse.globalTime1 << std::endl; 
  pulseList.push_back(aPulse);

  // third pulse (will be second)
  aPulse.startTime=2;
  aPulse.globalTime1=2;
  std::cout << "aPulse startTime:" << aPulse.startTime << " globalTime1 " << aPulse.globalTime1 << std::endl; 
  pulseList.push_back(aPulse);

  // now check pulses
  std::list<AngraMCPulse>::iterator pulseIt;
  for(pulseIt = pulseList.begin(); pulseIt!=pulseList.end(); ++pulseIt)
    std::cout << "aPulse startTime:" << pulseIt->startTime << " globalTime1 " << pulseIt->globalTime1 <<  std::endl; 


  // now sort and fill tree!
  pulseList.sort();
  while(!pulseList.empty()){
    std::cout << "aPulse startTime:" << pulseList.front().startTime << " globalTime1 " << pulseList.front().globalTime1 <<  std::endl; 
    aPulse=pulseList.front();
    tlTree.Fill();
    pulseList.pop_front();
  }

  f.Write();
  return 0;
}
