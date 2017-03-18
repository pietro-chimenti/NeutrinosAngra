//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation HIT header file
//
//  Authors: P.Chimenti
//
//  3-8-2009, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#ifndef AngraPMTHit_h
#define AngraPMTHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4LogicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4VPhysicalVolume.hh"

class G4VTouchable;

class AngraPMTHit : public G4VHit
{
public:
  
  AngraPMTHit();
  ~AngraPMTHit();
  AngraPMTHit(const AngraPMTHit &right);

  const AngraPMTHit& operator=(const AngraPMTHit &right);
  G4int operator==(const AngraPMTHit &right) const;

  inline void *operator new(size_t);
  inline void operator delete(void *aHit);
  
  inline void SetPhysVol(G4VPhysicalVolume* physVol){this->physVol=physVol;}
  inline G4VPhysicalVolume* GetPMTPhysVol(){return physVol;}

  inline void SetPos(G4double x,G4double y,G4double z){ pos=G4ThreeVector(x,y,z); }
  inline G4ThreeVector GetPos(){return pos;}


  inline void SetEn(G4double En){en=En;}
  inline G4double GetEn(){return en;}

  inline void SetTime(G4double Time){time=Time;}
  inline G4double GetTime(){return time;}

  inline void SetTrack(G4int Track){track=Track;}
  inline G4int GetTrack(){return track;}

private:
  G4double time;
  G4double en;
  G4ThreeVector pos;
  G4int track;
  G4VPhysicalVolume* physVol;

};

typedef G4THitsCollection<AngraPMTHit> AngraPMTHitsCollection;

extern G4Allocator<AngraPMTHit> AngraPMTHitAllocator;

inline void* AngraPMTHit::operator new(size_t){
  void *aHit;
  aHit = (void *) AngraPMTHitAllocator.MallocSingle();
  return aHit;
}

inline void AngraPMTHit::operator delete(void *aHit){
  AngraPMTHitAllocator.FreeSingle((AngraPMTHit*) aHit);
}

#endif


