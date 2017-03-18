//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation main file
//
//  Authors: P.Chimenti, R.Lima
//
//  30-4-2008, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "AngraDetectorConstruction.hh"
#include "AngraPhysicsList.hh"
#include "AngraPrimaryGeneratorAction.hh"
#include "AngraEventAction.hh"
#include "AngraTrackingAction.hh"
#include "AngraSteppingAction.hh"
#include "AngraMCLog.hh"
#include "AngraConstantMgr.hh"

#include "CLHEP/Random/Random.h"

#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"

struct AngraConfigurations {
  int Batch;//0: false, 1: true
  int Geometry;//0: Copo de caipirinha, 1: chooz etc... (veja o help)
  int Help;// 0: false, 1: true
  int Primary;// the random generator seed
  long Random;// the random generator seed
  char * ScriptName; // name of the script file to be used in batch
  char * OutputFileName; // name of the output file
  char * InputHepFileName; // name of the input HEPEV file, if needed.
};

int PrintHelp();

int main(int argc,char** argv)
{
  
  // start creating a configuration structure
  struct AngraConfigurations confs;
  confs.Batch=0;
  confs.Geometry=1;
  confs.Random=0;
  confs.Primary=0;
  confs.Help=0;
  confs.ScriptName=NULL;
  confs.OutputFileName=NULL;
  confs.InputHepFileName=NULL;

  AngraMCLog::Instance().SetHeaderOutLevel( HOL_ALL );
  AngraMCLog::Instance().SetEventOutLevel( EOL_ALL );
  AngraMCLog::Instance().SetTrajectoryOutLevel( TROL_NONE );
  AngraMCLog::Instance().SetTrackOutLevel( TOL_ALL );
  AngraMCLog::Instance().SetHitsOutLevel( HIOL_ALL );
  

  // parse the arguments to set configuration parameters

  if( argc==1 ){
    // if only one argument print help and exit
      PrintHelp();
      return 0;
  }



  int c;
  int l_vlevel;
  while ( (c = getopt( argc, argv, "hbg:s:o:r:p:v:i:" ) ) != -1 ) {
    
    switch( c ) {
      
    case 'h':
      confs.Help=1;
      printf( "Help menu.\n" );
      PrintHelp();
      return 0;
      break;
      
    case 'b':
      confs.Batch=1;
      printf( "Batch run selected\n");
      break;
      
    case 'g':
      confs.Geometry=atoi(optarg);
      printf( "Geometry number %d chosen\n", confs.Geometry );
      break;
      
    case 'p':
      confs.Primary=atoi(optarg);
      printf( "Primary generator %d chosen\n", confs.Primary );
      break;
      
    case 's':
      confs.ScriptName=optarg;
      printf( "Batch script chosen: %s \n",optarg );
      break;

    case 'r':
      confs.Random=atoi(optarg);
      printf( "Random seed chosen: %s \n",optarg );
      break;
      
    case 'o':
      confs.OutputFileName=optarg;
      printf( "Output file name chosen: %s \n",optarg );
      break;

    case 'i':
      confs.InputHepFileName=optarg;
      printf( "Input HepEv file name chosen: %s \n",optarg );
      break;

    case 'v':
      l_vlevel=atoi(optarg);
      switch( l_vlevel ) {
      case 10:
	AngraMCLog::Instance().SetHeaderOutLevel( HOL_NONE );
	break;
      case 11:
	AngraMCLog::Instance().SetHeaderOutLevel( HOL_ALL );
	break;
      case 20:
	AngraMCLog::Instance().SetEventOutLevel( EOL_NONE );
	break;
      case 21:
	AngraMCLog::Instance().SetEventOutLevel( EOL_ALL );
	break;
      case 30:
	AngraMCLog::Instance().SetTrajectoryOutLevel( TROL_NONE );
	break;
      case 31:
	AngraMCLog::Instance().SetTrajectoryOutLevel( TROL_ALL );
	break;
      case 40:
	AngraMCLog::Instance().SetTrackOutLevel( TOL_NONE );
	break;
      case 41:
	AngraMCLog::Instance().SetTrackOutLevel( TOL_ALL );
	break;
      case 50:
	AngraMCLog::Instance().SetHitsOutLevel( HIOL_NONE );
	break;
      case 51:
	AngraMCLog::Instance().SetHitsOutLevel( HIOL_ALL );
	break;
      default:
	printf( "Unrecognized Output level \n" );
      }
      break;
      
    case '?':
      printf( "Unrecognized option encountered -%c\n", optopt );
      
    default:
      exit(-1);
      
    }
    
  }
  
  for ( c = optind ; c < argc ; c++ ) {

    printf( "Non valid option %s\n", argv[c] );

  }


  // set the random generator seed
  CLHEP::HepRandom::setTheSeed(confs.Random);
  std::cout << " seed: " << confs.Random << std::endl;
  std::cout << " random: " <<  CLHEP::RandFlat::shoot(0.,1.)  << std::endl;

  // mandatory simulation definitions
  G4String oFile="SimulationOutput.G4";
  if( confs.OutputFileName != NULL) oFile=G4String(confs.OutputFileName) ;
  G4cout << oFile << std::endl; 
  std::ofstream*  outFile = new std::ofstream();
  outFile->open(oFile);
  AngraMCLog::Instance().SetOutFile(outFile);
  
  if( confs.InputHepFileName!= NULL) {AngraMCLog::Instance().SetInHepEvtFile(confs.InputHepFileName);}
    else AngraMCLog::Instance().SetInHepEvtFile("event.data");

  G4RunManager* runManager = new G4RunManager;
  G4VUserDetectorConstruction* detector = new AngraDetectorConstruction(geometryEnum(confs.Geometry));
  runManager->SetUserInitialization(detector);
  G4VUserPhysicsList* physics = new AngraPhysicsList;
  runManager->SetUserInitialization(physics);
  G4VUserPrimaryGeneratorAction* gen_action = new AngraPrimaryGeneratorAction(primaryEnum(confs.Primary),outFile);
  runManager->SetUserAction(gen_action);

  runManager->SetVerboseLevel(5);

 
  // other user actions

  G4UserEventAction* event_action = new AngraEventAction(outFile);
  runManager->SetUserAction(event_action);
  G4UserTrackingAction* tracking_action = new AngraTrackingAction(outFile);
  runManager->SetUserAction(tracking_action);
  G4UserSteppingAction* stepping_action = new AngraSteppingAction();
  runManager->SetUserAction(stepping_action);

  // initializer and run
  runManager->Initialize();
  G4UImanager* UI = G4UImanager::GetUIpointer();
  if (confs.Batch)   // batch mode  
    {
     G4String command = "/control/execute ";
     G4String fileName;
     if( confs.ScriptName == NULL)  fileName = "run1.mac";
     else fileName = confs.ScriptName;
     std::cout << "command = " <<  command+fileName << std::endl;
     UI->ApplyCommand(command+fileName);
    }
    
  else           // interactive mode
    { 
#ifdef G4VIS_USE
      G4VisManager* visManager = new G4VisExecutive;
      visManager->Initialize();
#endif    
     
      G4UIsession * session = 0;
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);      
#else
      session = new G4UIterminal();
#endif
      G4String command = "/control/execute ";
      G4String fileName;
      if( confs.ScriptName == NULL)  fileName = "vis.mac";
      else fileName = confs.ScriptName;
      UI->ApplyCommand(command+fileName);
      session->SessionStart();
      delete session;
     
#ifdef G4VIS_USE
      delete visManager;
#endif     
    }

  delete runManager;

  return 0;
}

int PrintHelp(){
  printf( "The possible options are:\n" );
  printf( "-h          : this menu\n" );
  printf( "-b          : BATCH mode\n" );
  printf( "-r SEED     : SEED of random engine\n" );
  printf( "-g GEOMETRY : GEOMETRY is 1 for New Geometry (need constants.wb1.dat -> constans.dat)\n" );
  printf( "            :             2 for Old Geometry (need constants.wb2.dat -> constans.dat)\n" );
  printf( "-p PRIMARY  : PRIMARY is 0 for point-like source\n" );
  printf( "            : 1 for HepEvt interface -  needs 'event.data' \n" );
  printf( "-i FILENAME : FILENAME is the name of the HepEvt file, instead of 'event.data' \n" );
  printf( "-s SCRIPT   : SCRIPT is the name of the G4script\n" );
  printf( "-o FILENAME : FILENAME is the name of file used to store the simulation results \n" );
  printf( "            : default value is 'SimulationOutput.G4' \n");
  printf( "-v OUTLEVEL : OUTLEVEL is the verbosity level of output (multiple choices possible): \n" );
  printf( "            : 10 for header info to NONE \n" );
  printf( "            : 11 for header info to ALL \n" );
  printf( "            : 20 for event info to NONE \n" );
  printf( "            : 21 for event info to ALL \n" );
  printf( "            : 30 for trajectory info to NONE \n" );
  printf( "            : 31 for trajectory info to ALL \n" );
  printf( "            : 40 for track info to NONE \n" );
  printf( "            : 41 for track info to ALL \n" );
  printf( "            : 50 for hits info to NONE \n" );
  printf( "            : 51 for hits info to ALL \n" );

  return 0;
}


