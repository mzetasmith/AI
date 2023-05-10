#include <iostream>
#include <vector>
#include <string>

#include "a_star.h"
#include "game_element.h"

using namespace std;

int main()
{
    int horizontal = 0;  
    cout << "Let's play a game! You will be X. I will be O" << endl;
    cout << "The goal is to avoid me for as long as possible" <<endl;
    cout << "You can move by typing l for left, r for right, u for up and d for down";
    cout << endl;
    cout << "At the end you'll get a score! How fun. Now let's begin!" << endl;
    cout << "Please enter a number. It will create a square for the board: ";
    cin >> horizontal;
    vector<vector<char>> board(horizontal, vector<char> (horizontal, ' '));

    Opponent opp(horizontal-1, 0);
    Player p1(0, horizontal-1);

    for(int i =0; i < board.size()-2; i++) {
      for (int j = 0; j < board.size(); j+=2) {
        board[rand()%board.size()][j] = 'T';
      }
    }
  
    board[p1.GetY()][p1.GetX()] = 'X';
    board[opp.GetY()][opp.GetX()] = 'O';
  
  char userInput = ' ';
  int score = 0;
  
//use below line to debug  
while (opp.isNext(p1) == true) { 
     for (int i =0; i < horizontal; i++) {
          for (int j = 0; j < horizontal; j++) {
              cout << "| " << board[i][j] << " |";
         }
         cout << "\n";
      }
    
    cout << "Please enter your move" <<endl;
    cin >> userInput;

    //remove previous player location and update with new location
    board[p1.GetY()][p1.GetX()] = ' ';
    p1.Move(userInput, board);
    board[p1.GetY()][p1.GetX()] = 'X';

    //test
    Pair src = make_pair(opp.GetY(), opp.GetX());
    Pair dest = make_pair(p1.GetY(), p1.GetX());
    Pair nextMove = aStarSearch(board, src, dest);

    //remove previous opp location and update with new location
    board[opp.GetY()][opp.GetX()] = ' ';
    opp.Move(nextMove.first, nextMove.second);
    board[opp.GetY()][opp.GetX()] = 'O';
    cout << endl;
  
    score ++;
  }
  for (int i =0; i < horizontal; i++) {
          for (int j = 0; j < horizontal; j++) {
              cout << "| " << board[i][j] << " |";
         }
         cout << "\n";
  }
  cout << "Oh I caught you. Thanks for playing! Your score is : " << score <<endl;
}
