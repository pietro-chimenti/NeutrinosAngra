#include "TMath.h"   
#include "AngraMCPulse.h"
#include "math.h"

ClassImp(AngraMCPulse)

bool AngraMCPulse::operator== (AngraMCPulse *other) const{
  double deltaTime=(this->globalTime1-other->globalTime1)*1e9+
                   (this->startTime-other->startTime);
  return deltaTime==0;
}

bool AngraMCPulse::operator< (AngraMCPulse *other) const{
  double deltaTime=(this->globalTime1-other->globalTime1)*1e9+
                   (this->startTime-other->startTime);
  return deltaTime<0;
}

bool AngraMCPulse::operator> (AngraMCPulse *other) const{
  double deltaTime=(this->globalTime1-other->globalTime1)*1e9+
                   (this->startTime-other->startTime);
  return deltaTime>0;
}

bool AngraMCPulse::operator<= (AngraMCPulse *other) const{
  double deltaTime=(this->globalTime1-other->globalTime1)*1e9+
                   (this->startTime-other->startTime);
  return deltaTime<=0;
}

bool AngraMCPulse::operator>= (AngraMCPulse *other) const{
  double deltaTime=(this->globalTime1-other->globalTime1)*1e9+
                   (this->startTime-other->startTime);
  return deltaTime>=0;
}

// dangerous cast!!
Int_t AngraMCPulse::Compare(const TObject* other) const
{
  AngraMCPulse *otherPulse = (AngraMCPulse*) other;
  double deltaTime=(this->globalTime1-otherPulse->globalTime1)*1e9+
                   (this->startTime-otherPulse->startTime);
  if ( deltaTime < 0 ) { return -1; }
  if ( deltaTime > 0 ) { return +1; }

  return 0;
}

Bool_t AngraMCPulse::IsSortable() const {return kTRUE;}

void   AngraMCPulse::Reset(){
  for(int i=0; i<TOT_PMTS; i++){
    hits[i]=0;
    times[i]= -HUGE_VAL;
  }
  startPmt=-1;
  startTime= -HUGE_VAL;
  globalTime1= -HUGE_VAL;
  globalTime2= -HUGE_VAL;
  run1=0;
  event1=0;
  run2=0; 
  event2=0;

}

Int_t  AngraMCPulse::hitsInIDU(){
  Int_t localHits=0;
  for(int i=0; i<16; i++) localHits+=hits[i];
  return localHits;
}

Int_t  AngraMCPulse::hitsInIDD(){
  Int_t localHits=0;
  for(int i=16; i<32; i++) localHits+=hits[i];
  return localHits;
}

Int_t  AngraMCPulse::hitsInVU(){
  Int_t localHits=0;
  for(int i=40; i<44; i++) localHits+=hits[i];
  return localHits;
}


Int_t  AngraMCPulse::hitsInVD(){
  Int_t localHits=0;
  for(int i=44; i<48; i++) localHits+=hits[i];
  return localHits;
}

Int_t  AngraMCPulse::hitsInIVU(){
  Int_t localHits=0;
  for(int i=32; i<36; i++) localHits+=hits[i];
  return localHits;
}

Int_t  AngraMCPulse::hitsInIVD(){
  Int_t localHits=0;
  for(int i=36; i<40; i++) localHits+=hits[i];
  return localHits;
}

