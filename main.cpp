#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::abs;

//state of the cells in the grid 
enum class State {kEmpty, kObstacle, kClosed};


vector<State> ParseLine(string line) {
    istringstream sline(line);
    int n;
    char c;
    vector<State> row;
    while (sline >> n >> c && c == ',') {
      if (n == 0) {
        row.push_back(State::kEmpty);
      } else {
        row.push_back(State::kObstacle);
      }
    }
    return row;
}


vector<vector<State>> ReadBoardFile(string path) {
  ifstream myfile (path);
  vector<vector<State>> board{};
  if (myfile) {
    string line;
    while (getline(myfile, line)) {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

// Write function to compare the f-value of two nodes here
bool Compare(vector<int> node1, vector<int> node2){
    int f1 = node1[2]+node1[3]; //f1=g1+h1
    int f2 = node2[2]+node2[3]; //f2=g2+h2
    return f1>f2; //true if f1>f2
}

 //Sort the two-dimensional vector of ints in descending order.
void CellSort(vector<vector<int>> *v) {
  sort(v->begin(), v->end(), Compare);
}

//Write the Heuristic function here.
int Heuristic (int x1, int y1, int x2, int y2){
    //manhattan distance
    int manhattanDist=abs(x2-x1)+abs(y2-y1);
    return manhattanDist;
}

// Write the AddToOpen function here.
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openNode, vector<vector<State>> &grid){
    // Add node to open vector, and mark grid cell as closed.
    openNode.push_back(vector<int>{x, y, g, h});
    grid[x][y] = State::kClosed;
}


//Implementation of A* search algorithm
 
// Search function
vector<vector<State>> Search (vector<vector<State>> grid, int origin [2], int goal [2]){
    // Create the vector of open nodes.
    vector<vector<int>> open {};
    // Initialize the starting node.
    int x = origin[0];
    int y = origin[1];
    int g = 0;
    int h = Heuristic (x,y,goal[0],goal[1]); 
    // Use AddToOpen to add the starting node to the open vector.
    AddToOpen(x,y,g,h,open, grid);

    //while open vector is non empty
  while (!open.empty()){
   //Sort the open list using CellSort, and get the current node. 
    CellSort(open); //sort the vectors in open by f-value...fist vector has lowest f value. CellSort function directly modifies open (*v)
    int x = open[0][0]; //x of first node in open which has the lowest f value
    int y = open[0][1]; //y of first node in open which has the lowest f value
    //set grid[x][y] to kPath
    grid [x][y] = state::kPath;

    
  }
   

    // TODO: Check if you've reached the goal. If so, return grid.

    
    // If we're not done, expand search to current node's neighbors. This step will be completed in a later quiz.
    // ExpandNeighbors
  
  //} // TODO: End while loop
  
  // We've run out of new nodes to explore and haven't found a path.

    cout << "No path found!" << "\n";
    return {};
}


string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    default: return "0   "; 
  }
}


void PrintBoard(const vector<vector<State>> board) {
  for (int i = 0; i < board.size(); i++) {
    for (int j = 0; j < board[i].size(); j++) {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}


int main() {
  // Declare "init" and "goal" arrays with values {0, 0} and {4, 5} respectively.
  int init[2]={0,0};
  int goal[2]={4,5};
  auto board = ReadBoardFile("1.board");
  // Call Search with "board", "init", and "goal". Store the results in the variable "solution".
  vector<vector<State>> solution = Search(board, init, goal);
  // Pass "solution" to PrintBoard.
  PrintBoard(solution);
}
