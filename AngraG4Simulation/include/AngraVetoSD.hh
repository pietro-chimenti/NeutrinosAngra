//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation veto SD header file
//
//  Authors: P.Chimenti
//
//  03-07-2010, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
#ifndef AngraVetoSD_h
#define AngraVetoSD_h 1

#include "G4VSensitiveDetector.hh"
#include "AngraVetoHit.hh"

class G4Step;
class G4HCofThisEvent;

class AngraVetoSD : public G4VSensitiveDetector
{
  public:
      AngraVetoSD(G4String);
     ~AngraVetoSD();

      void Initialize(G4HCofThisEvent*);
      G4bool ProcessHits(G4Step*, G4TouchableHistory*);
      void EndOfEvent(G4HCofThisEvent*);

  private:
      AngraVetoHitsCollection* vetoCollection;

};

#endif

