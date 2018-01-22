//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation implementation of primary generator action file
//
//  Authors: P.Chimenti, R.Lima
//
//  30-4-2008, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//====================================================================================================
//In this file are set the particles involved in the detector's physical processes.
// Here are also set the particles energies and their charges, the positions where they would interact and the direction of the
// products of the reactions
//==================================================================================================== 
#include "AngraPrimaryGeneratorAction.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandGauss.h"
#include "AngraHEPEvtInterface.hh"
#include "G4MuonPlus.hh"
#include "AngraConstantMgr.hh"
#include "AngraMCLog.hh"

using namespace CLHEP;

#define PHOTONENERGY 3.1

AngraPrimaryGeneratorAction::AngraPrimaryGeneratorAction()
{
  AngraPrimaryGeneratorAction(POINT);
}

//set the particle
AngraPrimaryGeneratorAction::AngraPrimaryGeneratorAction(primaryEnum p,std::ofstream *oFile )
{

  outFile=oFile;

  // set the primary generator
  primaryGen=p;

  if(primaryGen==HEPEV){
    const char * dataFileName = AngraMCLog::Instance().GetInHepEvtFile();
    HepEvt = new AngraHEPEvtInterface(dataFileName);
  }
  G4int n_particle = 1;
  particleGun = new G4ParticleGun(n_particle);
  G4String particleName;
  particleGun->SetParticleDefinition(G4OpticalPhoton::OpticalPhotonDefinition());
  particleGun->SetParticleEnergy(PHOTONENERGY*eV);//photon energy
  particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
}

AngraPrimaryGeneratorAction::~AngraPrimaryGeneratorAction()
{
  if(primaryGen==HEPEV)
    delete HepEvt;
  delete particleGun;
}

void AngraPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  switch(primaryGen)
    {
    case  POINT   : GeneratePoint(anEvent);
      break;
    case  HEPEV   : GenerateHEPEv(anEvent);
      break;

    default : printf("ERROR Primary Generator: no valid Primary Generator \n");      
    }   
}


// generator with fixed vertex and random direction
void AngraPrimaryGeneratorAction::GeneratePoint(G4Event* anEvent)
{
  //  G4ThreeVector v(CLHEP::RandFlat::shoot(-1.,1.),CLHEP::RandFlat::shoot(-1.,1.),CLHEP::RandFlat::shoot(-1.,1.));
  G4ThreeVector v(-1.,0.,0.);
  particleGun->SetParticleMomentumDirection(v);
  particleGun->GeneratePrimaryVertex(anEvent);
}


void AngraPrimaryGeneratorAction::GenerateHEPEv(G4Event* anEvent)
{
  HepEvt->GeneratePrimaryVertex(anEvent);
}


