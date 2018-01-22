//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraMCLog implementation file
//
//  Authors: P.Chimenti 
//
//  9-2-2010, v0.01 
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//==============================================================
//In this file is the class responsible for logging the simulation
//results
//==============================================================

#include "AngraMCLog.hh"

using namespace CLHEP;

bool AngraMCLog::setup=false;

bool AngraMCLog::Init()
{
  outFile= &std::cout;
  return true;
}

bool AngraMCLog::SetOutFile(std::ofstream *oFile)
{
  outFile=oFile;
  return true;
}
bool AngraMCLog::SetInHepEvtFile(char * hepevtFile)
{
  inputHepEvtFile = hepevtFile;
  return true;
}

char* AngraMCLog::GetInHepEvtFile()
{
  return inputHepEvtFile;
}

bool AngraMCLog::SaveHeader()
{
  if(hol==HOL_ALL)
    *outFile << "STANDARD" << std::endl;
  return true;
}

bool AngraMCLog::SaveEvent(const G4Event* evt)
{
  if(eol==EOL_ALL){

    *outFile << " EVENT " << evt->GetEventID() << std::endl;
    int nvertx = evt->GetNumberOfPrimaryVertex();
    *outFile << " VERTEXES " << nvertx << std::endl;

    int i = 0;
    for( i=0 ; i<nvertx ; i++ ){
      G4PrimaryVertex * vertex = evt->GetPrimaryVertex (i);
      *outFile << " VERTEX " << i << " " ;
      *outFile << vertex->GetX0()*mm  << " " ;
      *outFile << vertex->GetY0()*mm  << " " ;
      *outFile << vertex->GetZ0()*mm  << " " ;
      *outFile << vertex->GetT0()*ns  << " " ;
      *outFile << vertex->GetNumberOfParticle()  << " " ;
      *outFile << std::endl;

      int npart = vertex->GetNumberOfParticle();
      int ii = 0;
      for( ii=0 ; ii<npart ; ii++ ){
	G4PrimaryParticle * particle = vertex->GetPrimary(ii);
	*outFile << " PARTICLE " << ii << " " ;
	*outFile << particle->GetMass() << " " ;
	*outFile << particle->GetCharge() << " " ;
	*outFile << particle->GetPDGcode() << " " ;
	*outFile << particle->GetPx() << " " ;
	*outFile << particle->GetPy() << " " ;
	*outFile << particle->GetPz() << " " ;
	*outFile << particle->GetTrackID() << " " ;
	*outFile << particle->GetPolX() << " " ;
	*outFile << particle->GetPolY() << " " ;
	*outFile << particle->GetPolZ() << " " ;
	*outFile << std::endl;	
      }
    }
  }
  return true;
}

bool AngraMCLog::SaveTrajectories(G4TrajectoryContainer* trajectoryContainer)
{
  if(trol==TROL_ALL){
    int n_trajectories = 0;
    if (trajectoryContainer) n_trajectories = trajectoryContainer->entries();
    *outFile << " TRAJECTORIES " << " " << n_trajectories  << G4endl;
    for(int i=0; i< n_trajectories; i++){
      *outFile << " TRAJECTORY   " << i  << " ";
      *outFile << (*trajectoryContainer)[i]->GetTrackID() << " ";
      *outFile << (*trajectoryContainer)[i]->GetParentID() << " ";
      *outFile << (*trajectoryContainer)[i]->GetParticleName() << " ";
      *outFile << (*trajectoryContainer)[i]->GetCharge() << " ";
      *outFile << (*trajectoryContainer)[i]->GetPDGEncoding() << " ";
      *outFile << (*trajectoryContainer)[i]->GetInitialMomentum()[0] << " ";
      *outFile << (*trajectoryContainer)[i]->GetInitialMomentum()[1] << " ";
      *outFile << (*trajectoryContainer)[i]->GetInitialMomentum()[2] << " ";
      *outFile << (*trajectoryContainer)[i]->GetPointEntries() << " ";
      *outFile << G4endl;
      
      *outFile << " POINTS   " <<(*trajectoryContainer)[i]->GetPointEntries() << " ";
      for(int j=0; j<(*trajectoryContainer)[i]->GetPointEntries() ; j++){
	*outFile << (*trajectoryContainer)[i]->GetPoint(j)->GetPosition()[0] << " ";
	*outFile << (*trajectoryContainer)[i]->GetPoint(j)->GetPosition()[1] << " ";
	*outFile << (*trajectoryContainer)[i]->GetPoint(j)->GetPosition()[2] << " ";
      }
      *outFile << G4endl;
    } 
  }
  return true;
}

bool AngraMCLog::SaveHits(AngraPMTHitsCollection* PHC)
{
  if(hiol==HIOL_ALL){
    int pmts=PHC->entries();
    *outFile << " HITS " << " " << pmts  << G4endl;
    for(int i=0;i<pmts;i++){
      *outFile << " HIT "  << (*PHC)[i]->GetTrack() << " " <<    (*PHC)[i]->GetTime()*ns <<  " "       
	       << (*PHC)[i]->GetPos().x()*mm << " "
	       << (*PHC)[i]->GetPos().y()*mm << " "
	       << (*PHC)[i]->GetPos().z()*mm << " "
	       << (*PHC)[i]->GetEn() << " " 
	       << (*PHC)[i]->GetPMTPhysVol()->GetName() << std:: endl;
    }  
  }
  return true;
}

bool AngraMCLog::SaveHits(AngraVetoHitsCollection* VHC)
{
  if(hiol==HIOL_ALL){
    int hits=VHC->entries();
    *outFile << " VETOHITS " << " " << hits  << G4endl;
    for(int i=0;i<hits;i++){
      *outFile << " VETOHIT "  << (*VHC)[i]->GetTrackID() << " "
	       << (*VHC)[i]->GetVolName() << " "
	       << (*VHC)[i]->GetEdep() << " "
	       << (*VHC)[i]->GetPos().x()*mm << " "
	       << (*VHC)[i]->GetPos().y()*mm << " "
	       << (*VHC)[i]->GetPos().z()*mm << " "
	       << (*VHC)[i]->GetLocalPos().x()*mm << " "
	       << (*VHC)[i]->GetLocalPos().y()*mm << " "
	       << (*VHC)[i]->GetLocalPos().z()*mm << " " 
               << (*VHC)[i]->GetLocalTime()*ns << " "
               << (*VHC)[i]->GetGlobalTime()*ns << std:: endl;
    }  
  }
  return true;
}

bool AngraMCLog::SaveTrack(const G4Track* aTrack)
{
 if(tol==TOL_ALL){
          
    *outFile << "TRACK" << " ";
    //IDs
    *outFile << aTrack->GetTrackID() << " " << aTrack->GetParentID() << " ";
    
    //particle definition
    *outFile << aTrack->GetDefinition()->GetParticleType() << " " << aTrack->GetDefinition()->GetParticleSubType() << " ";
    
    if (aTrack->GetDefinition()->GetParticleType()!="opticalphoton"){
       *outFile << aTrack->GetDefinition()->GetPDGEncoding() << " ";
    } else *outFile << "22 ";
       
    *outFile <<  aTrack->GetDefinition()->GetPDGMass() << " " ;
    
    //vertex position, momentum, kinetic energy and volume
    *outFile << aTrack->GetVertexPosition().x()*mm << " "  << aTrack->GetVertexPosition().y()*mm << " "  << aTrack->GetVertexPosition().z()*mm << " " << aTrack->GetVertexMomentumDirection().x() << " " << aTrack->GetVertexMomentumDirection().y() << " " << aTrack->GetVertexMomentumDirection().z() << " " << aTrack->GetVertexKineticEnergy() << " " << aTrack->GetLogicalVolumeAtVertex()->GetName() << " "; 
    
    //time, position, energy, momentum
    *outFile << aTrack->GetGlobalTime()*ns << " "  << aTrack->GetLocalTime()*ns << " " << aTrack->GetPosition().x()*mm << " "  << aTrack->GetPosition().y()*mm << " "  << aTrack->GetPosition().z()*mm << " " << aTrack->GetKineticEnergy() << " " << aTrack->GetMomentum().x() << " " << aTrack->GetMomentum().y() << " " << aTrack->GetMomentum().z() << " " << aTrack->GetVolume()->GetName() << " ";

    //creator process
    *outFile << ( aTrack->GetCreatorProcess() ? (const char *)( aTrack->GetCreatorProcess()->GetProcessName() ) : "NULL_CREATOR_PROCESS" );
    
    *outFile << std::endl;
  }
  return true;
}


