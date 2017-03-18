//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation veto hit header file
//
//  Authors: P.Chimenti
//
//  03-07-2010, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
#ifndef AngraVetoHit_H
#define AngraVetoHit_H 1


#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"

#include <string>
#include <iostream>
#include <sstream>

class AngraVetoHit : public G4VHit
{
  public:

      AngraVetoHit();
     ~AngraVetoHit();
      AngraVetoHit(const AngraVetoHit&);
      const AngraVetoHit& operator=(const AngraVetoHit&);
      G4int operator==(const AngraVetoHit&) const;

      inline void* operator new(size_t);
      inline void  operator delete(void*);

      void Draw();
      void Print();

  public:
  
      void SetTrackID  (G4int track)      { trackID = track; };
      void SetVolName  (std::string   vn)      { volName = vn; };
      void SetEdep     (G4double de)      { edep = de; };
      void SetPos      (G4ThreeVector xyz){ pos = xyz; };
      void SetLocalPos      (G4ThreeVector xyz){ localPos = xyz; };
      void SetLocalTime (G4double t){ ltime = t;};  
      void SetGlobalTime (G4double t){ gtime = t;};    

      G4int GetTrackID()    { return trackID; };
      std::string GetVolName(){ return volName; };
      G4double GetEdep()    { return edep; };      
      G4ThreeVector GetPos(){ return pos; };
      G4ThreeVector GetLocalPos(){ return localPos; };
      G4double GetLocalTime(){return ltime;}; 
      G4double GetGlobalTime(){return gtime;};
    
 
  private:
  
      G4int         trackID;
      std::string   volName;
      G4double      edep;
      G4ThreeVector pos;
      G4ThreeVector localPos;
      G4double ltime;
      G4double gtime;
};

typedef G4THitsCollection<AngraVetoHit> AngraVetoHitsCollection;

extern G4Allocator<AngraVetoHit> AngraVetoHitAllocator;

inline void* AngraVetoHit::operator new(size_t)
{
  void *aHit;
  aHit = (void *) AngraVetoHitAllocator.MallocSingle();
  return aHit;
}

inline void AngraVetoHit::operator delete(void *aHit)
{
  AngraVetoHitAllocator.FreeSingle((AngraVetoHit*) aHit);
}

#endif
