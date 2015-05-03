/***********************************************************/
/* File:    object_recognition_single.cpp                  */
/* Author:  Travis Bueter                                  */
/* Desc:    This file demonstrates what the computer is    */
/*          "seeing" when it is trying to match a given    */
/*          image to objects in a video feed.              */
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
    //Requires the input of a single image for matching
	if( argc != 2 )
    { 
        readme(); return -1; 
    }

	//load training image
	Mat object = imread (argv[1], CV_LOAD_IMAGE_GRAYSCALE);
	if (!object.data){
		cout<<"Can't open image";
		return -1;
	}
	namedWindow("Good Matches", CV_WINDOW_AUTOSIZE);

	//SURF Detector, and descriptor parameters
	vector<KeyPoint> kpObject, kpImage;
	Mat desObject, desImage;

	int minHess=2000;
	SurfFeatureDetector detector(minHess);
	detector.detect(object, kpObject);
	SurfDescriptorExtractor extractor;
	extractor.compute(object, kpObject, desObject);
	FlannBasedMatcher matcher;

	//Initialize video and display window
	VideoCapture cap(0);  //camera 0 is webcam
	if (!cap.isOpened()) return -1;

	//Object corner points for plotting box
	vector<Point2f> obj_corners(4);
	obj_corners[0] = cvPoint(0,0);
	obj_corners[1] = cvPoint( object.cols, 0 );
    obj_corners[2] = cvPoint( object.cols, object.rows );
    obj_corners[3] = cvPoint( 0, object.rows );

    //video loop
    char escapeKey='k';
    double frameCount = 0;
    float thresholdMatchingNN=0.7;
    unsigned int thresholdGoodMatches=35;


    while(1)
    {
    	frameCount++;
    	Mat frame;
    	Mat image;
    	cap>>frame;
    	cvtColor(frame, image, CV_RGB2GRAY);

    	Mat des_image, img_matches, H;
    	vector<KeyPoint> kp_image;
    	vector< vector<DMatch> > matches;
    	vector<DMatch>  good_matches;
    	vector<Point2f> obj;
    	vector<Point2f> scene;
    	vector<Point2f> scene_corners(4);

    	detector.detect( image, kp_image );
    	extractor.compute( image, kp_image, des_image );
    	matcher.knnMatch(desObject, des_image, matches, 2);

        for(int i = 0; i < min(des_image.rows-1,(int) matches.size()); i++) //THIS LOOP IS SENSITIVE TO SEGFAULTS
        {
        	if((matches[i][0].distance < thresholdMatchingNN*(matches[i][1].distance)) && ((int) matches[i].size()<=2 && (int) matches[i].size()>0))
            {
                good_matches.push_back(matches[i][0]);
            }
        }

        //Draw only "good" matches
        drawMatches( object, kpObject, image, kp_image, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS );

        if (good_matches.size() >= thresholdGoodMatches)
        {

        	//Display that the object is found
        	putText(img_matches, "Object Found", cvPoint(10,50),FONT_HERSHEY_COMPLEX_SMALL, 2, cvScalar(0,0,250), 1, CV_AA);
            for(unsigned int i = 0; i < good_matches.size(); i++ )
            {
                //Get the keypoints from the good matches
                obj.push_back( kpObject[ good_matches[i].queryIdx ].pt );
                scene.push_back( kp_image[ good_matches[i].trainIdx ].pt );
            }

            H = findHomography( obj, scene, CV_RANSAC );

            perspectiveTransform( obj_corners, scene_corners, H);

            //Draw lines between the corners (the mapped object in the scene image )
            line( img_matches, scene_corners[0] + Point2f( object.cols, 0), scene_corners[1] + Point2f( object.cols, 0), Scalar(0, 255, 0), 4 );
            line( img_matches, scene_corners[1] + Point2f( object.cols, 0), scene_corners[2] + Point2f( object.cols, 0), Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[2] + Point2f( object.cols, 0), scene_corners[3] + Point2f( object.cols, 0), Scalar( 0, 255, 0), 4 );
            line( img_matches, scene_corners[3] + Point2f( object.cols, 0), scene_corners[0] + Point2f( object.cols, 0), Scalar( 0, 255, 0), 4 );
        }
        else
        {
            putText(img_matches, "", cvPoint(10,50), FONT_HERSHEY_COMPLEX_SMALL, 3, cvScalar(0,0,250), 1, CV_AA);  
        }

        //Show detected matches
        imshow( "Good Matches", img_matches );
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
