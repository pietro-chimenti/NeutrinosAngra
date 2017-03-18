//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation stepping action header file
//
//  Authors: P.Chimenti
//
//  04-08-2009, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
#ifndef AngraSteppingAction_H
#define AngraSteppingAction_H 1

#include "globals.hh"
#include "G4UserSteppingAction.hh"

class AngraEventAction;

class AngraSteppingAction : public G4UserSteppingAction
{
public:
  AngraSteppingAction();
  ~AngraSteppingAction();
  virtual void UserSteppingAction(const G4Step*);
  
private:
};

#endif
