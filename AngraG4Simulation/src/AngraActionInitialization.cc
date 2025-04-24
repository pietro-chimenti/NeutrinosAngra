//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation action initialization implementation file
//
//  Authors: G. Valdiviesso
//
//  24-04-2024, v0.03
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#include "AngraActionInitialization.hh"
#include "AngraPrimaryGeneratorAction.hh"
#include "AngraEventAction.hh"
#include "AngraTrackingAction.hh"
#include "AngraSteppingAction.hh"
#include "AngraMCLog.hh"
#include "G4Threading.hh"
#include <string>

AngraActionInitialization::AngraActionInitialization(primaryEnum p, std::ofstream* outFile)
 : G4VUserActionInitialization(),
   fPrimary(p),
   fOutFile(outFile)
{}

AngraActionInitialization::~AngraActionInitialization()
{}

void AngraActionInitialization::BuildForMaster() const
{
  // No actions needed for master thread
}

void AngraActionInitialization::Build() const
{
  // Create thread-specific output file if in MT mode
  std::ofstream* threadOutFile = nullptr;

  if (G4Threading::IsWorkerThread()) {
    // Create a thread-specific output file
    G4int threadId = G4Threading::G4GetThreadId();
    G4String fileName = AngraMCLog::GetThreadOutputFileName(threadId);

    threadOutFile = new std::ofstream();
    threadOutFile->open(fileName);

    // Write header to the thread-specific output file
    if (threadOutFile->is_open()) {
      *threadOutFile << "STANDARD" << G4endl;
    }

    // Set thread-specific output file in AngraMCLog
    AngraMCLog::Instance().SetOutFile(threadOutFile);

    // Initialize verbosity levels for this thread
    AngraMCLog::Instance().SetHeaderOutLevel(HOL_ALL);
    AngraMCLog::Instance().SetEventOutLevel(EOL_ALL);
    AngraMCLog::Instance().SetTrajectoryOutLevel(TROL_ALL);
    AngraMCLog::Instance().SetTrackOutLevel(TOL_ALL);
    AngraMCLog::Instance().SetHitsOutLevel(HIOL_ALL);
  } else {
    // Use the main output file for master thread
    threadOutFile = fOutFile;

    // Write header to the main output file if it's not already written
    if (threadOutFile && threadOutFile->is_open() && threadOutFile->tellp() == 0) {
      *threadOutFile << "STANDARD" << G4endl;
    }
  }

  // Primary generator action
  SetUserAction(new AngraPrimaryGeneratorAction(fPrimary, threadOutFile));

  // Event action
  SetUserAction(new AngraEventAction(threadOutFile));

  // Tracking action
  SetUserAction(new AngraTrackingAction(threadOutFile));

  // Stepping action
  SetUserAction(new AngraSteppingAction());
}
