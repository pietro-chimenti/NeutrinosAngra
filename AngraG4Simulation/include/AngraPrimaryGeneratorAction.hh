//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation primary generator header file
//
//  Authors: P.Chimenti, R.Lima
//
//  30-4-2008, v0.01 
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

class G4ParticleGun;
class G4Event;
class G4VPrimaryGenerator;

enum primaryEnum {POINT,HEPEV};

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


