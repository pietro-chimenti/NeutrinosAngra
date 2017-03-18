//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation interface to hep evt header file
//
//  Authors: P.Chimenti
//
//  10-3-2010 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
#ifndef AngraHEPEvtInterface_H
#define AngraHEPEvtInterface_H 1

#include <fstream>
#include <vector>
#include "globals.hh"
#include "G4VPrimaryGenerator.hh"


class G4PrimaryVertex;
class G4Event;

#define NMXHEP 2000

class AngraHEPEvtInterface:public G4VPrimaryGenerator
{
public: // with description

  AngraHEPEvtInterface(char* evfile);
  AngraHEPEvtInterface(G4String evfile);
  
public:

  ~AngraHEPEvtInterface();  
  void GeneratePrimaryVertex(G4Event* evt);
  
private:

  G4String fileName;
  std::ifstream inputFile;
  
  int nevhep, nhep, isthep[NMXHEP], idhep[NMXHEP], jmohep[2][NMXHEP], jdahep[2][NMXHEP];
  double phep[5][NMXHEP], vhep[4][NMXHEP];

};

#endif
