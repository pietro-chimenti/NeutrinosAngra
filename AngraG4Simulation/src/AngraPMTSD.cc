//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation Sensitive Detector  implementation file
//
//  Authors: P.Chimenti
//
//  3-08-2009, v0.01
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#include "AngraPMTSD.hh"
#include "AngraPMTHit.hh"
#include "AngraDetectorConstruction.hh"
//#include "AngraUserTrackInformation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleDefinition.hh"

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
AngraPMTSD::AngraPMTSD(G4String name)
  :G4VSensitiveDetector(name),pmtHitCollection(0)
{
  collectionName.insert("pmtHitCollection");
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
AngraPMTSD::~AngraPMTSD()
{}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void AngraPMTSD::Initialize(G4HCofThisEvent* HCE){
  pmtHitCollection = new AngraPMTHitsCollection
                      (SensitiveDetectorName,collectionName[0]);

  //Store collection with event and keep ID
  G4int HCID = GetCollectionID(0);
  HCE->AddHitsCollection(HCID, pmtHitCollection);
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
G4bool AngraPMTSD::ProcessHits(G4Step* aStep,G4TouchableHistory* ROhist){
  return ProcessHits_constStep(aStep,ROhist);
}

G4bool AngraPMTSD::ProcessHits_constStep(const G4Step* aStep,G4TouchableHistory* ){

  //need to know if this is an optical photon
  if(aStep->GetTrack()->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

  AngraPMTHit* hit= new AngraPMTHit();
  G4VPhysicalVolume* physVol=  aStep->GetPostStepPoint()->GetTouchable()->GetVolume();

  hit->SetPhysVol(physVol);
  hit->SetTime(aStep->GetPostStepPoint()->GetGlobalTime());
  hit->SetEn(aStep->GetPostStepPoint()->GetTotalEnergy());
  hit->SetPos(aStep->GetPostStepPoint()->GetPosition().x(),aStep->GetPostStepPoint()->GetPosition().y(),aStep->GetPostStepPoint()->GetPosition().z());
  hit->SetTrack(aStep->GetTrack()->GetTrackID());

  pmtHitCollection->insert(hit);


  return true;
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void AngraPMTSD::EndOfEvent(G4HCofThisEvent* ){
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void AngraPMTSD::clear(){
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void AngraPMTSD::DrawAll(){
}

//_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
void AngraPMTSD::PrintAll(){
}