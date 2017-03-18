#include <iostream>
#include "AngraMixerTimer.h"


int main(int argc, char **argv)
{
  std::cout << "Hello MixerTimer!" << std::endl;

  try{

    AngraMixerTimer aTimer(1);
    aTimer.PushBack(1.5);
    aTimer.PushBack(5.1);
    aTimer.Init();
    std::cout << " timer: Generator " << aTimer.GetCurrentGenerator() << " Time " << aTimer.GetCurrentTime() << std::endl;
    while(aTimer.Next()<1000){
      std::cout << " timer: Generator " << aTimer.GetCurrentGenerator() << " Time " << aTimer.GetCurrentTime() << std::endl;
    }
    std::cout << " timer: total events generator 0 " << aTimer.GetEvents(0) << std::endl;
    std::cout << " timer: total events generator 1 " << aTimer.GetEvents(1) << std::endl;

  }
  catch(AngraMixerTimer::AngraMTError amte){

    std::cout << "Angra Mixer Timer error " << std::endl;

  }
  return 0;
}
