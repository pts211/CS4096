#include <opencv2/core/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv\cv.h>
#include <opencv\highgui.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>       /* time */
#include <string>
#include <math.h>
#include <Windows.h>



using namespace cv;
using namespace std; 

void createTrackbars();

const string trackbarWindowName = "Trackbars";
int EDGE_NUM = 50;
int EDGE_NUM_MAX = 150;

int main()
{

	Mat img, edges, out, pic, modi;

	pic = cvLoadImage("test4.bmp", CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! pic.data )                              // Check for invalid input
    {
        //
    }



	VideoCapture cap(0); // open the video camera no. 0
	while(!cap.isOpened())  // if not success, exit program
    {
        cout << "error cannot read from the camera" << endl;
    }
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video
	cout << "Frame size : " << dWidth << " x " << dHeight << endl;//640x480
	


	int x_length = 200;
	int y_length = 200;

	int center_x = dWidth/2;
	int offset = 50;

	double** plot;
	plot = new double *[x_length];
	for(int i=0; i<x_length; i++)
	{
			plot[i] = new double[2]; // 0 is for x, 1 is for y
	}
	double** plot2;
	plot2 = new double *[x_length];
	for(int i=0; i<x_length; i++)
	{
			plot2[i] = new double[2]; // 0 is for x, 1 is for y
	}
	
	
	int plot_length, plot_length2;

	createTrackbars();
	Sleep(100);

	while(1)
	{
		bool bSuccess = cap.read(img); // read a new frame from video
        if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
			 Sleep(1000);
             break;
        }

		 cvtColor(img, edges, CV_BGR2GRAY);
         GaussianBlur(edges,edges, cv::Size(7,7), 1.5, 1.5);
         Canny(edges, edges, 0, EDGE_NUM, 3);

		 modi = edges.clone(); //NOT TEST
		 //cvtColor(pic, modi, CV_BGR2GRAY);///TEST ONLY

		 double left_slope = 0, right_slope = 0;
		 //FOR LEFT AND RIGHT SIDE
		 
			 
		Rect r(center_x - offset - x_length, dHeight - y_length, x_length, y_length);
		Rect r2(center_x + offset, dHeight - y_length, x_length, y_length);
			
		Mat smallImg = modi(r);
		Mat smallImg2 = modi(r2);

					 

		plot_length = 0; //left side
		plot_length2 = 0; //right side
		for(int x = 0; x < x_length; x++)
		{
			bool found = false;
			bool found2 = false;

			for(int y = 0; y < y_length; y++)
			{
				Scalar intensity = smallImg.at<uchar>(y,x);
				Scalar intensity2 = smallImg2.at<uchar>(y,x);

				if( intensity.val[0] == 255 & y!=0)
				{
				plot[ plot_length][0] = double(x);
				plot[plot_length][1] = double(y);
				found = true;
				}
				if( intensity2.val[0] == 255 & y!=0)
				{
				plot2[plot_length2][0] = double(x);
				plot2[plot_length2][1] = double(y);
				found2 = true;
				}
				
				 
			if(y==0||y==y_length-1||x==0||x==x_length-1)
			{
				smallImg.at<uchar>(y,x) = 255;
				smallImg2.at<uchar>(y,x) = 255;
			}





			}		 		
			if (found)
				plot_length++;
			if (found2)
				plot_length2++;
			 	
		}
		 

		//linear regression1
		double Sx,Sy,Sxy,x_bar,y_bar,b,a;

		x_bar = 0; y_bar = 0;
		for(int i=0; i<plot_length; i++)
		{
			x_bar+=plot[i][0];
			y_bar+=plot[i][1];
		}
		x_bar = x_bar / plot_length;
		y_bar = y_bar / plot_length;

		Sx = 0; Sy = 0; Sxy = 0;
		for(int i=0; i < plot_length; i++)
		{
			Sx += pow((plot[i][0] - x_bar),2);
			Sy += pow((plot[i][1] - y_bar),2);
			Sxy += (plot[i][0] - x_bar)*(plot[i][1] - y_bar);
		}

		b = Sxy/Sx;
		a = y_bar - (b * x_bar);
	// double x_intercept = ((0 - a) / b) + center_x - offset - x_length; //relative to modi
		int start_x = int( ((y_length-1 - a) / b) + center_x - offset - x_length );
		int start_y = dHeight - 1;
		Point pt1,pt2;
		pt1.x = start_x;
		pt1.y = start_y;

		int end_x = int( (dHeight-1)/(0-b) + start_x);
		int end_y = 0;
		pt2.x = end_x;
		pt2.y = end_y;

		line(modi,pt1,pt2,Scalar(255,0,0),2,8);
		left_slope = 0 - b;

		//linear regression2

		x_bar = 0; y_bar = 0;
		for(int i=0; i<plot_length2; i++)
		{
			x_bar+=plot2[i][0];
			y_bar+=plot2[i][1];
		}
		x_bar = x_bar / plot_length2;
		y_bar = y_bar / plot_length2;

		Sx = 0; Sy = 0; Sxy = 0;
		for(int i=0; i < plot_length2; i++)
		{
			Sx += pow((plot2[i][0] - x_bar),2);
			Sy += pow((plot2[i][1] - y_bar),2);
			Sxy += (plot2[i][0] - x_bar)*(plot2[i][1] - y_bar);
		}

		b = Sxy/Sx;
		a = y_bar - (b * x_bar);
	    // double x_intercept = ((0 - a) / b) + center_x - offset - x_length; //relative to modi
		start_x = int( ((y_length-1 - a) / b) + center_x + offset);
		start_y = dHeight - 1;
		
		pt1.x = start_x;
		pt1.y = start_y;

		end_x = int( (dHeight-1)/(0-b) + start_x);
		end_y = 0;
		pt2.x = end_x;
		pt2.y = end_y;

		line(modi,pt1,pt2,Scalar(255,0,0),2,8);
		right_slope = 0 - b;

			

		 



	 

	   namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	   imshow("MyVideo", modi); //show the frame in "MyVideo" window
	   if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
			break; 
       }

	   cout << "left, right, sum " << endl;
	   cout << left_slope << " " << right_slope << " " << left_slope + right_slope << endl;
	   if ( left_slope + right_slope < 0)
		   cout << "Move right" << endl;
	   else
		   cout << "Move left" << endl;




	 }//end while(1)

	/*
	for(int i=0; i<plot_length; i++)
	{
		cout << plot[i][0] << ", " << plot[i][1] << endl;
	}
	while(1);
	*/


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
	sprintf( TrackbarName, "Edges", EDGE_NUM);
	createTrackbar( "Edges", trackbarWindowName, &EDGE_NUM, EDGE_NUM_MAX, on_trackbar);

	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//    







}