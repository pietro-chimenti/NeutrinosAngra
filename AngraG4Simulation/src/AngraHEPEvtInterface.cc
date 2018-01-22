//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation implementation of hepevt interface
//
//  Authors: P.Chimenti
//
//  10-03-2010 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
#include "AngraHEPEvtInterface.hh"

#include "G4Types.hh"
#include "G4ios.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4Event.hh"
#include "CLHEP/Units/PhysicalConstants.h"

using namespace CLHEP;

#define SPEEDOL 299792458  // m/s

AngraHEPEvtInterface::AngraHEPEvtInterface(char* evfile)
{
  inputFile.open(evfile);
  if (inputFile) {
    fileName = evfile;
  }
  else {
    G4Exception("AngraHEPEvtInterface::AngraHEPEvtInterface()",
		"AngraError000",
		FatalException,"AngraHEPEvtInterface:: cannot open file.");
  }
  G4ThreeVector zero;
  particle_position = zero;
  particle_time = 0.0;
  
}

AngraHEPEvtInterface::AngraHEPEvtInterface(G4String evfile)
{
  const char* fn = evfile.data();
  inputFile.open((char*)fn);
  if (inputFile) {
    fileName = evfile;
  }
  else {
    G4Exception("AngraHEPEvtInterface::AngraHEPEvtInterface()",
		"AngraError000",
		FatalException,"AngraHEPEvtInterface:: cannot open file.");
  }
  G4ThreeVector zero;
  particle_position = zero;
  particle_time = 0.0;
}

AngraHEPEvtInterface::~AngraHEPEvtInterface()
{;}

void AngraHEPEvtInterface::GeneratePrimaryVertex(G4Event* evt)
{

  inputFile >> nevhep >> nhep;
  if( inputFile.eof() ) 
    {
    G4Exception("AngraHEPEvtInterface::GeneratePrimaryVertex()",
		"AngraError000",
		FatalException,"End-Of-File : HEPEvt input file");      
      return;
    }

  // loop to read particle entries
  for( G4int ihep=0; ihep<nhep; ihep++ )
    {

      // read particle data
      inputFile >> isthep[ihep]  >> idhep[ihep] //
		>> jmohep[0][ihep] >> jmohep[1][ihep] //
		>> jdahep[0][ihep] >> jdahep[1][ihep] //
		>> phep[0][ihep] >> phep[1][ihep] //
		>> phep[2][ihep] >> phep[3][ihep] //
		>> phep[4][ihep] >> vhep[0][ihep] //
		>> vhep[1][ihep] >> vhep[2][ihep] //
		>> vhep[3][ihep]; 

    }

  
  // loop to create particles and vertexes 
  //it is missing the mother<-->child relation (for now)
  for( G4int ihep=0; ihep<nhep; ihep++ )
    {
      // if particle did not decay
      if(isthep[ihep]==1){ 
	
	// create G4PrimaryParticle object
	G4PrimaryParticle* particle 
	  = new G4PrimaryParticle( idhep[ihep], phep[0][ihep]*GeV, phep[1][ihep]*GeV, phep[2][ihep]*GeV);
	particle->SetMass( phep[4][ihep]*GeV );
	particle->SetTrackID(ihep);
	
	// create G4PrimaryVertex object
	G4ThreeVector position(vhep[0][ihep]*mm,vhep[1][ihep]*mm,vhep[2][ihep]*mm);
	G4PrimaryVertex* vertex = new G4PrimaryVertex(position,vhep[3][ihep]*(0.001/SPEEDOL)*second);
	vertex->SetPrimary(particle);


	//add vertex to event
	evt->AddPrimaryVertex( vertex );
      }
    }

}

