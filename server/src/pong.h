#include <stdio.h>
#include <iostream>
#include <cmath>

class pong{

public:
  int ballx, bally, paddlex, paddley;
  double xSpeed, ySpeed;
  int paddleDirection
  
  int height, width;
  
  int score;
  int totalTries;

  pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddleStartX, int paddleStartY){

    height = screenHeight;
    width = screenWidth;
    
    ballx = ballStartX;
    bally = ballStartY;
    paddlex = paddleStartX;
    paddley = paddleStartY;

    score = 0;
    totalTries = 0;
}

  
  void setBallPos(int x, int y){
    ballx = x;
    bally = y;

  }

  void setPaddlePos(int x, int y){
    paddlex = x;
    paddley = y;

  }

  void update(){
    if(abs(ballx - paddlex)  < 10 && abs(bally - paddley) < 10){
      // close enough; counts as hit
      score ++;
      xspeed = -xspeed;
      yspeed = paddleDirection * yspeed / 2;
      ballx = ballx + 10;
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
    } else if( ballx > width){
      // ball hit right wall
      ballx = width - 10;
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
      bally = -5;
    } else if(bally < -height){
      // went through floor
      yspeed = -yspeed;
      bally = -(height - 5); // put just about the floor
    }
    
    
    
  }

  double distance(){
    double res = (ballx - paddlex)*(ballx - paddlex);
    res +=  (bally - paddley)*(bally - paddley);

    return sqrt(res);
  }

  void reset(){
    ballx = 0;
    bally = 0;
    paddley = height/2;
    paddlex = 0;
   
  }

  
  
};
