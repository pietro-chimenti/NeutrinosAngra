//--------------------------------------------------------------
//--------------------------------------------------------------
//
//  AngraMCLog implementation file
//
//  Authors: P.Chimenti, G. Valdiviesso
//
//  9-2-2010, v0.01
// 23-04-2025, implementing thread-safe output file merging
//
//--------------------------------------------------------------
//--------------------------------------------------------------
//==============================================================
//In this file is the class responsible for logging the simulation
//results
//==============================================================

#include "AngraMCLog.hh"
#include "G4RunManager.hh"
#include "G4Threading.hh"
#include <fstream>
#include <iostream>
#include <vector>
#include <regex>
#include <algorithm>
#include <string>
#include <map>
#include <queue>
#include <utility>
#include <limits>
#include <sstream>

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
  // Initialize verbosity levels to default values
  hol = HOL_ALL;
  eol = EOL_ALL;
  trol = TROL_ALL;
  tol = TOL_ALL;
  hiol = HIOL_ALL;

  // Write header to the output file
  SaveHeader();

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

// Get the thread-specific output file name
G4String AngraMCLog::GetThreadOutputFileName(G4int threadId)
{
  return "SimulationOutput_thread" + G4String(std::to_string(threadId)) + ".G4";
}

// Merge thread-specific output files into a single file
bool AngraMCLog::MergeOutputFiles(const G4String& outputFileName)
{
  // Only the master thread should merge files
  if (G4Threading::IsWorkerThread()) {
    return false;
  }

  G4cout << "Merging output files into " << outputFileName << G4endl;

  // Get the number of threads
  G4int numThreads = G4Threading::GetNumberOfRunningWorkerThreads();

  // Check if there are any thread files to merge
  bool foundThreadFiles = false;
  for (G4int threadId = 0; threadId < numThreads; ++threadId) {
    G4String threadFileName = GetThreadOutputFileName(threadId);
    std::ifstream testFile(threadFileName);
    if (testFile.is_open()) {
      foundThreadFiles = true;
      testFile.close();
      break;
    }
  }

  if (!foundThreadFiles) {
    G4cout << "No thread output files found to merge." << G4endl;
    return false;
  }

  // Open the output file
  std::ofstream outFile(outputFileName);
  if (!outFile.is_open()) {
    G4cerr << "Error: Could not open output file " << outputFileName << G4endl;
    return false;
  }

  // Write header
  outFile << "STANDARD" << G4endl;

  // Count of total events processed
  G4int totalEvents = 0;

  // Set buffer size for more efficient I/O
  const std::size_t bufferSize = 65536; // 64KB buffer for better performance
  char* outBuffer = new char[bufferSize];
  outFile.rdbuf()->pubsetbuf(outBuffer, bufferSize);

  // Process each thread's output file
  for (G4int threadId = 0; threadId < numThreads; ++threadId) {
    G4String threadFileName = GetThreadOutputFileName(threadId);

    // Open the thread's output file
    std::ifstream threadFile(threadFileName, std::ios::binary);
    if (!threadFile.is_open()) {
      G4cerr << "Warning: Could not open thread output file " << threadFileName << G4endl;
      continue;
    }

    // Skip header line
    std::string line;
    std::getline(threadFile, line);

    // Process the file line by line to count events, but use efficient copying
    std::string content;
    G4int threadEvents = 0;

    // Read the entire file content after the header
    std::stringstream buffer;
    buffer << threadFile.rdbuf();
    content = buffer.str();

    // Count events in the content
    size_t pos = 0;
    while ((pos = content.find(" EVENT ", pos)) != std::string::npos) {
      threadEvents++;
      pos += 7; // Move past " EVENT "
    }

    // Write the content to the output file
    outFile << content;

    // Update total events count
    totalEvents += threadEvents;

    // Close the file
    threadFile.close();

    // Remove the thread's output file only if we successfully processed it
    if (threadEvents > 0) {
      if (std::remove(threadFileName.c_str()) != 0) {
        G4cerr << "Warning: Could not remove thread output file " << threadFileName << G4endl;
      }
    } else {
      G4cout << "Preserving thread output file " << threadFileName << " (no events processed)" << G4endl;
    }

    G4cout << "Processed " << threadEvents << " events from thread " << threadId << G4endl;
  }

  // Close the output file
  outFile.close();

  // Clean up the output buffer
  delete[] outBuffer;

  G4cout << "Successfully merged " << totalEvents << " events into " << outputFileName << G4endl;

  return true;
}
