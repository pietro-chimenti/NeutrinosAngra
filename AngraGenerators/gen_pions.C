#include "hadrons/pions.cpp"

gen_pions(){

  Int_t N        = 3.4e4;
  Int_t numFiles = 1e0;
  Int_t seed     = 20131000;
  Int_t localseed;
  
  TString fileName   = "pions_34k_";
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
    pions(localseed,N,file);
  }

}