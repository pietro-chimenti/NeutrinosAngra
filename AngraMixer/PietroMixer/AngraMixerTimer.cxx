#include "AngraMixerTimer.h"

#include <iostream>
#include <algorithm>
#include <iterator>

AngraMixerTimer::AngraMixerTimer( int seed ) : amtRandom( seed ), amtFrequencies(), amtTimes(), amtTotEvents(), amtInit(false), currentTime(0), currentGenerator(-1)
{
  std::cout << "Hello Angra Mixer Timer " << std::endl;
}

void AngraMixerTimer::PushBack(double aFrequency)
{
  if(amtInit) throw AngraMixerTimer::AngraMTError();
  amtFrequencies.push_back(aFrequency);
  amtTimes.push_back(amtRandom.Exp(aFrequency));
  amtTotEvents.push_back(0);
}

void AngraMixerTimer::Init()
{
  if(amtInit) throw AngraMixerTimer::AngraMTError();
  std::vector<double>::iterator minPosition=std::min_element(amtTimes.begin(), amtTimes.end());
  currentTime=*minPosition;
  currentGenerator=std::distance(amtTimes.begin(),minPosition);
  amtTotEvents[currentGenerator]+=1;
  amtInit=true;
}

double AngraMixerTimer::GetCurrentTime()
{
  if(!amtInit) throw AngraMixerTimer::AngraMTError();
  return currentTime;
}

int AngraMixerTimer::GetCurrentGenerator()
{
  if(!amtInit) throw AngraMixerTimer::AngraMTError();
  return currentGenerator;
}

double AngraMixerTimer::Next()
{
  if(!amtInit) throw AngraMixerTimer::AngraMTError();
  double aFrequency = amtFrequencies[currentGenerator];
  amtTimes[currentGenerator]+=amtRandom.Exp(aFrequency);
  
  std::vector<double>::iterator minPosition=std::min_element(amtTimes.begin(), amtTimes.end());
  currentTime=*minPosition;
  currentGenerator=std::distance(amtTimes.begin(),minPosition);
  amtTotEvents[currentGenerator]+=1;
  return currentTime;
}

int AngraMixerTimer::GetEvents(int generators)
{
  if(!amtInit) throw AngraMixerTimer::AngraMTError();
  if(0<=generators && generators<amtTotEvents.size())
    return amtTotEvents[generators];
  else throw AngraMixerTimer::AngraMTError();
}
