#include "Navigation.h"
#include <fstream>
// #include <iostream>
// #include <string>
// #include <cstring>
// #include <dirent.h>

void Navigation::InputNodes() {
	ifstream in;
	string inStr;
	int numNodes;

	in.open("SampleFactory.txt");

	if(in.good()) { //if the file was successfully opened
		getline(in, inStr); //read the number of nodes
		numNodes = atoi(inStr.c_str()); //change the string we read to an int
		for(int i = 0; i < numNodes; i++) { //loop for each node we have
			getline(in, inStr); //read the name of a node
			Node n(inStr); //create a new node
			allNodes.push_back(n); //put that node in the list of all nodes
		}

		for(int i = 0; i < numNodes; i++) { //loop for each node we have
			getline(in, inStr); //read the number of neighbors a node has
			for(int j = 0; j < atoi(inStr.c_str()); j++) { //loop for each neighbor of a node
				getline(in, inStr, ','); //read the nodes neighbor
				allNodes[i].neighbors.push_back(&allNodes[atoi(inStr.c_str())]); //store the node in the list of neighbors
				getline(in, inStr, ':'); //read the neighbors weight
				weights[make_pair(allNodes[i], allNodes[i].neighbors[j])] = atoi(inStr.c_str()); //assign the weight
			}
		}
	}
}

vector<Node*> Navigation::reconstruct_path(map<Node*, Node*>& cameFrom, Node* current)
{
  vector<Node*> path;
  while(cameFrom.find(current) != cameFrom.end())
  {
    path.insert(path.begin(), current);
    current = cameFrom[current];
  }
  return path;
}

Node* Navigation::minCostNode(set<Node*>& openSet, map<Node*, int>& f_cost)
{
  Node* Node;
  int minCost = 100000;
  for(set<Node*>::iterator i = openSet.begin(); i != openSet.end(); ++i)
  {
    if(f_cost.find(*i) != f_cost.end())
      if(f_cost[*i] < minCost)
      {
        minCost = f_cost[*i];
        Node = *i;
      }
  }
  return Node;
}

void Navigation::NavigateToNode(Node* source, Node* sink)
{
  //cout << "NavigateToNode" << endl;
  //The set of nodes already evaluated.
  set<Node*> closedSet;
  //The set of tentative Nodes that need to be evaluated, starting with sink (searching backwards)
  set<Node*> openSet;
  openSet.insert(sink);
  //The map of navigated nodes.
  map<Node*, Node*> cameFrom;
  //Cost from start along best known path.
  map<Node*, int> g_score;
  g_score[source] = 0;
  //total cost of its possible path, f = g + manhattanDistance(current, sink)
  map<Node*, int> f_score;
  f_score[source] = manhattanDistance(source, sink);

  //while openset is not empty
  while(!openSet.empty())
  {
    //cout << "loop openSet.size() > 0 :: " << openSet.size() << endl;
    //current = the node in openset having the lowest f_score[] value
    Node* current = minCostNode(openSet, f_score);
    //if current = goal
    if(NodesAreEqual(current, sink))
      //return reconstruct_path(came_from, goal)
      return reconstruct_path(cameFrom, sink);

    //remove current from openset
    openSet.erase(current);
    //add current to closedset
    closedSet.insert(current);
    //for each neighbor in neighbor_nodes(current)
    vector<Node*> neighbors = getNeighbors(current);
    for(size_t i=0; i<neighbors.size(); i++)
    {
      Node* neighbor = neighbors[i];
      if(neighbor == NULL)
        continue;
      int tentative_g_score = g_score[current] + weights[NodesKeyAt(neighbor)];
      //if it's not a valid place to step
      if(!validSteps[NodesKeyAt(neighbor)])
      {
        //if we wanted to stop right before we got there and it's the sink, then we're done!
        if(stopBeforeArrival && NodesAreEqual(neighbor, sink))
        {
          sinkIsNeighbor = true;
          return reconstruct_path(cameFrom, current);
        }
        else //else this is just an invalid Node to step on :(
          continue;
      }

      //tentative_f_score = tentative_g_score + manhattanDistance(neighbor, goal)
      int tentative_f_score = tentative_g_score + manhattanDistance(neighbor, sink);

      if(f_score.find(neighbor) == f_score.end())
        f_score[neighbor] = 100000;
      //if neighbor in closedset
      if(closedSet.find(neighbor) != closedSet.end())
      {
        //if tentative_f_score >= f_score[neighbor]
        if(tentative_f_score >= f_score[neighbor])
            //continue
            continue;
      }

      //if neighbor not in openset or tentative_f_score < f_score[neighbor]
      if(openSet.find(neighbor) == openSet.end() || tentative_f_score < f_score[neighbor])
      {
        //came_from[neighbor] = current
        cameFrom[neighbor] = current;
        //g_score[neighbor] = tentative_g_score
        g_score[neighbor] = tentative_g_score;
        //f_score[neighbor] = tentative_f_score
        f_score[neighbor] = tentative_f_score;
        //if neighbor not in openset
        if(openSet.find(neighbor) == openSet.end())
          //add neighbor to openset
          openSet.insert(neighbor);
      }
    }

  }
  //return empty
  vector<Node*> path;
  return path;
}