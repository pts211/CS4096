/************************************************************
	Filename: Driver.cpp
	Programmer: Paul Sites
  Date: 2015.04.14
	Desc: Driver fr testing the Roomba.
************************************************************/             
#define google_test_enabled true

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <ftdi.h>
inline void delay( unsigned long ms ){usleep( ms * 1000 );}
#include "Roomba.h"

#if google_test_enabled
  #include <gtest/gtest.h>
  #include "test_Roomba.cpp"
#endif


using namespace std;

int main( int argc, char * argv[] )
{

#if google_test_enabled
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
#else

  Roomba r;
  cout<<"Starting the Roomba."<<endl;
  r.start();
  r.powerOn();
  delay(2000);
  //r.delay(5000);
  r.powerOff();
  
  //cout<<"Setting Roomba mode to FULL."<<endl;
  //r.setMode(Mode::FULL);
  //r.setBaud(Baud::B9600);

  return 0;
#endif   
}