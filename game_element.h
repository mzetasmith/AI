#include <cstdlib>
#include <iostream>

#include "a_star.h"

#ifndef PLAYER_H
#define PLAYER_H

class GameElement {
  public:
    GameElement(int x, int y): x_(x), y_(y){}

  int GetX() {return x_;}
  int GetY() {return y_;}
  void setX(int x) {x_ = x;}
  void setY(int y) {y_ = y;}

  protected:
    int x_;
    int y_;
};

class Player : public GameElement {
  public:
    Player() : Player(0, 0) {}
    Player(int x, int y): GameElement(x,y) {}

  void Move(char input, vector<vector<char>> board){
    switch(input) {
      case 'l' :
        if (GetX()-1 > -1) {
          if (board[GetY()][GetX()-1] != ' ') {
            cout << "Woah, a tree" << endl;
          } else {
            setX(GetX()-1);
          }
        } else {
          cout << "I don't think you can go there. Try again" << endl;
        }
        break;
      case 'r' :
        if (GetX() + 1 < board.size()) {
          if (board[GetY()][GetX()+1] != ' ') {
            cout << "Woah, a tree" << endl;
            break;
          } else {
            setX(GetX() + 1);
          }
        } else {
          cout << "I don't think you can go there. Try again" << endl;
        }
        break;
      case 'd' :
        if (GetY() + 1 > -1) {
          if (board[GetY()+1][GetX()] != ' ') {
            cout << "Woah, a tree" << endl;
            break;
          } else {
            setY(GetY() + 1);
          }
        } else {
          cout << "I don't think you can go there. Try again" << endl;
        }
        break;
      case 'u' :
        if (GetY() - 1 < board.size()) {
          if (board[GetY() -1][GetX()] != ' ') {
            cout << "Woah, a tree" << endl;
          } else {
            setY(GetY() - 1);
          }
        } else {
          cout << "I don't think you can go there. Try again" << endl;
        }
        break;
      default:
        cout << "I don't understand. Can you try again?" <<endl;
    }
  }
};

class Opponent : public GameElement {
  public:
    Opponent() : Opponent(0, 0) {}
    Opponent(int x, int y): GameElement(x,y) {}

  bool isNext(Player p1) {
    if ((abs(x_-p1.GetX()) == 1 && p1.GetY() == y_) || (abs(y_ - p1.GetY()) == 1) && p1.GetX() == x_) {
      return false;
    } else if(p1.GetX() == x_ && p1.GetY() == y_){
      return false;
    } else{
      return true;
    }
  }

  void Move(int x, int y) {
    setX(x);
    setY(y);
  }
};

#endif
