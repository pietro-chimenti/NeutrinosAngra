//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation action initialization implementation file
//
//  Authors: G. Valdiviesso
//
//  24-04-2024, v0.03 
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#include "AngraActionInitialization.hh"
#include "AngraPrimaryGeneratorAction.hh"
#include "AngraEventAction.hh"
#include "AngraTrackingAction.hh"
#include "AngraSteppingAction.hh"

AngraActionInitialization::AngraActionInitialization(primaryEnum p, std::ofstream* outFile)
 : G4VUserActionInitialization(),
   fPrimary(p),
   fOutFile(outFile)
{}

AngraActionInitialization::~AngraActionInitialization()
{}

void AngraActionInitialization::BuildForMaster() const
{
  // No actions needed for master thread
}

void AngraActionInitialization::Build() const
{
  // Primary generator action
  SetUserAction(new AngraPrimaryGeneratorAction(fPrimary, fOutFile));
  
  // Event action
  SetUserAction(new AngraEventAction(fOutFile));
  
  // Tracking action
  SetUserAction(new AngraTrackingAction(fOutFile));
  
  // Stepping action
  SetUserAction(new AngraSteppingAction());
}
