//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraG4Simulation command line parser header file
//
//  Authors: P.Chimenti, R.Lima, G. Valdiviesso
//
//  23-04-2025, fixing compatibility with Geant4 v11.3.0
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#ifndef AngraCommandLineParser_hh
#define AngraCommandLineParser_hh 1

#include <optional>
#include "globals.hh"

// Configuration structure to hold command line parameters
struct AngraConfigurations {
    bool Batch = false;      // 0: false, 1: true
    int Geometry = 1;        // 0: Copo de caipirinha, 1: chooz etc... (veja o help)
    bool Help = false;       // 0: false, 1: true
    int Primary = 0;         // 0: point-like source, 1: HepEvt interface
    long Random = 123456;    // the random generator seed
    int nThreads = 0;        // 0 means use sequential mode, >0 means use MT mode with that many threads
    std::optional<G4String> ScriptName;        // name of the script file to be used in batch
    std::optional<G4String> OutputFileName;    // name of the output file
    std::optional<G4String> InputHepFileName;  // name of the input HEPEV file, if needed.
};

// Parse command line arguments and return configuration
// Returns true if the program should continue, false if it should exit
bool ParseCommandLine(int argc, char** argv, AngraConfigurations& config);

// Print help information
void PrintHelp();

#endif // AngraCommandLineParser_hh
