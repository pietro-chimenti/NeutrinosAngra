#include "neutrinos/AngraNuGen.cpp"

gen_neutrinos(){

  Double_t Target_X = 2* 710.5;
  Double_t Target_Y = 2* 523.0;
  Double_t Target_Z = 2* 500.0;

  Long64_t    N = 1e5;
  Int_t    seed = 20131020;
  
  AngraNuGen(20131020,1e5,2,Target_X,Target_Y,Target_Z);
    
}