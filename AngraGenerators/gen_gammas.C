#include "environment/gammas.cpp"

void gen_gammas(){

  Int_t N        = 1000;
  Int_t numFiles = 2;
  Int_t seed     = 20131000;
  Int_t localseed;
  
  TString fileName   = "gammas_1.7M_";
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
    gammas(localseed,N,file);
  }

}
