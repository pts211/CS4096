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
	Node(string name, int g): name(name), g_score(g) {} //, f_score(f) {}
	string name;
	double g_score; //cost from source
	//double f_score; //g_score + weights[sink] a.k.a. full cost from source to sink when going through this node
	Node* parent;
	vector<Node*> neighbors;
	vector<double> weights;
	static double DNE;
};

class Navigation
{
public:
	Navigation(const char* filename);
	void inputNodes(const char* filename);
	bool travelFromSourceToSink(Node* source, Node* sink);
private:
	bool walkPath(const vector<Node*>& path);
	vector<Node*> findPath(Node* source, Node* sink);
	vector<Node*> reconstruct_path(Node* current);
	void outputPath(const vector<Node*>& path);
	void outputAllNodes();
	vector<Node> allNodes;
};

#endif