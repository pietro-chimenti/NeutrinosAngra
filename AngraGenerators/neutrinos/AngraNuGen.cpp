//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  Reactor Anti-neutrino Event Generator
//
//  Author: T.J.C.Bezerra
//
//  8-8-2013, v1.0
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//
// Upgrades:
//
// August 8th 2013: Code all re-written using the kinematics of the Inverse Beta Decay as described in arvix:9903554
//
// March 10th 2010: Take out the neutron rate.
// The positron and neutron are created at the same position in the same time.
// Change of the name of the output file.
//
// March 5th 2010: Finish of the first version

#include<stdio.h>
#include<fstream>
#include<iostream>
#include<string>
#include<sstream>
#include<math.h>
#include "TMath.h"
#include "TRandom3.h"
#include "TVector3.h"

using namespace std;

const Double_t PI     = TMath::Pi();
const Double_t LowE   = 1.875;
const Double_t HighE  = 8.125;

const Double_t m_e    = 0.51099893;//MeV
const Double_t M_p    = 938.27203;//MeV
const Double_t M_n    = 939.56536;//MeV
const Double_t dM     = M_n - M_p;//MeV
const Double_t M      = (M_p+M_n)/2;//MeV
const Double_t f      = 1.0;
const Double_t g      = 1.267;
const Double_t sigma0 = 0.952e-42/(f*f+3*g*g);
const Double_t f2     = 3.706;//mu_p - mu_n
const Double_t y2     = (dM*dM - m_e*m_e)/2.;

TVector3 vReactorPos(0,0,-30e3); //mm : baseline

Double_t isotope;

Int_t seed=0;

TF1* THespU235;
TF1* THespPu239;
TF1* THespPu241;
TF1* THespU238;

//Mean fission rates fraction (from Bugey experiment paper)
Double_t pU235  = 0.52;
Double_t pPu239 = 0.33;
Double_t pPu241 = 0.09;
Double_t pU238  = 0.06;

Double_t baseline;

void SetRefSpectrum(void)
{
//   *****************************************************************************************
//   * Parametrization of T.Mueller at al. Spectra Prediction					*
//   *****************************************************************************************

   THespU235 = new TF1("THespU235", "exp([0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**5)",LowE, HighE);
	THespU235->SetParameter(0,3.217);
	THespU235->SetParameter(1,-3.111);
	THespU235->SetParameter(2,1.395);
	THespU235->SetParameter(3,-3.690e-1);
	THespU235->SetParameter(4,4.445e-2);
	THespU235->SetParameter(5,-2.053e-3);
	THespU235->SetLineColor(2);
	THespU235->SetLineWidth(2);
	
  THespPu239 = new TF1("THespPu239", "exp([0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**5)",LowE, HighE);
	THespPu239->SetParameter(0,6.413);
	THespPu239->SetParameter(1,-7.432);
	THespPu239->SetParameter(2,3.535);
	THespPu239->SetParameter(3,-8.820e-1);
	THespPu239->SetParameter(4,1.025e-1);
	THespPu239->SetParameter(5,-4.550e-3);
	THespPu239->SetLineColor(4);
	THespPu239->SetLineWidth(2);
	
  THespPu241 = new TF1("THespPu241", "exp([0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**5)",LowE, HighE);
	THespPu241->SetParameter(0,3.251);
	THespPu241->SetParameter(1,-3.204);
	THespPu241->SetParameter(2,1.428);
	THespPu241->SetParameter(3,-3.675e-1);
	THespPu241->SetParameter(4,4.254e-2);
	THespPu241->SetParameter(5,-1.896e-3);
	THespPu241->SetLineColor(8);
	THespPu241->SetLineWidth(2);
	
  THespPu241 = new TF1("THespPu241", "exp([0]+[1]*x+[2]*x**2+[3]*x**3+[4]*x**4+[5]*x**5)",LowE, HighE);
	THespPu241->SetParameter(0,3.251);
	THespPu241->SetParameter(1,-3.204);
	THespPu241->SetParameter(2,1.428);
	THespPu241->SetParameter(3,-3.675e-1);
	THespPu241->SetParameter(4,4.254e-2);
	THespPu241->SetParameter(5,-1.896e-3);
	THespPu241->SetLineColor(8);
	THespPu241->SetLineWidth(2);
}

TVector3 GetPosition(Double_t Detector_Lx, Double_t Detector_Ly, Double_t Detector_Lz)
{
  Double_t xv,yv,zv;
  TVector3 vpos;

//   TRandom3 rand0, rand1, rand2;  
//   rand0.SetSeed(seed);
//   rand1.SetSeed(seed);
//   rand2.SetSeed(seed);
  
  xv = Detector_Lx*(2.*gRandom->Rndm()-1)/2;
  yv = Detector_Ly*(2.*gRandom->Rndm()-1)/2;
  zv = Detector_Lz*(2.*gRandom->Rndm()-1)/2;
      
  vpos.SetXYZ(xv,yv,zv);
  
  return vpos;
}

Double_t CalNeutrinoSpectrum(Double_t E_nu, Double_t cos_theta, Double_t pIsotope = 0.3)
{
  Double_t totalRate;

  const Double_t E_e0 = E_nu - dM;
  const Double_t p_e0 = sqrt(E_e0*E_e0 - m_e*m_e);
  const Double_t v_e0 = p_e0/E_e0;
  const Double_t E_e1 = E_e0*(1 - E_nu/M*(1-v_e0*cos_theta)) - y2/M;
  const Double_t p_e1 = sqrt(E_e1*E_e1 - m_e*m_e);
  const Double_t v_e1 = p_e1/E_e1;

  if (pIsotope <= pU235)
  {
    totalRate = THespU235->Eval(E_nu,0,0,0);
    isotope = 1;
  }
  else if ((pIsotope > pU235) && (pIsotope <= (pPu239+pU235) ))
  {
    totalRate = THespPu239->Eval(E_nu,0,0,0);
    isotope = 2;
  }
  else if ( (pIsotope > (pPu239+pU235) )  && (pIsotope <= (pPu241+pPu239+pU235) ) )
  {
    totalRate = THespPu241->Eval(E_nu,0,0,0);
    isotope = 3;
  }
  else 
  {
    totalRate = THespU238->Eval(E_nu,0,0,0);
    isotope = 4;
  }

  if(v_e0 <= 0) return 0;

  const Double_t Gamma = 2*(f+f2)*((2*E_e0+dM)*(1-v_e0*cos_theta) - m_e*m_e/E_e0)
                       + (f*f+g*g)*(dM*(1+v_e0*cos_theta) + m_e*m_e/E_e0)
                       + (f*f+3*g*g)*((E_e0+dM)*(1-1./v_e0*cos_theta) + dM)
		       + (f*f-g*g)*((E_e0+dM)*(1-1./v_e0*cos_theta) + dM)*v_e0*cos_theta;


  Double_t DCrossSection=sigma0/2*((f*f + 3*g*g) + (f*f - g*g)*v_e1*cos_theta)*E_e1*p_e1
                      - sigma0/2*Gamma/M*E_e0*p_e0;

  if(v_e0 <= 0) DCrossSection=0;

  return totalRate*DCrossSection;
}

TVector3 GetIBDMomentum(Double_t& E_pos, TVector3& fPositronMomemtum, TVector3& fNeutronMomemtum, Double_t& E_nue, TVector3 vpos)
{
  TVector3 fNeutrinoMomemtum;
  
//   TRandom3 rand0,rand1,rand2,rand3;
//   rand0.SetSeed(seed);
//   rand1.SetSeed(seed);
//   rand2.SetSeed(seed);
//   rand3.SetSeed(seed);
  
  Double_t E_nu, phi, theta, cos_theta;
  Double_t nu,ans,maxans,rate;
  Double_t pIsotope;

  pIsotope = gRandom->Rndm();

  do{
     maxans=CalNeutrinoSpectrum(3.5,-1);

     E_nu      = gRandom->Rndm()*(HighE-1.806)+1.806;
     cos_theta = 2.*gRandom->Rndm()-1.;
     ans       = CalNeutrinoSpectrum(E_nu,cos_theta);
     rate      = maxans*1.2*gRandom->Rndm();
    }while(rate>ans);

  E_nue = E_nu;

  theta = acos(cos_theta);

  phi=2.*PI*gRandom->Rndm();

  const Double_t E_e0 = E_nu - dM;
  const Double_t p_e0 = sqrt(E_e0*E_e0 - m_e*m_e);
  const Double_t v_e0 = p_e0/E_e0;
  const Double_t E_e1 = E_e0*(1 - E_nu/M*(1-v_e0*cos_theta)) - y2/M;
  const Double_t p_e1 = sqrt(E_e1*E_e1 - m_e*m_e);

  TVector3 v1,v2;

  fNeutrinoMomemtum = vpos - vReactorPos;
  fPositronMomemtum = fNeutrinoMomemtum;
  
  baseline = fNeutrinoMomemtum.Mag();
  
  v2 = fPositronMomemtum.Unit();

  v1 = fPositronMomemtum.Orthogonal();

  fPositronMomemtum.SetMag(p_e1);
  fPositronMomemtum.Rotate(theta,v1);
  fPositronMomemtum.Rotate(phi,v2);

  fNeutrinoMomemtum.SetMag(E_nue);

  fNeutronMomemtum = fNeutrinoMomemtum - fPositronMomemtum;

  E_pos = E_e1;
  
  return fNeutrinoMomemtum;
}

void PrintOutput(Int_t i, TVector3 ps, Double_t E_pos, TVector3 P_posi, TVector3 P_neu, int opt, FILE * pFile)
{
  if(opt == 0)
  {
    fprintf(pFile,"%d 2\n1 -11 0 0 0 0 %f %f %f %f %f %f %f %f %f\n",i,P_posi.X()*1e-3,P_posi.Y()*1e-3,P_posi.Z()*1e-3,E_pos*1e-3,m_e*1e-3,ps.X(),ps.Y(),ps.Z(),0);
    fprintf(pFile,"1 2112 0 0 0 0 %f %f %f %f %f %f %f %f %f\n",P_neu.X()*1e-3,P_neu.Y()*1e-3,P_neu.Z()*1e-3,sqrt(P_neu.Mag()*P_neu.Mag()+M_n*M_n)*1e-3,M_n*1e-3,ps.X(),ps.Y(),ps.Z(),0);
  }
  else if(opt == 1)
  {
    fprintf(pFile,"%d 1\n1 -11 0 0 0 0 %f %f %f %f %f %f %f %f %f\n",i,P_posi.X()*1e-3,P_posi.Y()*1e-3,P_posi.Z()*1e-3,E_pos*1e-3,m_e*1e-3,ps.X(),ps.Y(),ps.Z(),0);
  }
  else if(opt == 2)
  {
    fprintf(pFile,"%d 1\n1 2112 0 0 0 0 %f %f %f %f %f %f %f %f %f\n",i,P_neu.X()*1e-3,P_neu.Y()*1e-3,P_neu.Z()*1e-3,sqrt(P_neu.Mag()*P_neu.Mag()+M_n*M_n)*1e-3,M_n*1e-3,ps.X(),ps.Y(),ps.Z(),0);
  }
}

void AngraNuGen(int initrand = 0, int nevt = 1e2, Int_t opt = -1, Double_t Detector_Lx=1e3, Double_t Detector_Ly=1e3, Double_t Detector_Lz=1e3)
{
  
  delete gRandom;
  gRandom = new TRandom3(initrand);

  TVector3 ps;
  TVector3 P_posi, P_neu, P_nu;
  
  Double_t E_nue, E_pos, E_neu;
  
  seed = initrand;
  
  while((opt < 0) || (opt > 2))
  {
    cout<<"Choose one of the following options to print:\n 0: Print both Positron or Neutron event \n 1: Print only the Positron \n 2: Print only the Neutron\n";
    cin>>opt;
  }

  FILE *pFile;
  pFile = fopen("event.data","w");
  
  SetRefSpectrum();
  
  for(int i=0;i<nevt;i++)
  {
    ps = GetPosition(Detector_Lx, Detector_Ly, Detector_Lz);
//     cout << ps.X() << " " << ps.Y() << " "  << ps.Z() << endl;
    P_nu = GetIBDMomentum(E_pos, P_posi, P_neu, E_nue, ps);
    PrintOutput(i, ps, E_pos, P_posi, P_neu, opt, pFile);
  }
//hPeXY->Draw("colz");

}
