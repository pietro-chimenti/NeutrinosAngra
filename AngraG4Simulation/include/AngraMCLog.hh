//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraMCLog header file
//
//  Authors: P.Chimenti
//
//  09-02-2010, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//==============================================================
//In this file are the Class responsible for logging the simulation
//results
//==============================================================

#ifndef AngraMCLog_h
#define AngraMCLog_h 1

#include "G4Event.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "AngraPMTHit.hh"
#include "AngraVetoHit.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"

#include <iostream>
#include <fstream>


enum HeaderOutLevelEnum {HOL_NONE,HOL_ALL};
enum EventOutLevelEnum {EOL_NONE,EOL_ALL};
enum TrajectoryOutLevelEnum {TROL_NONE,TROL_ALL};
enum TrackOutLevelEnum {TOL_NONE,TOL_ALL};
enum HitsOutLevelEnum {HIOL_NONE,HIOL_ALL};


class AngraMCLog
{
public:
  static AngraMCLog& Instance()
  {
    static AngraMCLog MCLsingleton;
    if(setup==false){
      setup=MCLsingleton.Init();
    }
    return MCLsingleton;
  }

  bool Init();
  bool SetOutFile(std::ofstream *oFile);
  bool SetInHepEvtFile(char * hepevFile);
  char* GetInHepEvtFile();

  bool SaveHeader();  
  bool SaveEvent(const G4Event* evt);
  bool SaveTrajectories(G4TrajectoryContainer* trajectoryContainer);
  bool SaveHits(AngraPMTHitsCollection* PHC);
  bool SaveHits(AngraVetoHitsCollection* VHC);
  bool SaveTrack(const G4Track* aTrack);

  void SetHeaderOutLevel(HeaderOutLevelEnum l_hol){hol=l_hol;}
  void SetEventOutLevel(EventOutLevelEnum l_eol){eol=l_eol;}
  void SetTrajectoryOutLevel(TrajectoryOutLevelEnum l_trol){trol=l_trol;}
  void SetTrackOutLevel(TrackOutLevelEnum l_tol){tol=l_tol;}
  void SetHitsOutLevel(HitsOutLevelEnum l_hiol){hiol=l_hiol;}

  HeaderOutLevelEnum GetHeaderOutLevel(){return hol;}
  EventOutLevelEnum GetEventOutLevel(){return eol;}
  TrajectoryOutLevelEnum GetTrajectoryOutLevel(){ return trol;}
  TrackOutLevelEnum GetTrackOutLevel(){return tol;}
  HitsOutLevelEnum GetHitsOutLevel(){return hiol;}



private:
  AngraMCLog() {}                                  // Private constructor
  ~AngraMCLog() {}
  AngraMCLog(const AngraMCLog&);                 // Prevent copy-construction
  AngraMCLog& operator=(const AngraMCLog&);      // Prevent assignment
  
  static bool setup;
  std::ostream *outFile;
  char * inputHepEvtFile;

  HeaderOutLevelEnum hol;
  EventOutLevelEnum  eol;
  TrajectoryOutLevelEnum trol;
  TrackOutLevelEnum tol;
  HitsOutLevelEnum hiol;

};

#endif
