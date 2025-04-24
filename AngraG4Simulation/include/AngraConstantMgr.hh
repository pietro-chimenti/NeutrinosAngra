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

class AngraConstantMgr
{
public:
  static AngraConstantMgr& Instance()
  {
    static AngraConstantMgr CMGRsingleton;
    if(setup==false){
      setup=CMGRsingleton.Init();
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
