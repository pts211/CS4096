#ifndef RoombaSensors_H
#define RoombaSensors_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <memory>
#include <bitset>

using namespace std;
class RoombaSensors
{
  public:
    static const int MAX_SENSOR_BYTES = 26;
    
    bool getBump(int which = 0) { 
      if( which == -1 ) { return bump_left; }
      else if( which == 0 ) { return (bump_left || bump_right); }
      else if( which == 1 ) { return bump_right; }
    }
    
    bool getWheeldrop() { return wheeldrop; }
    bool getWall() { return wall; }
    
    bool getCliff(int which = 0) { 
      if( which == -2 ) { return cliff_left; }
      else if( which == -1 ) { return cliff_front_left; }
      else if( which == 0 ) { return (cliff_left || cliff_front_left || cliff_front_right || cliff_right); }
      else if( which == 1 ) { return cliff_front_right; }
      else if( which == 2 ) { return cliff_right; }
    }
    
    bool getVirtualWall() { return virtual_wall; }
    
    bool getOvercurrent(int which = 0) { 
      if( which == -1 ) { return overcurrent_left; }
      else if( which == 0 ) { return (overcurrent_left || overcurrent_right); }
      else if( which == 1 ) { return overcurrent_right; }
      
      else if( which == 2 ) { return overcurrent_brush; }
      else if( which == 3 ) { return overcurrent_vacuum; }
      else if( which == 4 ) { return overcurrent_side; }
    }
    
    bool getDirt(int which = 0) { 
      if( which == -1 ) { return dirt_left; }
      else if( which == 0 ) { return (dirt_left || dirt_right); }
      else if( which == 1 ) { return dirt_right; }
    }
    
    int getRemote() { return remote_cmd; }
    
    bool getBtnPower() { return btn_power; }
    bool getBtnSpot() { return btn_spot; }
    bool getBtnClean() { return btn_clean; }
    bool getBtnMax() { return btn_power; }
    
    short getDistance() { return distance; }
    
    double getAngle(int which = 0) { 
      if( which == 0 ) { return (360.0*(double)angle)/(258.0*PI); }   //Degrees
      else if( which == 1 ) { return (( 2.0*(double)angle)/258.0 ); } //Radians
      return 0; //Get rid of stupid compiler warnings.
    }
    
    char getChargingState() { return charging_state; }
    
    unsigned short getVoltage() { return voltage; }
    int getCurrent() { return current; }
    char getTemperature() { return temperature; }
    unsigned short getCharge() { return charge; }
    unsigned short getCapacity() { return capacity; }
    float getBatteryPercent() { return ( (float)charge/(float)capacity ); }
    
  //private:
    static constexpr double PI = 3.15159;
    bool bump_left;
    bool bump_right;
    bool wheeldrop;
    bool wall;
    
    bool cliff_left;
    bool cliff_front_left;
    bool cliff_front_right;
    bool cliff_right;
    
    bool virtual_wall;
    
    bool overcurrent_left;
    bool overcurrent_right;
    bool overcurrent_brush;
    bool overcurrent_vacuum;
    bool overcurrent_side;
    
    int dirt_left;  // 0-255
    int dirt_right; // 0-255
    
    int remote_cmd; // 0-255 (with some values unused)
    
    bool btn_power;
    bool btn_spot;
    bool btn_clean;
    bool btn_max;
    
    short distance;   // Distance in mm since last requested. Distance traveled by both wheels divided by two.
    
    short angle;      // Angle since last requested. ( angRad = (2*angle)/258 ; angDeg = (360*angle)/(258*pi) )
    
                         // 0: Not Charging
                         // 1: Charging Recovery
    char charging_state; // 2: Charging
                         // 3: Trickle Charging
                         // 4: Waiting
                         // 5: Charging Error
    
    unsigned int voltage; //Voltage in millivolts (mV).
    short current;  // The current in milliamps (mA) going in or out of the battery
    char temperature; // The temperature of the Roomba's battery in degrees Celsius.
    unsigned short charge; // The current charge of the Roomba's battery in milliamp-hours (mAh)
    unsigned short capacity; // Estimated charge capacity of the Roomba's battery. (when charge == capacity it's fully charged)
    
    unsigned char *data;
    
    
    bitset<8> ToBits(unsigned char byte)
    {
      return bitset<8>(byte);
    }
    
    void parseBumpers()
    {
      bitset<8> bw = bitset<8>(data[0]); //bitset<8>(data[0]);
      bump_right = bw[0];
      bump_left = bw[1];
      wheeldrop = bw[2];
    }
    
    bool parseSensorData(int pkt_cd, unsigned char* & sensor_data)
    {
      data = sensor_data;
      
      if(pkt_cd == 0)
      {
        int byte = 0;
        /* -------------------- BUMP/WHEELDROP -------------------- */
        //cout<<"Parsing BUMP/WHEELDROP"<<endl;
        bitset<8> bw = bitset<8>(data[byte++]);
        bump_right = bw[0];
        bump_left = bw[1];
        wheeldrop = bw[2];
        
        /* -------------------- WALL/CLIFF -------------------- */
        //cout<<"Parsing WALL/CLIFF"<<endl;
        wall = data[byte++];
        cliff_left = data[byte++];
        cliff_front_left = data[byte++];
        cliff_front_right = data[byte++];
        cliff_right = data[byte++];
        
        virtual_wall = data[byte++];
        
        /* -------------------- OVERCURRENTS -------------------- */
        //cout<<"Parsing OVERCURRENTS"<<endl;
        bitset<8> oc = bitset<8>(data[byte++]);
        overcurrent_left = oc[0];
        overcurrent_right = oc[1];
        overcurrent_brush = oc[2];
        overcurrent_vacuum = oc[3];
        overcurrent_side = oc[4];
        
        /* -------------------- DIRT DETECT/REMOTE -------------------- */
        //cout<<"Parsing DIRT DETECT/REMOTE"<<endl;
        dirt_left = data[byte++];
        dirt_right = data[byte++];
        remote_cmd = data[byte++];
        
        /* -------------------- BUTTONS -------------------- */
        //cout<<"Parsing BUTTONS"<<endl;
        bitset<8> bt = bitset<8>(data[byte++]);
        btn_max = bt[0];
        btn_clean = bt[1];
        btn_spot = bt[2];
        btn_power = bt[3];
        
        /* -------------------- DISTANCE/ANGLE -------------------- */
        //cout<<"Parsing DISTANCE/ANGLE"<<endl;
        distance = (data[byte++]<<8) | data[byte++]; //input=low | (high<<8)
        angle = (data[byte++]<<8) | data[byte++];
        
        /* -------------------- POWER -------------------- */
        //cout<<"Parsing POWER"<<endl;
        charging_state = data[byte++];
        voltage = (data[byte++]<<8) | data[byte++];
        current = (data[byte++]<<8) | data[byte++];
        temperature = data[byte++];
        charge = (data[byte++]<<8) | data[byte++];
        capacity = (data[byte++]<<8) | data[byte++];
      }
      else if(pkt_cd == 1)
      {
        int byte = 0;
        /* -------------------- BUMP/WHEELDROP -------------------- */
        //cout<<"Parsing BUMP/WHEELDROP"<<endl;
        bitset<8> bw = bitset<8>(data[byte++]);
        bump_right = bw[0];
        bump_left = bw[1];
        wheeldrop = bw[2];
        
        /* -------------------- WALL/CLIFF -------------------- */
        //cout<<"Parsing WALL/CLIFF"<<endl;
        wall = data[byte++];
        cliff_left = data[byte++];
        cliff_front_left = data[byte++];
        cliff_front_right = data[byte++];
        cliff_right = data[byte++];
        
        virtual_wall = data[byte++];
        
        /* -------------------- OVERCURRENTS -------------------- */
        //cout<<"Parsing OVERCURRENTS"<<endl;
        bitset<8> oc = bitset<8>(data[byte++]);
        overcurrent_left = oc[0];
        overcurrent_right = oc[1];
        overcurrent_brush = oc[2];
        overcurrent_vacuum = oc[3];
        overcurrent_side = oc[4];
        
        /* -------------------- DIRT DETECT/REMOTE -------------------- */
        //cout<<"Parsing DIRT DETECT/REMOTE"<<endl;
        dirt_left = data[byte++];
        dirt_right = data[byte++];
      }
      else if(pkt_cd == 2)
      {
        int byte = 0;
        /* -------------------- /REMOTE -------------------- */
        remote_cmd = data[byte++];
        
        /* -------------------- BUTTONS -------------------- */
        //cout<<"Parsing BUTTONS"<<endl;
        bitset<8> bt = bitset<8>(data[byte++]);
        btn_max = bt[0];
        btn_clean = bt[1];
        btn_spot = bt[2];
        btn_power = bt[3];
        
        /* -------------------- DISTANCE/ANGLE -------------------- */
        //cout<<"Parsing DISTANCE/ANGLE"<<endl;
        distance = (data[byte++]<<8) | data[byte++]; //input=low | (high<<8)
        angle = (data[byte++]<<8) | data[byte++];
      }
      else if(pkt_cd == 3)
      {
        int byte = 0;
        /* -------------------- POWER -------------------- */
        //cout<<"Parsing POWER"<<endl;
        charging_state = data[byte++];
        voltage = (data[byte++]<<8) | data[byte++];
        current = (data[byte++]<<8) | data[byte++];
        temperature = data[byte++];
        charge = (data[byte++]<<8) | data[byte++];
        capacity = (data[byte++]<<8) | data[byte++];
      }
      
      return true;
    }
    
    void print()
    {
        /* -------------------- BUMP/WHEELDROP -------------------- */
        
        cout<<" -------------------- ROOMBA SENSOR DATA -------------------- "<<endl;
        cout<<"Bump (Right): "<<bump_right<<endl;
        cout<<"Bump (Left): "<<bump_left<<endl;
        cout<<"Wheeldrop: "<<wheeldrop<<endl;
        
        /* -------------------- WALL/CLIFF -------------------- */
        cout<<"Cliff (Left): "<<cliff_left<<endl;
        cout<<"Cliff (Front Left): "<<cliff_front_left<<endl;
        cout<<"Cliff (Front Right): "<<cliff_front_right<<endl;
        cout<<"Cliff (Right): "<<cliff_right<<endl;
        cout<<"Virtual Wall: "<<virtual_wall<<endl;

        
        /* -------------------- OVERCURRENTS -------------------- */
        cout<<"Overcurrent (Left): "<<overcurrent_left<<endl;
        cout<<"Overcurrent (Right): "<<overcurrent_right<<endl;
        cout<<"Overcurrent (Brush): "<<overcurrent_brush<<endl;
        cout<<"Overcurrent (Vacuum): "<<overcurrent_vacuum<<endl;
        cout<<"Overcurrent (Side): "<<overcurrent_side<<endl;
        
        /* -------------------- DIRT DETECT/REMOTE -------------------- */
        cout<<"Dirt Detect (Left): "<<dirt_left<<endl;
        cout<<"Dirt Detect (Left): "<<dirt_right<<endl;
        cout<<"Remote Control Command: "<<remote_cmd<<endl;
        
        /* -------------------- BUTTON -------------------- */
        cout<<"Button (Power): "<<btn_power<<endl;
        cout<<"Button (Spot): "<<btn_spot<<endl;
        cout<<"Button (Clean): "<<btn_clean<<endl;
        cout<<"Button (Max): "<<btn_max<<endl;
        
        /* -------------------- DISTANCE/ANGLE -------------------- */
        cout<<"Distance: "<<distance<<endl;
        cout<<"Angle (mm): "<<angle<<"  Angle (deg): "<<getAngle()<<" Angle (rad): "<<getAngle(1)<<endl;
        
        /* -------------------- POWER -------------------- */
        cout<<"Charging State: "<<(int)charging_state<<endl;
        cout<<"Voltage: "<<voltage<<endl;
        cout<<"Current: "<<current<<endl;
        cout<<"Temperature: "<<(int)temperature<<endl;
        cout<<"Charge: "<<charge<<endl;
        cout<<"Capacity: "<<capacity<<endl;       
        cout<<"Battery Percentage: "<<getBatteryPercent()<<endl;
      
    }
    
};

#endif