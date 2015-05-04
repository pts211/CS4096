// Roomba.cpp
//


#include "Roomba.h"

Roomba::Roomba()
{
  led_pwr_c = 0;
  led_pwr_i = 0;
}

Roomba::~Roomba()
{
  int ret = 0;
  if ((ret = ftdi_usb_close(ftdi)) < 0){
    fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
    ftdi_free(ftdi);
  }
  ftdi_free(ftdi);
}

void Roomba::start()
{
  int ret = 0;
  if((ftdi = ftdi_new()) == 0){
    fprintf(stderr, "ftdi_new failed\n");
  }
  if ((ret = ftdi_usb_open(ftdi, 0x0403, 0x6001)) < 0){
    fprintf(stderr, "ERROR: Unable to open FTDI devide: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
    ftdi_free(ftdi);
  }
  
  setFTDIBaud(BAUDRATE);
  
  // setDTR(0);
  // write(Opcode::START);
  // write(Mode::CONTROL);
  // write(Mode::MAX);  
}

void Roomba::setBaud(unsigned char baud)
{
  write(Opcode::BAUD);
  write(baud);
  
  //TODO setFTDIBaud() Would need to figure out a way to get an int value of the baud to be set...
}

void Roomba::setMode(unsigned char mode)
{
  write(mode);
  delay(101);
}

bool Roomba::getSensors(int sensor_pkt)
{
  write(Opcode::SENSOR);
  write(sensor_pkt);
  
  switch(sensor_pkt)
  {
    case Sensor::ALL:
      read(26); //MAGIC NUMBERS!! WOOO. 
    break;
    case Sensor::ENVIRONMENT:
      read(10); //Actually, this is just the number of bytes
    break;
    case Sensor::PHYSICAL:
      read(6); //that will come back from the read call.
    break;
    case Sensor::SYSTEM:
      read(10); //I didn't feel like making constants.
    break;
  }
  
  return true;
}

void Roomba::turn(int degrees)
{
  
  int dir = (degrees < 0)?-1:1;
  const int GOAL = math.abs(degrees);
  int total_turned = 0;
  
  int speed = 100;
  
  if( !getSensors(SENSORS::PHYSICAL) ) {
    cout<<"Logic is literally broken."<<endl;
  }
  drive(speed,dir);
  while( total_turned < degrees )
  {
        
  
  }
  
    int degreesRotated = 0;
  int16_t speed = 50;
  int16_t direction = -1;

  // roomba.getSensors(Sensor::ALL).getAngle();
  roomba.drive(0,0);
  sensor.getAngle();
  if(degrees < 0)
  {
    roomba.drive(speed, direction);
    while((degreesRotated > degrees - 1) || (degreesRotated < degrees + 1))
    {
      usleep(100);
      // degreesRotated -= roomba.getSensors(Sensor::ALL).getAngle();
      degreesRotated -= sensor.getAngle();
    }    
  }
  else
  {
    roomba.drive(speed, 1);
    while((degreesRotated < degrees - 1) || (degreesRotated > degrees + 1))
    {
      usleep(100);
      // degreesRotated += roomba.getSensors(Sensor::ALL).getAngle();
      cout << "Degrees rotated: " << degreesRotated << endl;
      degreesRotated += sensor.getAngle();
    }
  }
  roomba.drive(0, 0); 
}

void Roomba::drive(int16_t velocity, int16_t radius)
{
  cout<<"Driving - Velocity: "<<velocity<<" Radius: "<<radius<<endl; 
  write(Opcode::DRIVE);
  write((velocity & 0xff00) >> 8);
  write(velocity & 0xff);
  write((radius & 0xff00) >> 8);
  write(radius & 0xff);
}

void Roomba::setLED(int led_id, bool state, int pwr_c, int pwr_i)
{
  if(pwr_c == -1 || pwr_i == -1){
    pwr_c = led_pwr_c;
    pwr_i = led_pwr_i;
  }else{
    led_pwr_c = pwr_c;
    led_pwr_i = pwr_i;
  }
  
  //led_bs.reset(); // To reset everything, if desired.
  led_bs.set(led_id, state);
  
  write(Opcode::LEDS);
  write( static_cast<unsigned char>(led_bs.to_ulong()) );
  write(pwr_c);
  write(pwr_i);
}

void Roomba::powerOn()
{
  setDTR(1);
  delay(500);
  setDTR(0);
  delay(500);
  setDTR(1);
  
  delay(101);
  
  write(Opcode::START);
  write(Mode::CONTROL);
  write(Mode::FULL);
  delay(101);
  
  setLED(0, false, 0, 255);
}

void Roomba::powerOff()
{
  write(Opcode::START);
  write(Mode::POWER);
}

/******************** PRIVATE ********************/
void Roomba::write(unsigned char cmd)
{
  ftdi_write_data(ftdi, &cmd, 1);
  //TODO This was previously 101ms. It may not work right at it's current value.
  delay(10);
}

void Roomba::read(int numBytes)
{
  unsigned char *rec = new unsigned char[MAX_SENSOR_BYTES];
  //TODO This was previously 200ms. It may not work right at it's current value.
  delay(20);
  while(ftdi_read_data(ftdi, rec, MAX_SENSOR_BYTES) > 0) {}
  /*
  {
    for (int i= 0; i < MAX_SENSOR_BYTES; i++) {
      int iCMD = rec[i];
      cout<<iCMD<<endl; //printf("%02X ", rec[i]);
    }
    
    cout<<"---------- ITERATION COMPLETE ----------"<<endl;
  }
  */
  //cout<<"LOOP FINISHED"<<endl;
  
  sensors.parseSensorData(0, rec);
}

void Roomba::setFTDIBaud(unsigned int baud)
{
  int f = ftdi_set_baudrate(ftdi, baud);
  if (f < 0){
      fprintf(stderr, "Unable to set baudrate: %d (%s)\n", f, ftdi_get_error_string(ftdi));
      exit(-1);
  }
}

bool Roomba::setDTR(const int state)
{
  return (ftdi_setdtr(ftdi, state) == 0)?true:false;
}


// Reads at most len bytes and stores them to dest
// If successful, returns true.
// If there is a timeout, returns false
// Blocks until all bytes are read
// Caller must ensure there is sufficient space in dest
/*
bool Roomba::read(uint8_t* dest, uint8_t len)
{
  ftdi_read_data(ftdi, &buff)
  while (len-- > 0)
  {
    unsigned long startTime = millis();
    while (!_serial->available())
    {
      // Look for a timeout
      if (millis() > startTime + ROOMBA_READ_TIMEOUT)
        return false; // Timed out
    }
    *dest++ = _serial->read();
  }
  return true;
}
*/
/*
void Roomba::sleep(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}
*/

void Roomba::delay(unsigned int howLong)
{
  usleep( howLong * 1000 );
}

/* OLD Version
void Roomba::delay(unsigned int howLong)
{
  struct timespec sleeper ;

  if (howLong ==   0)
    return ;
  else if (howLong  < 100)
    cout<<"NOT LONG ENOUGH!"<<endl;
  else
  {
    sleeper.tv_sec  = 0 ;
    sleeper.tv_nsec = (long)(howLong * 1000) ;
    nanosleep (&sleeper, NULL) ;
  }
}
*/

/*
// Resets the 
void Roomba::reset()
{
    _serial->write(7);
}

// Start OI
// Changes mode to passive
void Roomba::start()
{
    _serial->begin(_baud);
    _serial->write(128);
}

uint32_t Roomba::baudCodeToBaudRate(Baud baud)
{
    switch (baud)
    {
	case Baud300:
	    return 300;
	case Baud600:
	    return 600;
	case Baud1200:
	    return 1200;
	case Baud2400:
	    return 2400;
	case Baud4800:
	    return 4800;
	case Baud9600:
	    return 9600;
	case Baud14400:
	    return 14400;
	case Baud19200:
	    return 19200;
	case Baud28800:
	    return 28800;
	case Baud38400:
	    return 38400;
	case Baud57600:
	    return 57600;
	case Baud115200:
	    return 115200;
	default:
	    return 57600;
    }
}

void Roomba::baud(Baud baud)
{
    _serial->write(129);
    _serial->write(baud);

    _baud = baudCodeToBaudRate(baud);
    _serial->begin(_baud);
}

void Roomba::safeMode()
{
  _serial->write(131);
}

void Roomba::fullMode()
{
  _serial->write(132);
}

void Roomba::power()
{
  _serial->write(133);
}

void Roomba::dock()
{
  _serial->write(143);
}

void Roomba::demo(Demo demo)
{
  _serial->write(136);
  _serial->write(demo);
}

void Roomba::cover()
{
  _serial->write(135);
}

void Roomba::coverAndDock()
{
  _serial->write(143);
}

void Roomba::spot()
{
  _serial->write(134);
}

void Roomba::drive(int16_t velocity, int16_t radius)
{
  _serial->write(137);
  _serial->write((velocity & 0xff00) >> 8);
  _serial->write(velocity & 0xff);
  _serial->write((radius & 0xff00) >> 8);
  _serial->write(radius & 0xff);
}

void Roomba::leds(uint8_t leds, uint8_t powerColour, uint8_t powerIntensity)
{
  _serial->write(139);
  _serial->write(leds);
  _serial->write(powerColour);
  _serial->write(powerIntensity);
}

// Sets low side driver outputs on or off
void Roomba::drivers(uint8_t out)
{
  _serial->write(138);
  _serial->write(out);
}

// Define a song
// Data is 2 bytes per note
void Roomba::song(uint8_t songNumber, const uint8_t* data, int len)
{
    _serial->write(140);
    _serial->write(songNumber);
    _serial->write(len >> 1); // 2 bytes per note
    _serial->write(data, len);
}

void Roomba::playSong(uint8_t songNumber)
{
  _serial->write(141);
  _serial->write(songNumber); 
}

// Reads at most len bytes and stores them to dest
// If successful, returns true.
// If there is a timeout, returns false
// Blocks until all bytes are read
// Caller must ensure there is sufficient space in dest
bool Roomba::getData(uint8_t* dest, uint8_t len)
{
  while (len-- > 0)
  {
    unsigned long startTime = millis();
    while (!_serial->available())
    {
      // Look for a timeout
      if (millis() > startTime + ROOMBA_READ_TIMEOUT)
        return false; // Timed out
    }
    *dest++ = _serial->read();
  }
  return true;
}

bool Roomba::getSensors(uint8_t packetID, uint8_t* dest, uint8_t len)
{
  _serial->write(142);
  _serial->write(packetID);
  return getData(dest, len);
}
*/