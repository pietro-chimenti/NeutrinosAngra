//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  Cosmic neutrons generator (based on T.J.C.Bezerra script)
//
//  Author: T.J.C.Bezerra, P.Chimenti
//
//  17-3-2010 v0.1, 26-06-2013 v0.9 
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

void neutrons(int INI_RAND=9999,int MAXEVENTS=10000, TString file="event.hepev")
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
  Double_t neutronMass = 0.93956556;//units: GeV/c^2

  //
  // the model is from JEST89A JEDEC standard
  //

  // energy spectrum
  TF1 *histoFlux = new TF1("NeutronSpectrum","0.67*(1.006*10**-6 *exp(-0.35*(log(x))**2+2.1451*log(x))+1.011*10**-3*exp(-0.4106*(log(x))**2-0.667*log(x)))",1,10000);
  histoFlux->GetXaxis()->SetTitle("Energy [MeV]");
  histoFlux->GetYaxis()->SetTitle("Diferencial Flux [cm^{-2} s^{-1} MeV^{-1}]");

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
    TVector3  momentum3d;
    TVector3  position;
    Double_t  momentum, ltheta, phi, initX,initY,initZ, neutronEnergy;
  
  for(;ievent<MAXEVENTS; ievent++){

    outfile << ievent << " " << 1 << endl;
    
    // generate event variables
    momentum = histoFlux->GetRandom()*1e-3; // histoFlux was given in MeV, but momentum must be GeV.
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
    neutronEnergy = sqrt(momentum*momentum+neutronMass*neutronMass);
    
        outfile << 1 << " " << 2112  << " " //
	    << 0  << " " << 0  << " "//
	    << 0  << " " << 0  << " "//
	    << momentum3d.X()  << " " << momentum3d.Y()  << " " //
	    << momentum3d.Z()  << " " << neutronEnergy  << " " //
	    << neutronMass  << " " << position.X()*1000  << " " // position in mm
	    << position.Y()*1000  << " " << position.Z()*1000 << " " //
	    << 0 << endl;
        
  }


  delete histoFlux;
  delete thetaDistr;

}
