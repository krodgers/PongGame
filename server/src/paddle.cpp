/*
  Pong Paddle Object
  Kathryn Rodgers UCID: 39483825
  Kevin Malby UCID: 36928917

 */

#include "paddle.h"
#include <stdlib.h>

/*
  Constructor
  Sets paddle's initial position and direction
  startDirection:  -1 (down V) or 1 (right ^)
  
 */
PongPaddle::PongPaddle(int startY, int startDirection){
  paddleY = startY;
  
  paddleVelocity = startDirection == 1 || startDirection == -1 ? startDirection * DEFAULT_PADDLE_VELOCITY : DEFAULT_PADDLE_VELOCITY;

  
}

////// Accessors /////////

// Returns paddle's position at this moment in time
int  PongPaddle::getCurrentPosition(){
  return paddleY;
}

/* Set's the paddle's speed
   
   direction:  -1 (down V) or 1 (right ^)
 */
void PongPaddle::setVelocity(int speed, int direction){
  paddleDirection = direction == -1 || direction == 1 ? direction : 1; 
  paddleVelocity =  paddleDirection*speed;
  
}

// Sets the paddle's position to y
// Assumes the origin in the middle of the screen
void PongPaddle::setPosition( int y){
  
  paddleY = y;

}

// returns the speed of the paddle
int PongPaddle::getSpeed(){
  return abs(paddleVelocity);
}

// return the direction of the paddle
// -1 if down, 1 if up
int PongPaddle::getDirection(){

  return paddleDirection;
}




////// Utility Functions //////////////

/*
  Estimates paddle's new position based on current velocity 
 */
void PongPaddle::calculatePosition(int& outX, int& outY){
  //TODO :: IMPLEMENT :D

}



