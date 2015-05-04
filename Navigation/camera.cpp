#include "camera.h"
using namespace std;
using namespace cv;


const int P_NUM_MAX = 100;
const int B_LIM = 255;
const int G_LIM = 255;
const int R_LIM = 255;
const int L_LIM = 480;
const string trackbarWindowName1 = "Trackbar1";
const string trackbarWindowName2 = "Trackbar2";
const string trackbarWindowName3 = "Trackbar3";
const string trackbarWindowName4 = "Trackbar4";
const string trackbarWindowName5 = "Trackbar5";

void camera::update()
{

	pathisblocked = false; //NO IMPLEMENTATION YET

	floorsign = ""; //NO IMPLEMENTATION YET

	
	
	while(!cap.isOpened())  // if not success, exit program
  {
    cout << "error cannot read from the camera" << endl;
  }
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

	int y_length = L_NUM; //max value of dHeight = 480
  int x_length = int(dHeight);

	double** plot;
	plot = new double *[x_length];
	for(int i=0; i<x_length; i++)
	{
	  plot[i] = new double[2]; // 0 is for x, 1 is for y
	}
 	int plot_length = 0;
		


	bool bSuccess = cap.read(img); // read a new frame from video
  if (!bSuccess) //if not success, break loop
  {
    cout << "Cannot read a frame from video stream" << endl;
	}
 
  double x_center;
  double x_total;
  
  int sign_count = 0;
  int sign_color = 220;
	
	for(int y = dHeight-1; y >= dHeight-y_length; y--)
	{
    x_center = 0;
    x_total = 0;
    
    //cout << "y: " << y << endl;
 
		for(int x = dWidth-1; x >= 0; x--)
		{
     
     //cout << "x: " << x << endl;
   
			Vec3b &intensity = img.at<Vec3b>(y,x);
			if (intensity.val[0] > B_MIN && intensity.val[1] < G_MAX && intensity.val[2] < R_MAX)
      {
        x_center += x;
        x_total++;
        
      }
      else if( y>460 && x>310 && x<330 && intensity.val[0] > sign_color && intensity.val[1] > sign_color && intensity.val[2] > sign_color)
      {
         sign_count++;
      
      }
      else
			{
				intensity.val[0] = 0 ;
				intensity.val[1] = 0 ;
				intensity.val[2] = 0 ;
			}
    }
        
    if(x_total >= P_NUM) //if enough points collected, then a valid data point, add to plot
    {
      //cout << "x_center: " << x_center << endl;
      //cout << "x_total: " <<x_total << endl;
    
      x_center = x_center / x_total; //compute average x
      
      //cout << "x_center: " << x_center << endl;
    
      plot[plot_length][0] = dHeight-1 - y;
      plot[plot_length][1] = dWidth-1 - x_center;
      plot_length++;
      
      Vec3b &intensity = img.at<Vec3b>(y,x_center);
      
      intensity.val[0] = 0; 
      intensity.val[1] = 0;
      intensity.val[2] = 255; //RED      
       
    }    
	}		 
 
  if(sign_count >= 10)
  {
    floorsign = "A";
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
 
  /*
	int start_x = 0;
	int start_y = a;
	Point pt1,pt2;
	pt1.x = int(dWidth-1-start_y);
	pt1.y = int(dHeight-1-start_x);

	int end_x = dHeight-1;
	int end_y = b*(dHeight-1) + a;
	pt2.x = int(dWidth-1-start_y);
	pt2.y = int(dHeight-1-start_x);
  */
	//line(img,pt1,pt2,Scalar(0,255,0),2,8);
  intercept = a;
	slope = b;

	
	return;
}

void camera::output()
{    
 // namedWindow("img",0); //create a window 
 // imshow("img", img); //show the frame 
 
  // if (waitKey(1) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
  // 	{
  // 	cout << "esc key is pressed by user" << endl;
  //   cout << "Trackbars are: " << P_NUM << ", " << B_MIN << ", " << G_MAX << ", " << R_MAX << ", " << L_NUM << endl;   
   
  //  exit(1);
 	// }
  
  	cout << "[ " << pathisblocked << ", " << floorsign << ", " << intercept << ", " << slope << " ]" << endl;
  
    return;
}

void on_trackbar( int, void* )
{//This function gets called whenever a
    // trackbar position is changed
}

void camera::createTrackBars()
{
    //create window for trackbars
 
  namedWindow(trackbarWindowName1,1);
  namedWindow(trackbarWindowName2,1);
  namedWindow(trackbarWindowName3,1);
  namedWindow(trackbarWindowName4,1);
  namedWindow(trackbarWindowName5,1);

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

//ACCESSORS DEFINITIONS

Mat camera::getimg() const
{
	return img;
}

double camera::getintercept() const
{
	return intercept;
}

double camera::getslope() const
{
	return slope;
}

bool camera::getpathisblocked() const
{ 
	return pathisblocked;
}

string camera::getfloorsign() const
{
	return floorsign;
}
