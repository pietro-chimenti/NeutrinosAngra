void RunPTF() {

  PulseTreeFactory aFactory;

  TFile *file = new TFile("/home/drc01/kemp/angra/data/G4_Root/noTracks/000001.photons_v1_1M_20131000_NT_MCEVENT.root");
  aFactory.runNum=1;
  TTree *tree = (TTree*)file->Get("AngraMC");
  aFactory.SetInTree(tree);
  TFile oFile("000001.photons_v1_1M_20131000_PULSES.root","recreate");
  oFile.SetCompressionLevel(1);
  TTree * pulseTree = new TTree("PulseTree","Tree of unmixed Pulses");
  aFactory.SetPulseTree(pulseTree);
  aFactory.Loop();
  aFactory.Finalize();
  pulseTree->Write();
  oFile.Write();
  delete pulseTree;
  //------------------------------------------------------------
  TFile *file = new TFile("/home/drc01/kemp/angra/data/G4_Root/noTracks/000002.electrons_v1_1M_20131000_NT_MCEVENT.root");
  aFactory.runNum=2;
  TTree *tree = (TTree*)file->Get("AngraMC");
  aFactory.SetInTree(tree);
  TFile oFile("000002.electrons_v1_1M_20131000_PULSES.root","recreate");
  oFile.SetCompressionLevel(1);
  TTree * pulseTree = new TTree("PulseTree","Tree of unmixed Pulses");
  aFactory.SetPulseTree(pulseTree);
  aFactory.Loop();
  aFactory.Finalize();
  pulseTree->Write();
  oFile.Write();
  delete pulseTree;
  //------------------------------------------------------------
  TFile *file = new TFile("/home/drc01/kemp/angra/data/G4_Root/noTracks/000003.positrons_v1_620k_20131000_NT_MCEVENT.root");
  aFactory.runNum=3;
  TTree *tree = (TTree*)file->Get("AngraMC");
  aFactory.SetInTree(tree);
  TFile oFile("000003.positrons_v1_620k_20131000_PULSES.root","recreate");
  oFile.SetCompressionLevel(1);
  TTree * pulseTree = new TTree("PulseTree","Tree of unmixed Pulses");
  aFactory.SetPulseTree(pulseTree);
  aFactory.Loop( 0, 621000);
  aFactory.Finalize();
  pulseTree->Write();
  oFile.Write();
  delete pulseTree;
  //------------------------------------------------------------
  TFile *file = new TFile("/home/drc01/kemp/angra/data/G4_Root/noTracks/000004.muons_v1_140k_20131000_NT_MCEVENT.root");
  aFactory.runNum=4;
  TTree *tree = (TTree*)file->Get("AngraMC");
  aFactory.SetInTree(tree);
  TFile oFile("000004.muons_v1_140k_20131000_PULSES.root","recreate");
  oFile.SetCompressionLevel(1);
  TTree * pulseTree = new TTree("PulseTree","Tree of unmixed Pulses");
  aFactory.SetPulseTree(pulseTree);
  aFactory.Loop( 0, 143000);
  aFactory.Finalize();
  pulseTree->Write();
  oFile.Write();
  delete pulseTree;
  //------------------------------------------------------------
  TFile *file = new TFile("/home/drc01/kemp/angra/data/G4_Root/noTracks/000005.protons_v1_20k_20131000_NT_MCEVENT.root");
  aFactory.runNum=5;
  TTree *tree = (TTree*)file->Get("AngraMC");
  aFactory.SetInTree(tree);
  TFile oFile("000005.protons_v1_20k_20131000_PULSES.root","recreate");
  oFile.SetCompressionLevel(1);
  TTree * pulseTree = new TTree("PulseTree","Tree of unmixed Pulses");
  aFactory.SetPulseTree(pulseTree);
  aFactory.Loop();
  aFactory.Finalize();
  pulseTree->Write();
  oFile.Write();
  delete pulseTree;
  //------------------------------------------------------------
  TFile *file = new TFile("/home/drc01/kemp/angra/data/G4_Root/noTracks/000006.pions_v1_10k_20131000_NT_MCEVENT.root");
  aFactory.runNum=6;
  TTree *tree = (TTree*)file->Get("AngraMC");
  aFactory.SetInTree(tree);
  TFile oFile("000006.pions_v1_10k_20131000_PULSES.root","recreate");
  oFile.SetCompressionLevel(1);
  TTree * pulseTree = new TTree("PulseTree","Tree of unmixed Pulses");
  aFactory.SetPulseTree(pulseTree);
  aFactory.Loop( 0, 10300);
  aFactory.Finalize();
  pulseTree->Write();
  oFile.Write();
  delete pulseTree;
  //------------------------------------------------------------
  TFile *file = new TFile("/home/drc01/kemp/angra/data/G4_Root/noTracks/000007.neutrons_v1_50k_20131000_NT_MCEVENT.root");
  aFactory.runNum=7;
  TTree *tree = (TTree*)file->Get("AngraMC");
  aFactory.SetInTree(tree);
  TFile oFile("000007.neutrons_v1_50k_20131000_PULSES.root","recreate");
  oFile.SetCompressionLevel(1);
  TTree * pulseTree = new TTree("PulseTree","Tree of unmixed Pulses");
  aFactory.SetPulseTree(pulseTree);
  aFactory.Loop();
  aFactory.Finalize();
  pulseTree->Write();
  oFile.Write();
  delete pulseTree;
  //------------------------------------------------------------
  TFile *file = new TFile("/home/drc01/kemp/angra/data/G4_Root/noTracks/000009.neutrinos_v1_70k_20131000_NT_MCEVENT.root");
  aFactory.runNum=9;
  TTree *tree = (TTree*)file->Get("AngraMC");
  aFactory.SetInTree(tree);
  TFile oFile("000009.neutrinos_v1_70k_20131000_PULSES.root","recreate");
  oFile.SetCompressionLevel(1);
  TTree * pulseTree = new TTree("PulseTree","Tree of unmixed Pulses");
  aFactory.SetPulseTree(pulseTree);
  aFactory.Loop();
  aFactory.Finalize();
  pulseTree->Write();
  oFile.Write();
  delete pulseTree;
  //------------------------------------------------------------

  cout << " Bye bye! " << endl;  
}

