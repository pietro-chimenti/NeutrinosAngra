//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraG4Simulation command line parser implementation
//
//  Authors: P.Chimenti, R.Lima, G. Valdiviesso
//
//  23-04-2025, fixing compatibility with Geant4 v13.3.1
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#include "AngraCommandLineParser.hh"
#include "AngraMCLog.hh"

#include <unistd.h>
#include <iostream>

bool ParseCommandLine(int argc, char** argv, AngraConfigurations& config) {
    // Set default log levels
    AngraMCLog::Instance().SetHeaderOutLevel(HOL_ALL);
    AngraMCLog::Instance().SetEventOutLevel(EOL_ALL);
    AngraMCLog::Instance().SetTrajectoryOutLevel(TROL_NONE);
    AngraMCLog::Instance().SetTrackOutLevel(TOL_ALL);
    AngraMCLog::Instance().SetHitsOutLevel(HIOL_ALL);

    // Parse the arguments to set configuration parameters
    // If no arguments are passed, run with default values
    int c;
    int l_vlevel;

    // Reset optind to allow multiple calls to getopt
    optind = 1;

    while ((c = getopt(argc, argv, "hbg:s:o:r:p:v:i:j:")) != -1) {
        switch (c) {
            case 'h':
                config.Help = true;
                std::cout << "Help menu." << std::endl;
                PrintHelp();
                return false;  // Exit program

            case 'b':
                config.Batch = true;
                std::cout << "Batch run selected" << std::endl;
                break;

            case 'g':
                config.Geometry = std::stoi(optarg);
                std::cout << "Geometry number " << config.Geometry << " chosen" << std::endl;
                break;

            case 'p':
                config.Primary = std::stoi(optarg);
                std::cout << "Primary generator " << config.Primary << " chosen" << std::endl;
                break;

            case 's':
                config.ScriptName = optarg;
                std::cout << "Batch script chosen: " << *config.ScriptName << std::endl;
                break;

            case 'r':
                config.Random = std::stol(optarg);
                std::cout << "Random seed chosen: " << optarg << std::endl;
                break;

            case 'o':
                config.OutputFileName = optarg;
                std::cout << "Output file name chosen: " << *config.OutputFileName << std::endl;
                break;

            case 'i':
                config.InputHepFileName = optarg;
                std::cout << "Input HepEv file name chosen: " << *config.InputHepFileName << std::endl;
                break;

            case 'j':
                config.nThreads = std::stoi(optarg);
                std::cout << "Number of threads chosen: " << config.nThreads << std::endl;
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
                return false;  // Exit program

            default:
                std::cout << "Unknown error in command line parsing" << std::endl;
                PrintHelp();
                return false;  // Exit program
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
        return false;  // Exit program
    }

    return true;  // Continue program execution
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
