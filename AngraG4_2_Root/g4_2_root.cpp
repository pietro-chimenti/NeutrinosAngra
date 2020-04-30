// ===========================================
//
// G4 to Root macro
// 
// version 2.0
// 23/02/2013
//
// Authors: P. Chimenti and G. A. Valdiviesso
//
// ===========================================



int g4_2_root( TString inFile="SimulationOutput.G4", TString outFile="SimulationOutput.root" , Int_t split = 1, Int_t compression=0, Int_t max_event=-1)
{
  
  Int_t outCode = 0; // Output code: 0 unsuccessful and 1 successful
  
  gROOT->Reset();
  gSystem->Load("libPhysics");
//  gROOT->ProcessLine(".L /home/drc01/kemp/angra/v1/AngraMCEvent/AngraMCEvent.cxx+");
//  gROOT->ProcessLine(".L /home/drc01/kemp/angra/data/run_201310000/snapshot/AngraMCEvent/AngraMCEvent.cxx+");

  
  TString tag_header;  // tag holder for STANDARD header
  TString tag_event;   // tag holder for EVENT
  Int_t   event_num;   // event number  
  TString tag_vertexes;// tag holder for primaries VERTEXES
  Int_t   nvertx;      // number of vertexes
  TString tag_vertex;  // tag holder for a VERTEX
  TString tag_ppart;   // tag holder for a primary PARTICLE
  TString tag_track;   // tag holder for TRACK
  Int_t   num_tracks;  // number of tracks
  TString tag_trj;     // tag holder for a TRAJECTORIES
  Int_t   num_trjs;    // trajectories
  
  Int_t   points_pnt;  // points
  Int_t   num_hits;    // hits
  TString tag_hits;    //
  TString tag_vhits;   // veto hits
  Int_t   num_vhits;

// Other declarations

  Int_t total_hits = 0;
  // event
  Int_t evnum;
  string tag_time;
  Double_t event_time;
  string tag_file;
  string event_file;
  
  // vertex
  Double_t lx,ly,lz,lt;
  Int_t    num,lnp;
  
  // PPart
  Int_t    lppart_num;
  Double_t lppMass, lppCharge;
  Double_t lppPx, lppPy, lppPz;
  Double_t lppPolX,lppPolY,lppPolZ;
  Int_t    lppPDG, lppID;

  // track
  Int_t    lId_tr, lparentId_tr;
  string   lpartType_tr, lpartSubType_tr;
  Int_t    lpdgCode_tr;
  Double_t lpdgMass_tr;
  Double_t lvertexX_tr,lvertexY_tr,lvertexZ_tr;
  Double_t lmomentumDirX_tr,lmomentumDirY_tr,lmomentumDirZ_tr;
  Double_t lkinEnergy_tr;
  string   llogVolu_tr;
  Int_t    llogVoluNum_tr;
  Double_t lglobalTime_tr;
  Double_t llocalTime_tr;
  Double_t lposX_tr,lposY_tr,lposZ_tr;
  Double_t lkinEnerFinal_tr;
  Double_t lmomentumX_tr,lmomentumY_tr,lmomentumZ_tr;
  string   lfinalVol_tr;
  string   lcreatorProcess;
  
  // trajectory
  string tag_traj;
  Int_t   id_trj, trackId_trj, parentId_trj;
  string  partName_trj;
  Int_t   charge_trj;
  Int_t   pdgCode_trj;
  Float_t momentumX_trj, momentumY_trj, momentumZ_trj;
  Int_t points_trj;
    
  // hit  
  string tag_hit, logVolu_hit;
  Double_t  hitGlobalTime, hitLocalTime, hitX, hitY, hitZ, hitEnergy;
  Int_t hitTrack;
  Bool_t inOrder = 1;
  
  // veto hit  
  string tag_vhit,vhitVN;
  Int_t  vhitID;
  Float_t vhitED;
  Float_t vhitPx, vhitPy, vhitPz;
  Float_t vhitLPx, vhitLPy, vhitLPz;
  Float_t vhitLTime, vhitGTime;

  // Create Root File    
  TFile f(outFile.Data(),"recreate");
  f.SetCompressionLevel(compression);
  
  TTree *treeEvent = new TTree("AngraMC","Angra MC Event Tree");
  
  AngraMCEvent *event = new AngraMCEvent;
  treeEvent->Branch("Event","AngraMCEvent",&event,1048576,split);

  // start reading txt file
  ifstream    in;
  in.open(inFile.Data());
  
  if (!in.good()) {cout << "ERROR: Input file " <<inFile.Data()<<" broken or not found." << endl;}
  else{
    
  in >> tag_header;

  if (tag_header!="STANDARD" && tag_header!="EVENT"){
    cout << "ERROR: File " <<inFile.Data()<<" is currupted." << endl;}
  else{

  if (tag_header!="EVENT") {in >> tag_event;} else {tag_event = tag_header;}
    
  Int_t i_event=0;
  while (tag_event=="EVENT"&&(i_event<max_event||max_event<0)) {
    i_event++;
//    cout << i_event << "\t" << max_event << endl;
    in >> event_num;

//  *****************
//    cout << tag_event << "\t" << event_num << "\t" << i_event << "\t :: \t";
//  *****************

    event->NewEvent();

    in >> tag_file;

    if (tag_file=="FILE"){
       in >> event_file;
       event->SetSourceFileName(event_file);
       in >> tag_vertexes; // Load vertexes and primary particles
    }  else {
       event->SetSourceFileName(string(""));
       tag_vertexes = tag_file;
//        cout << "no file name" << endl;
    }

    in >> nvertx; // Load vertexes and primary particles

//  *****************
//    cout << nvertx << "\t";
//  *****************

    Int_t ip=0;

    AngraMCVertex   *vertex;
    AngraMCParticle *particle;
    
    for( Int_t i=0 ; i<nvertx ; i++ ){
      vertex = event->AddVertex();
      
      in >> tag_vertex;
      
      in >> num >> lx >> ly >> lz >> lt >> lnp;

      if (i==0) event->SetGlobalTime(lt);
      
      vertex->fID = num;
      vertex->fX  = lx;
      vertex->fY  = ly;
      vertex->fZ  = lz;
      vertex->fT  = lt;
      vertex->fNParticles = lnp;

      for( Int_t ii=0 ; ii<lnp ; ii++){
	in >> tag_ppart;
	in >> lppart_num;
	in >> lppMass >> lppCharge >> lppPDG;
	in >> lppPx   >> lppPy     >> lppPz;
	in >> lppID;
	in >> lppPolX >> lppPolY   >> lppPolZ;
	
	particle = event->AddPrimary();
	
	particle->fID       = lppart_num;
	particle->fVertexID = num;
	particle->fPDG      = lppPDG;
	particle->fTrackID  = lppID;
	particle->fPx = lppPx;
	particle->fPy = lppPy;
	particle->fPz = lppPz;
	particle->fPolarizationX = lppPolX;
	particle->fPolarizationY = lppPolY;
	particle->fPolarizationZ = lppPolZ;
      
      }
    }

   
    // Load tracks
    in >> tag_track;

    num_tracks=0;
            
    AngraMCTrack *track;
    
    while(tag_track == "TRACK"){
      track = event->AddTrack();
      
      in >> lId_tr >> lparentId_tr;
      in >> lpartType_tr >> lpartSubType_tr;
      in >> lpdgCode_tr;
      in >> lpdgMass_tr;
      in >> lvertexX_tr >> lvertexY_tr >> lvertexZ_tr;
      in >> lmomentumDirX_tr >> lmomentumDirY_tr >> lmomentumDirZ_tr;
      in >> lkinEnergy_tr;
      in >> llogVolu_tr;
      in >> lglobalTime_tr;
      in >> llocalTime_tr;
      in >> lposX_tr >> lposY_tr >> lposZ_tr;
      in >> lkinEnerFinal_tr;
      in >> lmomentumX_tr >> lmomentumY_tr >> lmomentumZ_tr;
      in >> lfinalVol_tr;
      in >> lcreatorProcess;

      track->fID = lId_tr;
      track->fParent = lparentId_tr;
      track->fPDG    = lpdgCode_tr;
      track->fGlobalTime = lglobalTime_tr;
      track->fLocalTime  = llocalTime_tr;
      track->fXi  = lvertexX_tr;
      track->fYi  = lvertexY_tr;
      track->fZi  = lvertexZ_tr;
      
      Float_t trackP = TMath::Sqrt(lkinEnergy_tr*(lkinEnergy_tr+2*lpdgMass_tr));
      
      track->fPix = trackP*lmomentumDirX_tr;
      track->fPiy = trackP*lmomentumDirY_tr;
      track->fPiz = trackP*lmomentumDirZ_tr;
      
      track->fKineticEi = lkinEnergy_tr;
      track->fVoli = llogVolu_tr;
     
      track->fXf = lposX_tr;
      track->fYf = lposY_tr;
      track->fZf = lposZ_tr;      
      track->fPfx = lmomentumX_tr;
      track->fPfy = lmomentumY_tr;
      track->fPfz = lmomentumZ_tr;
      track->fKineticEf = lkinEnerFinal_tr;
      track->fVolf = lfinalVol_tr;

      track->fProcess = lcreatorProcess;
      
      num_tracks++;
      in >> tag_track;
    }
//  *****************
//    cout << num_tracks << "\t";
//  *****************

    if (num_tracks!=0) event->SortTracks();    

    tag_trj = tag_track;
    
    if (tag_trj=="TRAJECTORIES"){
    // trajectories
    in >>  num_trjs;
    
    if (num_trjs>0){
//     MC_Trajectory traj;
//     Int_t npoints=0;
//     for(Int_t i = 0; i < num_trjs ; i++){
//       in >> tag_traj;
//       in >> id_trj >> trackId_trj >> parentId_trj;
//       in >> partName_trj;
//       in >> charge_trj;
//       in >> pdgCode_trj;
//       in >> momentumX_trj >> momentumY_trj >> momentumZ_trj;
//       in >> points_trj;
//       in >> tag_trj;
//       in >> points_trj;
//       
//       traj.id_trj=id_trj;
//       traj.trackId_trj=trackId_trj;
//       traj.parentId_trj=parentId_trj;
//       traj.partName_trj=partName_trj;
//       traj.charge_trj=charge_trj;
//       traj.pdgCode_trj=pdgCode_trj;
//       traj.momentumX_trj=momentumX_trj;
//       traj.momentumY_trj=momentumY_trj;
//       traj.momentumX_trj=momentumX_trj;
//       traj.points_trj=points_trj;
//       
// 
//       // points
//       for(Int_t ii = 0 ; ii < traj.points_trj; ii++){
// 	MC_Point po;
// 	in >> lx >> ly >> lz;
// 	point.x=lx;
// 	point.y=ly;
// 	point.z=lz;  
// 
// 	po.point_num=ii;
// 	po.point_traj=traj.id_trj;
// 	npoints++;
//       }
//     }
    } else { in >> tag_traj;}
    } else { tag_traj = tag_trj;}

    tag_hits = tag_traj;
    
    // Load Hits
    if (tag_hits=="HITS"){
      
    in >>  num_hits;

//  *****************
//    cout << num_hits << "\t";
//  *****************


    TString pmtName;
    for(Int_t i = 0; i < num_hits ; i++){
      
      AngraMCHit *hit;
      hit = event->AddHit();

      total_hits++;
      
      in >> tag_hit;
      in >> hitTrack;
      in >> hitLocalTime;
      in >> hitX;
      in >> hitY;
      in >> hitZ;
      in >> hitEnergy;
      in >> logVolu_hit;
      

      hit->fID      = i;
      hit->fTrackID = hitTrack;
      hit->fX       = hitX;
      hit->fY       = hitY;
      hit->fZ       = hitZ;
      hit->fGlobalTime = hitLocalTime + lt;
      hit->fLocalTime  = hitLocalTime;
      hit->fEn      = hitEnergy;

      pmtName = logVolu_hit;
      pmtName = pmtName.ReplaceAll("pmt","");
      pmtName = pmtName.ReplaceAll("_phys","");
      
      if (pmtName.Contains("Target"))   {
	 hit->fPMTLocation = kTarget;
         pmtName = pmtName.ReplaceAll("Target_","");
      } else if (pmtName.Contains("InnerVeto")) {
	 hit->fPMTLocation = kInnerVeto;
         pmtName = pmtName.ReplaceAll("InnerVeto_","");
      } else if (pmtName.Contains("BoxVeto"))   {
	 hit->fPMTLocation = kBoxVeto;
         pmtName = pmtName.ReplaceAll("BoxVeto_","");
      }
      
      if (pmtName.Contains("U")) {
	hit->fPMTLevel = kAUp;
	pmtName = pmtName.ReplaceAll("U","");
      } else if (pmtName.Contains("D")) {
	hit->fPMTLevel = kADown;
	pmtName = pmtName.ReplaceAll("D","");
      }
      
      hit->fPMTNumber = pmtName.Atoi();
    }
    
    if (num_hits!=0) {
       event->SortHits();
       // checking 
       inOrder = 1;
//       for (int k=0; k<num_hits-1; k++) {
//           inOrder *= ((event->GetHit(k+1)->GetLocalTime()-event->GetHit(k)->GetLocalTime())>0);        
//       }    
       //cout << inOrder << endl;
    }
    
    }
    
    // veto hits
    in >> tag_vhits;  
    in >>  num_vhits;
//     MC_VetoHit vhit;
//     for(Int_t i = 0; i < num_vhits ; i++){
//       in >> tag_vhit >> vhitID ;
//       in >> vhitVN;
//       in >> vhitED;
//       in >> vhitPx;
//       in >> vhitPy;
//       in >> vhitPz;
//       in >> vhitLPx;
//       in >> vhitLPy;
//       in >> vhitLPz;
//       in >> vhitLTime;
//       in >> vhitGTime;
//       
//       vhit.trID=vhitID;
//       vhit.VolumeName=vhitVN;
//       vhit.EDep=vhitED;
//       vhit.x=vhitPx;
//       vhit.y=vhitPy;
//       vhit.z=vhitPz;
//       vhit.lx=vhitLPx;
//       vhit.ly=vhitLPy;
//       vhit.lz=vhitLPz;
//       vhit.lt=vhitLTime;
//       vhit.gt=vhitGTime;
//       
//       vhit.num=i;
//       MC_VetoHit * vhitp = new(arVHit[i]) MC_VetoHit(vhit); // untested yet
//     }

    // Fill Tree
//     event->BuildTrackList();
    treeEvent->Fill();
    in >> tag_event;
//  *****************
//    cout << " #" << endl;
//  *****************

  }

// finalizing
//  treeEvent->Write();

  f.Write();
  f.Close();
  outCode = 1;
  }
  }
  cout << total_hits << endl;
  cout << hitLocalTime + lt << endl;
  return outCode;
}
