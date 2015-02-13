/*
  Pong Game State
  Kathryn Rodgers UCID: 39483825
  Kevin Malby UCID: 36928917

 */

#include "pongGame.h"
#include <iostream>

/*
  Basic Constructor

  Creates a game object, initializes the ball to
  start in the middle of the board and will start to the right (this
  direction ----->)

  Returns NULL if too many players or unable to allocate memory

 */
PongGame::PongGame(int numPeople){
  if(numPlayers > 2){
    std::cout << "Too many players for a ping-pong game" << std::endl;
    ball = NULL;
    paddle = NULL;
  }

  numPlayers = numPeople;
  try{
    ball = new PongBall(0,0,1);
    paddle = new PongPaddle(0, -1);
  } catch(int i ){
    std::cout << "Failed to allocate paddles" << std::endl;
    ball = NULL;
    paddle = NULL;
  }

  init();
}


/*
  Constructor Allows the starting position and direction (-1 for left
  <---- , 1 for right --->) of the ball to be specified
 */

PongGame::  PongGame(int numPeople, int ballStartX, int ballStartY, int ballStartDirection, int ballVelocity) {
  if(numPlayers > 2){
    std::cout << "Too many players for a ping-pong game" << std::endl;
    paddle = NULL;
    ball = NULL;
  }

  numPlayers = numPeople;
  try{
    paddle = new PongPaddle(0, -1);
    ball = new PongBall(ballStartX, ballStartY, ballStartDirection);
  
  } catch(int bad){
    std::cout << "Failed to allocate paddles" << std::endl;
    paddle = NULL;
    ball = NULL;
  }

  init();
}

// Initialize all the little variables
void PongGame::init(){
  score = 0;
  boardWidth = 500;
  boardHeight = 500;
}

// Returns the score of human player
int PongGame::getScore(){

  return score; // will have to be an array when we get 2 players
}

// Set the score of the human player
void PongGame::setScore(int newScore){
  score = newScore;
}

/* Sets the ball's speed and angle
 Angle is relative to the X axis
 Angles below the X axis are negative
 X axis is in the middle of the board 
*/
void PongGame::setBallVelocity(int speed, double ballAngle, int direction){
  ball->setVelocity(speed, ballAngle, direction);
}

// Set the board's size
void PongGame::setBoardSize(int width, int height){
  boardHeight = height;
  boardWidth = width;
}

// Set human player's paddle position
void PongGame::setPaddlePosition(int paddleY){
  paddle->setPosition(paddleY);
}

// Puts ball's current position in outX and outY
void PongGame::getBallCurrentPosition(int& outX, int& outY){
  int newX, newY;
  ball->getCurrentPosition(newX, newY);

  if(height - newX < 0){
    outX = height - 5;
    ball->setVelocity(ball->getVelocity(), 0, -1);
  } else if (newX < 0){
    outX = 5;
    ball->setVelocity(ball->getVelocity(), 0, -1);
  }
  outX = newX;
  outY = outY;
  
  

  
}

/* Put's ball's calculated position in outX and outY
   Calculations based on ballSpeed, ballDirection, ballAngle
   Use -1 for ballSpeed or ballAngle to use the ball's current values
   ballDirection must be set

 */
void PongGame::getBallPredictedPosition(int& outX, int& outY, int ballSpeed, int ballDirection, double ballAngle){
  if(ballSpeed != -1){
    if(ballAngle != -1)
      ball->setVelocity(ballSpeed, ballAngle, ballDirection);
    else
      ball->setVelocity(ballSpeed, ball->getAngle(), ballDirection);
  }
  int newX, newY;
  ball->calculatePosition(newX, newY);
  outX = newX;
  outY = outY;
  
}
