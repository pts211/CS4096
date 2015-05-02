#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <utility>
using namespace std;

struct Node
{
		Node(string name): name(name) {}
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
		vector<Node> allNodes;
		map< pair<Node*, Node*>, int> weights;
	};

#endif