
//#include <sstring>
#include <iomanip>

Int_t Smaller(Int_t N, Double_t *Tempo){
  Double_t t_menor;
  Int_t j=-1;
  do {j++;}while(Tempo[j]==0.);
  t_menor=Tempo[j];
  Int_t i_menor=j;
  for(Int_t i=j;i<=N;i++){
    if(Tempo[i]!=0. && Tempo[i]<t_menor){
      i_menor = i;
      t_menor = Tempo[i];
    }
  }
  return i_menor;
}



void WriteTimes(const Int_t nFiles, Int_t *parI, Double_t *parD, TString *parS, int NM=10){


  const Int_t N = NM;
  TString fileName = parS[2] + parS[3];
  ofstream timeStream;
  timeStream.open(fileName.Data());
   
  Double_t tau[N];
  Double_t ratio[N];
  Double_t next[N];
  TRandom3 *rnd = new TRandom3(parI[0]);

  Double_t runTime  = parD[0]*1.e9;
  Double_t darkRate = parD[1];
  TH1D *hFreq[nFiles+1];

  tau[0]  = 1./darkRate * 1.E+09;
  for (Int_t i = 0; i<=nFiles; i++) {
    if (i>0) {
      ratio[i] = parD[2*N+i];
      tau[i]  = 1./(parD[1*N+i]*ratio[i]) * 1.E+09;
    }
//     cout << i << "\t" << tau[i] << endl;
    next[i] = rnd->Exp(tau[i]);
    if (i>0) {hFreq[i] = new TH1D(parS[1*N+i].Data(),parS[1*N+i].Data(),1.e3,0.,10.*tau[i]);}
    else {hFreq[0] = new TH1D("dark_noise","Dark Noise",1.e3,0.,10.*tau[i]);}
  }
  
  if (nFiles>=1) hFreq[1]->SetLineColor(kOrange);
  if (nFiles>=2) hFreq[2]->SetLineColor(kGreen);
  if (nFiles>=3) hFreq[3]->SetLineColor(kBlue);
  if (nFiles>=4) hFreq[4]->SetLineColor(kMagenta);
  if (nFiles>=5) hFreq[5]->SetLineColor(kRed);
  if (nFiles>=6) hFreq[6]->SetLineColor(kGray);
  if (nFiles>=7) hFreq[7]->SetLineColor(kYellow-2);
  if (nFiles>=8) hFreq[8]->SetLineColor(kSpring-5);
  if (nFiles>=9) hFreq[9]->SetLineColor(kCyan);

  Double_t globalTime = 0.;
  Double_t DeltaT= 0.;
  Int_t iMenor;
  
  Double_t completion = 0.;
  Int_t verbose = 1;
  
  do{   
    iMenor=Smaller(nFiles,next);
    globalTime = next[iMenor];
    timeStream << iMenor << "\t" << fixed << setprecision(0) << next[iMenor] << endl;
    DeltaT = rnd->Exp(tau[iMenor]);
    next[iMenor] += DeltaT;
    hFreq[iMenor]->Fill(DeltaT);

    completion = globalTime/runTime*100;
    if (((int)completion)%10==0 && verbose) {cout << (int) completion << "% " <<  globalTime << "ns" << endl; verbose=0;}
    if ( (int)completion %10!=0)  {verbose=1;}
    
  }while(globalTime<runTime);


  timeStream.close();
  
  if (parS[4]!=""){
     TString plotFile = parS[2]+parS[4];
     cout << "\t" << "Saving plots into " << plotFile.Data() << endl;
     TFile *plots = new TFile( plotFile.Data(),"recreate");
     for(Int_t i = 1; i<=nFiles; i++){hFreq[i]->Write();}
     plots->Close();
  }  else {cout << "\t" << "Plots not saved (use OUTPUT_PLOT option)."<< endl;}

}
