//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation event action header file
//
//  Authors: P.Chimenti, R.Lima
//
//  25-6-2008, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#ifndef AngraEventAction_H
#define AngraEventAction_H 1

#include "G4UserEventAction.hh"

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
  
};

#endif

