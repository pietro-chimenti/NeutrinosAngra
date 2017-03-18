#ifndef ANGRA_MC_PULSE
#define ANGRA_MC_PULSE

#include "TROOT.h"
#include <iostream>

#define TOT_PMTS 48

enum AngraPMTLocation {kTarget=0, kInnerVeto=1, kBoxVeto=2};
enum AngraPMTLevel    {kUp=0, kDown=1};

Int_t pmtNum(Int_t loc, Int_t lev, Int_t num){
        if (loc==kTarget) {return (num-1)+lev*16;}
        else if (loc==kInnerVeto) {return 32+(num-1)+lev*4;}
        else {return 32+8+(num-1)+lev*4;}
}


class AngraMCPulse : public TObject {

using TObject::Compare;
using TObject::IsSortable;
  
public:
  
 Int_t    hits[TOT_PMTS]; // hits in each pmt
 Double_t times[TOT_PMTS]; //[ns] first hit time of each pmt 

 Int_t    startPmt; // first pmt 
 Double_t startTime;   //[ns] time of first pmt 


 // global time from mixer
 // two variable to allow unlikely coincidences 
 Double_t globalTime1; //[s] global time from first G4 event
 Double_t globalTime2; //[s] global time from second G4 event

 // link to full simulation trees  
 Int_t run1; // run of first g4 event
 Int_t event1; // event of first g4 event
 Int_t run2; // run of second g4 event
 Int_t event2; // event of second g4 event
 
 AngraMCPulse() {Reset(); }
 virtual ~AngraMCPulse() { }
 void Reset();
 

 // sortable!
 bool operator== (AngraMCPulse *) const;
 bool operator<  (AngraMCPulse *) const;
 bool operator>  (AngraMCPulse *) const;
 bool operator<= (AngraMCPulse *) const;
 bool operator>= (AngraMCPulse *) const;
 
 Bool_t IsSortable() const;
 virtual Int_t Compare(const TObject*) const;    

 Int_t hitsInIDU();
 Int_t hitsInIDD();
 Int_t hitsInVU();
 Int_t hitsInVD();
 Int_t hitsInIVU();
 Int_t hitsInIVD();


 
 ClassDef(AngraMCPulse,1)  //MCPulse: A summary of hits;
};


#endif // ANGRA_MC_PULSE
