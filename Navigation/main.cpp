#include "Navigation.h"
#include <iostream>
using namespace std;


int main(int argc, char const *argv[]) 
{
  bool testMode = false;
  if(argc > 1)
    if (argv[1] == "1")
  	  testMode = true;
  // Navigation defaultNav(testMode);

  Navigation realNav("BasementFactory.txt", testMode);
  // cout << "Power: "
  // defaultNav.inputNodes("BasementFactory.txt");
  // defaultNav.walkToStartingNode();

    // Roomba r;
    // r.start();
    // r.powerOn();
    // r.drive(0,0);

  return 0;
}

