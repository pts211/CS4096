#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "camera.h"
#include "Roomba.h"
// #include "Roomba.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
#include "time.h"
using namespace std;

enum CardinalDirections {NORTH, SOUTH, EAST, WEST};

struct Node
{
	Node(string name, int g): name(name), g_score(g) {} //, f_score(f) {}
	string name;
	double g_score; //cost from source
	//double f_score; //g_score + weights[sink] a.k.a. full cost from source to sink when going through this node
	Node* parent;
	// vector<std::tuple<Node*, int, Direction>> info;	
	vector<Node*> neighbors;
	vector<double> weights;
	vector<CardinalDirections> allDirections;
	CardinalDirections directionTraveled;
	static double DNE;
	static double LARGE_NUM;
};

class Navigation
{
public:
	Navigation(bool test) {roomba.start(); roomba.powerOn(); roomba.getSensors(Sensor::ALL); startTime = clock(); testMode = test;}
	Navigation(const char* filename, bool test);
	void inputNodes(const char* filename);
	bool travelFromSourceToSink(Node* source, Node* sink);
	Node* walkToStartingNode();
	Node* getNode(int index);

	//Used to test private functions
	vector<Node*> dbgReconstructPath(Node* source, Node* sink);
	vector<Node*> dbgFindPath(Node* source, Node* sink);

private:
	void moveForwardUntilSignOrBlockage();
	void walkPath(vector<Node*> path);
	vector<Node*> findPath(Node* source, Node* sink);
	vector<Node*> reconstructPath(Node* current, Node* sink);
	void incrementWeight(Node* n1, Node* n2);
	void outputPath(const vector<Node*>& path);
	void outputAllNodes();
	void turnAtIntersection(vector<Node*> path, int currentNode);
	void rotate(int degrees);
	string getFloorSign();
	string _getFloorSign();
	bool getPathIsBlocked();
	bool _getPathIsBlocked();
	vector<Node> allNodes;
	int numNodes;
	camera cam;
	Roomba roomba;
	RoombaSensors sensor;
	clock_t startTime;
	bool testMode;
};

#endif
