/*
  Pong Game State
  Kathryn Rodgers UCID: 39483825
  Kevin Malby UCID: 36928917

 */
#ifndef __PONG_GAME_H__
#define __PONG_GAME_H__

#include <stdio.h>
#include "ball.h"
#include "paddle.h"


class PongGame{
 public:
  
  int getScore();
  void setScore(int);
  

  PongGame(int numPeople);
  PongGame(int numPeople, int ballStartX, int ballStartY, int ballStartDirection, int ballVelocity);

  void setBallVelocity(int speed, double ballAngle, int direction);
  void setBoardSize(int width, int height);
  void setPaddlePosition(int paddleY);

  void getBallCurrentPosition(int& outX, int& outY);
  void getBallPredictedPosition(int& outX, int& outY, int ballSpeed, int ballDirection, double ballAngle); // use -1 to use ball's current values
  
  
    
 private:
  int score;
  PongBall* ball;
  PongPaddle* paddle;
  int numPlayers;
  int boardWidth; // default is 500
  int boardHeight; // default is 500

  void init();



  



};

#endif
