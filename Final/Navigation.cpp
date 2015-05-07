#include "Navigation.h"
#include <fstream>
#include <queue>
#include <math.h>
// #include <iostream>
// #include <string>
// #include <cstring>
// #include <dirent.h>

using namespace std;

double Node::DNE = 1000000.0;
double Node::LARGE_NUM = 1000.0;
// const int edge = 80;
// const int red = 110;
// const int green = 110;
// const int blue = 170;
const int STRAIGHT = 32768;
const int REG_SPEED = 200;
const int TURN_SPEED = 150;

Navigation::Navigation(const char* filename, bool test)
{
  roomba.start();
  roomba.powerOn();
  roomba.getSensors(Sensor::ALL);
  startTime = clock();
  testMode = test;
  inputNodes(filename);
  sleep(2);
  if(allNodes.size() > 1) //keep in mind, the output path doesn't include the source
  {
    cout << "from " << allNodes[0].name << " to " << allNodes[allNodes.size()-1].name << endl;
    travelFromSourceToSink(&allNodes[0], &allNodes[allNodes.size()-1]); 
    // travelFromSourceToSink(walkToStartingNode(), &allNodes[allNodes.size()-1]);
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
    cout << "is good" << endl;
    in >> inStr; //read the number of nodes
    numNodes = atoi(inStr.c_str()); //change the string we read to an int
    cout << "num nodes " << numNodes << endl;
    for(int i = 0; i < numNodes; i++)
    { //loop for each node we have
      in >> inStr; //read the name of a node
      Node n(inStr, Node::DNE); //create a new node with nonexistant g value
      n.parent = NULL;
      allNodes.push_back(n); //put that node in the list of all nodes
    }

    for(int i = 0; i < numNodes; i++)
    { //loop for each node we have
      in >> inStr; //read the number of neighbors a node has
      numNeighbors = atoi(inStr.c_str());
      for(int j = 0; j < numNeighbors; j++)
      { //loop for each neighbor of a node
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
        switch(direction)
        { //assign the direction we need to go in to reach the neighbor
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
  //#ifdef DEBUG 
  outputPath(path); 
  //#endif
  cout << "size: " << path.size() << endl;
  walkPath(path);
  return true;
}

Node* Navigation::getNode(int index)
{
  return &allNodes[index];
}

vector<Node*> Navigation::dbgReconstructPath(Node* source, Node* sink)
{
  vector<Node*> path;

  #ifdef TEST
    return reconstructPath(source, sink);
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

void Navigation::rotate(int degrees)
{
  double degreesRotated = 0;
  int16_t speed = 50;

  // roomba.getSensors(Sensor::ALL).getAngle();
  cout << "\t\t\t\t\t\t\t\t\t\tTURNING " << degrees << " DEGREES" << endl;
  cout << "\t\t\t\t\tSTOP DRIVE" << endl;
  roomba.drive(0,0);
  // roomba.getSensors(Sensor::ALL);
  roomba.getSensor().getAngle();  
  if(degrees < 0)
  {
    roomba.drive(speed, -1);
    while((degreesRotated > degrees + 5) || (degreesRotated < degrees - 5))
    {
      // degreesRotated -= roomba.getSensors(Sensor::ALL).getAngle();
      if(roomba.getSensors(Sensor::ALL)) {
        degreesRotated += roomba.getSensor().getAngle();
        //cout << "Degrees Rotated: " << degreesRotated << endl;
        // cout << "getAngle: " << roomba.getSensor().getAngle();
        //usleep(50000);
      }
    }    
  }
  else
  {
    roomba.drive(speed, 1);
    while((degreesRotated < degrees -5) || (degreesRotated > degrees + 5))
    {
      // degreesRotated += roomba.getSensors(Sensor::ALL).getAngle();
      if(roomba.getSensors(Sensor::ALL)) {
        //cout << "Degrees Rotated: " << degreesRotated << endl;
        degreesRotated -= roomba.getSensor().getAngle();
        //usleep(50000);
      }
    }
  }
  roomba.drive(0, 0);    
}

Node* Navigation::walkToStartingNode()
{
  while(1)
  {
    cam.output();
    // sleep(2);
    moveForwardUntilSignOrBlockage();
    if(!getFloorSign().empty())
      for(int i = 0; i < allNodes.size() - 1; i++)
        if(getFloorSign() == allNodes[i].name)
          return &allNodes[i];

    if(getPathIsBlocked())
    {    
      rotate(180);
    }  
  }
}

void Navigation::moveForwardUntilSignOrBlockage()
{
  // roomba.getSensors(Sensor::ALL);
  cout << "Percent Power: " << (float)roomba.getSensor().getCharge() / (float)roomba.getSensor().getCapacity() << endl;
  cout << "moveForwardUntilSignOrBlockage" << endl;
  cout << "\t\t\t\t\tDRIVING STRAIGHT" << endl;
  roomba.drive(REG_SPEED, STRAIGHT);
  // cout << "start cam check loop" << endl;
  cam.update();
  while(getFloorSign().empty() && !getPathIsBlocked())
  {
    // cout << "about to update" << endl;
    cam.update();
    //cam.output();
    cout << "[" << getPathIsBlocked() << ", " << getFloorSign() << ", " << cam.getslope() << "]" << endl;
    // cout << "updated" << endl;

    if((cam.getslope() != cam.getslope())) //if the value is NaN
    {
      cout << "\t\t\t\t\tSTOP DRIVE" << endl;
      roomba.drive(0,0);
      // cout << "NAN" << endl;
    }
    else
    {
      if(cam.getslope() < -.4)
      { //arbitrary tolerances. slope will be 0 if we are going straight
        cout << "\t\t\t\t\tTURNING TO THE RIGHT" << endl;
        roomba.drive(TURN_SPEED, -1500);
        while(cam.getslope() < -.1 && getFloorSign().empty())
        {
          // rotate(5);
          cam.update();
          //cam.output();
          cout << "[" << getPathIsBlocked() << ", " << getFloorSign() << ", " << cam.getslope() << "]" << endl;
        }
        cout << "\t\t\t\t\tDRIVING STRAIGHT" << endl;
        roomba.drive(REG_SPEED, STRAIGHT);
      }
      else if(cam.getslope() > .4)
      {
        cout << "\t\t\t\t\tTURNING TO THE LEFT" << endl;
        roomba.drive(TURN_SPEED, 1500);
        while(cam.getslope() > .1 && getFloorSign().empty())
        {
          // rotate(-5);
          cam.update();
          //cam.output();
          cout << "[" << getPathIsBlocked() << ", " << getFloorSign() << ", " << cam.getslope() << "]" << endl;
        }
        cout << "\t\t\t\t\tDRIVING STRAIGHT" << endl;
        roomba.drive(REG_SPEED, STRAIGHT);
      }

    }
  }
  if(!getFloorSign().empty())
  {
    cout << "got a floor sign" << endl;
    cout << "[" << getPathIsBlocked() << ", " << getFloorSign() << ", " << cam.getslope() << "]" << endl;
    //move forward just a bit more so we're on top of it.
    cout << "\t\t\t\t\tDRIVING STRAIGHT" << endl;
    roomba.drive(REG_SPEED, STRAIGHT); //temp arbitrary numbers
    // while(!getFloorSign().empty()) 
    // {
    //   cout << "[" << getPathIsBlocked() << ", " << getFloorSign() << ", " << cam.getslope() << "]" << endl;
    // }
    sleep(3.5);
  }
  cout << "end moveForwardUntilSignOrBlockage" << endl;
}

void Navigation::walkPath(vector<Node*> path)
{
  cout << "walkPath(path)" << endl;
  //walk the walk using roomba commands
  if(path.empty())
    return;
  //assume we're on node path[0] and turn towards path[1].
  
  for(size_t i=1; i<path.size(); ++i)
  {
    string nodeGoal = path[i]->name;
    cout << "about to move forward" << endl;
    moveForwardUntilSignOrBlockage();
    if(getPathIsBlocked())
    {
      cout << "Path is blocked. Turning around." << endl;
      rotate(180); //turn around        
      //move towards last intersection
      moveForwardUntilSignOrBlockage();                //ASSUMING NO BLOCKAGE HERE

      //update weights where blockage exists
      incrementWeight(path[i-1], path[i]);

        //we turned around, so switch direction we traveled
      switch(path[i]->directionTraveled)
      {
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
      walkPath(path);
    }
    else
    // else if(!getFloorSign().empty())      
    {
      turnAtIntersection(path, i);
      cam.update();
      // if(getFloorSign() == path[path.size()-1]->name)
      // {
      //   arrived = true;
      // }
      // else
      // {
        
      // }
    }
  }
  cout << "\t\t\t\t\tSTOP DRIVE" << endl;
  roomba.drive(0,0);
}

void Navigation::turnAtIntersection(vector<Node*> path, int currentNode)
{
  cout << "turnAtIntersection" << endl;
  switch(path[currentNode]->directionTraveled)
  {
    case NORTH:
      switch(path[currentNode+1]->directionTraveled)
      {
        case NORTH:          
          break;
        case SOUTH:
          rotate(180);
          break;
        case EAST:
          rotate(-90);
          break;
        case WEST:
          rotate(90);
          break;
      }
      break;
    case SOUTH:
      switch(path[currentNode+1]->directionTraveled)
      {
        case NORTH:
          rotate(180);
          break;
        case SOUTH:
          break;
        case EAST:
          rotate(90);
          break;
        case WEST:
          rotate(-90);
          break;
      }
      break;
    case EAST:
      switch(path[currentNode+1]->directionTraveled)
      {
        case NORTH:
          rotate(90);
          break;
        case SOUTH:
          rotate(-90);
          break;
        case EAST:
          break;
        case WEST:
          rotate(180);
          break;
      }
      break;
    case WEST:
      switch(path[currentNode+1]->directionTraveled)
      {
        case NORTH:
          rotate(-90);
          break;
        case SOUTH:
          rotate(90);
          break;
        case EAST:
          rotate(180);
          break;
        case WEST:
          break;
      }
      break;
  }
}

vector<Node*> Navigation::findPath(Node* source, Node* sink)
{
  cout << "findPath" << endl;
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
  cout << "find path returning" << endl;
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
  cout << "reconstructPath returning" << endl;
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

string Navigation::getFloorSign()
{
  if(testMode)
  {
    return _getFloorSign();
  }
  else
  {
    return cam.getfloorsign(); 
  }
}

string Navigation::_getFloorSign()
{
  double firstTurn = 11.61;
  double timePassed = ((double)clock() - (double)startTime)/CLOCKS_PER_SEC;
  cout << "timePassed: " << timePassed << endl;
  if (timePassed < firstTurn)
  {
    return "";
  }
  else if (timePassed < firstTurn + .09)
  {
    return "testA";
  }
  else if (timePassed < firstTurn + 7)
  {
    return "";
  }
  else if (timePassed < firstTurn + 7.09)
  {
    return "testB";
  }
  return "";
}

bool Navigation::getPathIsBlocked()
{
  if(testMode)
  {
    return _getPathIsBlocked();
  }
  else
  {
    return cam.getpathisblocked();
  }
}

bool Navigation::_getPathIsBlocked()
{
  double timePassed = ((double)clock() - (double)startTime)/CLOCKS_PER_SEC;
  if (timePassed < 10)
  {
    return false;
  }
  else if (timePassed < 12)
  {
    return false;
  }
  else
  {
    return false;
  }
  return false;
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

