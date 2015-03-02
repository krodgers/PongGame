/**
   Kevin Malby UCID: 36928917
   Kathryn Rodgers UCID: 39483825



**/

#ifndef __PONG_H__
#define __PONG_H__
#include <stdlib.h>
#include <iostream>
#include <string>


class pong{

 private:
  double xspeed, yspeed;
  int paddleHeight, paddleWidth;
  int  paddlex, paddley;
  int paddleDirection;


 public:
  double ballx, bally;
  int ballradius;
  int boardHeight, boardWidth;
  int score;
  int totalTries;

  pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddleStartX, int paddleStartY);

  pong(int h, int w);
  pong();

  void setBallPos(int x, int y);
  void setBallRadius(int);
  void setBallSpeed(double speedX, double speedY);

  void setPaddleDirection(int);
  void setPaddlePos(int x, int y);
  void setPaddleDimensions(int h , int w);

  void update(double delTime);
  void update(int paddleDirection, int paddleY, double delTime);

  double distance();

  void reset();


};

#endif
