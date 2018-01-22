//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation detector construction implementation file
//
//  Authors: P.Chimenti, R.Lima, G. Valdiviesso
//
//  05-01-2012, v0.02
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//===================================================================================================================================
//This file constructs the geometries, defines each subsystem's materials and also the properties of each of such materials.
//===================================================================================================================================

#include "AngraDetectorConstruction.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Polycone.hh"
#include "G4LogicalVolume.hh"
#include "G4UnionSolid.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4OpticalSurface.hh"
#include "G4MaterialPropertyVector.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4UnitsTable.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "AngraPMTSD.hh" 
#include "AngraConstantMgr.hh"

#define PHOTON_BINS 18
#define NM2EV 1240.

#include <string>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;
using namespace CLHEP;

//this is constructor lets you choose 
AngraDetectorConstruction::AngraDetectorConstruction():trackingLimit(0){;}
   
AngraDetectorConstruction::AngraDetectorConstruction(geometryEnum g){
myGeometry=g;
}

AngraDetectorConstruction::~AngraDetectorConstruction()
{
  if(trackingLimit) delete trackingLimit;
}

G4VPhysicalVolume* AngraDetectorConstruction::Construct()
{

  G4VPhysicalVolume* rootVolume = NULL;
 
  switch(myGeometry)
    {
    case WATERBOX_1 : rootVolume=ConstructWaterbox_1();
      break;     
    case WATERBOX_2 : rootVolume=ConstructWaterbox_2();
      break;     
    default : printf("ERROR Detector Construction: No geometry constructed \n");      
    }   
  return rootVolume;
}

//==========================================================================================
// construct the materials
//==========================================================================================
void AngraDetectorConstruction::ConstructMaterials()
{
  // material manager

  G4NistManager* man= G4NistManager::Instance();


  // basic declarations
  G4double density; 
  G4String name;     
  G4String symbol;  
  G4int ncomponents;   //numero de componentes numa molécula
  G4int natoms;     //numero de átomos de um determinado elemento num composto
  G4double fractionmass;

  //	------------- Materials -------------

  // Elements
  G4Element* O  = man->FindOrBuildElement("O");
  G4Element* H  = man->FindOrBuildElement("H");
  G4Element* C  = man->FindOrBuildElement("C");
  G4Element* B  = man->FindOrBuildElement("B");
  G4Element* Gd = man->FindOrBuildElement("Gd");

  // ========  Materials =======================

  // Basic Materials
  
  //Air
  G4Material* Air   = man->FindOrBuildMaterial("G4_AIR");
  //Water
  G4Material* H2O   = man->FindOrBuildMaterial("G4_WATER");
  
  // Composite Materials

 // Gadolinium Water
  G4double density_water = AngraConstantMgr::Instance().GetValue("Density_Water")*g/cm3;
  G4double frac_gd_gdw   = AngraConstantMgr::Instance().GetValue("Frac_Gd_GDW")*perCent;
  G4double frac_h2o_gdw  = 100.*perCent - frac_gd_gdw;
  G4Material* GDW = new G4Material(name="GdW",density=density_water,ncomponents=2);
  GDW->AddMaterial(H2O,fractionmass = frac_h2o_gdw);
  GDW->AddElement (Gd, fractionmass = frac_gd_gdw );
  
  // Polypropylene
  G4double density_pp = AngraConstantMgr::Instance().GetValue("Density_PP")*g/cm3;
  G4Material* PP = new G4Material(name="PP",density= density_pp, ncomponents=2);
  PP->AddElement(C,natoms=3);
  PP->AddElement(H,natoms=6);

  // Acrilic
  G4double density_acrilic          = AngraConstantMgr::Instance().GetValue("Density_Acrilic")*g/cm3;
  G4double content_carbon_acrilic   = AngraConstantMgr::Instance().GetValue("Content_Carbon_Acrilic");
  G4double content_hidrogen_acrilic = AngraConstantMgr::Instance().GetValue("Content_Hidrogen_Acrilic");
  G4double content_oxigen_acrilic   = AngraConstantMgr::Instance().GetValue("Content_Oxigen_Acrilic");
  G4Material* Acrilic = new G4Material(name="Acrilic",density= density_acrilic, ncomponents=3);
  Acrilic->AddElement(C,natoms = content_carbon_acrilic);
  Acrilic->AddElement(O,natoms = content_oxigen_acrilic);
  Acrilic->AddElement(H,natoms = content_hidrogen_acrilic);

  // ============ Optical Properties ================
  
  
  const G4int N_ACRILIC = AngraConstantMgr::Instance().GetValue("N_Acrilic_Bins");
  G4MaterialPropertyVector *AcrilicRefractive = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *AcrilicAbsLength  = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *AcrilicScintilFast  = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *AcrilicScintilSlow  = new G4MaterialPropertyVector();

  for(int i = 0; i<N_ACRILIC; i++){
    ostringstream num;
    num << i;
    string binName("OpAcrilic_");
    binName += num.str();
    G4double binEn = 1240./AngraConstantMgr::Instance().GetValue(binName)*eV;

    binName = "AbsAcrilic_";
    binName += num.str();        
    AcrilicAbsLength->InsertValues(binEn,AngraConstantMgr::Instance().GetValue(binName)*m);

    binName = "AcrilicRIndex";
    binName += num.str();        
    AcrilicRefractive->InsertValues(binEn,AngraConstantMgr::Instance().GetValue(binName));

    binName = "AcrilicScint_Fast";
    binName += num.str();        
    AcrilicScintilFast->InsertValues(binEn,AngraConstantMgr::Instance().GetValue(binName));
    
    binName = "AcrilicScint_Slow";
    binName += num.str();        
    AcrilicScintilSlow->InsertValues(binEn,AngraConstantMgr::Instance().GetValue(binName));
  }


  G4MaterialPropertiesTable* ScintProp = new G4MaterialPropertiesTable();
  ScintProp->AddProperty("RINDEX",       AcrilicRefractive );
  ScintProp->AddProperty("ABSLENGTH",    AcrilicAbsLength  );
  ScintProp->AddProperty("FASTCOMPONENT",AcrilicScintilFast);
  ScintProp->AddProperty("SLOWCOMPONENT",AcrilicScintilSlow);
  ScintProp->AddConstProperty("SCINTILLATIONYIELD",AngraConstantMgr::Instance().GetValue("Scint_Eff")/MeV);
  ScintProp->AddConstProperty("RESOLUTIONSCALE",   AngraConstantMgr::Instance().GetValue("Scint_Res"));
  ScintProp->AddConstProperty("FASTTIMECONSTANT",  AngraConstantMgr::Instance().GetValue("Scint_Fast")*ns);
  ScintProp->AddConstProperty("SLOWTIMECONSTANT",  AngraConstantMgr::Instance().GetValue("Scint_Slow")*ns);
  ScintProp->AddConstProperty("YIELDRATIO",        AngraConstantMgr::Instance().GetValue("Scint_Ratio"));

  //Air
  G4MaterialPropertyVector *AirRefractive = new G4MaterialPropertyVector();
  AirRefractive->InsertValues(1240./1000.*eV, AngraConstantMgr::Instance().GetValue("Air_RIndex"));
  AirRefractive->InsertValues(1240./200.*eV,  AngraConstantMgr::Instance().GetValue("Air_RIndex"));
  G4MaterialPropertiesTable* AirProp = new G4MaterialPropertiesTable();
  AirProp->AddProperty("RINDEX", AirRefractive);
  Air->SetMaterialPropertiesTable(AirProp);
  
  //Acrilic
  G4MaterialPropertiesTable* AcrilicProp = new G4MaterialPropertiesTable();
  AcrilicProp->AddProperty("RINDEX",       AcrilicRefractive );
  AcrilicProp->AddProperty("ABSLENGTH",    AcrilicAbsLength  );
  Acrilic->SetMaterialPropertiesTable(AcrilicProp);  
   
  // Optical Properties of Water  (Refractive Index from doi:10.1364/AO.12.000555)
  const G4int N_WATER = AngraConstantMgr::Instance().GetValue("N_Water_Bins");
  G4MaterialPropertyVector *WaterRefractive = new G4MaterialPropertyVector();
  G4MaterialPropertyVector *WaterAbsLength = new G4MaterialPropertyVector();

  for(int i = 0; i<N_WATER; i++){
    ostringstream num,lamb;
    num << i;
    string binName("OpWater_");
    binName += num.str();
    G4double binEn = 1240./AngraConstantMgr::Instance().GetValue(binName)*eV;

    binName = "RefIndx_";
    binName += num.str();        
    WaterRefractive->InsertValues(binEn,AngraConstantMgr::Instance().GetValue(binName));

    binName = "AbsLength_";
    binName += num.str();        
    WaterAbsLength->InsertValues(binEn,AngraConstantMgr::Instance().GetValue(binName)*m);
  }

  G4MaterialPropertiesTable* OpWaterProperties = new G4MaterialPropertiesTable();
  OpWaterProperties->AddProperty("RINDEX",   WaterRefractive);
  OpWaterProperties->AddProperty("ABSLENGTH",WaterAbsLength);

  GDW->SetMaterialPropertiesTable(OpWaterProperties);
  H2O->SetMaterialPropertiesTable(OpWaterProperties);

  
  // Materials that

  // Glass
  G4Material* Glass = man->FindOrBuildMaterial("G4_GLASS_PLATE");

  //Boron Water
  G4double frac_b_bw     = AngraConstantMgr::Instance().GetValue("Frac_B_BW")*perCent;
  G4double frac_water_bw = 100.*perCent - frac_b_bw;
  G4Material* BW = new G4Material(name="BoronWater",density=density_water,ncomponents=2);
  BW->AddMaterial(H2O,fractionmass = frac_water_bw);
  BW->AddElement (B,  fractionmass = frac_b_bw    );
  BW ->SetMaterialPropertiesTable(OpWaterProperties);
 
  // Scintillator
  G4double density_ls          = AngraConstantMgr::Instance().GetValue("Density_LS")*g/cm3;
  G4double content_carbon_ls   = AngraConstantMgr::Instance().GetValue("Content_Carbon_LS");
  G4double content_hidrogen_ls = AngraConstantMgr::Instance().GetValue("Content_Hidrogen_LS");
  G4Material* LS = new G4Material(name="Base",density= density_ls, ncomponents=2);
  LS->AddElement(C,natoms = content_carbon_ls);
  LS->AddElement(H,natoms = content_hidrogen_ls);

  // Gadolinium  Scintillator
  G4double content_carbon_gdls     = AngraConstantMgr::Instance().GetValue("Content_Carbon_GDLS");
  G4double content_hidrogen_gdls   = AngraConstantMgr::Instance().GetValue("Content_Hidrogen_GDLS");
  G4double content_gadolinium_gdls = AngraConstantMgr::Instance().GetValue("Content_Gadolinium_GDLS");
  G4Material* GDLS = new G4Material(name="GadoliniumDoped",density= density_ls, ncomponents=3);
  GDLS->AddElement(C, natoms = content_carbon_gdls    );
  GDLS->AddElement(H, natoms = content_hidrogen_gdls  );
  GDLS->AddElement(Gd,natoms = content_gadolinium_gdls);

  LS->SetMaterialPropertiesTable(ScintProp);
  GDLS->SetMaterialPropertiesTable(ScintProp);
  Glass->SetMaterialPropertiesTable(AirProp);
  
}



