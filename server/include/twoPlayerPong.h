/**
   Kevin Malby UCID: 36928917
   Kathryn Rodgers UCID: 39483825



**/

#ifndef __TWO_PONG_H__
#define __TWO_PONG_H__
#include <stdlib.h>
#include <iostream>
#include <string>

#define PLAYER_ONE 0
#define PLAYER_TWO 1

class pong{

 private:
  double xspeed, yspeed;
  int paddleHeight, paddleWidth;
  int  paddlex[2], paddley[2]; // player zero is on Left, player one is on Right
  int paddleDirection[];
  int score[2]; // player zero is on Left, player one is on Right
  int totalTries[2]; // player zero is on Left, player one is on Right


 public:
  double ballx, bally;
  int ballradius;

  int boardHeight, boardWidth;
  bool gameObjectSet;

  pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddle1StartX, int paddle1StartY, int paddle2StartX, int paddle2StartY);

  pong(int h, int w);
  pong();

  void setBallPos(int x, int y);
  void setBallRadius(int);
  void setBallSpeed(double speedX, double speedY);

  void setPaddleDirection(int player, int direction);
  void setPaddlePos(int player, int x, int y);
  void setPaddleDimensions(int h , int w);

  void getScore(int player);
  void getTotalTries(int player);
  
  void update(double delTime);

  double distance();

  void reset();


};

#endif
