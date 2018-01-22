//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation detector construction implementation file
//  Final Version of the Geometry
//
//  Authors: P.Chimenti, R.Lima, G. Valdiviesso
//
//  27-12-2011, v0.02 
//
//--------------------------------------------------------------
//--------------------------------------------------------------


//===================================================================================================================================
//This file constructs the geometries, defines each subsystem's materials and also the properties of each such materials.
//===================================================================================================================================

#include "AngraDetectorConstruction.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Ellipsoid.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"

#include "G4SDManager.hh"
#include "AngraPMTSD.hh" 
#include "AngraVetoSD.hh" 
#include "AngraConstantMgr.hh"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using namespace CLHEP;

G4VPhysicalVolume* AngraDetectorConstruction::ConstructWaterbox_2(){
  
  ConstructMaterials();
  G4NistManager* man= G4NistManager::Instance();
  
  // Get Geometry parameters from Constant Manager
  G4int check = AngraConstantMgr::Instance().GetValue("CheckForCollisions");
    
  // experimental hall (world volume)
  G4double expHall_x = AngraConstantMgr::Instance().GetValue("ExpHall_X")*mm;
  G4double expHall_y = AngraConstantMgr::Instance().GetValue("ExpHall_Y")*mm;
  G4double expHall_z = AngraConstantMgr::Instance().GetValue("ExpHall_Z")*mm;   

  // external shield
  G4double shield_X = AngraConstantMgr::Instance().GetValue("Shield_X")*mm;
  G4double shield_Y = AngraConstantMgr::Instance().GetValue("Shield_Y")*mm;
  G4double shield_Z = AngraConstantMgr::Instance().GetValue("Shield_Z")*mm;   
  G4double shield_T = AngraConstantMgr::Instance().GetValue("Shield_Struc_Thick")*mm;   

  // inner veto
  G4double iv_X  = AngraConstantMgr::Instance().GetValue("Inner_X")*mm;
  G4double iv_Y  = AngraConstantMgr::Instance().GetValue("Inner_Y")*mm;
  G4double iv_Z  = AngraConstantMgr::Instance().GetValue("Inner_Z")*mm;   
  G4double iv_T1 = AngraConstantMgr::Instance().GetValue("Inner_Struc_Thick1")*mm;   
  G4double iv_T2 = AngraConstantMgr::Instance().GetValue("Inner_Struc_Thick2")*mm;   
  G4double iv_OS = AngraConstantMgr::Instance().GetValue("Inner_Y_Offset")*mm;

  // central target
  G4double target_X = AngraConstantMgr::Instance().GetValue("Target_X")*mm;
  G4double target_Y = AngraConstantMgr::Instance().GetValue("Target_Y")*mm;
  G4double target_Z = AngraConstantMgr::Instance().GetValue("Target_Z")*mm;   
  G4double target_T = AngraConstantMgr::Instance().GetValue("Target_Struc_Thick")*mm;   
  
  // upper and bottom boxes
  G4double box_Y = AngraConstantMgr::Instance().GetValue("Box_Height")*mm;
  
  // pmt's
  int iUmax = (int) AngraConstantMgr::Instance().GetValue("N_pmt_U");  
  int iDmax = (int) AngraConstantMgr::Instance().GetValue("N_pmt_D");   
  G4double pmt_R = AngraConstantMgr::Instance().GetValue("Pmt_R")*mm;
  G4double pmt_H = AngraConstantMgr::Instance().GetValue("Pmt_H")*mm;
  G4double pmt_T = AngraConstantMgr::Instance().GetValue("Pmt_T")*mm;

  G4double pmtN_InnerR = AngraConstantMgr::Instance().GetValue("PmtN_Inner_R")*mm;
  G4double pmtN_OuterR = AngraConstantMgr::Instance().GetValue("PmtN_Outer_R")*mm;
  G4double pmtN_Height = AngraConstantMgr::Instance().GetValue("PmtN_Height")*mm;
  G4double pmtN_Offset = -(pmtN_Height+pmt_H*sqrt(1.-pow(pmtN_InnerR/pmt_R,2.)))*mm;
  
  int N_PMT_BINS    = (int) AngraConstantMgr::Instance().GetValue("N_Pmt_Bins");   
  int N_GORE_BINS   = (int) AngraConstantMgr::Instance().GetValue("N_Gore_Bins");   
  int N_PHOTON_BINS = (int) AngraConstantMgr::Instance().GetValue("N_Photon_Bins");   
    
  // pmt's quantum efficiency
  G4MaterialPropertyVector *PMTEfficiency = new G4MaterialPropertyVector();
  for(int i = 0; i<N_PMT_BINS; i++){
    ostringstream num;
    num << i;
    string numStr=num.str();
    string binName("OpPMT_");
    binName += numStr;
    G4double binEn = 1240./AngraConstantMgr::Instance().GetValue(binName)*eV; // reads bin wavelenght
    
    binName = "QuantumEff_";
    binName += numStr;
    G4double  binEff = AngraConstantMgr::Instance().GetValue(binName);   // reads bin efficiency
    PMTEfficiency->InsertValues(binEn,binEff/100.);
  }
  
  // Gore Table (from Gore Datasheet).
  G4MaterialPropertyVector *GoreReflectivity  = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *GoreEfficiency    = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *GoreSpecularLobe  = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *GoreBackscatter   = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *GoreLambertian    = new G4MaterialPropertyVector();
  for(int i = 0; i<N_GORE_BINS; i++){
    ostringstream num,lamb;
    num << i;
    string numStr=num.str();
    string binName("OpGore_");
    binName += numStr;
    G4double binEn = 1240./AngraConstantMgr::Instance().GetValue(binName)*eV;
    G4double binProp;
    
    binName = "ReflGore_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    GoreReflectivity->InsertValues(binEn,binProp);

    binName = "EffGore_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    GoreEfficiency->InsertValues(binEn,binProp);

    binName = "SLbGore_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    GoreSpecularLobe->InsertValues(binEn,binProp);

    binName = "BScGore_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    GoreBackscatter->InsertValues(binEn,binProp);

    binName = "LbtGore_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    GoreLambertian->InsertValues(binEn,binProp);
  }


  // rotation for each face
  G4RotationMatrix *rotBowlUp   = new G4RotationMatrix; // Rotates the Bowl so that it can be placed on the UP surface.
  G4RotationMatrix *rotBowlDown = new G4RotationMatrix; // Rotates the Bowl so that it can be placed on the DOWN surface.
  rotBowlUp  -> rotateX(-M_PI*rad/2.);
  rotBowlDown-> rotateX( M_PI*rad/2.);
  
  // rotation for each corner of the upper/bottom box
  G4RotationMatrix *rot1 = new G4RotationMatrix;
  G4RotationMatrix *rot2 = new G4RotationMatrix;
  G4RotationMatrix *rot3 = new G4RotationMatrix;
  G4RotationMatrix *rot4 = new G4RotationMatrix;
  G4double theta = atan(shield_Z/shield_X);
  
  vector<G4RotationMatrix*> rotVector;
  rotVector.push_back(rot1);
  rotVector.push_back(rot2);
  rotVector.push_back(rot3);
  rotVector.push_back(rot4);
  
  
  // Visualization attributes (doesn't change physics)
  G4VisAttributes *waterVisAttr = new G4VisAttributes();
  waterVisAttr -> SetColor(0.2,0.2,1.,.2); 
  G4VisAttributes *bowlVisAttr = new G4VisAttributes();
  bowlVisAttr -> SetColor(1.,0.5,0.,1.);   
  G4VisAttributes *domeVisAttr = new G4VisAttributes();
  domeVisAttr -> SetColor(0.4,0.4,0.4,1.); 
  
  // logical and physical poiters
  G4LogicalVolume   *experimentalHall_log;
  G4LogicalVolume   *shieldStruc_log;
  G4LogicalVolume   *shieldWater_log;
  G4LogicalVolume   *innerVetoStruc_log;
  G4LogicalVolume   *innerVetoWater_log;    
  G4LogicalVolume   *targetStruc_log;
  G4LogicalVolume   *targetWater_log;    
  G4LogicalVolume   *thinWallWide_log;
  G4LogicalVolume   *thinWallShort_log;
  G4LogicalVolume   *uBoxVetoStruc_log;
  G4LogicalVolume   *uBoxVetoWater_log;
  G4LogicalVolume   *bBoxVetoStruc_log;
  G4LogicalVolume   *bBoxVetoWater_log;
  G4VPhysicalVolume *experimentalHall_phys;
  G4VPhysicalVolume *shieldStruc_phys;
  G4VPhysicalVolume *shieldWater_phys;
  G4VPhysicalVolume *innerVetoStruc_phys;
  G4VPhysicalVolume *innerVetoWater_phys;
  G4VPhysicalVolume *targetStruc_phys;
  G4VPhysicalVolume *targetWater_phys;
  G4VPhysicalVolume *thinWallDF_phys;
  G4VPhysicalVolume *thinWallDL_phys;
  G4VPhysicalVolume *thinWallDR_phys;
  G4VPhysicalVolume *thinWallDB_phys;
  G4VPhysicalVolume *uBoxVetoStruc_phys;
  G4VPhysicalVolume *uBoxVetoWater_phys;
  G4VPhysicalVolume *bBoxVetoStruc_phys;
  G4VPhysicalVolume *bBoxVetoWater_phys;
  
  
  // Constructing the Geometry
  
  // Geometry::World Volume  
  G4Box *experimentalHall_box = new G4Box("expHall_box",expHall_x,expHall_y,expHall_z);
  experimentalHall_log        = new G4LogicalVolume(experimentalHall_box,man->FindOrBuildMaterial("G4_AIR"),"expHall_log",0,0,0);
  experimentalHall_phys       = new G4PVPlacement(0,G4ThreeVector(),experimentalHall_log,"expHall",0,false,0,check);  
  experimentalHall_log -> SetVisAttributes (G4VisAttributes::Invisible);

  // set limits

  trackingLimit = new G4UserLimits(
    AngraConstantMgr::Instance().GetValue("StepLimit")*mm,
    AngraConstantMgr::Instance().GetValue("TrackLimit")*mm,
    AngraConstantMgr::Instance().GetValue("TimeLimit")*ns,
    0.,0.);
   experimentalHall_log->SetUserLimits(trackingLimit);

  std::cout << "limits " << std::endl;
  std::cout << AngraConstantMgr::Instance().GetValue("StepLimit")*mm << std::endl;
  std::cout << AngraConstantMgr::Instance().GetValue("TrackLimit")*mm << std::endl;
  std::cout << AngraConstantMgr::Instance().GetValue("TimeLimit")*mm << std::endl;


  // Geometry::Structure and Water Volumes
  
  // 1. External Shield
  G4Box *shieldStruc_box = new G4Box("shieldStruc_box", shield_X+shield_T, shield_Y+shield_T, shield_Z+shield_T);
  shieldStruc_log  = new G4LogicalVolume(shieldStruc_box,man->FindOrBuildMaterial("PP"),"shieldStruc_log",0,0,0);  
  shieldStruc_phys = new G4PVPlacement (0,G4ThreeVector(0.,0.,0.),shieldStruc_log,"shieldStruc_phys",experimentalHall_log,false,0,check);
  
  G4Box *shieldWater_box = new G4Box("shieldWater_box", shield_X, shield_Y, shield_Z);
  shieldWater_log  = new G4LogicalVolume(shieldWater_box,man->FindOrBuildMaterial("G4_WATER"),"shieldWater_log",0,0,0);
  shieldWater_phys = new G4PVPlacement (0,G4ThreeVector(0.,0.,0.),shieldWater_log,"shieldWater_phys",shieldStruc_log,false,0,check);
  shieldWater_log -> SetVisAttributes(waterVisAttr);
  
  // 2. Inner Veto
  G4Box *innerVetoStruc_box = new G4Box("innerVetoStruc_box", iv_X+iv_T2, iv_Y+iv_T1, iv_Z+iv_T2);
  innerVetoStruc_log  = new G4LogicalVolume(innerVetoStruc_box,man->FindOrBuildMaterial("PP"),"innerVetoStruc_log",0,0,0);  
  innerVetoStruc_phys = new G4PVPlacement (0,G4ThreeVector(0.,iv_OS,0.),innerVetoStruc_log,"innerVetoStruc_phys",shieldWater_log,false,0,check);  
  
  G4Box *innerVetoWater_box = new G4Box("innerVetoWater_box", iv_X, iv_Y, iv_Z);
  innerVetoWater_log  = new G4LogicalVolume(innerVetoWater_box,man->FindOrBuildMaterial("GdW"),"innerVetoWater_log",0,0,0);  
  innerVetoWater_phys = new G4PVPlacement (0,G4ThreeVector(0.,0.,0.),innerVetoWater_log,"innerVetoWater_phys",innerVetoStruc_log,false,0,check);  
  innerVetoWater_log -> SetVisAttributes(waterVisAttr);

  // 3. Central Target
  G4Box *targetStruc_box = new G4Box("targetStruc_box", target_X+target_T, target_Y+target_T, target_Z+target_T);
  targetStruc_log  = new G4LogicalVolume(targetStruc_box,man->FindOrBuildMaterial("PP"),"targetStruc_log",0,0,0);  
  targetStruc_phys = new G4PVPlacement (0,G4ThreeVector(0.,0.,0.),targetStruc_log,"targetStruc_phys",innerVetoWater_log,false,0,check);

  G4Box *targetWater_box = new G4Box("targetWater_box", target_X, target_Y, target_Z);
  targetWater_log  = new G4LogicalVolume(targetWater_box,man->FindOrBuildMaterial("GdW"),"targetWater_log",0,0,0);  
  targetWater_phys = new G4PVPlacement (0,G4ThreeVector(0.,0.,0.),targetWater_log,"targetWater_phys",targetStruc_log,false,0,check);
  targetWater_log -> SetVisAttributes(waterVisAttr);
  
  // 4. Support walls under the target
  G4double wallHeight = (iv_Y-target_Y-target_T)/2.;
  G4double wall_Y_OS = -(target_Y+target_T+wallHeight);

  G4Box *thinWallWide_box = new G4Box("thinWallWide_box", target_X+target_T, wallHeight, iv_T2); 
  thinWallWide_log = new G4LogicalVolume(thinWallWide_box,man->FindOrBuildMaterial("PP"),"thinWallDF_log",0,0,0);
  thinWallDF_phys  = new G4PVPlacement (0,G4ThreeVector(0.,wall_Y_OS,  target_Y-target_T-iv_T2/2.), thinWallWide_log,"thinWallDF_phys",innerVetoWater_log,false,0,check);
  thinWallDB_phys  = new G4PVPlacement (0,G4ThreeVector(0.,wall_Y_OS,-(target_Y-target_T-iv_T2/2.)),thinWallWide_log,"thinWallDB_phys",innerVetoWater_log,false,0,check);
  
  G4Box *thinWallShort_box = new G4Box("thinWallShort_box", iv_T2, wallHeight, target_Z/2.); 
  thinWallShort_log = new G4LogicalVolume(thinWallShort_box,man->FindOrBuildMaterial("PP"),"thinWallDR_log",0,0,0);
  thinWallDR_phys   = new G4PVPlacement (0,G4ThreeVector(  target_X+target_T, wall_Y_OS,0.),thinWallShort_log,"thinWallDR_phys",innerVetoWater_log,false,0,check);
  thinWallDL_phys   = new G4PVPlacement (0,G4ThreeVector(-(target_X+target_T),wall_Y_OS,0.),thinWallShort_log,"thinWallDL_phys",innerVetoWater_log,false,0,check);
   
  // 5. Upper Box Veto
  G4double box_Y_pos = shield_Y+2.*shield_T+box_Y;
  G4Box *uBoxVetoStruc_box = new G4Box("uBoxVetoStruc_box", shield_X+shield_T, box_Y+shield_T, shield_Z+shield_T);
  uBoxVetoStruc_log  = new G4LogicalVolume(uBoxVetoStruc_box,man->FindOrBuildMaterial("PP"),"uBoxVetoStruc_log",0,0,0);  
  uBoxVetoStruc_phys = new G4PVPlacement (0,G4ThreeVector(0.,box_Y_pos,0.),uBoxVetoStruc_log,"uBoxVetoStruc_phys",experimentalHall_log,false,0,check);

  G4Box *uBoxVetoWater_box = new G4Box("uBoxVetoWater_box", shield_X, box_Y, shield_Z);
  uBoxVetoWater_log  = new G4LogicalVolume(uBoxVetoWater_box,man->FindOrBuildMaterial("G4_WATER"),"uBoxVetoWater_log",0,0,0);  
  uBoxVetoWater_phys = new G4PVPlacement (0,G4ThreeVector(0.,0.,0.),uBoxVetoWater_log,"uBoxVetoWater_phys",uBoxVetoStruc_log,false,0,check);
  uBoxVetoWater_log -> SetVisAttributes(waterVisAttr);

  // 6. Bottom Box Veto
  G4Box *bBoxVetoStruc_box = new G4Box("bBoxVetoStruc_box", shield_X+shield_T, box_Y+shield_T, shield_Z+shield_T);
  bBoxVetoStruc_log  = new G4LogicalVolume(bBoxVetoStruc_box,man->FindOrBuildMaterial("PP"),"dBoxVetoStruc_log",0,0,0);  
  bBoxVetoStruc_phys = new G4PVPlacement (0,G4ThreeVector(0.,-box_Y_pos,0.),bBoxVetoStruc_log,"dBoxVetoStruc_phys",experimentalHall_log,false,0,check);

  G4Box *bBoxVetoWater_box = new G4Box("bBoxVetoWater_box", shield_X, box_Y, shield_Z);
  bBoxVetoWater_log  = new G4LogicalVolume(bBoxVetoWater_box,man->FindOrBuildMaterial("G4_WATER"),"dBoxVetoWater_log",0,0,0);  
  bBoxVetoWater_phys = new G4PVPlacement (0,G4ThreeVector(0.,0.,0.),bBoxVetoWater_log,"dBoxVetoWater_phys",bBoxVetoStruc_log,false,0,check);
  bBoxVetoWater_log -> SetVisAttributes(waterVisAttr);

  // 7. PMT's Bowl - Upper glass shell
  G4Ellipsoid *pmtGlass  = new G4Ellipsoid ("pmtGlass" ,pmt_R, pmt_R, pmt_H, -0.1*mm, pmt_H);
  G4Ellipsoid *pmtVacuum = new G4Ellipsoid ("pmtVacuum",pmt_R-pmt_T, pmt_R-pmt_T,pmt_H-pmt_T,-0.2*mm,pmt_H-pmt_T);
  G4SubtractionSolid * pmtBowl = new G4SubtractionSolid("pmtBowl",pmtGlass,pmtVacuum);
  G4LogicalVolume* pmtBowl_log = new G4LogicalVolume(pmtBowl,man->FindOrBuildMaterial("G4_GLASS_PLATE"),"pmtBowl_log",0,0,0);
  pmtBowl_log -> SetVisAttributes(bowlVisAttr);
  
  // 8. PMT's Dome and Neck - Bottom support
  G4Ellipsoid *pmtDome  = new G4Ellipsoid ("pmtDome" ,pmt_R, pmt_R, pmt_H, -pmt_H,0.1*mm); 
  G4Tubs* pmtDomeNeck   = new G4Tubs("pmtDomeNeck", pmtN_InnerR, pmtN_OuterR, pmtN_Height,0*deg,360*deg);
  G4UnionSolid* pmtBody = new G4UnionSolid("pmtDome",pmtDome, pmtDomeNeck,0,G4ThreeVector(0,0,pmtN_Offset));
  G4LogicalVolume* pmtDome_log = new G4LogicalVolume(pmtBody,man->FindOrBuildMaterial("PP"),"pmtDome_log",0,0,0);
  pmtDome_log -> SetVisAttributes(domeVisAttr);

  // Surface Water-Target::Gore

  // General Surface Properties (need calibration?)
  // In the UNIFIED model, there are 6 parameters for Surfaces:
  // Reflectivity
  // Efficiency
  // SpecularLobe
  // SpecularSpike
  // Backscatter
  // Lambertian
  // The sum of the last 4 has to be 1.00. So it is only needed to define three of them.
  G4MaterialPropertyVector *PMTReflectivity   = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *PMTSpecularLobe   = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *PMTSpecularSpike  = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *PMTBackscatter    = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *TyvekReflectivity = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *TyvekEfficiency   = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *TyvekSpecularLobe = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *TyvekBackscatter  = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *TyvekLambertian   = new G4MaterialPropertyVector();
    
  for(int i = 0; i<N_PHOTON_BINS; i++){
    ostringstream num;
    num << i;
    string numStr=num.str();
    string binName("PEnergy_");
    binName += numStr;    
    G4double binEnergy = AngraConstantMgr::Instance().GetValue(binName);
    G4double binProp;
    
    binName = "PMTSurface_Reflectivity_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    PMTReflectivity -> InsertValues(binEnergy*eV,binProp);

    binName = "PMTSurface_SpecularLobe_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    PMTSpecularLobe -> InsertValues(binEnergy*eV,binProp);
    
    binName = "PMTSurface_SpecularSpike_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    PMTSpecularSpike -> InsertValues(binEnergy*eV,binProp);

    binName = "PMTSurface_Backscatter_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    PMTBackscatter -> InsertValues(binEnergy*eV,binProp);

    binName = "TyvekSurface_Reflectivity_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    TyvekReflectivity -> InsertValues(binEnergy*eV,binProp);

    binName = "TyvekSurface_Efficiency_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    TyvekEfficiency -> InsertValues(binEnergy*eV,binProp);

    binName = "TyvekSurface_SpecularLobe_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    TyvekSpecularLobe -> InsertValues(binEnergy*eV,binProp);

    binName = "TyvekSurface_Backscatter_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    TyvekBackscatter -> InsertValues(binEnergy*eV,binProp);

    binName = "TyvekSurface_Lambertian_";
    binName += num.str();    
    binProp = AngraConstantMgr::Instance().GetValue(binName);
    TyvekLambertian -> InsertValues(binEnergy*eV,binProp);
  }

  G4OpticalSurface *OpGore = new G4OpticalSurface("GoreSurface");
  OpGore -> SetType(dielectric_metal);
  OpGore -> SetFinish(ground);
  OpGore -> SetModel(unified);
  OpGore -> SetSigmaAlpha(M_PI/2.);

  G4MaterialPropertiesTable *OpGoreSurfaceProperty = new G4MaterialPropertiesTable();
  
  // The SpecularSpike will be complementary to the Lambertian Reflection
  //OpGoreSurfaceProperty -> AddProperty("RINDEX",WaterRefractive);
  OpGoreSurfaceProperty -> AddProperty("REFLECTIVITY",GoreReflectivity);
  OpGoreSurfaceProperty -> AddProperty("EFFICIENCY",  GoreEfficiency);
  OpGoreSurfaceProperty -> AddProperty("SPECULARLOBECONSTANT", GoreSpecularLobe);
  OpGoreSurfaceProperty -> AddProperty("BACKSCATTERCONSTANT",  GoreBackscatter);
  OpGoreSurfaceProperty -> AddProperty("LAMBERTIANCONSTANT",   GoreLambertian);
   
  OpGore -> SetMaterialPropertiesTable(OpGoreSurfaceProperty);
   
  // Surface Water-Glass::PMT
  G4OpticalSurface *OpPMTSurface = new G4OpticalSurface("PMTSurface");

  OpPMTSurface->SetType(dielectric_metal);
  OpPMTSurface->SetFinish(polished);
  OpPMTSurface->SetModel(unified);
  
  G4MaterialPropertiesTable *OpPMTSurfaceProperty = new G4MaterialPropertiesTable();

  //OpPMTSurfaceProperty -> AddProperty("RINDEX",WaterRefractive);
  OpPMTSurfaceProperty -> AddProperty("EFFICIENCY",PMTEfficiency);
  OpPMTSurfaceProperty -> AddProperty("REFLECTIVITY",PMTReflectivity);
  OpPMTSurfaceProperty -> AddProperty("SPECULARLOBECONSTANT",PMTSpecularLobe);
  OpPMTSurfaceProperty -> AddProperty("SPECULARSPIKECONSTANT",PMTSpecularSpike);
  OpPMTSurfaceProperty -> AddProperty("BACKSCATTERCONSTANT",PMTBackscatter);
  
  OpPMTSurface -> SetMaterialPropertiesTable(OpPMTSurfaceProperty);
  
  // Surface Water-PP::Tyvek
  G4OpticalSurface *OpTyvek = new G4OpticalSurface("Tyvek");

  OpTyvek -> SetType(dielectric_metal);
  OpTyvek -> SetFinish(ground);
  OpTyvek -> SetModel(unified);
  OpTyvek -> SetSigmaAlpha(M_PI/2.);

  G4MaterialPropertiesTable *OpTyvekSurfaceProperty = new G4MaterialPropertiesTable();
  
   // The SpecularSpike will be complementary to the Lambertian Reflection
   //OpTyvekSurfaceProperty -> AddProperty("RINDEX",WaterRefractive);
   OpTyvekSurfaceProperty -> AddProperty("REFLECTIVITY",        TyvekReflectivity);
   OpTyvekSurfaceProperty -> AddProperty("EFFICIENCY",          TyvekEfficiency);
   OpTyvekSurfaceProperty -> AddProperty("SPECULARLOBECONSTANT",TyvekSpecularLobe);
   OpTyvekSurfaceProperty -> AddProperty("BACKSCATTERCONSTANT", TyvekBackscatter);
   OpTyvekSurfaceProperty -> AddProperty("LAMBERTIANCONSTANT",  TyvekLambertian);
   
   OpTyvek -> SetMaterialPropertiesTable(OpTyvekSurfaceProperty);
  
  // Applying Border Surfaces to logial volumes.
  G4LogicalBorderSurface *targetGore   = new G4LogicalBorderSurface("TargetGore",targetWater_phys,targetStruc_phys,OpGore);
  G4LogicalBorderSurface *shieldTyvek1 = new G4LogicalBorderSurface("shieldTyvek1",shieldWater_phys,shieldStruc_phys,OpTyvek);
  G4LogicalBorderSurface *shieldTyvek2 = new G4LogicalBorderSurface("shieldTyvek2",shieldWater_phys,innerVetoStruc_phys,OpTyvek);
  G4LogicalBorderSurface *innerVTyvek1 = new G4LogicalBorderSurface("innerVTyvek1",innerVetoWater_phys,innerVetoStruc_phys,OpTyvek);
  G4LogicalBorderSurface *innerVTyvek2 = new G4LogicalBorderSurface("innerVTyvek2",innerVetoWater_phys,targetStruc_phys,OpTyvek);  
  G4LogicalBorderSurface *uBoxVTyvek = new G4LogicalBorderSurface("uBoxVTyvek",uBoxVetoWater_phys,uBoxVetoStruc_phys,OpTyvek);
  G4LogicalBorderSurface *bBoxVTyvek = new G4LogicalBorderSurface("bBoxVTyvek",bBoxVetoWater_phys,bBoxVetoStruc_phys,OpTyvek);

  G4LogicalBorderSurface *thinWallDFTyvek = new G4LogicalBorderSurface("thinWallDFTyvek",innerVetoWater_phys,thinWallDF_phys,OpTyvek);
  G4LogicalBorderSurface *thinWallDBTyvek = new G4LogicalBorderSurface("thinWallDBTyvek",innerVetoWater_phys,thinWallDB_phys,OpTyvek);
  G4LogicalBorderSurface *thinWallDLTyvek = new G4LogicalBorderSurface("thinWallDLTyvek",innerVetoWater_phys,thinWallDL_phys,OpTyvek);
  G4LogicalBorderSurface *thinWallDRTyvek = new G4LogicalBorderSurface("thinWallDRTyvek",innerVetoWater_phys,thinWallDR_phys,OpTyvek);

  
  // Storing logical and physical structure
  logicalVolumesVector.push_back(experimentalHall_log);
  logicalVolumesVector.push_back(shieldStruc_log);
  logicalVolumesVector.push_back(shieldWater_log);
  logicalVolumesVector.push_back(innerVetoStruc_log);
  logicalVolumesVector.push_back(innerVetoWater_log);
  logicalVolumesVector.push_back(targetStruc_log);
  logicalVolumesVector.push_back(targetWater_log);
  logicalVolumesVector.push_back(uBoxVetoStruc_log);
  logicalVolumesVector.push_back(uBoxVetoWater_log);
  logicalVolumesVector.push_back(bBoxVetoStruc_log);
  logicalVolumesVector.push_back(bBoxVetoWater_log);
  logicalVolumesVector.push_back(pmtBowl_log);
  logicalVolumesVector.push_back(pmtDome_log);
  logicalVolumesVector.push_back(thinWallWide_log);
  logicalVolumesVector.push_back(thinWallShort_log);
  
  physicalVolumesVector.push_back(experimentalHall_phys);
  physicalVolumesVector.push_back(shieldStruc_phys);
  physicalVolumesVector.push_back(shieldWater_phys);
  physicalVolumesVector.push_back(innerVetoStruc_phys);
  physicalVolumesVector.push_back(innerVetoWater_phys);
  physicalVolumesVector.push_back(targetStruc_phys);
  physicalVolumesVector.push_back(targetWater_phys);
  physicalVolumesVector.push_back(uBoxVetoStruc_phys);
  physicalVolumesVector.push_back(uBoxVetoWater_phys);
  physicalVolumesVector.push_back(bBoxVetoStruc_phys);
  physicalVolumesVector.push_back(bBoxVetoWater_phys);
  physicalVolumesVector.push_back(thinWallDF_phys);
  physicalVolumesVector.push_back(thinWallDB_phys);
  physicalVolumesVector.push_back(thinWallDL_phys);
  physicalVolumesVector.push_back(thinWallDR_phys);
  // Physical PMTs will be stored douring loops 9 to 12.
  
  vector<G4LogicalBorderSurface*> logialBorderSurface;
  logialBorderSurface.push_back(targetGore);
  logialBorderSurface.push_back(shieldTyvek1);
  logialBorderSurface.push_back(shieldTyvek2);
  logialBorderSurface.push_back(innerVTyvek1);
  logialBorderSurface.push_back(innerVTyvek2);
  logialBorderSurface.push_back(uBoxVTyvek);
  logialBorderSurface.push_back(bBoxVTyvek);
  logialBorderSurface.push_back(thinWallDFTyvek);
  logialBorderSurface.push_back(thinWallDBTyvek);
  logialBorderSurface.push_back(thinWallDLTyvek);
  logialBorderSurface.push_back(thinWallDRTyvek);
  
  // 9. Placing the UPPER PMT's inside the target.
  if (iUmax!=0)
  for(int i = 1; i<=iUmax; i++){
    std::ostringstream oss;
    oss << i;
    std::string numstr=oss.str();

    std::string coord_x_name("BowlX_U_");
    std::string coord_z_name("BowlZ_U_");
    coord_x_name+=numstr;
    coord_z_name+=numstr;

    G4double xpos = AngraConstantMgr::Instance().GetValue(coord_x_name.c_str())*mm;
    G4double zpos = AngraConstantMgr::Instance().GetValue(coord_z_name.c_str())*mm;
    G4double yposB = target_Y-0.1*mm;  // center the PMT at the surface of the target.
    G4double yposD = target_Y+target_T+0.1*mm;  // center the PMT at the surface of the target.
 
    std::string name("pmtTarget_U");
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtBowl_phys  = new G4PVPlacement(rotBowlUp,G4ThreeVector(xpos,yposB,zpos),pmtBowl_log,name.c_str(),targetWater_log,false,0,check);

    name = "pmtDome_U";
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtDome_phys = new G4PVPlacement(rotBowlUp,G4ThreeVector(xpos,yposD,zpos),pmtDome_log,name.c_str(),innerVetoWater_log,false,0,check);
    
    physicalVolumesVector.push_back(pmtBowl_phys);
    physicalVolumesVector.push_back(pmtDome_phys);

    std::string surfaceName("PMTsurface_U");
    surfaceName+=numstr;
    G4LogicalBorderSurface* PMTSurface;
    PMTSurface = new G4LogicalBorderSurface(surfaceName.c_str(), targetWater_phys,pmtBowl_phys,OpPMTSurface);    
  }
  
  // 10. Placing the BOTTOM PMT's inside thetarget.
  if (iDmax!=0)
  for(int i = 1; i<=iDmax; i++){
    std::ostringstream oss;
    oss << i;
    std::string numstr=oss.str();

    std::string coord_x_name("BowlX_D_");
    std::string coord_z_name("BowlZ_D_");
    coord_x_name+=numstr;
    coord_z_name+=numstr;

    G4double xpos = AngraConstantMgr::Instance().GetValue(coord_x_name.c_str())*mm;
    G4double zpos = AngraConstantMgr::Instance().GetValue(coord_z_name.c_str())*mm;
    G4double yposB = -target_Y+0.1*mm;  // center the PMT at the surface of the target.
    G4double yposD = -target_Y-target_T-0.1*mm;  // center the PMT at the surface of the target.
    std::string name("pmtTarget_D");
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtBowl_phys  = new G4PVPlacement(rotBowlDown,G4ThreeVector(xpos,yposB,zpos),pmtBowl_log,name.c_str(),targetWater_log,false,0,check);

    name = "pmtDome_D";
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtDome_phys = new G4PVPlacement(rotBowlDown,G4ThreeVector(xpos,yposD,zpos),pmtDome_log,name.c_str(),innerVetoWater_log,false,0,check);

    physicalVolumesVector.push_back(pmtBowl_phys);
    physicalVolumesVector.push_back(pmtDome_phys);

    std::string surfaceName("PMTsurface_D");
    surfaceName+=numstr;
    G4LogicalBorderSurface* PMTSurface;
    PMTSurface = new G4LogicalBorderSurface(surfaceName.c_str(), targetWater_phys,pmtBowl_phys,OpPMTSurface);
    
  }
  
  
  
  // 11. Placing the UPPER PMT's inside the inner veto. 
  for(int i = 1; i<=4; i++){
    std::ostringstream oss;
    oss << i;
    std::string numstr=oss.str();

    std::string coord_x_name("BowlInVetoX_U_");
    std::string coord_z_name("BowlInVetoZ_U_");
    coord_x_name+=numstr;
    coord_z_name+=numstr;

    G4double xpos=AngraConstantMgr::Instance().GetValue(coord_x_name.c_str())*mm;    
    G4double zpos=AngraConstantMgr::Instance().GetValue(coord_z_name.c_str())*mm;
    G4double yposD= iv_Y + pmtN_Offset - pmtN_Height-0.2*mm;
    G4double yposB= iv_Y + pmtN_Offset - pmtN_Height;
     
    std::string name("pmtInnerVeto_U");
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtBowl_phys  = new G4PVPlacement(rotBowlUp,G4ThreeVector(xpos,yposD,zpos),pmtBowl_log,name.c_str(),innerVetoWater_log,false,0,check);

    name = "pmtInnerVetoDome_U";
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtDome_phys  = new G4PVPlacement(rotBowlUp,G4ThreeVector(xpos,yposB,zpos),pmtDome_log,name.c_str(),innerVetoWater_log,false,0,check);

    physicalVolumesVector.push_back(pmtDome_phys);
    physicalVolumesVector.push_back(pmtBowl_phys);

    std::string surfaceName("ivPMTsurface_U");
    surfaceName+=numstr;
    G4LogicalBorderSurface* PMTSurface;
    PMTSurface = new G4LogicalBorderSurface(surfaceName.c_str(), innerVetoWater_phys,pmtBowl_phys,OpPMTSurface);
    
  }

  // 12. Placing the BOTTOM PMT's inside the inner veto.
  for(int i = 1; i<=4; i++){
    std::ostringstream oss;
    oss << i;
    std::string numstr=oss.str();

    std::string coord_x_name("BowlInVetoX_D_");
    std::string coord_z_name("BowlInVetoZ_D_");
    coord_x_name+=numstr;
    coord_z_name+=numstr;

    G4double xpos=AngraConstantMgr::Instance().GetValue(coord_x_name.c_str())*mm;    
    G4double zpos=AngraConstantMgr::Instance().GetValue(coord_z_name.c_str())*mm;
    G4double yposD= -iv_Y - pmtN_Offset + pmtN_Height+0.2*mm;
    G4double yposB= -iv_Y - pmtN_Offset + pmtN_Height;
 
    std::string name("pmtInnerVeto_D");
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtBowl_phys  = new G4PVPlacement(rotBowlDown,G4ThreeVector(xpos,yposD,zpos),pmtBowl_log,name.c_str(),innerVetoWater_log,false,0,check);

    name = "pmtInnerVetoDome_D";
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtDome_phys  = new G4PVPlacement(rotBowlDown,G4ThreeVector(xpos,yposB,zpos),pmtDome_log,name.c_str(),innerVetoWater_log,false,0,check);

    physicalVolumesVector.push_back(pmtDome_phys);
    physicalVolumesVector.push_back(pmtBowl_phys);

    std::string surfaceName("ivPMTsurface_D");
    surfaceName+=numstr;
    G4LogicalBorderSurface* PMTSurface;
    PMTSurface = new G4LogicalBorderSurface(surfaceName.c_str(), innerVetoWater_phys,pmtBowl_phys,OpPMTSurface);
    
  }


  // 13. Placing the PMTs inside the UPPER box veto.
  for(int i = 1; i<=4; i++){
    std::ostringstream oss;
    oss << i;
    std::string numstr=oss.str();

    std::string pmt_a_name("BowlBoxVetoAngle_");
    pmt_a_name+=numstr;    
    G4double pmtBoxAngle = AngraConstantMgr::Instance().GetValue(pmt_a_name)*deg;

    G4double rposD = -pmtN_Offset+2.*pmtN_Height;
    G4double rposB = rposD+0.2*mm;
    G4double dpos = pmt_R+0.1*mm;
    
    G4double thetaY;
    G4double xposB = shield_X - dpos;
    G4double zposB = shield_Z - dpos;;
    G4double xposD = xposB;
    G4double zposD = zposB;

	switch(i){
      case 1:
	thetaY = theta - pmtBoxAngle;
	xposB = -xposB + rposB*cos(thetaY);	  
	zposB = -zposB + rposB*sin(thetaY);	  
	xposD = -xposD + rposD*cos(thetaY);	  
	zposD = -zposD + rposD*sin(thetaY);	  
	break;
      case 2:
	thetaY = M_PI - theta - pmtBoxAngle;
	xposB =  xposB + rposB*cos(thetaY);	  
	zposB = -zposB + rposB*sin(thetaY);	  
	xposD =  xposD + rposD*cos(thetaY);	  
	zposD = -zposD + rposD*sin(thetaY);	  
	break;
      case 3:
	thetaY = M_PI + theta - pmtBoxAngle;
	xposB =  xposB + rposB*cos(thetaY);	  
	zposB =  zposB + rposB*sin(thetaY);	  
	xposD =  xposD + rposD*cos(thetaY);	  
	zposD =  zposD + rposD*sin(thetaY);	  
	break;
      case 4:
	thetaY = 2.*M_PI -theta + pmtBoxAngle;
	xposB = -xposB + rposB*cos(thetaY);	  
	zposB =  zposB + rposB*sin(thetaY);	  
	xposD = -xposD + rposD*cos(thetaY);	  
	zposD =  zposD + rposD*sin(thetaY);	  
	break;
    }
    
    rotVector[i-1]->rotateY((-M_PI/2.+thetaY)*rad);
    
    std::string name("pmtBoxVeto_U");
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtBowl_phys  = new G4PVPlacement(rotVector[i-1],G4ThreeVector(xposB,0.,zposB),pmtBowl_log,name.c_str(),uBoxVetoWater_log,false,0,check);

    name = "pmtBoxVetoDome_U";
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtDome_phys  = new G4PVPlacement(rotVector[i-1],G4ThreeVector(xposD,0.,zposD),pmtDome_log,name.c_str(),uBoxVetoWater_log,false,0,check);

    physicalVolumesVector.push_back(pmtDome_phys);
    physicalVolumesVector.push_back(pmtBowl_phys);

    std::string surfaceName("BoxVetoPMTsurface_U");
    surfaceName+=numstr;
    G4LogicalBorderSurface* PMTSurface;
    PMTSurface = new G4LogicalBorderSurface(surfaceName.c_str(), uBoxVetoWater_phys,pmtBowl_phys,OpPMTSurface);
    
  }

  // 14. Placing the PMTs inside the BOTTOM box veto.
  for(int i = 1; i<=4; i++){
    std::ostringstream oss;
    oss << i;
    std::string numstr=oss.str();

    std::string pmt_a_name("BowlBoxVetoA_");
    pmt_a_name+=numstr;    
    G4double pmtBoxAngle = AngraConstantMgr::Instance().GetValue(pmt_a_name)*deg;

    G4double rposD = -pmtN_Offset+2.*pmtN_Height;
    G4double rposB = rposD+0.2*mm;
    G4double dpos = pmt_R+0.1*mm;
    
    G4double thetaY;
    G4double xposB = shield_X - dpos;
    G4double zposB = shield_Z - dpos;;
    G4double xposD = xposB;
    G4double zposD = zposB;

	switch(i){
      case 1:
	thetaY = theta - pmtBoxAngle;
	xposB = -xposB + rposB*cos(thetaY);	  
	zposB = -zposB + rposB*sin(thetaY);	  
	xposD = -xposD + rposD*cos(thetaY);	  
	zposD = -zposD + rposD*sin(thetaY);	  
	break;
      case 2:
	thetaY = M_PI - theta - pmtBoxAngle;
	xposB =  xposB + rposB*cos(thetaY);	  
	zposB = -zposB + rposB*sin(thetaY);	  
	xposD =  xposD + rposD*cos(thetaY);	  
	zposD = -zposD + rposD*sin(thetaY);	  
	break;
      case 3:
	thetaY = M_PI + theta - pmtBoxAngle;
	xposB =  xposB + rposB*cos(thetaY);	  
	zposB =  zposB + rposB*sin(thetaY);	  
	xposD =  xposD + rposD*cos(thetaY);	  
	zposD =  zposD + rposD*sin(thetaY);	  
	break;
      case 4:
	thetaY = 2.*M_PI -theta + pmtBoxAngle;
	xposB = -xposB + rposB*cos(thetaY);	  
	zposB =  zposB + rposB*sin(thetaY);	  
	xposD = -xposD + rposD*cos(thetaY);	  
	zposD =  zposD + rposD*sin(thetaY);	  
	break;
    }
    
    std::string name("pmtBoxVeto_D");
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtBowl_phys  = new G4PVPlacement(rotVector[i-1],G4ThreeVector(xposB,0.,zposB),pmtBowl_log,name.c_str(),bBoxVetoWater_log,false,0,check);

    name = "pmtBoxVetoDome_D";
    name+=numstr;
    name+="_phys";
    G4VPhysicalVolume *pmtDome_phys  = new G4PVPlacement(rotVector[i-1],G4ThreeVector(xposD,0.,zposD),pmtDome_log,name.c_str(),bBoxVetoWater_log,false,0,check);

    physicalVolumesVector.push_back(pmtDome_phys);
    physicalVolumesVector.push_back(pmtBowl_phys);

    std::string surfaceName("BoxVetoPMTsurface_D");
    surfaceName+=numstr;
    G4LogicalBorderSurface* PMTSurface;
    PMTSurface = new G4LogicalBorderSurface(surfaceName.c_str(), bBoxVetoWater_phys,pmtBowl_phys,OpPMTSurface);
    
  }
  
  // Setting PMTBowl as Sensitive Detectors
    G4SDManager* SDman = G4SDManager::GetSDMpointer();

    AngraPMTSD* pmt_SD = new AngraPMTSD("/pmtSD");
    SDman->AddNewDetector(pmt_SD);
    pmtBowl_log->SetSensitiveDetector(pmt_SD);
  
    AngraVetoSD* veto_SD = new AngraVetoSD("/vetoSD");
    SDman->AddNewDetector(veto_SD);


  return experimentalHall_phys;
}


