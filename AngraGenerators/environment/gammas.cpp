//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  environmental gammas generator
//
//  Author: P.Chimenti
//
//  28-03-2012, v0.1, 26-06-2013 v0.9
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

void gammas(int INI_RAND=9999,int MAXEVENTS=10000, TString file="event.hepev")
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

  // spectrum from measurements at Angra

  // load spectrum
  ifstream inFile;
  inFile.open("environment/BKG_LabNe_G33_12h.TKA");
  int nbins=10000;
  TH1F * histoFlux =new TH1F ("GammaFlux","GammaFlux",nbins,0,0.008235);// histo up to 0.008235GeV
  Double_t inNumber;
  for(int i=0; i<8192; i++){
    inFile >> inNumber;
    histoFlux->SetBinContent(i+1,inNumber);
  }
  histoFlux->Draw();

  // now azimuthal angle
  TF1 *thetaDistr = new TF1("Theta Distribution","TMath::Sin(x)",0,pi);

  // open out file
  ofstream outfile;
  outfile.open(file);

  // start generating
  int ievent=0;
//   ROOT::Math::XYZVector v1();// why that? Misteries of cint!
    TVector3  momentum3d; // y is vertical, x and z horizonthal 
    TVector3  position;
    Double_t  momentum, ltheta, phi, initX,initY,initZ, gammaEnergy;
    
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
    gammaEnergy = sqrt(momentum*momentum);

    
    outfile << 1 << " " << 22  << " " //
	    << 0  << " " << 0  << " "//
	    << 0  << " " << 0  << " "//
	    << momentum3d.X()  << " " << momentum3d.Y()  << " " //
	    << momentum3d.Z()  << " " << gammaEnergy  << " " //
	    << 0  << " " << position.X()*1000.  << " " //
	    << position.Y()*1000.  << " " << position.Z()*1000. << " " //
	    << 0 << endl;        
  }

  delete histoFlux;
  delete thetaDistr; 
}
