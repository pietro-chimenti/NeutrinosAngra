#include "hadrons/neutrons.cpp"

void gen_neutrons(){

  Int_t N        = 5.7e4;
  Int_t numFiles = 1e1;
  Int_t seed     = 20131000;
  Int_t localseed;
  
  TString fileName   = "neutrons_57k_";
  TString filePrefix = "data/";
  TString fileAfix   = ".hepevt";
  TString file;
  
  for (Int_t i = 1; i<=numFiles; i++){
    file = filePrefix + fileName;
    localseed = seed + i;
    file += seed;
    file += "_";
    file += i;
    file += fileAfix;
    cout << file << endl;
    neutrons(localseed,N,file);
  }

}
