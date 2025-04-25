//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation implementation of primary generator action file
//
//  Authors: P.Chimenti, R.Lima, G. Valdiviesso
//
//  30-04-2008, v0.01
//  23-04-2025, fixing compatibility with Geant4 v11.3.0
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
#include "G4RunManager.hh"
#include "Randomize.hh"
#include "globals.hh"
#include "CLHEP/Random/Random.h"
#include "CLHEP/Random/RandGauss.h"
#include "AngraHEPEvtInterface.hh"
#include "G4MuonPlus.hh"
#include "AngraConstantMgr.hh"
#include "AngraMCLog.hh"

using namespace CLHEP;

#define DEFAULTENERGY 3.1

AngraPrimaryGeneratorAction::AngraPrimaryGeneratorAction()
	: primaryGen(POINT_LIKE), HepEvt(nullptr), outFile(nullptr)
{
	// Initialize with point-like source
	// Set default values (will be used if not specified in the macro)
	auto n_particle = 1;
	particleGun = new G4ParticleGun(n_particle);

	// Set default particle to electron
	auto particleTable = G4ParticleTable::GetParticleTable();
	auto particle = particleTable->FindParticle("e-");
	if (particle) {
		particleGun->SetParticleDefinition(particle);
	} else {
		// Fallback to optical photon if electron not found
		particleGun->SetParticleDefinition(G4OpticalPhoton::OpticalPhotonDefinition());
	}

	// Default energy
	particleGun->SetParticleEnergy(DEFAULTENERGY*eV);

	// Default position
	particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));

	// Default direction
	particleGun->SetParticleMomentumDirection(G4ThreeVector(1.0, 0.0, 0.0));
}

//set the particle
AngraPrimaryGeneratorAction::AngraPrimaryGeneratorAction(primaryEnum p, std::ofstream *oFile)
	: primaryGen(p), HepEvt(nullptr), outFile(oFile)
{
	// Initialize HepEvt interface if needed
	if(primaryGen == HEPEVT) {
		auto dataFileName = AngraMCLog::Instance().GetInHepEvtFile();
		HepEvt = new AngraHEPEvtInterface(dataFileName);
	}

	// Create the particle gun with default settings
	// These settings will be overridden by any /gun/ commands in the macro file
	auto n_particle = 1;
	particleGun = new G4ParticleGun(n_particle);

	// Set default values (will be used if not specified in the macro)
	auto particleTable = G4ParticleTable::GetParticleTable();
	auto particle = particleTable->FindParticle("e-");
	if (particle) {
		particleGun->SetParticleDefinition(particle);
	} else {
		// Fallback to optical photon if electron not found
		particleGun->SetParticleDefinition(G4OpticalPhoton::OpticalPhotonDefinition());
	}

	// Default energy (will be overridden by /gun/energy command in macro)
	particleGun->SetParticleEnergy(DEFAULTENERGY*eV);

	// Default position (will be overridden by /gun/position command in macro)
	particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));

	// Default direction (will be overridden by /gun/direction command in macro)
	particleGun->SetParticleMomentumDirection(G4ThreeVector(1.0, 0.0, 0.0));
}

AngraPrimaryGeneratorAction::~AngraPrimaryGeneratorAction()
{
	if(primaryGen==HEPEVT)
		delete HepEvt;
	delete particleGun;
}

void AngraPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	switch(primaryGen)
		{
		case  POINT_LIKE : GeneratePoint(anEvent);
			break;
		case  HEPEVT     : GenerateHEPEv(anEvent);
			break;

		default : G4cerr << "ERROR Primary Generator: no valid Primary Generator" << G4endl;
		}
}


// Generator for point-like source
// Uses the current settings of the particle gun (from macro file)
void AngraPrimaryGeneratorAction::GeneratePoint(G4Event* anEvent)
{
	// Generate the primary vertex using the current particle gun settings
	// All settings (position, direction, energy, particle type) come from the macro file
	// through the /gun/ commands
	particleGun->GeneratePrimaryVertex(anEvent);

	// Log information about the generated particle if verbose level is high enough
	// This uses the Geant4 verbosity system instead of writing to a file
	auto verboseLevel = G4RunManager::GetRunManager()->GetVerboseLevel();
	if (verboseLevel > 1) {
		auto particle = particleGun->GetParticleDefinition();
		auto position = particleGun->GetParticlePosition();
		auto direction = particleGun->GetParticleMomentumDirection();
		auto energy = particleGun->GetParticleEnergy();

		G4cout << "EVENT: " << anEvent->GetEventID() << G4endl;
		G4cout << "  Particle: " << particle->GetParticleName() << G4endl;
		G4cout << "  Position: " << position.x() << " " << position.y() << " " << position.z() << " mm" << G4endl;
		G4cout << "  Direction: " << direction.x() << " " << direction.y() << " " << direction.z() << G4endl;
		G4cout << "  Energy: " << energy/CLHEP::MeV << " MeV" << G4endl;
		G4cout << G4endl;

	}
}


void AngraPrimaryGeneratorAction::GenerateHEPEv(G4Event* anEvent)
{
	HepEvt->GeneratePrimaryVertex(anEvent);
}


