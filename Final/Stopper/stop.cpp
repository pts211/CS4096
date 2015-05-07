#include "Roomba.h"
#include <iostream>
using namespace std;


int main(int argc, char const *argv[]) 
{
  bool testMode = false;
  if(argc > 1)
    testMode = true;
  // Navigation defaultNav(testMode);
  if(testMode)
    cout << "Test Mode enabled" << endl;
  //Navigation realNav("BasementFactory.txt", testMode);
  // cout << "Power: "
  // defaultNav.inputNodes("BasementFactory.txt");
  // defaultNav.walkToStartingNode();

    Roomba r;
    r.start();
    //r.powerOn();
    r.drive(0,0);
    r.powerOff();

  return 0;
}

