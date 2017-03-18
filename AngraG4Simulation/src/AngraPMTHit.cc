//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation HIT implementation file
//
//  Authors: P.Chimenti
//
//  3-08-2009, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#include "AngraPMTHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

G4Allocator<AngraPMTHit> AngraPMTHitAllocator;

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
AngraPMTHit::AngraPMTHit()
  :time(0), en(0), pos(0.,0.,0.), track(0), physVol(0)
{}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
AngraPMTHit::~AngraPMTHit()
{}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
AngraPMTHit::AngraPMTHit(const AngraPMTHit &right)
  : G4VHit()
{
  time    = right.time;
  en      = right.en;
  pos     = right.pos;
  track   = right.track;
  physVol = right.physVol;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
const AngraPMTHit& AngraPMTHit::operator=(const AngraPMTHit &right){
  time    = right.time;
  en      = right.en;
  pos     = right.pos;
  track   = right.track;
  physVol = right.physVol;
  return *this;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
G4int AngraPMTHit::operator==(const AngraPMTHit &right) const{
  return (en==right.en&&pos==right.pos&&time==right.time&&track==right.track);
}








