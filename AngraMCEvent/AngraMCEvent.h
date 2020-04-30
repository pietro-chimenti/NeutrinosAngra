#ifndef ANGRA_MC_EVENT
#define ANGRA_MC_EVENT

#include "TClonesArray.h"
#include "TRefArray.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TDatabasePDG.h"
#include "TROOT.h"
#include <iostream>


TDatabasePDG *fPDGTable = new TDatabasePDG();

enum AngraPMTLocation {kTarget=0, kInnerVeto=1, kBoxVeto=2};
enum AngraPMTLevel    {kAUp=0, kADown=1};

class AngraMCVertex : public TObject {

public:
   Int_t     fID;
   Float_t   fX;
   Float_t   fY;
   Float_t   fZ;
   Float_t   fT;
   Int_t     fNParticles;

public:
   AngraMCVertex() { }
   virtual ~AngraMCVertex(){ }
   
   inline Int_t GetID() {return fID;}

   inline Float_t GetX() {return fX;}
   inline Float_t GetY() {return fY;}
   inline Float_t GetZ() {return fZ;}
   inline Float_t GetT() {return fT;}
      
   inline Int_t GetNParticles() {return fNParticles;}

   ClassDef(AngraMCVertex,1)
};

class AngraMCParticle : public TObject {

public:
   Int_t     fID;
   Int_t     fPDG;
   Int_t     fTrackID;
   Int_t     fVertexID;
   Float_t   fPx;
   Float_t   fPy;
   Float_t   fPz;
   Float_t   fPolarizationX;
   Float_t   fPolarizationY;
   Float_t   fPolarizationZ;
   
   inline Int_t GetID() {return fID;}
   inline Int_t GetPDG() {return fPDG;}
   inline Int_t GetTrackID() {return fTrackID;}
   inline Int_t GetVertexID()  {return fVertexID;}
   inline Int_t GetVertex()  {return fVertexID;}

   inline Float_t GetPx() {return fPx;}
   inline Float_t GetPy() {return fPy;}
   inline Float_t GetPz() {return fPz;}

   inline Float_t GetPolX() {return fPolarizationX;}
   inline Float_t GetPolY() {return fPolarizationY;}
   inline Float_t GetPolZ() {return fPolarizationZ;}

public:
   AngraMCParticle() { }
   virtual ~AngraMCParticle(){ }

   ClassDef(AngraMCParticle,1)
};

class AngraMCTrack : public TObject {

public:
  
  
   Int_t   fID;
   Int_t   fParent;
   Int_t   fPDG;
   Float_t fGlobalTime;
   Float_t fLocalTime;
   
   Float_t fXi;
   Float_t fYi;
   Float_t fZi;
   Float_t fPix;           
   Float_t fPiy;           
   Float_t fPiz;           
   Float_t fKineticEi;
   TString fVoli;


   Float_t fXf;
   Float_t fYf;
   Float_t fZf;
   Float_t fPfx;           
   Float_t fPfy;           
   Float_t fPfz;           
   Float_t fKineticEf;
   TString fVolf;

   TString fProcess;

public:
   AngraMCTrack() { }
   virtual ~AngraMCTrack() { }
   
   inline Int_t GetID() {return fID;}
   inline Int_t GetParent() {return fParent;}
   inline Int_t GetPDG() {return fPDG;}

   inline Float_t GetLocalTime() {return fGlobalTime;} // time since the primary
   inline Float_t GetTrackTime() {return fLocalTime;}  // time of this track
   
   inline Float_t GetXi() {return fXi;}
   inline Float_t GetYi() {return fYi;}
   inline Float_t GetZi() {return fZi;}
   inline Float_t GetPxi() {return fPix;}
   inline Float_t GetPyi() {return fPiy;}
   inline Float_t GetPzi() {return fPiz;}
   inline Float_t GetKi() {return fKineticEi;}
   
   inline Float_t GetXf() {return fXf;}
   inline Float_t GetYf() {return fYf;}
   inline Float_t GetZf() {return fZf;}
   inline Float_t GetPxf() {return fPfx;}
   inline Float_t GetPyf() {return fPfy;}
   inline Float_t GetPzf() {return fPfz;}
   inline Float_t GetKf() {return fKineticEf;}

   inline TString GetVolume(Int_t i=0) {if (i) {return fVoli;} else return fVolf;}
   inline TString GetProcess() {return fProcess;}

   Bool_t IsSortable() const;
   virtual Int_t Compare(const TObject*) const;    

   ClassDef(AngraMCTrack,1)
};

class AngraMCTrackList : public TObject {
  
public:
  
  Int_t fTrackPosition;
  
  AngraMCTrackList() { }
  virtual ~AngraMCTrackList() { }
   
  ClassDef(AngraMCTrackList,1)
  
  
};


class AngraMCHit : public TObject {

using TObject::Compare;
using TObject::IsSortable;
  
public:
  
   Int_t    fID;
   Int_t    fTrackID;
   Double_t fGlobalTime;
   Double_t fLocalTime;   
   Float_t  fX;           //X of hit
   Float_t  fY;           //Y of hit
   Float_t  fZ;           //Z of hit
   Float_t  fEn;
   AngraPMTLocation fPMTLocation;
   AngraPMTLevel    fPMTLevel;
   Int_t            fPMTNumber;

public:
   AngraMCHit() { }
   virtual ~AngraMCHit() { }

   inline Int_t GetID()      {return fID;}
   inline Int_t GetTrackID() {return fTrackID;}
   inline Float_t GetX() {return fX;}
   inline Float_t GetY() {return fY;}
   inline Float_t GetZ() {return fZ;}

   inline Float_t GetGlobalTime(){return fGlobalTime;}
   inline Float_t GetLocalTime() {return fLocalTime;}
   inline Float_t GetEn()   {return fEn;}
   
   inline void SetGlobalTime(Double_t time) {fGlobalTime = time;}

   inline AngraPMTLocation GetPMTLocation() {return fPMTLocation;}
   inline AngraPMTLevel    GetPMTLevel()    {return fPMTLevel;}
   inline Int_t            GetPMTNumber()    {return fPMTNumber;}
   
   bool operator== (AngraMCHit *) const;
   bool operator<  (AngraMCHit *) const;
   bool operator>  (AngraMCHit *) const;
   bool operator<= (AngraMCHit *) const;
   bool operator>= (AngraMCHit *) const;
   
   Bool_t IsSortable() const;
   virtual Int_t Compare(const TObject*) const;    
   
   ClassDef(AngraMCHit,1)  //A p.e. hit
};


class AngraMCEvent : public TObject {

private:
   Int_t fNvertex;   //Number of vertexes
   Int_t fNparticle; //Number of primaries
   Int_t fNtrack;    //Number of tracks
   Int_t fNhit;      //Number of hits on PMTs
   Double_t fGlobalTime;
   Bool_t fHasTrackList;
   string fFileName;

   TClonesArray  *fVertexes;
   TClonesArray  *fParticles;
   TClonesArray  *fTracks;
   TClonesArray  *fHits;
      
   static TClonesArray *fgVertexes;
   static TClonesArray *fgParticles;
   static TClonesArray *fgTracks;
   static TClonesArray *fgHits;  
      
public:
    
   AngraMCEvent();
   virtual ~AngraMCEvent();
   void          NewEvent();
   void          Clear(Option_t *option ="");
   void          Reset(Option_t *option ="");
   void          BuildTrackList();
   Int_t         GetNvertex()    const { return fNvertex; }
   Int_t         GetNparticle()  const { return fNparticle; }
   Int_t         GetNtrack()     const { return fNtrack; }
   Int_t         GetNhit()       const { return fNhit; }
   Double_t      GetGlobalTime() const { return fGlobalTime; }
   string	 GetSourceFileName() const {return fFileName; }
   void          SetGlobalTime(Double_t time)  {fGlobalTime = time; }
   void 	 SetSourceFileName(const string name){fFileName = name; }
   void          SetNvertex(Int_t vtx)    { fNvertex=vtx; }
   void          SetNparticle(Int_t ptc)  { fNparticle=ptc; }
   void          SetNtrack(Int_t trk)     { fNtrack=trk; }
   void          SetNhit(Int_t hit)       { fNhit=hit; }
   AngraMCVertex   *AddVertex();
   AngraMCParticle *AddPrimary();
   AngraMCTrack    *AddTrack();
   AngraMCHit      *AddHit();
   
   inline void SortHits()   {fHits->Sort();}
   inline void SortTracks() {fTracks->Sort();}
   
   vector<Int_t> fTrackList;
   
   inline Bool_t  WasHit() { if (fNhit!=0) {return 1;} else {return 0;}}
   inline Float_t GetPDGMass(Int_t pdg) {return fPDGTable->GetParticle(pdg)->Mass()*1000.;}
   inline Float_t GetPDGCharge(Int_t pdg) {return fPDGTable->GetParticle(pdg)->Charge();}
   
   AngraMCVertex   *GetVertex(Long64_t i);
   AngraMCParticle *GetPrimary(Long64_t i=0);
   AngraMCTrack    *GetTrack(Long64_t i);   
   AngraMCHit      *GetHit(Long64_t i);
   
   ClassDef(AngraMCEvent,1)
};

#endif // ANGRA_MC_EVENT
