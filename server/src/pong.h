#include <stdio.h>
#include <iostream>
#include <cmath>

class pong{

public:
  int  ballx, bally, paddlex, paddley;
  double xspeed, yspeed;
  int paddleDirection;
  
  int height, width;
  
  int score;
  int totalTries;

  pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddleStartX, int paddleStartY);

  
  void setBallPos(int x, int y);

  void setPaddlePos(int x, int y);

  void update(int paddleDirection, int paddleY);

  double distance();

  void reset();
  
  
};
