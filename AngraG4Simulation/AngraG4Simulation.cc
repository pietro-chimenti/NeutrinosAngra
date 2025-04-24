//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraG4Simulation main file
//
//  Authors: P.Chimenti, R.Lima, G. Valdiviesso
//
//  30-04-2008, v0.01
//   23-04-2025, fixing compatibility with Geant4 v13.3.1
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4UIExecutive.hh"
#include "QGSP_BIC_HP.hh"
#include "G4OpticalPhysics.hh"

#include "G4VisExecutive.hh"

#include "AngraDetectorConstruction.hh"
#include "AngraActionInitialization.hh"
#include "AngraPrimaryGeneratorAction.hh"
#include "AngraEventAction.hh"
#include "AngraTrackingAction.hh"
#include "AngraSteppingAction.hh"
#include "AngraMCLog.hh"
#include "AngraConstantMgr.hh"

#include "CLHEP/Random/Random.h"

#include <unistd.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <filesystem>
#include <optional>
#include <cstring>

namespace fs = std::filesystem;

struct AngraConfigurations {
	bool Batch = false;      // 0: false, 1: true
	int Geometry = 1;        // 0: Copo de caipirinha, 1: chooz etc... (veja o help)
	bool Help = false;       // 0: false, 1: true
	int Primary = 0;         // 0: point-like source, 1: HepEvt interface
	long Random = 123456;    // the random generator seed
	int nThreads = 0;        // 0 means use sequential mode, >0 means use MT mode with that many threads
	std::optional<std::string> ScriptName;        // name of the script file to be used in batch
	std::optional<std::string> OutputFileName;    // name of the output file
	std::optional<std::string> InputHepFileName;  // name of the input HEPEV file, if needed.
};

void PrintHelp();

int main(int argc, char** argv)
{
	// Create configuration structure with default values
	AngraConfigurations confs;

	AngraMCLog::Instance().SetHeaderOutLevel( HOL_ALL );
	AngraMCLog::Instance().SetEventOutLevel( EOL_ALL );
	AngraMCLog::Instance().SetTrajectoryOutLevel( TROL_NONE );
	AngraMCLog::Instance().SetTrackOutLevel( TOL_ALL );
	AngraMCLog::Instance().SetHitsOutLevel( HIOL_ALL );


	// Parse the arguments to set configuration parameters
	// If no arguments are passed, run with default values (no need to do anything special here)

	int c;
	int l_vlevel;
	while ((c = getopt(argc, argv, "hbg:s:o:r:p:v:i:j:")) != -1) {
		switch (c) {
			case 'h':
				confs.Help = true;
				std::cout << "Help menu." << std::endl;
				PrintHelp();
				return 0;

			case 'b':
				confs.Batch = true;
				std::cout << "Batch run selected" << std::endl;
				break;

			case 'g':
				confs.Geometry = std::stoi(optarg);
				std::cout << "Geometry number " << confs.Geometry << " chosen" << std::endl;
				break;

			case 'p':
				confs.Primary = std::stoi(optarg);
				std::cout << "Primary generator " << confs.Primary << " chosen" << std::endl;
				break;

			case 's':
				confs.ScriptName = optarg;
				std::cout << "Batch script chosen: " << *confs.ScriptName << std::endl;
				break;

			case 'r':
				confs.Random = std::stol(optarg);
				std::cout << "Random seed chosen: " << optarg << std::endl;
				break;

			case 'o':
				confs.OutputFileName = optarg;
				std::cout << "Output file name chosen: " << *confs.OutputFileName << std::endl;
				break;

			case 'i':
				confs.InputHepFileName = optarg;
				std::cout << "Input HepEv file name chosen: " << *confs.InputHepFileName << std::endl;
				break;

			case 'j':
				confs.nThreads = std::stoi(optarg);
				std::cout << "Number of threads chosen: " << confs.nThreads << std::endl;
				break;

			case 'v':
				l_vlevel = std::stoi(optarg);
				switch (l_vlevel) {
					case 10:
						AngraMCLog::Instance().SetHeaderOutLevel(HOL_NONE);
						break;
					case 11:
						AngraMCLog::Instance().SetHeaderOutLevel(HOL_ALL);
						break;
					case 20:
						AngraMCLog::Instance().SetEventOutLevel(EOL_NONE);
						break;
					case 21:
						AngraMCLog::Instance().SetEventOutLevel(EOL_ALL);
						break;
					case 30:
						AngraMCLog::Instance().SetTrajectoryOutLevel(TROL_NONE);
						break;
					case 31:
						AngraMCLog::Instance().SetTrajectoryOutLevel(TROL_ALL);
						break;
					case 40:
						AngraMCLog::Instance().SetTrackOutLevel(TOL_NONE);
						break;
					case 41:
						AngraMCLog::Instance().SetTrackOutLevel(TOL_ALL);
						break;
					case 50:
						AngraMCLog::Instance().SetHitsOutLevel(HIOL_NONE);
						break;
					case 51:
						AngraMCLog::Instance().SetHitsOutLevel(HIOL_ALL);
						break;
					default:
						std::cout << "Unrecognized Output level" << std::endl;
				}
				break;

			case '?':
				std::cout << "Unrecognized option encountered -" << static_cast<char>(optopt) << std::endl;
				std::cout << "Printing help information:" << std::endl;
				PrintHelp();
				return 0;
				break;  // Not reached due to return, but good practice

			default:
				std::cout << "Unknown error in command line parsing" << std::endl;
				PrintHelp();
				return 0;
		}
	}

	// Check for any remaining arguments
	if (optind < argc) {
		std::cout << "Non-option arguments encountered:" << std::endl;
		for (int i = optind; i < argc; i++) {
			std::cout << "  " << argv[i] << std::endl;
		}
		std::cout << "Printing help information:" << std::endl;
		PrintHelp();
		return 0;
	}


	// Set the random generator seed
	CLHEP::HepRandom::setTheSeed(confs.Random);
	std::cout << " seed: " << confs.Random << std::endl;
	std::cout << " random: " << CLHEP::RandFlat::shoot(0., 1.) << std::endl;

	// Mandatory simulation definitions
	G4String oFile = "SimulationOutput.G4";
	if (confs.OutputFileName.has_value()) {
		oFile = G4String(confs.OutputFileName->c_str());
	}
	G4cout << oFile << std::endl;

	auto outFile = new std::ofstream();
	outFile->open(oFile);
	AngraMCLog::Instance().SetOutFile(outFile);

	if (confs.InputHepFileName.has_value()) {
		// Need to create a persistent copy of the string for C API
		char* inputFileName = new char[confs.InputHepFileName->length() + 1];
		std::strcpy(inputFileName, confs.InputHepFileName->c_str());
		AngraMCLog::Instance().SetInHepEvtFile(inputFileName);
	} else {
		AngraMCLog::Instance().SetInHepEvtFile(const_cast<char*>("event.data"));
	}

	// Create run manager - either sequential or MT depending on command line option
	G4RunManager* runManager = nullptr;
	if (confs.nThreads > 0) {
		// Multi-threaded mode
		auto mtRunManager = new G4MTRunManager;
		mtRunManager->SetNumberOfThreads(confs.nThreads);
		G4cout << "Running in multi-threaded mode with " << confs.nThreads << " threads" << G4endl;
		runManager = mtRunManager;
	} else {
		// Sequential mode
		runManager = new G4RunManager;
		G4cout << "Running in sequential mode" << G4endl;
	}

	// Initialize detector construction
	G4VUserDetectorConstruction* detector = new AngraDetectorConstruction(geometryEnum(confs.Geometry));
	runManager->SetUserInitialization(detector);

	// Initialize physics list
	G4VModularPhysicsList* physics = new QGSP_BIC_HP;
	physics->RegisterPhysics(new G4OpticalPhysics(0));
	runManager->SetUserInitialization(physics);

	// Initialize action initialization
	G4VUserActionInitialization* actionInit = new AngraActionInitialization(primaryEnum(confs.Primary), outFile);
	runManager->SetUserInitialization(actionInit);

	runManager->SetVerboseLevel(5);

	// Initialize and run
	runManager->Initialize();
	G4UImanager* UI = G4UImanager::GetUIpointer();

	if (confs.Batch) {
		// Batch mode
		G4String command = "/control/execute ";
		G4String fileName = confs.ScriptName.value_or("run1.mac");
		std::cout << "command = " << command + fileName << std::endl;
		UI->ApplyCommand(command + fileName);
	} else {
		// Interactive mode
		// Initialize visualization
		G4VisManager* visManager = new G4VisExecutive;
		visManager->Initialize();

		// Initialize UI executive
		G4UIExecutive* ui = new G4UIExecutive(argc, argv);

		// Execute macro file
		G4String command = "/control/execute ";
		G4String fileName = confs.ScriptName.value_or("vis_qt.mac"); // Use Qt visualization by default
		UI->ApplyCommand(command + fileName);

		// Start UI session
		ui->SessionStart();
		delete ui;

		// Clean up visualization
		delete visManager;
	}

	delete runManager;

	return 0;
}

void PrintHelp() {
	std::cout << "The possible options are:\n"
			  << "-h          : this menu\n"
			  << "-b          : BATCH mode\n"
			  << "-r SEED     : SEED of random engine\n"
			  << "-g GEOMETRY : GEOMETRY is 1 for New Geometry (need constants.wb1.dat -> constans.dat)\n"
			  << "            :             2 for Old Geometry (need constants.wb2.dat -> constans.dat)\n"
			  << "-p PRIMARY  : PRIMARY is 0 for point-like source\n"
			  << "            : 1 for HepEvt interface -  needs 'event.data'\n"
			  << "-i FILENAME : FILENAME is the name of the HepEvt file, instead of 'event.data'\n"
			  << "-j NTHREADS : NTHREADS is the number of threads for multithreading (0 for sequential mode)\n"
			  << "-s SCRIPT   : SCRIPT is the name of the G4script\n"
			  << "-o FILENAME : FILENAME is the name of file used to store the simulation results\n"
			  << "            : default value is 'SimulationOutput.G4'\n"
			  << "-v OUTLEVEL : OUTLEVEL is the verbosity level of output (multiple choices possible):\n"
			  << "            : 10 for header info to NONE\n"
			  << "            : 11 for header info to ALL\n"
			  << "            : 20 for event info to NONE\n"
			  << "            : 21 for event info to ALL\n"
			  << "            : 30 for trajectory info to NONE\n"
			  << "            : 31 for trajectory info to ALL\n"
			  << "            : 40 for track info to NONE\n"
			  << "            : 41 for track info to ALL\n"
			  << "            : 50 for hits info to NONE\n"
			  << "            : 51 for hits info to ALL" << std::endl;
}



