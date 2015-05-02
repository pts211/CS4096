#include "Navigation.h"
#include <fstream>
#include <queue>
// #include <iostream>
// #include <string>
// #include <cstring>
// #include <dirent.h>
>>>>>>> FETCH_HEAD

using namespace std;

double Node::DNE = 1000000.0;

Navigation::Navigation(const char* filename)
{
  inputNodes(filename); //forgot what your formwat was clayton and don't want to deal with it too much
  if(allNodes.size() > 1) //keep in mind, the output path doesn't include the source
  {
    cout << "from " << allNodes[0].name << " to " << allNodes[allNodes.size()-1].name << endl;
    travelFromSourceToSink(&allNodes[0], &allNodes[allNodes.size()-1]); 
  }
}

void Navigation::inputNodes(const char* filename)
{
	ifstream in;
	string inStr;
	int numNodes;

	in.open(filename);

	if(in.good())
  { //if the file was successfully opened
		getline(in, inStr); //read the number of nodes
		numNodes = atoi(inStr.c_str()); //change the string we read to an int
		for(int i = 0; i < numNodes; i++)
    { //loop for each node we have
			getline(in, inStr); //read the name of a node
      //cout << inStr << endl;
			Node n(inStr, Node::DNE); //create a new node with nonexistant g value
      n.parent = NULL;
			allNodes.push_back(n); //put that node in the list of all nodes
		}

		for(int i = 0; i < numNodes; i++)
    { //loop for each node we have
			getline(in, inStr); //read the number of neighbors a node has
      cout << "numNeighbors :: " << atoi(inStr.c_str()) << endl; //output is wrong here
			for(int j = 0; j < atoi(inStr.c_str()); j++)
      { //loop for each neighbor of a node
				getline(in, inStr, ','); //read the nodes neighbor
				allNodes[i].neighbors.push_back(&allNodes[atoi(inStr.c_str())]); //store the node in the list of neighbors
				getline(in, inStr, ':'); //read the neighbors weight
				allNodes[i].weights.push_back(atoi(inStr.c_str())); //assign the weight
			}
		}
	}
}

bool Navigation::travelFromSourceToSink(Node* source, Node* sink)
{
  vector<Node*> path;
  //while not yet there...
  bool arrived = false;
  while(!arrived)
  {
    //path from CURRENT place to sink Node                                                  FIX THIS
    path = findPath(source, sink); //should be (current, sink) with current starting at source
    outputPath(path);
    arrived = walkPath(path);
  }

  return true; //or false if ???
}

bool Navigation::walkPath(const vector<Node*>& path)
{
  //walk the walk using roomba commands

  //if blocked, restart pathfinding from current spot.
  return true;
}

//this is now BFS not A*... because we don't know a heuristic and it don't matter anyway (small graphs)
vector<Node*> Navigation::findPath(Node* source, Node* sink)
{
  //cout << "findPath" << endl;
  for(size_t i=0; i<allNodes.size(); ++i)
  {
    allNodes[i].g_score = Node::DNE;
    allNodes[i].parent = NULL;
  }

  source->g_score = 0.0;
  //The set of tentative Nodes that need to be evaluated, starting with sink 
  queue<Node*> openSet;
  openSet.push(source);

  //while openset is not empty
  while(!openSet.empty())
  {
    //cout << "loop openSet.size() :: " << openSet.size() << endl;
    Node* current = openSet.front();
    cout << current->name << endl;
    openSet.pop();

    //for each neighbor in current->neighbors
    //cout << "neighbors.size() :: " << current->neighbors.size() << endl;
    for(size_t i=0; i<current->neighbors.size(); ++i)
    {
      Node* neighbor = current->neighbors[i];
      if(neighbor == NULL)
        continue;
      double tempScore = current->g_score + current->weights[i];
      //if neighbor not yet evaluated or better score
      if(neighbor->g_score == Node::DNE || tempScore < neighbor->g_score)
      {
        //if neighbor not yet evaluated
        if(neighbor->g_score == Node::DNE)
          //add neighbor to openset
          openSet.push(neighbor);
        //came_from[neighbor] = current
        neighbor->parent = current;
        //g_score[neighbor] = tentative_g_score
        neighbor->g_score= tempScore;
      }
    }
  }

  return reconstruct_path(sink);
}

vector<Node*> Navigation::reconstruct_path(Node* current)
{
  vector<Node*> path;
  while(current->parent != NULL)
  {
    path.insert(path.begin(), current);
    current = current->parent;
  }
  return path;
}

void Navigation::outputPath(const vector<Node*>& path)
{
  cout << endl << "start path" << endl;
  for(size_t i=0; i<path.size(); ++i)
  {
    cout << "\t" << path[i]->name << endl;
  }
  cout << "end path\n" << endl;
}

void Navigation::outputAllNodes()
{
  for(size_t i=0; i<allNodes.size(); ++i)
  {
    cout << allNodes[i].name << " " << allNodes[i].neighbors.size() << endl;
  }
}

