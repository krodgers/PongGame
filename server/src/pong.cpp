#include <stdio.h>
#include <iostream>
#include <cmath>
#include "pong.h"

#define WALL_OFFSET 4


pong::pong(){
 xspeed = 10;
  yspeed = 1;
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
  gameObjectSet = false;

}
pong::pong(int screenHeight, int screenWidth){
  xspeed = 10;
  yspeed = 1;
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
  gameObjectSet = false;
}

pong::pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddleStartX, int paddleStartY){

  xspeed = 10;
  yspeed = 1;

  boardHeight = screenHeight;
  boardWidth = screenWidth;
    
  ballx = ballStartX;
  bally = ballStartY;
  ballradius = 5;
  paddlex = paddleStartX;
  paddley = paddleStartY;

  score = 0;
  totalTries = 0;
  gameObjectSet = false;
}

void pong::setBallRadius(int r){
  ballradius = r;
}
  
void  pong::setBallPos(int x, int y){
  ballx = x;
  bally = y;

}

void  pong::setPaddlePos(int x, int y){
  paddlex = x;
  paddley = y;

}
void pong::setPaddleDimensions(int h, int w){
  paddleWidth = w;
  paddleHeight = h;
}


// paddleDirection: 1/-1 for up/down
// paddleX assumed to be constant
void  pong::update(int paddleDirection, int paddleY){
  paddley = paddleY;
  if(std::abs(ballx+ballradius - paddlex)  <= paddleWidth && std::abs(bally+ballradius - paddley) <= paddleHeight){
    // close enough; counts as hit
    score ++;
    xspeed = -xspeed;
    yspeed = std::floor(paddleDirection * yspeed / 2) + 1;
    ballx = ballx + WALL_OFFSET;
    return;
  }
  ballx += xspeed;
  bally += yspeed;
    
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
  ballx = 0;
  bally = 0;
  paddley = std::floor(boardHeight/2);
  paddlex = 0;
  xspeed = 10;
  yspeed = 0;
  xspeed = 10;
  yspeed = 1;
  ballx = std::floor(boardWidth/2);
  bally = -1*std::floor(boardHeight/2);
  paddlex = 0;
  paddley = bally;
  paddleDirection = 1;
  ballradius = 5;
}

 
