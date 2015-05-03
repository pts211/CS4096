/***********************************************************/
/* File:    objectdetection-contours-video.cpp             */
/* Author:  Travis Bueter                                  */
/* Desc:    This file tests the use of identifying objects */
/*          based on their contours of a video feed.       */
/*          Based on a tutorial from the OpenCV website.   */
/***********************************************************/

#include <opencv2/opencv.hpp>
#include <highgui.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>       /* time */
#include <string>
#include <math.h>
//#include "opencv2/imgcodecs.hpp"
using namespace cv;
using namespace std;

int main()
{

    Mat img, edges, out, modi;
    
    //Set up camera
    VideoCapture cap(0); // open the video camera no. 0
	while(!cap.isOpened())  // if not success, exit program
    {
        cout << "error cannot read from the camera" << endl;
    }
	
	while(1)
	{
	    bool bSuccess = cap.read(img); // read a new frame from video
        if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
        
        //converting the original image into grayscale
	    Mat imgGrayScale = Mat(img.size(), 8, 1); 
	    cvtColor(img,imgGrayScale,CV_BGR2GRAY);
	    
	    //Attempted to use for smoothing spots in the image
	    //GaussianBlur(imgGrayScale,imgGrayScale,Size(7,7),1.5,1.5);

	    //thresholding the grayscale image to get better results
	    threshold(imgGrayScale,imgGrayScale,128,255,CV_THRESH_BINARY);

        vector< vector<Point> > contours;
        vector<Vec4i> hierarchy;
	
	    //finding all contours in the image
	    findContours(imgGrayScale, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	
	    vector< vector<Point> > contours_poly(contours.size());
		
	    //iterating through each contour
	    for(int i = 0; i < contours.size(); i++)
	    {
		    //obtain a sequence of points of the countour
		    approxPolyDP(Mat(contours[i]), contours_poly[i], arcLength(contours[i],true)*0.02, true);
				           
		    //if there are 3 vertices  in the contour(It should be a triangle)
		    vector<Point> result = contours_poly[i];
		    if(result.size() == 3)
		    {			
			    //drawing lines around the triangle
			    for(int j = 0; j < result.size()-1; j++)
			    {
			        line(img, result[j], result[j+1], cvScalar(255,0,0),4);
			    }
			    line(img, result[result.size()-1], result[0], cvScalar(255,0,0),4);		
		    }
		    
		    if(result.size() == 4)
		    {			
			    //drawing lines around the triangle
			    for(int j = 0; j < result.size()-1; j++)
			    {
			        line(img, result[j], result[j+1], cvScalar(0,255,0),4);
			    }
			    line(img, result[result.size()-1], result[0], cvScalar(0,255,0),4);
		    }
		    
		    if(result.size() == 5)
		    {			
			    //drawing lines around the triangle
			    for(int j = 0; j < result.size()-1; j++)
			    {
			        line(img, result[j], result[j+1], cvScalar(0,0,255),4);
			    }
			    line(img, result[result.size()-1], result[0], cvScalar(0,0,255),4);
		    }
		    
		    if(result.size() == 6)
		    {			
			    //drawing lines around the triangle
			    for(int j = 0; j < result.size()-1; j++)
			    {
			        line(img, result[j], result[j+1], cvScalar(255,0,127),4);
			    }
			    line(img, result[result.size()-1], result[0], cvScalar(255,0,127),4);
		    }
		    
		    if(result.size() == 10)
		    {			
			    //drawing lines around the triangle
			    for(int j = 0; j < result.size()-1; j++)
			    {
			        line(img, result[j], result[j+1], cvScalar(0,255,255),4);
			    }
			    line(img, result[result.size()-1], result[0], cvScalar(0,255,255),4);
		    }

	    }


        namedWindow("MyVideo",CV_WINDOW_AUTOSIZE);
        imshow("MyVideo", img);       
        
        if (waitKey(30) == 27)// 'Esc' key
        {
            cout << "Esc key is pressed by user." << endl;
	        break; 
        }
        
        
	}

    cap.release();
	return 0;
}



