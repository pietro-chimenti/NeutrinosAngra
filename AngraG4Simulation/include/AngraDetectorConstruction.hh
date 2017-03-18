//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation detector construction header file
//
//  Authors: P.Chimenti, R.Lima
//
//  30-4-2008, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//==============================================================
//In this file are included the classes responsable by the 
//construction of the volumes, the materials and their properties.
//==============================================================

#ifndef AngraDetectorConstruction_H
#define AngraDetectorConstruction_H 1

//Logical and Physical volumes in the vectors
#include <vector>

//lets you choose a geometry 
#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4UserLimits.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
//class G4UserLimits;


enum geometryEnum { WATERBOX_1 = 1,  WATERBOX_2 = 2};

class AngraDetectorConstruction : public G4VUserDetectorConstruction
{
public:
  
  AngraDetectorConstruction();
  AngraDetectorConstruction(geometryEnum g);
  ~AngraDetectorConstruction();
  
  G4VPhysicalVolume* Construct();
  
private:
  
  
  geometryEnum myGeometry;
  
  // To build different geometries (used by Construct())
  G4VPhysicalVolume* ConstructWaterbox_1();
  G4VPhysicalVolume* ConstructWaterbox_2();
  
  void ConstructMaterials();
  
  G4UserLimits* trackingLimit;      

  
  // Logical volumes
  
  std::vector<G4LogicalVolume*> logicalVolumesVector;
  
  // Physical volumes
  
  std::vector<G4VPhysicalVolume*> physicalVolumesVector;
  
};

#endif

