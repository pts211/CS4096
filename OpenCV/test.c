#include "cv.h"
#include "highgui.h"

int main( int arc, char ** argv)
{
int W = 640;
int H = 480;

// Images are stored in IplImage structs. The function cvCreateImage
// allocates space for an image of a certain type, in this case an
// 8 bit, 1 channel (grayscale) image.
IplImage *image = cvCreateImage(cvSize(W, H), IPL_DEPTH_8U, 1);

// Create a named window and show our image on it
cvNamedWindow("My First OpenCV Program", 1);
cvShowImage("My First OpenCV Program", image);

// Wait for any keystroke and then release the image from memory and exit
// the program
cvWaitKey(0);
cvReleaseImage(&image);
return 0;
}