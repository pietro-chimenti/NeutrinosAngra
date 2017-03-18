{
  gSystem->Load("libPhysics");
  gROOT->ProcessLine(".L ./AngraMCEvent/AngraMCEvent.cxx+");
  gROOT->ProcessLine(".L ./AngraMCPulse/AngraMCPulse.cxx+");
}
