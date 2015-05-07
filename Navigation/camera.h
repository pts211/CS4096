//camera.h

#ifndef CAMERA_H
#define CAMERA_H

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
  
  	Mat img;
  
  	double intercept; // Intercept of the slope
  	double slope; //indicator of direction based upon color (controlled by trackbars)
  
  	bool pathisblocked; //if false then good to go!
  
  	string floorsign; // "" if no sign is detected
   
   int P_NUM;
   int B_MIN;
   int G_MAX;
   int R_MAX;
   int L_NUM;
  
  public:
  
  	camera(): P_NUM(20), B_MIN(85), G_MAX(167), R_MAX(102),L_NUM(81),cap(0), intercept(0), slope(0), pathisblocked(false), floorsign("") {};
  
  	~camera() {cap.release();}
  
  	//MUTATOR
  
  	void update();
  
  	//ACCESSORS
   
    void output();
  
  	Mat getimg() const;
  	double getintercept() const;
  	double getslope() const;
  	bool getpathisblocked() const;
  	string getfloorsign() const;

};//end class camera

#endif