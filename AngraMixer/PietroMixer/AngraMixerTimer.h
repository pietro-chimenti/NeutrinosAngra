#ifndef AngraMixerTimer_h
#define AngraMixerTimer_h

#include "TRandom2.h"
#include <vector>
class AngraMixerTimer {
 public:
  
  AngraMixerTimer( int );

  void  PushBack(double aFrequency);
  void  Init();

  double GetCurrentTime();
  int    GetCurrentGenerator();
  int    GetEvents(int generator);

  double Next();

  class AngraMTError {
    
  };

 private:

  TRandom2 amtRandom;

  std::vector<double> amtFrequencies;
  std::vector<double> amtTimes;
  std::vector<int>    amtTotEvents;
  bool amtInit;

  double currentTime;
  int    currentGenerator;

};

#endif //AngraMixerTimer_h
