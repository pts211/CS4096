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



//For creating trackbars for debugging
void createTrackbars();

const string trackbarWindowName = "Trackbars";
int EDGE_NUM = 50;
int EDGE_NUM_MAX = 255;
int B_MIN = 40;
const int B_LIM = 255;
int G_MAX = 80;
const int G_LIM = 255;
int R_MAX = 80;
const int R_LIM = 255;




//<< operator overload for debugging
std::ostream& operator<< (std::ostream& out, const camera& cam);

class camera
{
private:

	Mat img, edges, out, pic, modi, input;

	double e_slope; //indicator of 
	double c_slope; //indicator of direction based upond color (controlled by trackbars)

	bool pathisblocked; //if false then good to go!

	string floorsign; // "" if no sign is detected

public:

	camera(): e_slope(0), c_slope(0), pathisblocked(false), floorsign("") {createTrackbars();};

	~camera() {};

	//MUTATOR

	void update();

	//ACCESSORS

	Mat getinput() const;

	double gete_slope() const;
	double getc_slope() const;

	bool getpathisblocked() const;

	string getfloorsign() const;
	

};//end class camera





std::ostream& operator<< (std::ostream& out, const camera& cam)
{


	namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	imshow("MyVideo", cam.getinput()); //show the frame in "MyVideo" window
	if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
	{
		out << "esc key is pressed by user\n";
	}

	out << "[ " << cam.getpathisblocked() << ", " << cam.getfloorsign() << ", " << cam.gete_slope() << ", " << cam.getc_slope() << " ]";


  return out;
}

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed
}

void createTrackbars()
{
	//create window for trackbars

	namedWindow(trackbarWindowName,0);
	char TrackbarName[512];
	
	sprintf( TrackbarName, "Edges", EDGE_NUM);
	sprintf( TrackbarName, "B_MIN", B_MIN);
	sprintf( TrackbarName, "G_MAX", G_MAX);
	sprintf( TrackbarName, "R_MAX", R_MAX);	
	createTrackbar( "Edges", trackbarWindowName, &EDGE_NUM, EDGE_NUM_MAX, on_trackbar);
	createTrackbar( "B_MIN", trackbarWindowName, &B_MIN, B_LIM, on_trackbar );
	createTrackbar( "G_MAX", trackbarWindowName, &G_MAX, G_LIM, on_trackbar );
	createTrackbar( "R_MAX", trackbarWindowName, &R_MAX, R_LIM, on_trackbar );

	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//    
	return;
}


#endif