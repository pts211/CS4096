#include "Navigation.h"
#include <fstream>
#include <queue>
// #include <iostream>
// #include <string>
// #include <cstring>
// #include <dirent.h>

using namespace std;

double Node::DNE = 1000000.0;
double Node::LARGE_NUM = 1000.0;

Navigation::Navigation(const char* filename)
{
  inputNodes(filename); //forgot what your format was clayton and don't want to deal with it too much
  if(allNodes.size() > 1) //keep in mind, the output path doesn't include the source
  {
    cout << "from " << allNodes[0].name << " to " << allNodes[allNodes.size()-1].name << endl;
    // travelFromSourceToSink(&allNodes[0], &allNodes[allNodes.size()-1]); 
    travelFromSourceToSink(walkToStartingNode(), &allNodes[allNodes.size()-1]);
  }
}

void Navigation::inputNodes(const char* filename)
{
	ifstream in;
	string inStr;
	int numNeighbors;
	int neighbor;
	int weight;
	int direction;

	in.open(filename);

	if(in.good()){ //if the file was successfully opened
		in >> inStr; //read the number of nodes
		numNodes = atoi(inStr.c_str()); //change the string we read to an int
		for(int i = 0; i < numNodes; i++){ //loop for each node we have
			in >> inStr; //read the name of a node
			Node n(inStr, Node::DNE); //create a new node with nonexistant g value
      		n.parent = NULL;
			allNodes.push_back(n); //put that node in the list of all nodes
		}

		for(int i = 0; i < numNodes; i++){ //loop for each node we have
			in >> inStr; //read the number of neighbors a node has
			numNeighbors = atoi(inStr.c_str());
			for(int j = 0; j < numNeighbors; j++) { //loop for each neighbor of a node
				in >> inStr;
				neighbor = atoi(inStr.c_str()); //read the nodes neighbor
				in >> inStr;
				weight = atoi(inStr.c_str());
				in >> inStr;
				direction = atoi(inStr.c_str());
				// allNodes[i].info[j] = make_tuple(&allNodes[neighbor], weight, direction);
				allNodes[i].neighbors.push_back(&allNodes[neighbor]); //store the node in the list of neighbors
				// in >> inStr; //read the neighbors weight
				allNodes[i].weights.push_back(weight); //assign the weight
				switch(direction) { //assign the direction we need to go in to reach the neighbor
					case 1:
						allNodes[i].allDirections.push_back(NORTH);
						break;
					case 2:
						allNodes[i].allDirections.push_back(SOUTH);
						break;
					case 3:
						allNodes[i].allDirections.push_back(EAST);
						break;
					case 4:
						allNodes[i].allDirections.push_back(WEST);
						break;
				}				 
			}
		}
	}
}

bool Navigation::travelFromSourceToSink(Node* source, Node* sink)
{
  vector<Node*> path = findPath(source, sink); //should be (current, sink) with current starting at source
  #ifdef DEBUG 
  outputPath(path); 
  #endif
  return walkPath(path);
}

Node* Navigation::getNode(int index)
{
  return &allNodes[index];
}

vector<Node*> Navigation::dbgReconstructPath(Node* current)
{
  vector<Node*> path;

  #ifdef TEST
    return reconstructPath(current);
  #else
    cout << "This function is only for testing." << endl;   
  #endif

  return path;
}

vector<Node*> Navigation::dbgFindPath(Node* source, Node* sink)
{
  vector<Node*> path;

  #ifdef TEST
    return findPath(source, sink);
  #else
    cout << "This function is only for testing." << endl;   
  #endif

  return path;
}

Node* Navigation::walkToStartingNode()
{
	while(1) {
		moveForwardUntilSignOrBlockage();
		if(!cam->getfloorsign().empty()) {
			for(int i = 0; i < allNodes.size() - 1; i++) {
				if(cam->getfloorsign() == allNodes[i].name) {
					return &allNodes[i];
				}
			}
		}

		if(cam->getpathisblocked()) {
			roomba.drive(0,180);
		}	
	}
}

void Navigation::moveForwardUntilSignOrBlockage()
{
  roomba.drive(500, 0); //                              is drive temporary or constant until you drive(0, 0)???????
  while(cam->getfloorsign().empty() && !cam->getpathisblocked())
  {
    cam->update();
    if(cam->getc_slope() > 5) { //arbitrary tolerances. slope will be 0 if we are going straight    	
    	while(cam->getc_slope() > 2) {
    		roomba.drive(250, 10); //arbitrary radius, change later
    		cam->update();
    	}
    } else if(cam->getc_slope() < -5) {
    	while(cam->getc_slope() < -2) {
    		roomba.drive(250, -10);
    		cam->update();
    	}
    }
    roomba.drive(500,0);
  }
  if(!cam->getfloorsign().empty())
  {
    //move forward just a bit more so we're on top of it.
    roomba.drive(100, 0); //temp arbitrary numbers
  }
}

bool Navigation::walkPath(vector<Node*> path)
{
  //walk the walk using roomba commands
  if(path.empty())
    return false;
  //assume we're on node path[0] and turn towards path[1].
  bool arrived = false;
	
  for(size_t i=1; i<path.size(); ++i)
  {
    string nodeGoal = path[i]->name;
  	while(!arrived) //is this loop actually doing anything? i.e. does it always only run once?
    {
  		moveForwardUntilSignOrBlockage();
  		if(cam->getpathisblocked())
        {
          cout << "Path is blocked. Turning around." << endl;
  		  roomba.drive(0, 180); //turn around  		  
  			//move towards last intersection
          moveForwardUntilSignOrBlockage();                //ASSUMING NO BLOCKAGE HERE

        //update weights where blockage exists
          incrementWeight(path[i-1], path[i]);

          //we turned around, so switch direction we traveled
          switch(path[i]->directionTraveled) {
          	case NORTH:
          		path[i-1]->directionTraveled = SOUTH;
          		break;
          	case SOUTH:
          		path[i-1]->directionTraveled = NORTH;
          		break;
          	case EAST:
          		path[i-1]->directionTraveled = WEST;
          		break;
          	case WEST:
          		path[i-1]->directionTraveled = EAST;
          		break;
          }

  		  path = findPath(path[i-1], path[path.size()-1]);
  		  turnAtIntersection(path, 0);
          return walkPath(path);
  		}
      	else if(!cam->getfloorsign().empty())
        {
  			if(cam->getfloorsign() == path[path.size()-1]->name) {
  				arrived = true;
  			} else {
  				turnAtIntersection(path, i);
  			}
  		}
  	}
  }

  return arrived;
}

void Navigation::turnAtIntersection(vector<Node*> path, int currentNode) {

	switch(path[currentNode]->directionTraveled) {
		case NORTH:			
			switch(path[currentNode+1]->directionTraveled) {
				case NORTH:
					roomba.drive(0, 0);
					break;
				case SOUTH:
					roomba.drive(0, 180);
					break;
				case EAST:
					roomba.drive(0, -90);
					break;
				case WEST:
					roomba.drive(0, 90);
					break;
			}
			break;
		case SOUTH:
			switch(path[currentNode+1]->directionTraveled) {
				case NORTH:
					roomba.drive(0, 180);
					break;
				case SOUTH:
					roomba.drive(0, 0);
					break;
				case EAST:
					roomba.drive(0, 90);
					break;
				case WEST:
					roomba.drive(0, -90);
					break;
			}
			break;
		case EAST:
			switch(path[currentNode+1]->directionTraveled) {
				case NORTH:
					roomba.drive(0, 90);
					break;
				case SOUTH:
					roomba.drive(0, -90);
					break;
				case EAST:
					roomba.drive(0, 0);
					break;
				case WEST:
					roomba.drive(0, 180);
					break;
			}
			break;
		case WEST:
			switch(path[currentNode+1]->directionTraveled) {
				case NORTH:
					roomba.drive(0, -90);
					break;
				case SOUTH:
					roomba.drive(0, 90);
					break;
				case EAST:
					roomba.drive(0, 180);
					break;
				case WEST:
					roomba.drive(0, 0);
					break;
			}
			break;
	}
}

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
    // cout << current->name << endl;
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
        //set the direction we will travel
        neighbor->directionTraveled = current->allDirections[i];
        //g_score[neighbor] = tentative_g_score
        neighbor->g_score= tempScore;
      }
    }
  }

  return reconstructPath(source, sink);
}

vector<Node*> Navigation::reconstructPath(Node* source, Node* sink)
{
  Node* current = sink;
  vector<Node*> path;
  while(current->parent != NULL)
  {
    path.insert(path.begin(), current);
    current = current->parent;
  }
  path.insert(path.begin(), source);
  return path;
}

void Navigation::incrementWeight(Node* n1, Node* n2)
{
  for(size_t i=0; i<n1->neighbors.size(); ++i)
  {
    if(n1->neighbors[i] == n2)
    {
      n1->weights[i] += Node::LARGE_NUM;
      break;
    }
  }
  for(size_t i=0; i<n2->neighbors.size(); ++i)
  {
    if(n2->neighbors[i] == n1)
    {
      n2->weights[i] += Node::LARGE_NUM;
      break;
    }
  }
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

