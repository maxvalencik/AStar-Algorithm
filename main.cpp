#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> 
using std::sort;
using std::cout;
using std::ifstream;
using std::istringstream;
using std::string;
using std::vector;
using std::abs;

//state of the cells in the grid 
enum class State {kEmpty, kObstacle, kClosed, kPath, kStart, kFinish};


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

// CheckValidCell check that the 
// cell is on the grid and not an obstacle (i.e. equals kEmpty).
bool CheckValidCell (int x, int y, vector<vector<State>> &grid){
  if (y<grid[0].size() && x<grid.size()){
    return grid[x][y]==State::kEmpty; // true if cell empty and on grid
  } else{
    return false;
  }
}

// AddToOpen function
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openNode, vector<vector<State>> &grid){
    // Add node to open vector, and mark grid cell as closed.
    openNode.push_back(vector<int>{x, y, g, h});
    grid[x][y] = State::kClosed;
}

// directional deltas
const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
//Expand current nodes's neighbors and add them to the open list.
void ExpandNeighbors(const vector<int> &currentNode, int goal[2], vector<vector<int>> &openlist, vector<vector<State>> &grid) {
  // Get current node's data.
  int x = currentNode[0];
  int y = currentNode[1];
  int g = currentNode[2];

  // Loop through current node's potential neighbors.
  for (int i = 0; i < 4; i++) {
    int x2 = x + delta[i][0];
    int y2 = y + delta[i][1];

    // Check that the potential neighbor's x2 and y2 values are on the grid and not closed.
    if (CheckValidCell(x2, y2, grid)) {
      // Increment g value and add neighbor to open list.
      int g2 = g + 1;
      int h2 = Heuristic(x2, y2, goal[0], goal[1]);
      AddToOpen(x2, y2, g2, h2, openlist, grid);
    }
  }
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
      CellSort(&open); //sort open in descending values
      auto currentNode=open.back();//latest element of open (x,y,g,h)
      open.pop_back(); //remove node from open
      int x = currentNode[0];
      int y = currentNode[1]; 
      //set grid[x][y] to kPath
      grid [x][y] = State::kPath;

      //Check if you've reached the goal. If so, return grid.
      if (x==goal[0] && y==goal[1]){
        grid[origin[0]][origin[1]] = State::kStart;
        grid[goal[0]][goal[1]] = State::kFinish;
        return grid;
      }
      // Expand search to current node's neighbors
      ExpandNeighbors(currentNode, goal, open, grid);
    }
    
  // Ran out of new nodes to explore and haven't found a path.
    cout << "No path found!" << "\n";
    return {};
}


string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return "⛰️   ";
    case State::kPath: return "🚗   ";
    case State::kStart: return "🚦   ";
    case State::kFinish: return "🏁   ";
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
  int origin[2]={0,0};
  int goal[2]={4,5};
  auto board = ReadBoardFile("1.board");
  // Call Search with "board", "init", and "goal". Store the results in the variable "solution".
  vector<vector<State>> solution = Search(board, origin, goal);
  // Pass "solution" to PrintBoard.
  PrintBoard(solution);
}
