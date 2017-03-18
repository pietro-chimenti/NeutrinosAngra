//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation veto hit implementation file
//
//  Authors: P.Chimenti
//
//  03-07-2010, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
#include "AngraVetoHit.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"

G4Allocator<AngraVetoHit> AngraVetoHitAllocator;

AngraVetoHit::AngraVetoHit() {}

AngraVetoHit::~AngraVetoHit() {}

AngraVetoHit::AngraVetoHit(const AngraVetoHit& right)
  : G4VHit()
{
  trackID   = right.trackID;
  edep      = right.edep;
  pos       = right.pos;
  localPos       = right.localPos;
  volName = right.volName;
  ltime = right.ltime;
  gtime = right.gtime;
}

const AngraVetoHit& AngraVetoHit::operator=(const AngraVetoHit& right)
{
  trackID   = right.trackID;
  edep      = right.edep;
  pos       = right.pos;
  localPos       = right.localPos;
  volName=right.volName;
  ltime=right.ltime;
  gtime=right.gtime;
  return *this;
}

G4int AngraVetoHit::operator==(const AngraVetoHit& right) const
{
  return (this==&right) ? 1 : 0;
}

void AngraVetoHit::Draw()
{
  G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
  if(pVVisManager)
  {
    G4Circle circle(pos);
    circle.SetScreenSize(2.);
    circle.SetFillStyle(G4Circle::filled);
    G4Colour colour(1.,0.,0.);
    G4VisAttributes attribs(colour);
    circle.SetVisAttributes(attribs);
    pVVisManager->Draw(circle);
  }
}

void AngraVetoHit::Print()
{
  G4cout << "  trackID: " << trackID << " Volume Name " << volName
         << "  energy deposit: " << G4BestUnit(edep,"Energy")
	 << "  position: " << G4BestUnit(pos,"Length")
	 << "  local position: " << G4BestUnit(localPos,"Length") 
         << "  local time: " << G4BestUnit(ltime,"Time")
         << "  global time: " << G4BestUnit(gtime,"Time") << G4endl;
}

