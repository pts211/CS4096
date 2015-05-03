//main.cpp tester for camera.h
#include "camera.h"

using namespace cv;
using namespace std; 

void createTrackbars();

const string trackbarWindowName = "Trackbars";
int EDGE_NUM = 50;
const int EDGE_NUM_MAX = 255;
int B_MIN = 40;
const int B_LIM = 255;
int G_MAX = 80;
const int G_LIM = 255;
int R_MAX = 80;
const int R_LIM = 255;


int main()
{
	camera cam;
 
  createTrackbars();
 
  usleep(100);
  
	while(1)
	{
		cam.update(EDGE_NUM,B_MIN,G_MAX,R_MAX);
		cam.output();
	}


	 return 0;


}

void on_trackbar( int, void* )
{//This function gets called whenever a
	// trackbar position is changed
}

void createTrackbars()
{
	//create window for trackbars
 
	namedWindow(trackbarWindowName,0);

	char TrackbarName[50];
	
 
	sprintf( TrackbarName, "Edges", EDGE_NUM_MAX);
	//sprintf( TrackbarName, "B_MIN", B_LIM);
	//sprintf( TrackbarName, "G_MAX", G_LIM);
	//sprintf( TrackbarName, "R_MAX", R_LIM);	
 
 
	createTrackbar( TrackbarName, trackbarWindowName, &EDGE_NUM, EDGE_NUM_MAX, on_trackbar);
	//createTrackbar( TrackbarName, trackbarWindowName, &B_MIN, B_LIM, on_trackbar );
	//createTrackbar( TrackbarName, trackbarWindowName, &G_MAX, G_LIM, on_trackbar );
	//createTrackbar( TrackbarName, trackbarWindowName, &R_MAX, R_LIM, on_trackbar );

	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//    
	return;
}



