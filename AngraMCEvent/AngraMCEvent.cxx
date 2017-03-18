#include "TMath.h"   
#include "TRandom.h"   
#include "AngraMCEvent.h"

ClassImp(AngraMCVertex)
ClassImp(AngraMCTrack)
ClassImp(AngraMCHit)
ClassImp(AngraMCEvent)

TClonesArray *AngraMCEvent::fgVertexes  = 0;
TClonesArray *AngraMCEvent::fgParticles = 0;
TClonesArray *AngraMCEvent::fgTracks    = 0;
TClonesArray *AngraMCEvent::fgHits      = 0;

using namespace std;



bool AngraMCHit::operator== (AngraMCHit *other) const{
  return this->fLocalTime == other->GetLocalTime();
}

bool AngraMCHit::operator< (AngraMCHit *other) const{
  return this->fLocalTime < other->GetLocalTime();
}

bool AngraMCHit::operator> (AngraMCHit *other) const{
  return this->fLocalTime > other->GetLocalTime();
}

bool AngraMCHit::operator<= (AngraMCHit *other) const{
  return this->fLocalTime <= other->GetLocalTime();
}

bool AngraMCHit::operator>= (AngraMCHit *other) const{
  return this->fLocalTime >= other->GetLocalTime();
}

Int_t AngraMCHit::Compare(const TObject* other) const
{
  AngraMCHit *otherHit = (AngraMCHit*) other;

  if ( this->fLocalTime <  otherHit->GetLocalTime() ) { return -1; }
  if ( this->fLocalTime >  otherHit->GetLocalTime() ) { return +1; }
  return 0;
  
}

Bool_t AngraMCHit::IsSortable() const {return kTRUE;}

//______________________________________________________________________________
AngraMCEvent::AngraMCEvent()
{
   if (!fgVertexes)  fgVertexes  = new TClonesArray("AngraMCVertex", 10);
   if (!fgParticles) fgParticles = new TClonesArray("AngraMCParticle", 10);
   if (!fgTracks)    fgTracks    = new TClonesArray("AngraMCTrack", 100000);
   if (!fgHits)      fgHits      = new TClonesArray("AngraMCHit",   100000);
   fVertexes  = fgVertexes;
   fParticles = fgParticles;
   fTracks    = fgTracks;
   fHits      = fgHits;
   
   fHasTrackList = 0;
   
}

//______________________________________________________________________________
AngraMCEvent::~AngraMCEvent()
{
   Reset();
}

//______________________________________________________________________________
void AngraMCEvent::NewEvent() {

   Clear();

   fNvertex   = 0;
   fNparticle = 0;
   fNtrack    = 0;
   fNhit      = 0;
   fFileName  = "";
   fHasTrackList = 0;   
            
}


//______________________________________________________________________________
AngraMCVertex *AngraMCEvent::AddVertex()
{
   TClonesArray &vertexes = *fVertexes;
   AngraMCVertex  *vertex = new(vertexes[fNvertex++]) AngraMCVertex();
   return vertex;
}

//______________________________________________________________________________
AngraMCParticle *AngraMCEvent::AddPrimary()
{
   TClonesArray  &primaries = *fParticles;
   AngraMCParticle *particle = new(primaries[fNparticle++]) AngraMCParticle();
   return particle;
}

//______________________________________________________________________________
AngraMCTrack *AngraMCEvent::AddTrack()
{
   TClonesArray &tracks = *fTracks;
   AngraMCTrack *track = new(tracks[fNtrack++]) AngraMCTrack();
   return track;
}


//______________________________________________________________________________
AngraMCHit *AngraMCEvent::AddHit()
{
   TClonesArray &hits = *fHits;
   AngraMCHit *hit = new(hits[fNhit++]) AngraMCHit();
   return hit;
}

//______________________________________________________________________________
void AngraMCEvent::Clear(Option_t *option)
{
   fVertexes ->Clear(option);
   fParticles->Clear(option);
   fTracks   ->Clear(option);
   fHits     ->Clear(option);   
}

//______________________________________________________________________________
void AngraMCEvent::Reset(Option_t *)
{
   delete fgVertexes;  fgVertexes  = 0;
   delete fgParticles; fgParticles = 0;
   delete fgTracks;    fgTracks    = 0;
   delete fgHits;      fgHits      = 0;
}

//______________________________________________________________________________
void AngraMCEvent::BuildTrackList()
{
  fTrackList.resize(fNtrack);
  for(Int_t i = 0; i<fNtrack; i++){
    TClonesArray &tracks = *fTracks;
    AngraMCTrack *track = (AngraMCTrack*)tracks[i];  
    fTrackList[track->fID] = i;
  }

  fHasTrackList = 1;

}


//______________________________________________________________________________
AngraMCVertex   *AngraMCEvent::GetVertex(Long64_t i){
  
  if(fNvertex==0){return 0;} else {
    
    return (AngraMCVertex*)fVertexes->At(i);
    
  }  
}

//______________________________________________________________________________
AngraMCParticle   *AngraMCEvent::GetPrimary(Long64_t i){
  
  if(fNvertex==0){return 0;} else {
    return (AngraMCParticle*)fParticles->At(i);
    
  }  
}

//______________________________________________________________________________
AngraMCTrack   *AngraMCEvent::GetTrack(Long64_t i){
  
  if(fNtrack==0){return 0;} else {
    if (fHasTrackList) {return (AngraMCTrack*)fTracks->At(fTrackList[i]);}
    else return (AngraMCTrack*)fTracks->At(i);    
  }
}

//______________________________________________________________________________
Int_t AngraMCTrack::Compare(const TObject* other) const
{
  AngraMCTrack *otherTrack = (AngraMCTrack*) other;

  if ( this->fID <  otherTrack->GetID() ) {return -1;}
  if ( this->fID >  otherTrack->GetID() ) {return +1;}
  if ( this->fID == otherTrack->GetID() ) {
    if (this->fGlobalTime < otherTrack->GetLocalTime()) {return -1;}
    else {return +1;}    
  }

  return 0;
  
}

//______________________________________________________________________________
Bool_t AngraMCTrack::IsSortable() const {return kTRUE;}


//______________________________________________________________________________
AngraMCHit   *AngraMCEvent::GetHit(Long64_t i){
  
  if(fNhit==0){return 0;} else {
    
    return (AngraMCHit*)fHits->At(i);
    
  }
}


