//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation stepping action implementation file
//
//  Authors: P.Chimenti
//
//  04-08-2008, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
#include "AngraSteppingAction.hh"
#include "AngraEventAction.hh"
#include "AngraTrackingAction.hh"
#include "AngraPMTSD.hh"

#include "G4SteppingManager.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4ProcessManager.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4OpBoundaryProcess.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
AngraSteppingAction::AngraSteppingAction()
{}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
AngraSteppingAction::~AngraSteppingAction()
{}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void AngraSteppingAction::UserSteppingAction(const G4Step * theStep){
 
  G4Track* theTrack = theStep->GetTrack();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();
  G4OpBoundaryProcessStatus boundaryStatus=Undefined;
  static G4OpBoundaryProcess* boundary=NULL;
  
  //find the boundary process only once
  if(!boundary){
    G4ProcessManager* pm 
      = theStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pm->GetProcessListLength();
    G4ProcessVector* pv = pm->GetProcessList();
    G4int i;
    for( i=0;i<nprocesses;i++){
      if((*pv)[i]->GetProcessName()=="OpBoundary"){
	boundary = (G4OpBoundaryProcess*)(*pv)[i];
	break;
      }
    }
  }

  if(!thePostPV){//out of world
    return;
  }

  G4ParticleDefinition* particleType = theTrack->GetDefinition();
  if(particleType==G4OpticalPhoton::OpticalPhotonDefinition()){

    boundaryStatus=boundary->GetStatus();
    if(thePostPoint->GetStepStatus()==fGeomBoundary){
       switch(boundaryStatus){
      case Absorption:
	break;
      case Detection:
	{
	  //	std::cout << " Detected " <<  
	  //thePostPoint->GetPosition().x() <<  " " << 
	  //thePostPoint->GetPosition().y() <<  " " <<
	  //thePostPoint->GetPosition().z() <<  " " <<
	  //std::endl;
	  
	  G4SDManager* SDman = G4SDManager::GetSDMpointer();
	  G4String sdName="/pmtSD";
	  AngraPMTSD* pmtSD = (AngraPMTSD*)SDman
	    ->FindSensitiveDetector(sdName);
	  if(pmtSD)
	    pmtSD->ProcessHits_constStep(theStep,NULL);
	  break;
	}
      case FresnelReflection:
      case TotalInternalReflection:
      case SpikeReflection:
	break;
      case NoRINDEX:
	break;
      default:
	break;
      }
    }
  }
  
}


















