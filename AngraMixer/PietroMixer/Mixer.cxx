#include <stdlib.h>
#include <iostream>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TStopwatch.h"
#include "TString.h"

#include "AngraMCPulse.h"
#include "PulseTree.h"
#include "AngraMCMixer.h"

using namespace std;

int main(int argc, char **argv)
{
  std::cout << "Hello Mixer!" << std::endl;

  TString outFileName = "mixed.root";
  TString prefix      = "./";
  TString sSeed, sTime;
  int seed = 20142015;
  double mixTime = 3600.;

  if(argc>=2){
    // arg 1 must be SEED
    sSeed = argv[1];
    seed=sSeed.Atoi();
    // arg 2 must be Output File Name
    if (argc>=3) {
	outFileName = argv[2];
       if (argc>=4) {
	  prefix = argv[3];
	  if(argc>=5){
	    sTime = argv[4];
            mixTime = sTime.Atoi();
	  }
	}
    }
  }

cout << "Using "<< seed << " as random seed" << endl;
cout << "Prefix "<< prefix << endl;
cout << "File   "<< outFileName << endl;
cout << "Time   "<< mixTime << endl;

  //load first tree

  TStopwatch aWatch;
  aWatch.Start();

  TFile f1("/home/drc01/kemp/angra/data/Pulses/000001.photons_v1_1M_20131000_PULSES.root");
  TTree* pulseTreeP1;
  f1.GetObject("PulseTree;2",pulseTreeP1);
  PulseTree pt1(pulseTreeP1);
  AngraMCMixer aMixer;
  aMixer.AddPrimary(&pt1,1./855.);
  f1.Close();

  TFile f2("/home/drc01/kemp/angra/data/Pulses/000002.electrons_v1_1M_20131000_PULSES.root");      
  TTree* pulseTreeP2;
  f2.GetObject("PulseTree;2",pulseTreeP2);
  PulseTree pt2(pulseTreeP2);
  aMixer.AddPrimary(&pt2,1./71.0);
  f2.Close();


  TFile f3("/home/drc01/kemp/angra/data/Pulses/000003.positrons_v1_621000_20131000_PULSES.root");  
  TTree* pulseTreeP3;
  f3.GetObject("PulseTree;2",pulseTreeP3);
  PulseTree pt3(pulseTreeP3);
  aMixer.AddPrimary(&pt3,1./68.4);
  f3.Close();

//  TFile f4("/home/drc01/kemp/angra/data/Pulses/000004.muons_v1_143100_20131000_PULSES.root");      
//  TTree* pulseTreeP4;
//  f4.GetObject("PulseTree;2",pulseTreeP4);
//  PulseTree pt4(pulseTreeP4);
//  aMixer.AddPrimary(&pt4,1./3);
//  f4.Close();

  TFile f5("/home/drc01/kemp/angra/data/Pulses/000005.protons_v1_20k_20131000_PULSES.root");       
  TTree* pulseTreeP5;
  f5.GetObject("PulseTree;2",pulseTreeP5);
  PulseTree pt5(pulseTreeP5);
  aMixer.AddPrimary(&pt5,1./6.38);
  f5.Close();

  TFile f6("/home/drc01/kemp/angra/data/Pulses/000006.pions_v1_10300_20131000_PULSES.root");       
  TTree* pulseTreeP6;
  f6.GetObject("PulseTree;2",pulseTreeP6);
  PulseTree pt6(pulseTreeP6);
  aMixer.AddPrimary(&pt6,1./0.962);
  f6.Close();

  TFile f7("/home/drc01/kemp/angra/data/Pulses/000007.neutrons_v1_50k_20131000_PULSES.root");
  TTree* pulseTreeP7;
  f7.GetObject("PulseTree;2",pulseTreeP7);
  PulseTree pt7(pulseTreeP7);
  aMixer.AddPrimary(&pt7,1./112.9);
  f7.Close();

/*  TFile f9("/home/drc01/kemp/angra/data/Pulses/000009.neutrinos_v1_71800_20131000_PULSES.root");
  TTree* pulseTreeP9;
  f9.GetObject("PulseTree;2",pulseTreeP9);
  PulseTree pt9(pulseTreeP9);
  aMixer.AddPrimary(&pt9,1./0.053);
  f9.Close();
*/
  TFile f11("/home/drc01/kemp/angra/data/Pulses/000011.muons_v1_250k_20131001_PULSES.root");
  TTree* pulseTreeP11;
  f11.GetObject("PulseTree;2",pulseTreeP11);
  PulseTree pt11(pulseTreeP11);
  aMixer.AddPrimary(&pt11,1./76.14);
  f11.Close();

  TFile f12("/home/drc01/kemp/angra/data/Pulses/000012.muons_v1_250k_20131002_PULSES.root");
  TTree* pulseTreeP12;
  f12.GetObject("PulseTree;2",pulseTreeP12);
  PulseTree pt12(pulseTreeP12);
  aMixer.AddPrimary(&pt12,1./76.14);
  f12.Close();

  TFile f13("/home/drc01/kemp/angra/data/Pulses/000013.muons_v1_250k_20131003_PULSES.root");
  TTree* pulseTreeP13;
  f13.GetObject("PulseTree;2",pulseTreeP13);
  PulseTree pt13(pulseTreeP13);
  aMixer.AddPrimary(&pt13,1./76.14);
  f13.Close();

  TFile f14("/home/drc01/kemp/angra/data/Pulses/000014.muons_v1_250k_20131004_PULSES.root");
  TTree* pulseTreeP14;
  f14.GetObject("PulseTree;2",pulseTreeP14);
  PulseTree pt14(pulseTreeP14);
  aMixer.AddPrimary(&pt14,1./75.87);
  f14.Close();

  aWatch.Print();
  aWatch.Continue();
  std::cout << " sizes " << aMixer.GetNPrimaries() << " " //
	    << aMixer.GetNEvents(1) << " " //
	    << aMixer.GetNPulses(1,10) << std::endl;
  std::cout << " a pulse from db " << aMixer.GetAPulse(1,2,0).run1 //
	    << " " << aMixer.GetAPulse(1,2,0).event1 <<  std::endl;

  TString fName = prefix + outFileName;
  aMixer.Init(fName.Data(),seed);
  //aMixer.FillSequentially();
  aWatch.Print();
  aWatch.Continue();
  aMixer.Fill(mixTime);
  aWatch.Print();
  aWatch.Continue();
  aMixer.Finalize();
  aWatch.Print();
  return 0;
}
