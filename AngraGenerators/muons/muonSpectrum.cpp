/*************************************************************************
 This is a function which is based on the surface muon spectrum
 that was developed after hep-ph/0604145.  It will return the rate of muon
 events at a given depth as a function of muon momentum and the 
 zenith angle.  Energy loss is calculated based on the geometric 
 path length to the surface and an energy loss of 217 MeV / mwe
 which is consistent with "standard rock".  The input and output 
 are defined as follows:
  Inputs:
      muMomentum  --  momentum of the detected muon in GeV
      cosTheta    --  cos of the zenith angle (no units)
      vDepth      --  The vertical depth directly over the detector in
                      meters of water equivalent (mwe)
      overAngle   --  Angle from flat (in degrees) of the overburden.
                      this yeilds a generic cone in which the peak of the 
                      overburden is assumed to be directly over the 
                      detector with height vDepth and base at detector 
                      level
                             0 = a flat overburden
                        1 - 89 = a cone 
                            -1 = a hemisphere (special case)
  Output:
      muonSpec    -- muon rate in Hz/cm2/sr/GeV

 D. Reyna  14 November 2006
 P.Chimenti: modified for C++ 24/08/2009 

**************************************************************************/

#include <iostream>
#include <math.h>

using namespace std;


double muonSpec(double muMomentum, double cosTheta, double vDepth, 
		double overAngle)
{
  double zeta, Eloss, Alpha, ctAlpha;
  double delta = 3.38;
  double result = 0;

  if (muMomentum < 0) {
    cout << "Error: unphysical muon momentum: " << muMomentum << endl;
    return(0);
  }

  /* Calculate energy loss for generic overburden */
  Eloss = vDepth;                                //hemisphere
  if ((overAngle >= 0) && (overAngle < 90)) {    //generic cone (includes flat)
    Alpha = (90 - overAngle)*3.14159265/180.;
    ctAlpha = cos(Alpha)/sin(Alpha);
    Eloss /= cosTheta + ctAlpha*sqrt(1-pow(cosTheta,2));
  }
  /* assume 217 MeV loss per meter of water equivalent which is roughly true
     for standard rock */
  Eloss *= 0.217;


  /* calculate rate based on the starting momentum at the surface */
  zeta = cosTheta*(muMomentum + Eloss) + delta;

  result = pow(cosTheta,2.94)*0.14*pow(zeta,-2.7)*
              (1/(1 + 1.11*zeta/115) + 1/(1 + 1.11*zeta/850));
    
  return(result);
}


