#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "pong.h"

#define WALL_OFFSET 4


pong::pong(){
 xspeed = 10;
 yspeed = rand() % 3;
  boardHeight = 500;
  boardWidth = 500;
  ballx = 10;
  bally = 10;
  paddlex = 0;
  paddley = bally;
  score = 0;
  totalTries = 0;
  paddleDirection = 1;
  ballradius = 5;

}
pong::pong(int screenHeight, int screenWidth){
  xspeed = 10;
 yspeed = rand() % 3;
  boardHeight = screenHeight;
  boardWidth = screenWidth;
  ballx = std::floor(screenWidth/2);
  bally = -1*std::floor(screenHeight/2);
  paddlex = 0;
  paddley = bally;
  score = 0;
  totalTries = 0;
  paddleDirection = 1;
  ballradius = 5;
}

pong::pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddleStartX, int paddleStartY){

  xspeed = 10;
 yspeed = rand() % 3;

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

void pong::update() {
    update(paddleDirection, paddley);
}

// paddleDirection: 1/-1 for up/down
// paddleX assumed to be constant
void  pong::update(int paddleDirection, int paddleY){
  paddley = paddleY;

  ballx += xspeed;
  bally += yspeed;
  
  bool closeX = (ballx - ballradius)  <= paddlex + paddleWidth; // left wall; 
 
// closeX = closeX || (ballx + ballradius) + paddleWidth >= paddlex+paddleWidth; // right wall; need to add for 2 players
  if(closeX && (bally-ballradius <= paddley && bally+ballradius >= (paddley - paddleHeight) )){
    // close enough; counts as hit
    score ++;
    totalTries ++;
    xspeed = -xspeed;
    switch(paddleDirection){
    case 1:
      yspeed = abs(yspeed) + 1/2.0; // faster up
      break;
    case -1:
      yspeed = -yspeed - 1/2.0; // faster down
      break;
    case 0:
    default:
      yspeed = -yspeed; // bounce 90 deg (?)
      
    }
    ballx = ballx + WALL_OFFSET;
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
    ballx = boardWidth - WALL_OFFSET;
    xspeed = -xspeed;
    if(yspeed > 0){
      // going up, needs to go down
      yspeed = -5;
    } else
      yspeed = 5; // going down, needs to go up

  }

  if(bally+ballradius > 0){
    // went through top
    yspeed = -yspeed;
    bally = -WALL_OFFSET;
  } else if(bally-ballradius < -boardHeight){
    // went through floor
    yspeed = -yspeed;
    bally = -(boardHeight - WALL_OFFSET); // put just about the floor
  }



}

double  pong::distance(){
  double res = (ballx - paddlex)*(ballx - paddlex);
  res +=  (bally - paddley)*(bally - paddley);

  return sqrt(res);
}

void  pong::reset(){
  paddley = std::floor(boardHeight/2);
  paddlex = 0;
  xspeed = 10;
  yspeed = rand() % 3;
  ballx = std::floor(boardWidth/2);
  bally = -1*std::floor(boardHeight/2);
  paddlex = 0;
  paddley = bally;
  paddleDirection = 1;
  ballradius = 5;
}


