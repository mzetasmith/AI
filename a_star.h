#include <utility>
#include <vector>
#include <cmath>
#include <iostream>
#include <stack>
#include <string.h>
#include <set>

using namespace std;

#ifndef A_STAR_H
#define A_STAR_H

typedef pair<int, int> Pair;
typedef pair<double, pair<int, int> > doubPair;

struct Cell {
  int parent_i;
  int parent_j;
  double h, f, g;
};

bool isValid(int row, int col, int size) {
  return (row > -1 && row < size && col > -1 && col < size );
}

bool isUnblocked(vector<vector <char>> board, int row, int col) {
  if (board[row][col] != 'T' ){//|| board[row][col] == 'X') {
    return (true);
  } else {
    return (false);
  }
}

bool isDestination(int row, int col, Pair dest) {
  if (row == dest.first && col == dest.second) {
    return true;
  } else {
    return false;
  }
}

double calcHvalue(int row, int col, Pair dest) {
  return ((double)sqrt((row-dest.first)*(row-dest.first) + (col-dest.second) *(col-dest.second)));
}
Pair tracePath(vector<vector<Cell> > cellDetails, Pair dest)
{
  int row = dest.first;
  int col = dest.second;
 
    stack<Pair> Path;
 
    while (!(cellDetails[row][col].parent_i == row
             && cellDetails[row][col].parent_j == col)) {
        Path.push(make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
 
    Path.push(make_pair(row, col));
    vector<pair<int,int>> storage;
    while (!Path.empty()) {
        pair<int, int> p = Path.top();
        Path.pop();
        storage.push_back(p);
    }
    return storage.back();
}
Pair aStarSearch(vector<vector<char>> board, Pair src, Pair dest)
{
    // If the source is out of range
    if (isValid(src.first, src.second, board.size()) == false) {
        printf("Source is invalid\n");
        return Pair(0,0);
    }
 
    // If the destination is out of range
    if (isValid(dest.first, dest.second, board.size()) == false) {
        printf("Destination is invalid\n");
        return Pair(0,0);
    }
 
    // Either the source or the destination is blocked
    if (isUnblocked(board, src.first, src.second) == false
        || isUnblocked(board, dest.first, dest.second)
               == false) {
        printf("Source or the destination is blocked\n");
        return Pair (0,0);
    }
 
    // If the destination cell is the same as source cell
    if (isDestination(src.first, src.second, dest)
        == true) {
        printf("We are already at the destination\n");
        return Pair (0,0);
    }
 
    // Create a closed list and initialise it to false which
    // means that no cell has been included yet This closed
    // list is implemented as a boolean 2D array
    bool closedList[board.size()][board.size()];
    memset(closedList, false, sizeof(closedList));
 
    // Declare a 2D array (it's a vector now lol) of structure to hold the details
    // of that cell
    vector<vector <Cell>> cellDetails (board.size(), vector<Cell>(board.size()));
 
    int i, j;
 
    for (i = 0; i < board.size(); i++) {
        for (j = 0; j < board.size(); j++) {
            cellDetails[i][j].f = 100000;
            cellDetails[i][j].g = 100000;
            cellDetails[i][j].h = 100000;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
        }
    }
 
    // Initialising the parameters of the starting node
    i = src.first, j = src.second;
    cellDetails[i][j].f = 0.0;
    cellDetails[i][j].g = 0.0;
    cellDetails[i][j].h = 0.0;
    cellDetails[i][j].parent_i = i;
    cellDetails[i][j].parent_j = j;
 
    /*
     Create an open list having information as-
     <f, <i, j>>
     where f = g + h,
     and i, j are the row and column index of that cell
     Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
     This open list is implemented as a set of pair of
     pair.*/
    set<doubPair> openList;
 
    // Put the starting cell on the open list and set its
    // 'f' as 0
    openList.insert(make_pair(0.0, make_pair(i, j)));
 
    // We set this boolean value as false as initially
    // the destination is not reached.
    bool foundDest = false;
 
    while (!openList.empty()) {
        doubPair p = *openList.begin();
 
        // Remove this vertex from the open list
        openList.erase(openList.begin());
 
        // Add this vertex to the closed list
        i = p.second.first;
        j = p.second.second;
        closedList[i][j] = true;
 
        /*
         Generating all the 4 successor of this cell
 
             N.W   N   N.E
               \   |   /
                \  |  /
             W----Cell----E
                  / | \
                /   |  \
             S.W    S   S.E
 
         Cell-->Popped Cell (i, j)
         N -->  North       (i-1, j)
         S -->  South       (i+1, j)
         E -->  East        (i,j+1 )
         W -->  West           (i, j-1)*/
 
        // To store the 'g', 'h' and 'f' of the 4 successors
        double gNew, hNew, fNew;
 
        //----------- 1st Successor (North) ------------
 
        // Only process this cell if this is a valid one
        if (isValid(i-1, j, board.size()) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i-1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i-1][j].parent_i = i;
                cellDetails[i-1][j].parent_j = j;
                Pair p = tracePath(cellDetails, dest);
                foundDest = true;
                return p;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i-1][j] == false
                     && isUnblocked(board, i-1, j)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calcHvalue(i-1, j, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j-1].f == 100000
                    || cellDetails[i][j-1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i-1, j)));
 
                    // Update the details of this cell
                    cellDetails[i-1][j].f = fNew;
                    cellDetails[i-1][j].g = gNew;
                    cellDetails[i-1][j].h = hNew;
                    cellDetails[i-1][j].parent_i = i;
                    cellDetails[i-1][j].parent_j = j;
                }
            }
        }
 
        //----------- 2nd Successor (South) ------------
 
        // Only process this cell if this is a valid one
        if (isValid(i +1, j, board.size()) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i+1, j, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i+1][j].parent_i = i;
                cellDetails[i+1][j].parent_j = j;
                //printf("The destination cell is found\n");
                Pair p = tracePath(cellDetails, dest);
                foundDest = true;
                return p;
            }
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i+1][j] == false
                     && isUnblocked(board, i+1, j)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calcHvalue(i+1, j, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i+1][j].f == 100000
                    || cellDetails[i+1][j].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i+1, j)));
                    // Update the details of this cell
                    cellDetails[i+1][j].f = fNew;
                    cellDetails[i+1][j].g = gNew;
                    cellDetails[i+1][j].h = hNew;
                    cellDetails[i+1][j].parent_i = i;
                    cellDetails[i+1][j].parent_j = j;
                }
            }
        }
 
        //----------- 3rd Successor (East) ------------
 
        // Only process this cell if this is a valid one
        if (isValid(i, j+1, board.size()) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j+1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j+1].parent_i = i;
                cellDetails[i][j+1].parent_j = j;
                //printf("The destination cell is found\n");
                Pair p = tracePath(cellDetails, dest);
                foundDest = true;
                return p;
            }
 
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j+1] == false
                     && isUnblocked(board, i, j+1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calcHvalue(i, j+1, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j+1].f == 100000
                    || cellDetails[i][j+1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j+1)));
 
                    // Update the details of this cell
                    cellDetails[i][j+1].f = fNew;
                    cellDetails[i][j+1].g = gNew;
                    cellDetails[i][j+1].h = hNew;
                    cellDetails[i][j+1].parent_i = i;
                    cellDetails[i][j+1].parent_j = j;
                }
            }
        }
 
        //----------- 4th Successor (West) ------------
 
        // Only process this cell if this is a valid one
        if (isValid(i, j-1, board.size()) == true) {
            // If the destination cell is the same as the
            // current successor
            if (isDestination(i, j-1, dest) == true) {
                // Set the Parent of the destination cell
                cellDetails[i][j-1].parent_i = i;
                cellDetails[i][j-1].parent_j = j;
                //printf("The destination cell is found\n");
                Pair p = tracePath(cellDetails, dest);
                foundDest = true;
                return p;
            }
 
            // If the successor is already on the closed
            // list or if it is blocked, then ignore it.
            // Else do the following
            else if (closedList[i][j-1] == false
                     && isUnblocked(board, i , j -1)
                            == true) {
                gNew = cellDetails[i][j].g + 1.0;
                hNew = calcHvalue(i, j-1, dest);
                fNew = gNew + hNew;
 
                // If it isn’t on the open list, add it to
                // the open list. Make the current square
                // the parent of this square. Record the
                // f, g, and h costs of the square cell
                //                OR
                // If it is on the open list already, check
                // to see if this path to that square is
                // better, using 'f' cost as the measure.
                if (cellDetails[i][j-1].f == 100000
                    || cellDetails[i][j-1].f > fNew) {
                    openList.insert(make_pair(
                        fNew, make_pair(i, j-1)));
 
                    // Update the details of this cell
                    cellDetails[i][j-1].f = fNew;
                    cellDetails[i][j-1].g = gNew;
                    cellDetails[i][j-1].h = hNew;
                    cellDetails[i][j-1].parent_i = i;
                    cellDetails[i][j-1].parent_j = j;
                }
            }
        }
    }
    if (foundDest == false)
        printf("Failed to find the Destination Cell\n");
 
    return Pair(0,0);
}
#endif 
