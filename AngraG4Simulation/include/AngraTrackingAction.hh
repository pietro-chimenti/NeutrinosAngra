//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation definition of tracking action 
//
//  Authors: P.Chimenti
//
//  25-3-2009, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#ifndef AngraTrackingAction_h
#define AngraTrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4ios.hh"

class AngraTrackingAction : public G4UserTrackingAction {
  
public:
  AngraTrackingAction(){};
  AngraTrackingAction(std::ofstream *oFile){outFile=oFile;};
  virtual ~AngraTrackingAction(){};
   
  virtual void PreUserTrackingAction(const G4Track*){};
  virtual void PostUserTrackingAction(const G4Track*);
  
private:
  std::ofstream* outFile;
  
  
};

#endif
