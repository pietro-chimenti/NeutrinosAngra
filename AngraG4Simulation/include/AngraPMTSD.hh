//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation Sensitive Detector header file
//
//  Authors: P.Chimenti
//
//  3-8-2009, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#ifndef AngraPMTSD_h
#define AngraPMTSD_h 1

#include "G4DataVector.hh"
#include "G4VSensitiveDetector.hh"
#include "AngraPMTHit.hh"
class G4Step;
class G4HCofThisEvent;

class AngraPMTSD : public G4VSensitiveDetector
{

public:
  AngraPMTSD(G4String name);
  ~AngraPMTSD();
  
  void Initialize(G4HCofThisEvent* HCE);
  G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  
  G4bool ProcessHits_constStep(const G4Step* aStep,
			       G4TouchableHistory* ROhist);
  void EndOfEvent(G4HCofThisEvent* HCE);
  void clear();
  void DrawAll();
  void PrintAll();
  
private:
  AngraPMTHitsCollection* pmtHitCollection;
};

#endif

