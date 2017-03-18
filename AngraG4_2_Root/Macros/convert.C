//
// This is a sample macro showing how to use g4_2_root
// 
// author: G. A. Valdiviesso, 17/02/2013
//

#include "TStopwatch.h"
#include "g4_2_root.cpp"
#include "g4_2_PE.cpp"

void convert(){

  TString    inFile = "/home/drc01/kemp/angra/data/G4/000011.muons_v1_250k_20131001.G4";
  TString   outFile = "/home/drc01/kemp/angra/data/G4/000011.muons_v1_250k_20131001.root";
  Int_t       split = 1; // do not change this!
  Int_t compression = 1; // 9 = compact but very slow to read
  Int_t  max_events =-1; // -1 = "read all"
  
  TStopwatch clock;
  
  clock.Start();
  Int_t success = g4_2_root(inFile,outFile,split,compression); // call the routine
  clock.Stop();
  
  // sumary of what was done
  if (success) {
     cout << "Convertion of " << inFile << " to " << outFile << " was a success." << endl;
     cout << "    total time = " << clock.RealTime() << "s,   CPU time = " << clock.CpuTime() << "s." << endl;
  }
  else         {cout << "Convertion failed!" << endl;}

}
