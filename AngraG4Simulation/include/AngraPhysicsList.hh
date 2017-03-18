//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraSimulation Physics list header file
//
//  Authors: P.Chimenti, R.Lima, K.Ribeiro
//
//  30-4-2008, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//==============================================================
//In this file are the classes and the methods that define the particles 
//and their interaction processes
//==============================================================

#ifndef AngraPhysicsList_h
#define AngraPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class AngraPhysicsList: public G4VUserPhysicsList
{
public:
  AngraPhysicsList();  
  ~AngraPhysicsList(); 

  void ConstructParticle(); 
  void ConstructProcess();
  void SetCuts();        

private:
    virtual void ConstructBosons();
    virtual void ConstructLeptons();
    virtual void ConstructMesons();
    virtual void ConstructBaryons();
    virtual void ConstructIons();

    virtual void ConstructGeneral();
    virtual void ConstructHadronic();
    virtual void ConstructEM();
    virtual void ConstructOp();
    virtual void ConstructNuclear();

  void AddLimits(); 

};

#endif

