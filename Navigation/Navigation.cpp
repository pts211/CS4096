#include "Navigation.h"
#include <fstream>
// #include <iostream>
// #include <string>
// #include <cstring>
// #include <dirent.h>

Node::Node(string name) {
	this->name = name;
}

void Node::SetChild(Node* child) {
	this->child = child;
}

void Node::AddNodeToNeighbors(Node* n) {
	this->neighbors.push_back(n);
}

void Navigation::InputNodes() {
	// DIR* FD;
	// dirent* in_file;

	// if(NULL == (FD = opendir(nodeFilePath.c_str()))) {
	// 	cout << "ERROR: Node file not found." << endl;
	// }

	// while((in_file = readdir(FD))) {

	// }
	ifstream in;
	string inStr;
	int numNodes;

	if(in.good()) {
		getline(in, inStr);
		numNodes = atoi(inStr.c_str());
		for(int i = 0; i < numNodes; i++) {
			
		}
	}
}