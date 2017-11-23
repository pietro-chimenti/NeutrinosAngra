#include "muons/muons.cpp"

void gen_muons(){

  Int_t N        = 1.3e5;
  Int_t numFiles = 1e1;
  Int_t seed     = 20131000;
  Int_t localseed;
  
  TString fileName   = "muons_130k_";
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
    muons(localseed,N,file);
  }

}
