/************************************************************
	Filename: serialTest.cpp
	Programmer: Paul Sites
  Date: 2015.04.14
	Desc: Test using the WiringPi library
************************************************************/             

/*
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
*/

/*
#include <wiringPi.h>
#include <wiringSerial.h>
*/

//int main( int argc, char * argv[] )

/* simple.c

   Simple libftdi usage example

   This program is distributed under the GPL, version 2
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <ftdi.h>

using namespace std;

struct ftdi_context *ftdi;

bool setDTR(const int state);

int main(void)
{
    int ret;
    if ((ftdi = ftdi_new()) == 0)
   {
        fprintf(stderr, "ftdi_new failed\n");
        return EXIT_FAILURE;
    }

    if ((ret = ftdi_usb_open(ftdi, 0x0403, 0x6001)) < 0)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        return EXIT_FAILURE;
    }

    // Read out FTDIChip-ID of R type chips
    if (ftdi->type == TYPE_R)
    {
        unsigned int chipid;
        printf("ftdi_read_chipid: %d\n", ftdi_read_chipid(ftdi, &chipid));
        printf("FTDI chipid: %X\n", chipid);
    }
    
    // Set baudrate
    int baudrate = 57600;
    int f = ftdi_set_baudrate(ftdi, baudrate);
    if (f < 0)
    {
        fprintf(stderr, "unable to set baudrate: %d (%s)\n", f, ftdi_get_error_string(ftdi));
        exit(-1);
    }
    
    /*
    unsigned char buff[1024];
    while( true )
    {
      ftdi_read_data(ftdi, buff, sizeof(buff));
      cout<<buff<<endl;
      
    }
    */
    
    unsigned char opc;
    while( true )
    {
      cout<<"Enter an opcode: "<<endl;
      
      int in;
      cin >> in;
      
      opc = (unsigned char)in;
      
      switch(opc){
        case 9:
          cout<<"Exiting..."<<endl;
          exit(0);
          break;
        case 5:
          cout<<"DTR set (0): "<<setDTR(0)<<endl;
          break;
        case 6:
          cout<<"DTR set (1): "<<setDTR(1)<<endl;
          break;
        default:
            cout<<"Sending: "<<opc<<endl;
            ftdi_write_data(ftdi, &opc, 1);
          break;
      }
    }    
    
    cout<<"\nExiting...\n\n"<<endl;

    if ((ret = ftdi_usb_close(ftdi)) < 0)
    {
        fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        return EXIT_FAILURE;
    }
    ftdi_free(ftdi);

    return EXIT_SUCCESS;
}

bool setDTR(const int state)
{
  return (ftdi_setdtr(ftdi, state) == 0)?true:false;
}


/*

switch(val){
      case 0:
        cout<<"0: FTDI SET"<<endl;
        break;
      case -1:
        cout<<"-1: Set DTR failed!"<<endl;
        break;
      case -2:
        cout<<"-2: USB device unavailable."<<endl;
        break;
    }

int main(void)
{
    int ret;
    struct ftdi_context *ftdi;
    struct ftdi_version_info version;
    if ((ftdi = ftdi_new()) == 0)
   {
        fprintf(stderr, "ftdi_new failed\n");
        return EXIT_FAILURE;
    }
    version = ftdi_get_library_version();
    printf("Initialized libftdi %s (major: %d, minor: %d, micro: %d, snapshot ver: %s)\n",
        version.version_str, version.major, version.minor, version.micro,
        version.snapshot_str);
    if ((ret = ftdi_usb_open(ftdi, 0x0403, 0x6001)) < 0)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        return EXIT_FAILURE;
    }
    // Read out FTDIChip-ID of R type chips
    if (ftdi->type == TYPE_R)
    {
        unsigned int chipid;
        printf("ftdi_read_chipid: %d\n", ftdi_read_chipid(ftdi, &chipid));
        printf("FTDI chipid: %X\n", chipid);
    }
    if ((ret = ftdi_usb_close(ftdi)) < 0)
    {
        fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
        ftdi_free(ftdi);
        return EXIT_FAILURE;
    }
    ftdi_free(ftdi);
    return EXIT_SUCCESS;
}
*/

/*
int main( void )
{

  
  wiringPiSetup ();
  int fd;
  if((fd=serialOpen("/dev/ttyUSB0",9600))<0)
  {
    fprintf(stderr,"unable to open serial device%s\n",strerror(errno));
  } 
  
  return 0 ;
}
*/