#include <stdio.h>
#include <iostream>
#include <cmath>
#include "pong.h"

#define WALL_OFFSET 4

  pong::pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddleStartX, int paddleStartY){

    xspeed = 10;
    yspeed = 0;

    height = screenHeight;
    width = screenWidth;
    
    ballx = ballStartX;
    bally = ballStartY;
    paddlex = paddleStartX;
    paddley = paddleStartY;

    score = 0;
    totalTries = 0;
}

  
  void  pong::setBallPos(int x, int y){
    ballx = x;
    bally = y;

  }

  void  pong::setPaddlePos(int x, int y){
    paddlex = x;
    paddley = y;

  }

// paddleDirection: 1/-1 for up/down
// paddleX assumed to be constant
void  pong::update(int paddleDirection, int paddleY){
  paddley = paddleY;
    if(std::abs(ballx - paddlex)  < 5 && std::abs(bally - paddley) < 10){
      // close enough; counts as hit
      score ++;
      xspeed = -xspeed;
      yspeed = std::floor(paddleDirection * yspeed / 2) + 1;
      ballx = ballx + WALL_OFFSET;
      return;
    }
    ballx += xspeed;
    bally += yspeed;
    
    if(ballx < 0 ){
      // went through left wall
      xspeed  = -xspeed;
      reset();
      totalTries++;
      return;
    } else if( ballx >= width){
      // ball hit right wall
      ballx = width - WALL_OFFSET;
      xspeed = -xspeed;
      if(yspeed > 0){
	// going up, needs to go down
	yspeed = -5;
      } else
	yspeed = 5; // going down, needs to go up
	
    }

    if(bally > 0){
      // went through top
      yspeed = -yspeed;
      bally = -WALL_OFFSET;
    } else if(bally < -height){
      // went through floor
      yspeed = -yspeed;
      bally = -(height - WALL_OFFSET); // put just about the floor
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
    paddley = std::floor(height/2);
    paddlex = 0;
    xspeed = 10;
    yspeed = 0;
  }

 
