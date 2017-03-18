#include "electromagnetic/electrons.cpp"

gen_electrons(){

  Int_t N        = 8e6;
  Int_t numFiles = 1e2;
  Int_t seed     = 20131000;
  Int_t localseed;
  
  TString fileName   = "electrons_8M_";
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
    electrons(localseed,N,file);
  }

}