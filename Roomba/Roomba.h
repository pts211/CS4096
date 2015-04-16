//#ifdef Roomba_H
//#define Roomba_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ftdi.h>

using namespace std;

/* -------------------- MY ROOMBA -------------------- */
/*
enum class SENSOR : std::int8_t {
  WALL = 1,
  CLIFF_LEFT = 2,
  CLIFF_FRONT_LEFT = 3,
  CLIFF_FRONT_RIGHT = 3,
  CLIFF_RIGHT = 2,
  VIRTUAL_WALL = 2,
};
enum class OPCODE : unsigned char {
  START = 128,
  BAUD = 129,
};
enum class MODE : unsigned char {
  CONTROL = 130,
  SAFE = 131,
  FULL = 132,
  POWER = 133,
  SPOT = 134,
  CLEAN = 135,
  MAX = 136,
};
enum class BAUD : unsigned char {
  B300    = 0,
  B600    = 1,
  B1200   = 2,
  B2400   = 3,
  B4800   = 4,
  B9600   = 5,
  B14400  = 6,
  B19200  = 7,
  B28800  = 8,
  B38400  = 9,
  B57600  = 10,
  B115200 = 11,
};
enum class MOTORS : std::int8_t {
  MAIN_BRUSH = 0,  // BIT 2
  VACUUM = 1,      // BIT 1
  SIDE_BRUSH = 2   // BIT 0
};
*/

class Roomba
{
  public:
    static const int BAUDRATE = 57600
    
    Roomba();
    //Roomba( BAUD );
    
  /* Roomba SCI Commands */
    //void start();
    //void setBaud( BAUD b );
    void setBaud( int b );
    //Set the Roomba operating mode.
    /*
    setMode( ENUM::MODE );
    
    //Make the Roomba drive at a velocity (mm/s) at a specified radius.
    drive(int Velocity, int Radius);
    
    //A list of the motors to turn on or off.
    motors( ENUM::MOTORS [] )
    
    //Set the state of the button LEDS on the Roomba
    leds();
    
    //Write a song, play the song.
    song();
    play();
    
    //Request the sensor data.
    sensors();
    
    //Force the Roomba to start hunting for its dock.
    forceSeekingDock();
    
  */
  /* Courtesy Functions */
  /*
    //Read the value one of the Roomba sensors.
    bool/auto getSensor( ENUM::SENSOR );
    
    //Turn the Roomba X degrees (zero-turn)
    turn(degrees);
    
    //Return the current battery voltage.
    float getBatteryVoltage();
    
    //Calculate the percent power remaining and return. --Might not be possible. (translation. accurate enough to be useful)
    float getBatteryPercent();    
  */
  private:
    struct ftdi_context *ftdi;
};


/* -------------------- MY ROOMBA END -------------------- */
//#endif
