//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation implementation of event action file
//
//  Authors: P.Chimenti, R.Lima
//
//  25-6-2008, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//==============================================================================
// At the moment just for saving Trajectories and Hits 
//==============================================================================
#include "AngraEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "AngraPMTHit.hh"
#include "AngraMCLog.hh"


 
void AngraEventAction::BeginOfEventAction(const G4Event* evt)
{
  AngraMCLog::Instance().SaveEvent(evt);
}

void AngraEventAction::EndOfEventAction(const G4Event* evt)
{

  G4TrajectoryContainer* trajectoryContainer = evt->GetTrajectoryContainer();
  
  AngraMCLog::Instance().SaveTrajectories(trajectoryContainer);

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  G4int pmtCollID=SDman->GetCollectionID("pmtHitCollection");
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  AngraPMTHitsCollection* PHC = 0;
  // now Hits:
  if(HCE){
    PHC = (AngraPMTHitsCollection*)(HCE->GetHC(pmtCollID));
  }
  if(PHC){
    AngraMCLog::Instance().SaveHits(PHC);
  }

  G4int vetoCollID=SDman->GetCollectionID("vetoHitCollection");
  AngraVetoHitsCollection* VHC = 0;
  if(HCE){
    VHC = (AngraVetoHitsCollection*)(HCE->GetHC(vetoCollID));
  }
  if(VHC){
    AngraMCLog::Instance().SaveHits(VHC);
  }


}


