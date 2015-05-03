#include "Navigation.h"
#include <iostream>

using namespace std;

int main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		cout << "usage:\n\t./a.out inputFileName" << endl;
		return 0;
	}
	Navigation sluttyClayton(argv[1]);

	return 0;
}
