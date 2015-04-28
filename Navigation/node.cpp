#include "node.h"

Node::Node(string name) {
	this->name = name;
}

void Node::SetChild(Node* child) {
	this->child = child;
}

void Node::AddNodeToNeighbors(Node* n) {
	this->neighbors.push_back(n);
}