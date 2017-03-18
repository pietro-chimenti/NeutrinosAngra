#ifndef AngraMCMixer_h
#define AngraMCMixer_h

#include <vector>
#include <list>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TRandom2.h"

#include "AngraMCPulse.h"
#include "PulseTree.h"
#include "AngraMixerTimer.h"

class AngraMCMixer {
 public:

  AngraMCMixer();

  void AddPrimary(PulseTree * aPulseTree, double rate);

  int GetNPrimaries();
  int GetNEvents(int primary);
  int GetNPulses(int primary, int event);

  AngraMCPulse GetAPulse(int primary, int event, int pulse);

  void Init(std::string fileName, int seed);
  void Finalize();

  void FillSequentially();
  void Fill(double time);

  class AngraMCMExepction {};

 private:

  int currentPrimary;
  int currentEvent;
  double currentTime;

  TFile outFile;
  TTree* theTree;
  AngraMCPulse * thePulseP;
  AngraMixerTimer * theTimer;
  TRandom2* theRGenerator;
  void  LoadPulse(int ip, AngraMCPulse &aPulse, PulseTree * aPulseTree);

  std::vector<std::vector< std::vector< AngraMCPulse > > > pulseDB;
  std::vector< double > rates;
  std::list<AngraMCPulse> pulseBuffer;
  unsigned int bufferSize;

  void StoreEvent();
  void Flush();
  void FlushAll();

  static const unsigned int buffersize = 1e6;
  static const unsigned int minbuffersize = 1e4;

};


#endif  // AngraMCMixer_h
