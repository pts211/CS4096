#include "Navigation.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[]) 
{
  bool testMode = false;
  if(argc > 1)
  	testMode = true;
  Navigation defaultNav(testMode);

  // cout << "Power: "
  defaultNav.inputNodes("SampleFactory.txt");
  defaultNav.walkToStartingNode();

    // Roomba r;
    // r.start();
    // r.powerOn();
    // r.drive(0,0);

  return 0;
}
