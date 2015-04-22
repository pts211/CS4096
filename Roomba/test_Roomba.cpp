/************************************************************
	Filename: test_Roomba.cpp
	Programmer: Paul Sites
  Date: 2015.04.21
	Desc:
		This is the unit-test file to test the Roobma!
************************************************************/
#if google_test_enabled

#include <iostream>
#include <unistd.h>
#include <gtest/gtest.h>

using std::cout;
using std::endl;

//bool pass;
string ans;

TEST(RoombaTest, Power_On)
{
  cin.ignore();
  /********** TEST **********/
  Roomba r;
  r.start();
  
  r.powerOn();
  
  /********** TEST **********/
  getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

TEST(RoombaTest, Power_Off)
{
  cin.ignore();
  /********** TEST **********/
  Roomba r;
  r.start();
  
  r.powerOff();
  
  /********** TEST **********/
  getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

TEST(RoombaTest, Power_Off)
{
  cin.ignore();
  /********** TEST **********/
  Roomba r;
  r.start();
  
  r.powerOff();
  
  /********** TEST **********/
  getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

#endif
