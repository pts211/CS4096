#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
using namespace std;

class Node
{
	public:
		Node(string name);
		void AddNodeToNeighbors(Node* n);
		void SetChild(Node* child);
	private:
		string name;
		Node* child;
		vector<Node*> neighbors;
};

class Navigation
{
	public:
		Navigation();
		void InputNodes();
		vector<Node*> reconstruct_path(map<Node*, Node*>& cameFrom, Node* current);
		Node* minCostNode(set<Node*>& openSet, map<Node*, int>& f_cost);
		void NavigateToNode(Node* source, Node* sink);
	private:
		void WalkPath(vector<Node*> path);
		vector<Node*> allNodes;
		
};

#endif