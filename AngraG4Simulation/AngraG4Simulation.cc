//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraG4Simulation main file
//
//  Authors: P.Chimenti, R.Lima, G. Valdiviesso
//
//  30-04-2008, v0.01
//  23-04-2025, fixing compatibility with Geant4 v13.3.1
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

#include "AngraCommandLineParser.hh"

int main(int argc, char** argv)
{
	// Create configuration structure with default values
	AngraConfigurations confs;

	// Parse command line arguments
	if (!ParseCommandLine(argc, argv, confs)) {
		return 0;  // Exit if parsing indicates we should stop
	}


	// Set the random generator seed
	CLHEP::HepRandom::setTheSeed(confs.Random);
	G4cout << " seed: " << confs.Random << "\n";
	G4cout << " random: " << CLHEP::RandFlat::shoot(0., 1.) << G4endl;

	// Mandatory simulation definitions
	G4String oFile = "SimulationOutput.G4";
	if (confs.OutputFileName.has_value()) {
		oFile = G4String(confs.OutputFileName->c_str());
	}
	G4cout << oFile << G4endl;

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
	physics->RegisterPhysics( new G4OpticalPhysics );
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
		G4cout << "command = " << command + fileName << G4endl;
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





