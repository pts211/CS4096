#include "Navigation.h"
#include <iostream>
#include <assert.h>

using namespace std;

void testInputNodes(const char* file);
void testReconstructPath(const char* file);
void testFindPath(const char* file);
void testTravelFromSourceToSink(const char* file);

int main(int argc, char const *argv[])
{
	if(argc != 2)
	{
		cout << "usage:\n\t./a.out inputFileName" << endl;
		return 0;
	}

	system("clear");
	cout << "Running test on Navigation::inputNodes" << endl;
	testInputNodes(argv[1]);
	cout << "[PASS] Navigation::inputNodes" << endl << endl;
	cout << "Running test on Navigation::reconstructPath" << endl;
	testReconstructPath(argv[1]);
	cout << "[PASS] Navigation::reconstructPath" << endl << endl;
	testFindPath(argv[1]);
	cout << "Running test on Navigation::findPath" << endl;
	cout << "[PASS] Navigation::findPath" << endl << endl;
	testTravelFromSourceToSink(argv[1]);
	cout << "Running test on Navigation::travelFromSourceToSink" << endl;
	cout << "[PASS] Navigation::travelFromSourceToSink" << endl << endl;

	return 0;
}

void testInputNodes(const char* file)
{
	Navigation defaultNav;

	defaultNav.inputNodes(file);

	assert(defaultNav.getNode(0)->name=="node0");
	assert(defaultNav.getNode(0)->neighbors[0]->name=="node1");
	assert(defaultNav.getNode(0)->neighbors[1]->name=="node3");
	assert(defaultNav.getNode(0)->weights[0]==10);
	assert(defaultNav.getNode(0)->weights[1]==25);

	assert(defaultNav.getNode(1)->name=="node1");
	assert(defaultNav.getNode(1)->neighbors[0]->name=="node0");
	assert(defaultNav.getNode(1)->neighbors[1]->name=="node2");
	assert(defaultNav.getNode(1)->weights[0]==10);
	assert(defaultNav.getNode(1)->weights[1]==5);

	assert(defaultNav.getNode(2)->name=="node2");
	assert(defaultNav.getNode(2)->neighbors[0]->name=="node1");
	assert(defaultNav.getNode(2)->neighbors[1]->name=="node3");
	assert(defaultNav.getNode(2)->neighbors[2]->name=="node4");
	assert(defaultNav.getNode(2)->weights[0]==5);
	assert(defaultNav.getNode(2)->weights[1]==5);
	assert(defaultNav.getNode(2)->weights[2]==20);

	assert(defaultNav.getNode(3)->name=="node3");
	assert(defaultNav.getNode(3)->neighbors[0]->name=="node0");
	assert(defaultNav.getNode(3)->neighbors[1]->name=="node2");
	assert(defaultNav.getNode(3)->neighbors[2]->name=="node4");
	assert(defaultNav.getNode(3)->weights[0]==25);
	assert(defaultNav.getNode(3)->weights[1]==5);
	assert(defaultNav.getNode(3)->weights[2]==5);

	assert(defaultNav.getNode(4)->name=="node4");
	assert(defaultNav.getNode(4)->neighbors[0]->name=="node2");
	assert(defaultNav.getNode(4)->neighbors[1]->name=="node3");
	assert(defaultNav.getNode(4)->weights[0]==20);
	assert(defaultNav.getNode(4)->weights[1]==5);
}

void testReconstructPath(const char* file)
{
	Navigation defaultNav;
	vector<Node*> path;

	defaultNav.inputNodes(file);

	defaultNav.getNode(4)->parent = defaultNav.getNode(3);
	defaultNav.getNode(3)->parent = defaultNav.getNode(2);
	defaultNav.getNode(2)->parent = defaultNav.getNode(1);
	defaultNav.getNode(1)->parent = defaultNav.getNode(0);
	defaultNav.getNode(0)->parent = NULL;

	path = defaultNav.dbgReconstructPath(defaultNav.getNode(4));

	assert(!path.empty());
	assert(path[0] == defaultNav.getNode(1));
	assert(path[1] == defaultNav.getNode(2));
	assert(path[2] == defaultNav.getNode(3));
	assert(path[3] == defaultNav.getNode(4));
}

void testFindPath(const char* file)
{
	Navigation defaultNav;
	vector<Node*> path;

	defaultNav.inputNodes(file);
	path = defaultNav.dbgFindPath(defaultNav.getNode(0), defaultNav.getNode(4));

	assert(!path.empty());
	assert(path[0] == defaultNav.getNode(1));
	assert(path[1] == defaultNav.getNode(2));
	assert(path[2] == defaultNav.getNode(3));
	assert(path[3] == defaultNav.getNode(4));	
}

void testTravelFromSourceToSink(const char* file)
{
	Navigation defaultNav;
	bool arrived;

	defaultNav.inputNodes(file);

	arrived = defaultNav.travelFromSourceToSink(defaultNav.getNode(0), defaultNav.getNode(4));

	assert(arrived);
}