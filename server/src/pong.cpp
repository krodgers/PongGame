
/**
   Kevin Malby UCID: 36928917
   Kathryn Rodgers UCID: 39483825
**/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "pong.h"

#define WALL_OFFSET 8
#define X_VELOCITY 400
#define Y_VELOCITY 50
#define Y_ADJUST 50


pong::pong(){
  xspeed = X_VELOCITY;
  yspeed = rand() % Y_VELOCITY;
  boardHeight = 500;
  boardWidth = 500;
  ballx = 250;
  bally = 250;
  paddlex = 0;
  paddley = bally;
  score = 0;
  totalTries = 0;
  paddleDirection = 1;
  ballradius = 5;

}
pong::pong(int screenHeight, int screenWidth){
  xspeed = X_VELOCITY;
  yspeed = rand() % Y_VELOCITY;
  boardHeight = screenHeight;
  boardWidth = screenWidth;
  ballx = std::floor(screenWidth/2);
  bally = std::floor(screenHeight/2);
  paddlex = 0;
  paddley = bally;
  score = 0;
  totalTries = 0;
  paddleDirection = 1;
  ballradius = 5;
}

pong::pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddleStartX, int paddleStartY){

  xspeed = X_VELOCITY;
 yspeed = rand() % Y_VELOCITY;

  boardHeight = screenHeight;
  boardWidth = screenWidth;

  ballx = ballStartX;
  bally = ballStartY;
  ballradius = 5;
  paddlex = paddleStartX;
  paddley = paddleStartY;

  score = 0;
  totalTries = 0;
}

void pong::setBallRadius(int r){
  ballradius = r;
}

void  pong::setBallPos(int x, int y){
  ballx = x;
  bally = y;

}

void pong::setBallSpeed(double speedX, double speedY){
  xspeed = speedX;
  yspeed = speedY;
}

void pong::setPaddleDirection(int direction) {
    paddleDirection = direction;
}

void  pong::setPaddlePos(int x, int y){
  paddlex = x;
  paddley = y;

}
void pong::setPaddleDimensions(int h, int w){
  paddleWidth = w;
  paddleHeight = h;
}

void pong::update(double delTime) {
    update(paddleDirection, paddley, delTime);
}

// paddleDirection: 1/-1 for up/down
// paddleX assumed to be constant
void  pong::update(int paddleDirection, int paddleY, double delTime){
  paddley = paddleY;

  ballx += (xspeed*delTime);
  bally += (yspeed*delTime);

  bool closeX = (ballx - ballradius)  <= paddlex + paddleWidth; // left wall;
  if (closeX) {
    int x = 9 + ballx;
  }

// closeX = closeX || (ballx + ballradius) + paddleWidth >= paddlex+paddleWidth; // right wall; need to add for 2 players
  if(closeX && (bally+ballradius >= paddley &&( bally-ballradius <= (paddley + paddleHeight)) )){
    // close enough; counts as hit
    score ++;
    totalTries ++;
    xspeed = -xspeed;
    std::cout << paddleDirection << std::endl;
    xspeed += xspeed * 0.07;
    switch(paddleDirection){
    case 1:
      yspeed += Y_ADJUST;
      break;
    case -1:
      yspeed -= Y_ADJUST;
    case 0:
    default:
        break;

    }
    ballx = ballx + WALL_OFFSET + ballradius;
    return;
  }

  if(ballx-ballradius < 0 ){
    // went through left wall
    xspeed  = -xspeed;
    reset();
    totalTries++;
    return;
  } else if( ballx+ballradius >= boardWidth){
    // ball hit right wall
    ballx = boardWidth - WALL_OFFSET - ballradius;
    xspeed = -xspeed;
//    if(yspeed > 0){
//      // going up, needs to go down
//      yspeed = -Y_VELOCITY;
//    } else
//      yspeed = Y_VELOCITY; // going down, needs to go up

  }

  if(bally-ballradius < 0){
    // went through top
    yspeed = -yspeed;
    bally = WALL_OFFSET + ballradius;
  } else if(bally+ballradius > boardHeight){
    // went through floor
    yspeed = -yspeed;
    bally = boardHeight - WALL_OFFSET - ballradius; // put just about the floor
  }

  if (xspeed >= boardWidth)
    xspeed = boardWidth;
  if (yspeed >= boardHeight)
    yspeed = boardHeight;

}

// distance between ball and paddle
double  pong::distance(){
  double res = (ballx - paddlex)*(ballx - paddlex);
  res +=  (bally - paddley)*(bally - paddley);

  return sqrt(res);
}

void  pong::reset(){
  paddley = std::floor(boardHeight/2);
  paddlex = 0;
  xspeed = X_VELOCITY;
  yspeed = rand() % Y_VELOCITY;
  ballx = std::floor(boardWidth/2);
  bally = std::floor(boardHeight/2);
  paddlex = 0;
  paddley = std::floor(boardHeight/2);;
  paddleDirection = 1;
}


