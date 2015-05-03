#include "camera.h"
using namespace std;
using namespace cv;

void camera::update(int EDGE_NUM,int B_MIN,int G_MAX,int R_MAX)
{

	pathisblocked = false; //NO IMPLEMENTATION YET

	floorsign = ""; //NO IMPLEMENTATION YET

	
	
	while(!cap.isOpened())  // if not success, exit program
    {
        cout << "error cannot read from the camera" << endl;
    }
	double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

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
	double** cplot;
	cplot = new double *[x_length];
	for(int i=0; i<x_length; i++)
	{
			cplot[i] = new double[2]; // 0 is for x, 1 is for y
	}
	double** cplot2;
	cplot2 = new double *[x_length];
	for(int i=0; i<x_length; i++)
	{
			cplot2[i] = new double[2]; // 0 is for x, 1 is for y
	}
	
	
	int plot_length, plot_length2;
	int cplot_length, cplot_length2;

	
	//Sleep(100);

	bool bSuccess = cap.read(input); // read a new frame from video
    if (!bSuccess) //if not success, break loop
    {
            cout << "Cannot read a frame from video stream" << endl;
			// Sleep(1000);
    }

	img = input.clone();

	
	for(int x=0; x<dWidth; x++)
	{
		for(int y=0; y<dHeight; y++)
		{
			Vec3b &intensity = img.at<Vec3b>(y,x);
			if (intensity.val[0] < B_MIN && intensity.val[1] > G_MAX && intensity.val[2] > R_MAX)
			{
				intensity.val[0] = 0 ;
				intensity.val[1] = 0 ;
				intensity.val[2] = 0 ;

			}


		}
	}
	



		cvtColor(input, edges, CV_BGR2GRAY);
        GaussianBlur(edges,edges, cv::Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, EDGE_NUM, 3);

		modi = edges.clone(); //NOT TEST
		//cvtColor(pic, modi, CV_BGR2GRAY);///TEST ONLY

		double left_slope = 0, right_slope = 0;
		double cleft_slope = 0, cright_slope = 0;
		//FOR LEFT AND RIGHT SIDE
		 
			 
	Rect r(center_x - offset - x_length, dHeight - y_length, x_length, y_length);
	Rect r2(center_x + offset, dHeight - y_length, x_length, y_length);
			
	Mat smallImg = modi(r);
	Mat csmallImg = img(r);
		
	Mat smallImg2 = modi(r2);
	Mat csmallImg2 = img(r2);

					 

	plot_length = 0; //left side
	cplot_length = 0;
	plot_length2 = 0; //right side
	cplot_length2 = 0;
	for(int x = 0; x < x_length; x++)
	{
		bool found = false;
		bool found2 = false;
		double color_center = 0;
		double color_center2 = 0;

		double total_color = 0;
		double total_color2 = 0;

		for(int y = 0; y < y_length; y++)
		{
			Scalar intensity = smallImg.at<uchar>(y,x);
			Vec3b &cintensity = csmallImg.at<Vec3b>(y,x);	

			Scalar intensity2 = smallImg2.at<uchar>(y,x);
			Vec3b &cintensity2 = csmallImg2.at<Vec3b>(y,x);

			if( intensity.val[0] == 255 & y!=0)
			{
			plot[ plot_length][0] = double(x);
			plot[plot_length][1] = double(y);
			found = true;
			}
			if( cintensity.val[0] > B_MIN && cintensity.val[1] < G_MAX && cintensity.val[2] < R_MAX)
			{
				color_center+=y;
				total_color ++;

			}				

			if( intensity2.val[0] == 255 & y!=0)
			{
			plot2[plot_length2][0] = double(x);
			plot2[plot_length2][1] = double(y);
			found2 = true;
			}
			if( cintensity2.val[0] > B_MIN && cintensity2.val[1] < G_MAX && cintensity2.val[2] < R_MAX)
			{
				color_center2+=y;
				total_color2 ++;
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

		color_center = color_center / total_color;
		color_center2 = color_center2 / total_color2;

		if(total_color>0)
		{
			cplot[cplot_length][0] = double(x);
			cplot[cplot_length][1] = color_center;
			cplot_length++;
		}
			
		if(total_color2>0)
		{
			cplot2[cplot_length2][0] = double(x);
			cplot2[cplot_length2][1] = color_center2;
			cplot_length2++;
		}


			 	
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

	line(input,pt1,pt2,Scalar(255,255,255),2,8);
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

	line(input,pt1,pt2,Scalar(255,255,255),2,8);
	right_slope = 0 - b;

	//linear regression3!

	x_bar = 0; y_bar = 0;
	for(int i=0; i<cplot_length2; i++)
	{
		x_bar+=cplot2[i][0];
		y_bar+=cplot2[i][1];
	}

	x_bar = x_bar / cplot_length2;
	y_bar = y_bar / cplot_length2;

	Sx = 0; Sy = 0; Sxy = 0;
	for(int i=0; i < cplot_length2; i++)
	{
		Sx += pow((cplot2[i][0] - x_bar),2);
		Sy += pow((cplot2[i][1] - y_bar),2);
		Sxy += (cplot2[i][0] - x_bar)*(cplot2[i][1] - y_bar);
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

	line(input,pt1,pt2,Scalar(255,0,0),2,8);
	cright_slope = 0 - b;

	//linear regression4!!!!!!

	x_bar = 0; y_bar = 0;
	for(int i=0; i<cplot_length; i++)
	{
		x_bar+=cplot[i][0];
		y_bar+=cplot[i][1];
	}
	x_bar = x_bar / cplot_length;
	y_bar = y_bar / cplot_length;

	Sx = 0; Sy = 0; Sxy = 0;
	for(int i=0; i < cplot_length; i++)
	{
		Sx += pow((cplot[i][0] - x_bar),2);
		Sy += pow((cplot[i][1] - y_bar),2);
		Sxy += (cplot[i][0] - x_bar)*(cplot[i][1] - y_bar);
	}

	b = Sxy/Sx;
	a = y_bar - (b * x_bar);
	// double x_intercept = ((0 - a) / b) + center_x - offset - x_length; //relative to modi
	start_x = int( ((y_length-1 - a) / b) + center_x - offset - x_length);
	start_y = dHeight - 1;
		
	pt1.x = start_x;
	pt1.y = start_y;

	end_x = int( (dHeight-1)/(0-b) + start_x);
	end_y = 0;
	pt2.x = end_x;
	pt2.y = end_y;

	line(input,pt1,pt2,Scalar(255,0,0),2,8);
	cleft_slope = 0 - b;


	e_slope = left_slope + right_slope;
	c_slope = cleft_slope + cright_slope;


	 

	  

	/*
	for(int i=0; i<plot_length; i++)
	{
		cout << plot[i][0] << ", " << plot[i][1] << endl;
	}
	while(1);
	*/
	return;
}

void camera::output()
{
    
  	namedWindow("Input",0); //create a window called "MyVideo"
  	imshow("Input", input); //show the frame in "MyVideo" window
   
    namedWindow("Edges",0);
    imshow("Edges", edges);
    
    namedWindow("Color",0);
    imshow("Color",img); 
    
   
  	if (waitKey(1) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
  	{
  		cout << "esc key is pressed by user" << endl;
      exit(1);
  	}
  
  	cout << "[ " << pathisblocked << ", " << floorsign << ", " << e_slope << ", " << c_slope << " ]" << endl;
  
    return;
}


Mat camera::getinput() const
{
	return input;
}

double camera::gete_slope() const
{
	return e_slope;
}

double camera::getc_slope() const
{
	return c_slope;
}

bool camera::getpathisblocked() const
{ 
	return pathisblocked;
}

string camera::getfloorsign() const
{
	return floorsign;
}
