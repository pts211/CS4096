#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
using namespace std;

class Node {
	public:
		Node(string name);
		void AddNodeToNeighbors(Node* n);
		void SetChild(Node* child);
	private:
		string name;
		Node* child;
		vector<Node> neighbors;
};

#endif