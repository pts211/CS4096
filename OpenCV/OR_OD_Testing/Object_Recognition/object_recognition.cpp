/***********************************************************/
/* File:    object_recognition.cpp                         */
/* Author:  Travis Bueter                                  */
/* Desc:    This file tests the use of identifying objects */
/*          using a SURF detector and descriptors.         */
/*          Based on a tutorial from the OpenCV website    */
/*          and a demo project created by Frank            */
/*          Bergschneider:                                 */
/*http://frankbergschneider.weebly.com/home/category/opencv*/
/***********************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/nonfree/nonfree.hpp"

//Name spaces used
using namespace cv;
using namespace std;

void readme();

int main( int argc, char** argv )
{
    //Need at least one image for finding
	if( argc < 2 )
    { 
        readme(); return -1; 
    }

	//load training image
	int num_obj = argc-1;
	vector<Mat> object(num_obj);
	for(int i = 0; i < num_obj; i++)
	{
	    object[i] = imread (argv[i+1], CV_LOAD_IMAGE_GRAYSCALE);
	    if (!object[i].data)
	    {
		    cout<<"Can't open image";
		    return -1;
	    }
	}
	namedWindow("Good Matches", CV_WINDOW_AUTOSIZE);

	//SURF Detector, and descriptor parameters
	vector< vector<KeyPoint> > kpObject(num_obj);
	vector<KeyPoint> kpImage;
	vector<Mat> desObject(num_obj); 
	Mat desImage;
	int minHess=2000;
	
	SurfFeatureDetector detector(minHess);
	for(int i = 0; i < num_obj; i++)
	{
	    detector.detect(object[i], kpObject[i]);
	}
	SurfDescriptorExtractor extractor;
	for(int i = 0; i < num_obj; i++)
	{
	    extractor.compute(object[i], kpObject[i], desObject[i]);
	}

	FlannBasedMatcher matcher;

	//Initialize video and display window
	VideoCapture cap(0);  //camera 0 is webcam
	if (!cap.isOpened()) return -1;

    //video loop
    float thresholdMatchingNN=0.7;
    unsigned int thresholdGoodMatches=10;

    while(1)
    {
    	Mat frame;
    	Mat image;
    	
    	//Get frame from camera
    	cap>>frame;
    	cvtColor(frame, image, CV_RGB2GRAY);

    	Mat des_image, img_matches, H;
    	vector<KeyPoint> kp_image;

    	detector.detect( image, kp_image );
    	extractor.compute( image, kp_image, des_image );
    	string str = "";
    	
    	//Loop through all the options
    	for(int k = 0; k < num_obj; k++)
    	{
    	    vector< vector<DMatch> > matches;
        	vector<DMatch>  good_matches;
        	vector<Point2f> obj;
        	vector<Point2f> scene;
        	vector<Point2f> scene_corners(4);
    	
        	matcher.knnMatch(desObject[k], des_image, matches, 2);

            //
            for(int i = 0; i < min(des_image.rows-1,(int) matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
            {
            	if((matches[i][0].distance < thresholdMatchingNN*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))
                {
                    good_matches.push_back(matches[i][0]);
                }
            }

            if (good_matches.size() >= thresholdGoodMatches)
            {
            	//Get the option in which a match was made
            	if(k == 0)
            	{
            	    str += "(A)";
        	    }
        	    else if(k == 1)
        	    {
        	        str += "(B)";
    	        }
        	    else if(k == 2)
        	    {
        	        str += "(C)";
    	        }
        	    else if(k == 3)
        	    {
        	        str += "(D)";
    	        }
    	        else if(k == 4)
    	        {
    	            str += "(S)";
	            }

            }

        }
        
        //Print the list of matches to the screen.
        putText(frame, str, cvPoint(10,50),FONT_HERSHEY_COMPLEX_SMALL, 2, cvScalar(0,0,250), 1, CV_AA);
                
        //Show detected matches
        imshow( "Good Matches", frame );
        if (waitKey(30) == 27)// 'Esc' key
        {
            cout << "Esc key is pressed by user." << endl;
            break; 
        }

    }

    //Release camera and exit
    cap.release();
	return 0;
}

/** @function readme */
void readme()
{ 
    std::cout << " Usage: ./a.out <img_file> ..." << std::endl; 
}
