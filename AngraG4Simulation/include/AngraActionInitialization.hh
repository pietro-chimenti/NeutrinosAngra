//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation action initialization header file
//
//  Authors: G. Valdiviesso
//
//  24-04-2024, v0.03 
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#ifndef AngraActionInitialization_h
#define AngraActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "globals.hh"
#include <fstream>

enum primaryEnum { POINT_LIKE = 0, HEPEVT = 1 };

class AngraActionInitialization : public G4VUserActionInitialization
{
public:
  AngraActionInitialization(primaryEnum p, std::ofstream* outFile);
  virtual ~AngraActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

private:
  primaryEnum fPrimary;
  std::ofstream* fOutFile;
};

#endif
