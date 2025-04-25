//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation primary generator header file
//
//  Authors: P.Chimenti, R.Lima, G. Valdiviesso
//
//  30-04-2008, v0.01
//  23-04-2025, fixing compatibility with Geant4 v11.3.0
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//==============================================================
//In this file are the processes that define the primaries generated
//inside the detector
//==============================================================

#ifndef AngraPrimaryGeneratorAction_h
#define AngraPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include <iostream>
#include <fstream>
#include "AngraActionInitialization.hh"

class G4ParticleGun;
class G4Event;
class G4VPrimaryGenerator;

class AngraPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  AngraPrimaryGeneratorAction();
  AngraPrimaryGeneratorAction(std::ofstream *oFile){outFile=oFile;};
  ~AngraPrimaryGeneratorAction();
  AngraPrimaryGeneratorAction(primaryEnum p,std::ofstream *oFile = 0 );

public:
  void GeneratePrimaries(G4Event* anEvent);

private:
  G4ParticleGun* particleGun;
  void GeneratePoint(G4Event* anEvent);
  void GenerateHEPEv(G4Event* anEvent);

  primaryEnum primaryGen;

  G4VPrimaryGenerator * HepEvt;

  std::ofstream *outFile;

};

#endif


