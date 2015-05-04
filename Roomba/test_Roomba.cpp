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


bool AUTO_ADVANCE = false;
bool AUTO_PASS = true;
int DELAY_MS = 100;

TEST(RoombaTest, LED_Power)
{
  /********** TEST **********/
  cout<<"Please wait..."<<endl;
  /*
  Roomba r;
  r.start();
  r.powerOn();
  
  for(int i=0; i<255; i=i+5){
    r.setLED(0, false, 0, i);
    delay(5);
  }
  delay(500);
  for(int i=255; i>0; i=i-5){
    r.setLED(0, false, 0, i);
    delay(5);
  }
  
  r.setLED(0, false, 0, 255);
  delay(500);
  for(int j=0; j<255; j=j+5) {
    r.setLED(0, false, j, 255);
    delay(5);
  }
  delay(500);
  for(int j=255; j>0; j=j-5) {
    r.setLED(0, false, j, 255);
    delay(5);
  }
   */
  /********** TEST **********/

  cout<<"Pass/Fail? ";
  AUTO_PASS?(ans=="t"):getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
  //r.setLED(0, false, 0, 0);
}

TEST(RoombaTest, LED_Others)
{
  /********** TEST **********/
  cout<<"Please wait..."<<endl;
  
  Roomba r;
  r.start();
  r.powerOn();
  
  for(int i=0; i<6; i++){
    r.setLED(i, true);
    delay(200);
  }
  for(int i=0; i<6; i++){
    r.setLED(i, false);
    delay(200);
  }
  
   
  /********** TEST **********/

  cout<<"Pass/Fail? ";
  AUTO_PASS?(ans=="t"):getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
  r.setLED(0, false, 0, 0);
}

TEST(RoombaTest, Sensors_Packet_ALL)
{
  cout<<"Ready?"<<endl;
  cin.ignore();
  /********** TEST **********/
  cout<<"Please wait..."<<endl;
  
  Roomba r;
  r.start();
  r.powerOn();
  
  cout<<"Ready?"<<endl;
  cin.ignore();
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  delay(1000);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  delay(1000);
  r.powerOff();
  /********** TEST **********/
  cout<<"Pass/Fail? ";
  getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

TEST(RoombaTest, Power_On)
{
  cout<<"Ready?"<<endl;
  if(!AUTO_ADVANCE){ cin.ignore(); }
  /********** TEST **********/
  
  Roomba r;
  r.start();
  
  r.powerOn();
  
  /********** TEST **********/
  
  cout<<"Pass/Fail? ";
  AUTO_PASS?(ans=="t"):getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

TEST(RoombaTest, Power_Off)
{
  cout<<"Ready?"<<endl;
  if(!AUTO_ADVANCE){ cin.ignore(); }
  /********** TEST **********/
  cout<<"Please wait..."<<endl;
  
  Roomba r;
  r.start();
  
  r.powerOff();
  
  /********** TEST **********/
  cout<<"Pass/Fail? ";
  AUTO_PASS?(ans=="t"):getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

TEST(RoombaTest, Turn_90)
{
  cout<<"Ready?"<<endl;
  if(!AUTO_ADVANCE){ cin.ignore(); }
  /********** TEST **********/
  cout<<"Please wait..."<<endl;
  
  Roomba r;
  r.start();
  r.powerOn();
  
  //r.write(131);
  delay(DELAY_MS);
  
  r.turn(90);
  
  r.powerOff();
  /********** TEST **********/
  cout<<"Pass/Fail? ";
  AUTO_PASS?(ans=="t"):getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

TEST(RoombaTest, Turn_-90)
{
  cout<<"Ready?"<<endl;
  if(!AUTO_ADVANCE){ cin.ignore(); }
  /********** TEST **********/
  cout<<"Please wait..."<<endl;
  
  Roomba r;
  r.start();
  r.powerOn();
  
  //r.write(131);
  delay(DELAY_MS);
  
  r.turn(-90);
  
  r.powerOff();
  /********** TEST **********/
  cout<<"Pass/Fail? ";
  AUTO_PASS?(ans=="t"):getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}


TEST(RoombaTest, Drive_BWD)
{
  cout<<"Ready?"<<endl;
  if(!AUTO_ADVANCE){ cin.ignore(); }
  /********** TEST **********/
  cout<<"Please wait..."<<endl;
  
  Roomba r;
  r.start();
  r.powerOn();
  
  //r.write(131);
  delay(DELAY_MS);
  
  //r.drive(200, 1000);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  delay(1000);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  r.drive(-200, 32768);
  delay(1000);
  r.drive(0, 0);
  delay(50);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  
  r.powerOff();
  /********** TEST **********/
  cout<<"Pass/Fail? ";
  AUTO_PASS?(ans=="t"):getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

TEST(RoombaTest, Drive_FWD)
{
  cout<<"Ready?"<<endl;
  if(!AUTO_ADVANCE){ cin.ignore(); }
  /********** TEST **********/
  cout<<"Please wait..."<<endl;
  
  Roomba r;
  r.start();
  r.powerOn();
  
  //r.write(131);
  delay(DELAY_MS);
  
  //r.drive(200, 1000);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  delay(1000);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  r.drive(200, 32768);
  delay(1000);
  r.drive(0, 0);
  delay(50);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  
  r.powerOff();
  /********** TEST **********/
  cout<<"Pass/Fail? ";
  AUTO_PASS?(ans=="t"):getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

TEST(RoombaTest, Drive_CW)
{
  cout<<"Ready?"<<endl;
  if(!AUTO_ADVANCE){ cin.ignore(); }
  /********** TEST **********/
  cout<<"Please wait..."<<endl;
  
  Roomba r;
  r.start();
  r.powerOn();
  
  //r.write(131);
  delay(DELAY_MS);
  
  //r.drive(200, 1000);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  delay(1000);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  r.drive(500, -1);
  delay(1000);
  r.drive(0, 0);
  delay(50);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  
  r.powerOff();
  /********** TEST **********/
  cout<<"Pass/Fail? ";
  AUTO_PASS?(ans=="t"):getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

TEST(RoombaTest, Drive_CCW)
{
  cout<<"Ready?"<<endl;
  if(!AUTO_ADVANCE){ cin.ignore(); }
  /********** TEST **********/
  cout<<"Please wait..."<<endl;
  
  Roomba r;
  r.start();
  r.powerOn();
  
  //r.write(131);
  delay(DELAY_MS);
  
  //r.drive(200, 1000);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  delay(1000);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  r.drive(500, 1);
  delay(1000);
  r.drive(0, 0);
  delay(50);
  r.getSensors(Sensor::ALL);
  r.getSensor().print();
  
  r.powerOff();
  /********** TEST **********/
  cout<<"Pass/Fail? ";
  AUTO_PASS?(ans=="t"):getline(cin, ans);
  ASSERT_EQ(!(ans=="f"),true);
}

#endif
