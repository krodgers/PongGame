#ifndef __PONG_H__
#define __PONG_H__


class pong{

 private:
  double xspeed, yspeed;
  int paddleHeight, paddleWidth;
  int  paddlex, paddley;
  int paddleDirection;
  

 public:
  int  ballx, bally,ballradius;

  int boardHeight, boardWidth;
  int score;
  int totalTries;
  bool gameObjectSet;

  pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddleStartX, int paddleStartY);

  pong(int h, int w);
  pong();

  void setBallPos(int x, int y);
  void setBallRadius(int);
  void setBallSpeed(double speedX, double speedY);

  void setPaddlePos(int x, int y);
  void setPaddleDimensions(int h , int w);
  void update(int paddleDirection, int paddleY);

  double distance();

  void reset();
  
  
};

#endif 
