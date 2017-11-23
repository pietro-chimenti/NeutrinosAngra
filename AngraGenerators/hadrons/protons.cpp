//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  Cosmic protons generator
//
//  Author: P.Chimenti
//
//  14-9-2010 v0.1, 10-06-2013 v0.9 
//
//--------------------------------------------------------------
//--------------------------------------------------------------

#include "TMatrixD.h"
#include "TVectorD.h"
#include "TMath.h"


#ifndef __CINT__
#include "Math/Point3D.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"
#include "Math/Rotation3D.h"
#include "Math/EulerAngles.h"
#include "Math/AxisAngle.h"
#include "Math/Quaternion.h"
#include "Math/RotationX.h"
#include "Math/RotationY.h"
#include "Math/RotationZ.h"
#include "Math/RotationZYX.h"
#include "Math/LorentzRotation.h"
#include "Math/Boost.h"
#include "Math/BoostX.h"
#include "Math/BoostY.h"
#include "Math/BoostZ.h"
#include "Math/Transform3D.h"
#include "Math/Plane3D.h"
#include "Math/VectorUtil.h"

using namespace ROOT::Math;

#endif

void protons(int INI_RAND=9999,int MAXEVENTS=10000, TString file="event.hepev")
{

  // ROOT initialization
#ifdef __CINT__
  gSystem->Load("libMathCore");
  using namespace ROOT::Math;
#endif
  delete gRandom;
  gRandom = new TRandom3(INI_RAND);

  // constant definitions
  const Double_t pi = TMath::Pi();

  //geometric definitions
  const Double_t SURF_HALF_LEN=2.5;//units: m 
  const Double_t SURF_DIST=4.0;//units: m

  //physical definitions
  const Double_t  protonMass=0.938272046;//units: GeV/c^2 

  //
  // The model is from P.K.F.Grieder, "Cosmic rays at earth", pg 321
  // angular dependence: n=2.0
  // 

  // points taken with gimp: need to transform to physical values
  // set initial values of spectrum data
  // values to tranform points to momentum or flux
  // flux in cm^-2 s^-1 sr^-1 GeV^-1
  // energy (momentum) in GeV (/c)

  // set initial values of spectrum data
  // values to tranform points to momentum or flux

  const double delPoints=128.1;
  const double iniPx=1082;
  const double finPx=3644;
  const double iniPy=4044;
  const double finPy=384;
  const double iniMx=-1.;
  const double finMx=1.;
  const double iniFy=-7.;
  const double finFy=-3.;

  // data vectors
  const int nbins=20;
  double xPoints[nbins+1]={
    1116.05-delPoints/2., 1244.15-delPoints/2., 1372.25-delPoints/2.,
    1500.35-delPoints/2., 1628.45-delPoints/2., 1756.55-delPoints/2.,
    1884.65-delPoints/2., 2012.75-delPoints/2., 2141.85-delPoints/2.,
    2269.95-delPoints/2., 2397.05-delPoints/2., 2525.15-delPoints/2.,
    2653.25-delPoints/2., 2781.35-delPoints/2., 2909.45-delPoints/2.,
    3037.55-delPoints/2., 3166.65-delPoints/2., 3294.75-delPoints/2.,
    3422.25-delPoints/2., 3500.35-delPoints/2., 3500.35+delPoints/2.
  };

  double yPoints[nbins]={
    1870, 1704, 1543,
    1357, 1261, 1144,
    1065, 1036, 1069,
    1228, 1422, 1618,
    1816, 2026, 2238,
    2445, 2653, 2874,
    3091, 3307 };

  // now translate point coordinates to momentum and flux
  double xMomentumLog[nbins+1];
  double xMomentum[nbins+1];
  for(int i=0; i<nbins+1; i++){
    xMomentumLog[i]=((xPoints[i]-iniPx)/(finPx-iniPx))*(finMx-iniMx)+iniMx;
    xMomentum[i]=pow(10,xMomentumLog[i]);
  }

  double yFluxLog[nbins+1];
  double yFlux[nbins+1];
  for(int i=0; i<nbins; i++){
    yFluxLog[i]=((yPoints[i]-iniPy)/(finPy-iniPy))*(finFy-iniFy)+iniFy;
    yFlux[i]=pow(10,yFluxLog[i]);
  }

  // now set histo
  TH1F* histoFlux =new TH1F ("protonFlux","protonFlux",nbins,xMomentum);
  for(int i=0; i<nbins; i++){
    histoFlux->SetBinContent(i+1,yFlux[i]);
  }

  // just check histo
//   histoFlux->Draw();

  
  // now azimuthal angle
  double thetaIndex=2.;
  TF1 *thetaDistr = new TF1("Theta Distribution","TMath::Sin(x)*(TMath::Cos(x))**[0]",0,pi/2);
  thetaDistr->SetParameter(0,thetaIndex);

  // open out file
  ofstream outfile;
  outfile.open(file);

  // start generating
  int ievent=0;
//   ROOT::Math::XYZVector v1();// why that? Misteries of cint!
  TVector3  momentum3d; // y is vertical, x and z horizonthal 
  TVector3  position;
  Double_t  momentum, ltheta, phi, initX,initY,initZ, protonEnergy;
 
  for(;ievent<MAXEVENTS; ievent++){

    outfile << ievent << " " << 1 << endl;
    
    // generate event variables
    momentum = histoFlux->GetRandom();
    ltheta = thetaDistr->GetRandom();
    phi = gRandom->Uniform(0.,2*pi);
 

    // generate verticla particle and then rotates
    // momentum first
    momentum3d = TVector3(0.0, -1.0*momentum, 0.0); // y is vertical, x and z horizonthal 
    momentum3d.RotateX(ltheta);
    momentum3d.RotateY(phi);

    // position
    initX = gRandom->Uniform(-1.*SURF_HALF_LEN,SURF_HALF_LEN);
    initZ = gRandom->Uniform(-1.*SURF_HALF_LEN,SURF_HALF_LEN);
    initY = SURF_DIST;
    position = TVector3(initX,initY,initZ);
    position.RotateX(ltheta);
    position.RotateY(phi);
    
    
    // energy
    protonEnergy = sqrt(momentum*momentum+protonMass*protonMass);
    
    outfile << 1 << " " << 2212  << " " //
	    << 0  << " " << 0  << " "//
	    << 0  << " " << 0  << " "//
	    << momentum3d.X()  << " " << momentum3d.Y()  << " " //
	    << momentum3d.Z()  << " " << protonEnergy  << " " //
	    << protonMass  << " " << position.X()*1000  << " " // position in mm
	    << position.Y()*1000  << " " << position.Z()*1000 << " " // position in mm
	    << 0 << endl;        
  }

  delete histoFlux;
  delete thetaDistr;  
}
