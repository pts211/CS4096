//main.cpp tester for camera.h
#include "camera.h"

using namespace cv;
using namespace std; 

void createTrackbars();

const string trackbarWindowName1 = "Trackbar1";
const string trackbarWindowName2 = "Trackbar2";
const string trackbarWindowName3 = "Trackbar3";
const string trackbarWindowName4 = "Trackbar4";
const string trackbarWindowName5 = "Trackbar5";
int P_NUM = 30;
const int P_NUM_MAX = 100;
int B_MIN = 144;
const int B_LIM = 255;
int G_MAX = 139;
const int G_LIM = 255;
int R_MAX = 111;
const int R_LIM = 255;
int L_NUM = 231;
const int L_LIM = 480;



int main()
{
  usleep(2000);
	camera cam;
 
  createTrackbars();
 
  usleep(100);
  
	while(1)
	{
		cam.update(P_NUM,B_MIN,G_MAX,R_MAX,L_NUM);
		cam.output(P_NUM,B_MIN,G_MAX,R_MAX,L_NUM);
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
 
	namedWindow(trackbarWindowName1,2);
  namedWindow(trackbarWindowName2,2);
  namedWindow(trackbarWindowName3,2);
  namedWindow(trackbarWindowName4,2);
  namedWindow(trackbarWindowName5,2);

	char TrackbarName1[50];
  char TrackbarName2[50];
  char TrackbarName3[50];
  char TrackbarName4[50];
  char TrackbarName5[50];
	
 
	sprintf( TrackbarName1, "Edges", P_NUM_MAX);
	sprintf( TrackbarName2, "B_MIN", B_LIM);
	sprintf( TrackbarName3, "G_MAX", G_LIM);
	sprintf( TrackbarName4, "R_MAX", R_LIM);	
  sprintf( TrackbarName5, "L_NUM", L_LIM);
 
	createTrackbar( TrackbarName1, trackbarWindowName1, &P_NUM, P_NUM_MAX, on_trackbar);
	createTrackbar( TrackbarName2, trackbarWindowName2, &B_MIN, B_LIM, on_trackbar );
	createTrackbar( TrackbarName3, trackbarWindowName3, &G_MAX, G_LIM, on_trackbar );
	createTrackbar( TrackbarName4, trackbarWindowName4, &R_MAX, R_LIM, on_trackbar );
  createTrackbar( TrackbarName5, trackbarWindowName5, &L_NUM, L_LIM, on_trackbar );
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//    
	return;
}



