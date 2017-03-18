//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation implementation of tracking action 
//
//  Authors: P.Chimenti
//
//  25-3-2009, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
#include "AngraTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "AngraMCLog.hh"

void AngraTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{

  AngraMCLog::Instance().SaveTrack(aTrack);  

}




