//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation event action header file
//
//  Authors: P.Chimenti, R.Lima, G. Valdiviesso
//
//  25-6-2008, v0.01
//  23-04-2025, fixing compatibility with Geant4 v11.3.0
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#ifndef AngraEventAction_H
#define AngraEventAction_H 1

#include "G4UserEventAction.hh"
#include "G4Types.hh"

#include <iostream>
#include <fstream>

class G4Event;

class AngraEventAction : public G4UserEventAction
{
public:
  AngraEventAction(){};
  AngraEventAction(std::ofstream *oFile){outFile=oFile;};
  ~AngraEventAction(){};

public:
  void BeginOfEventAction(const G4Event*);
  void EndOfEventAction(const G4Event*);

private:
  std::ofstream *outFile;
  G4int pmtCollID = -1;
  G4int vetoCollID = -1;
};

#endif

