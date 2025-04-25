//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation implementation of event action file
//
//  Authors: P.Chimenti, R.Lima, G. Valdiviesso
//
//  25-6-2008, v0.01
//  23-04-2025, fixing compatibility with Geant4 v11.3.0
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
#include "AngraVetoHit.hh"
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

  // Get collection IDs only once per thread
  if (pmtCollID < 0) {
    pmtCollID = SDman->GetCollectionID("pmtHitCollection");
  }

  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  AngraPMTHitsCollection* PHC = 0;

  // now Hits:
  if (HCE && pmtCollID >= 0) {
    PHC = (AngraPMTHitsCollection*)(HCE->GetHC(pmtCollID));
    if (PHC) {
      AngraMCLog::Instance().SaveHits(PHC);
    }
  }

  // Get veto collection ID only once per thread
  if (vetoCollID < 0) {
    vetoCollID = SDman->GetCollectionID("vetoHitCollection");
  }

  AngraVetoHitsCollection* VHC = 0;
  if (HCE && vetoCollID >= 0) {
    VHC = (AngraVetoHitsCollection*)(HCE->GetHC(vetoCollID));
    if (VHC) {
      AngraMCLog::Instance().SaveHits(VHC);
    }
  }
}


