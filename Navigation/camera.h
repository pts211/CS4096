//camera.h

#ifndef CAMERA_H
#define CAMERA_H


//#include <opencv2/highgui/highgui.hpp>
//#include <opencv\cv.h>
//#include <opencv\highgui.h>
//#include <time.h>       /* time */
//#include <Windows.h>
#include <opencv2/core/core.hpp>
#include <cv.h>
#include <highgui.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <math.h>
#include <unistd.h>

using namespace std;
using namespace cv;




class camera
{
private:

  VideoCapture cap; 

	Mat img, edges, out, pic, modi, input;

	double e_slope; //indicator of 
	double c_slope; //indicator of direction based upond color (controlled by trackbars)

	bool pathisblocked; //if false then good to go!

	string floorsign; // "" if no sign is detected

public:

	camera(): cap(0), e_slope(0), c_slope(0), pathisblocked(false), floorsign("") {};

	~camera() {};

	//MUTATOR

	void update(int EDGE_NUM,int B_MIN,int G_MAX,int R_MAX);

	//ACCESSORS
 
  void output();

	Mat getinput() const;

	double gete_slope() const;
	double getc_slope() const;

	bool getpathisblocked() const;

	string getfloorsign() const;

};//end class camera

#endif