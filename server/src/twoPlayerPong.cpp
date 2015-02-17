
/**
   Kevin Malby UCID: 36928917
   Kathryn Rodgers UCID: 39483825
**/

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cmath>
#include "twoPlayerPong.h"

#define WALL_OFFSET 8
#define X_VELOCITY 400
#define Y_VELOCITY 50
#define Y_ADJUST 50


pong::pong(){
  init();
}
pong::pong(int screenHeight, int screenWidth){
  init();
  boardHeight = screenHeight;
  boardWidth = screenWidth;
  ballx = std::floor(screenWidth/2);
  bally = std::floor(screenHeight/2);
}
pong::pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddle1StartX, int paddle1StartY, int paddle2StartX, int paddle2StartY){

  init();
  
  boardHeight = screenHeight;
  boardWidth = screenWidth;

  ballx = ballStartX;
  bally = ballStartY;
  paddlex[PLAYER_ONE] = paddle1StartX;
  paddley[PLAYER_ONE] = paddle1StartY;
  paddlex[PLAYER_TWO] = paddle2StartX;
  paddley[PLAYER_TWO] = paddle2StartY;

}

// Initialize all of the things
void pong::init(){
  xspeed = X_VELOCITY;
  yspeed = rand() % Y_VELOCITY;
  paddleHeight = 10;
  paddleWidth = 1;
  paddlex[PLAYER_ONE] = paddlex[PLAYER_TWO] = 0;
  paddley[PLAYER_ONE] = paddley[PLAYER_TWO]  = bally;
  paddleDirection[PLAYER_ONE] = paddleDirection[PLAYER_TWO] = 1;
  score[PLAYER_ONE] = score[PLAYER_TWO] = 0;
  totalTries[PLAYER_ONE] = totalTries[PLAYER_TWO] = 0;
  playerID[0] = "A";
  playerID[1] = "B";
  ballx = 250;
  bally = 250;
  ballradius = 5;
  boardHeight = 500;
  boardWidth = 500;
  gameObjectSet = false;
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

void pong::setPaddleDirection(std::string player, int direction) {
  paddleDirection[playerIDtoIndex(player)] = direction;
}

void  pong::setPaddlePos(std::string player, int x, int y){
  
    paddlex[playerIDtoIndex(player)] = x;
    paddley[playerIDtoIndex(player)] = y;

}
void pong::setPaddleDimensions(int h, int w){
  paddleWidth = w;
  paddleHeight = h;
}


void pong::setPlayerNames(std::string playerOne, std::string playerTwo) {
  playerID[0] = playerOne;
  playerID[1] = playerTwo;
}


std::string pong::getPlayerName(int whichPlayer){
  return playerID[whichPlayer-1];
}

int pong::getPlayerNumber(std::string player){
  return playerIDtoIndex(player) + 1;
}


// paddleDirection: 1/-1 for up/down
// paddleX assumed to be constant
void pong::update( double delTime) {

  ballx += (xspeed*delTime);
  bally += (yspeed*delTime);


  // Score for Left Player
  
  bool closeX = (ballx - ballradius)  <= paddlex[PLAYER_ONE] + paddleWidth; // left wall;
  if(closeX && (bally+ballradius >= paddley[PLAYER_ONE] &&( bally-ballradius <= (paddley[PLAYER_ONE] + paddleHeight)) )){
    // close enough; counts as hit
    score[PLAYER_ONE] ++;
    totalTries[PLAYER_ONE] ++;
    xspeed = -xspeed;
    //    std::cout << paddleDirection << std::endl;
    xspeed += xspeed * 0.07;
    switch(paddleDirection[PLAYER_ONE]){
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


   // Score for Right Player

  closeX = (ballx + ballradius)  >= paddlex[PLAYER_TWO]; // right wall
  if(closeX && (bally+ballradius >= paddley[PLAYER_TWO] &&( bally-ballradius <= (paddley[PLAYER_TWO] + paddleHeight)) )){
    // close enough; counts as hit
    score[PLAYER_TWO] ++;
    totalTries[PLAYER_TWO] ++;
    xspeed = -xspeed;
    //    std::cout << paddleDirection << std::endl;
    xspeed += xspeed * 0.07;
    switch(paddleDirection[PLAYER_TWO]){
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
    totalTries[PLAYER_ONE]++;
    return;
  } else if( ballx+ballradius >= boardWidth){
    // ball hit right wall
    xspeed  = -xspeed;
    reset();
    totalTries[PLAYER_TWO]++;
    return;
    // ballx = boardWidth - WALL_OFFSET - ballradius;
    // xspeed = -xspeed;

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


void  pong::reset(){

  xspeed = X_VELOCITY;
  yspeed = rand() % Y_VELOCITY;
  ballx = std::floor(boardWidth/2);
  bally = std::floor(boardHeight/2);
  paddlex[PLAYER_ONE] = paddlex[PLAYER_TWO] = 0;
  paddley[PLAYER_ONE] = paddley[PLAYER_TWO]  = bally;
  paddleDirection[PLAYER_ONE] = paddleDirection[PLAYER_TWO] = 1;
  
  
}

int pong::playerIDtoIndex(std::string id){
  if(id.compare(playerID[0]) == 0)
    return PLAYER_ONE;
  if(id.compare(playerID[1]) == 0)
    return PLAYER_TWO;
  
  printf("WARNING: Unknown player %s\n", id.c_str());
  return PLAYER_ONE; // default to player one
}


