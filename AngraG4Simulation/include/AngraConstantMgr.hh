//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraConstantMgr header file
//
//  Authors: P.Chimenti
//
//  24-03-2010, v0.01
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//==============================================================
//In this file is the Class responsible for managing the constants
//of the detector descriptions (dimensions, optical constants etc.)
//==============================================================

#ifndef AngraConstantMgr_h
#define AngraConstantMgr_h 1

#include <iostream>
#include <fstream>
#include <map>
#include "globals.hh"

#include "G4Threading.hh"
#include "G4AutoLock.hh"

class AngraConstantMgr
{
public:
  static AngraConstantMgr& Instance()
  {
    // Use a single instance for all threads since constants are read-only after initialization
    static AngraConstantMgr CMGRsingleton;
    static G4Mutex setupMutex = G4MUTEX_INITIALIZER;

    if(setup==false){
      G4AutoLock lock(&setupMutex);
      if(setup==false) {
        setup=CMGRsingleton.Init();
      }
    }
    return CMGRsingleton;
  }

  float GetValue(const G4String& key);

private:
  AngraConstantMgr() {}                                  // Private constructor
  ~AngraConstantMgr() {}
  AngraConstantMgr(const AngraConstantMgr&);                 // Prevent copy-construction
  AngraConstantMgr& operator=(const AngraConstantMgr&);      // Prevent assignment
  bool Init();

  static bool setup;
  std::ifstream *inFile;
  std::map<G4String,float> constants;
};

#endif
