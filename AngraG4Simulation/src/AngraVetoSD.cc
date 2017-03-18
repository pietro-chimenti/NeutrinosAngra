//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation veto SD implementation file
//
//  Authors: P.Chimenti
//
//  04-08-2008, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
#include "AngraVetoSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

AngraVetoSD::AngraVetoSD(G4String name)
:G4VSensitiveDetector(name)
{
  G4String HCname;
  collectionName.insert(HCname="vetoHitCollection");
}

AngraVetoSD::~AngraVetoSD(){ }

void AngraVetoSD::Initialize(G4HCofThisEvent* HCE)
{
  vetoCollection = new AngraVetoHitsCollection
                          (SensitiveDetectorName,collectionName[0]); 
  static G4int HCID = -1;
  if(HCID<0)
  { HCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]); }
  HCE->AddHitsCollection( HCID, vetoCollection ); 
}

G4bool AngraVetoSD::ProcessHits(G4Step* aStep,G4TouchableHistory*)
{
  G4double edep = aStep->GetTotalEnergyDeposit();

  if(edep==0.) return false;

  AngraVetoHit* newHit = new AngraVetoHit();
  newHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
  newHit->SetVolName(aStep->GetPreStepPoint()->GetPhysicalVolume()
                                                 ->GetName());
  newHit->SetEdep     (edep);
  newHit->SetPos      (aStep->GetPostStepPoint()->GetPosition());
  G4TouchableHandle theTouchable = aStep->GetPreStepPoint()->GetTouchableHandle();
  G4ThreeVector worldPos = aStep->GetPostStepPoint()->GetPosition();
  G4ThreeVector locPoint = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
  newHit->SetLocalPos(locPoint);
  newHit->SetLocalTime(aStep->GetPostStepPoint()->GetLocalTime());
  newHit->SetGlobalTime(aStep->GetPostStepPoint()->GetGlobalTime());
  vetoCollection->insert( newHit );
  
  //newHit->Print();
  //newHit->Draw();

  return true;
}

void AngraVetoSD::EndOfEvent(G4HCofThisEvent*)
{
  if (verboseLevel>0) { 

     G4int NbHits = vetoCollection->entries();
     G4cout << "\n-------->Hits Collection: in this event they are " << NbHits 
            << " hits in the veto chambers: " << G4endl;
     for (G4int i=0;i<NbHits;i++) (*vetoCollection)[i]->Print();

    } 
}

