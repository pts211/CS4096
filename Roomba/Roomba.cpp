// Roomba.cpp
//
// Copyright (C) 2010 Mike McCauley
// $Id: Roomba.cpp,v 1.1 2010/09/27 21:58:32 mikem Exp mikem $

#include "Roomba.h"
/*
Roomba::Roomba(HardwareSerial* serial, Baud baud)
{
  _serial = serial;
  _baud = baudCodeToBaudRate(baud);
  _pollState = PollStateIdle;
}
*/

Roomba::Roomba()
{
  //
}

Roomba::~Roomba()
{
  if ((ret = ftdi_usb_close(ftdi)) < 0){
    fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
    ftdi_free(ftdi);
    return EXIT_FAILURE;
  }
  ftdi_free(ftdi);
}

void Roomba::start()
{
  if((ftdi = ftdi_new()) == 0){
    fprintf(stderr, "ftdi_new failed\n");
    return EXIT_FAILURE;
  }
  if ((ret = ftdi_usb_open(ftdi, 0x0403, 0x6001)) < 0){
    fprintf(stderr, "ERROR: Unable to open FTDI devide: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
    ftdi_free(ftdi);
    return EXIT_FAILURE;
  }

}

//void Roomba::setBaud(BAUD b)
void Roomba::setBaud(int b)
{
  cout<<b<<endl;
  
  int f = ftdi_set_baudrate(ftdi, BAUDRATE);
  if (f < 0){
      fprintf(stderr, "Unable to set baudrate: %d (%s)\n", f, ftdi_get_error_string(ftdi));
      exit(-1);
  }
  //ftdi_write_data(ftdi, OPCODE::BAUD, 1);
  //ftdi_write_data(ftdi, &opc, 1);
}
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