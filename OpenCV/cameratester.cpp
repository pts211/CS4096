//main.cpp tester for camera.h
#include "camera.h"

using namespace cv;
using namespace std; 


int main()
{
	camera cam;
	while(1)
	{
		cam.update();
		cout << cam << endl;
	}


	 return 0;


}




