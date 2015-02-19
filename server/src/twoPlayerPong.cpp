
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
#define PLAYER_ONE 0
#define PLAYER_TWO 1


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
  //score[PLAYER_ONE] = score[PLAYER_TWO] = 0;
  scores.push_back(0);
  scores.push_back(0);
  totalTries.push_back(0);
  totalTries.push_back(0);
  playerName[0] = "!";
  playerName[1] = "*";
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

// Returns coordinates corrected for both players being on the left
// i.e. when the ball is really on the right and player 2 wants coordinates,
//                  will give coordinates as if that player 2 is on the left
void pong::getBallPosition(std::string player, int& outX, int& outY){
  if(PLAYER_ONE == playerNametoIndex(player)){
      outX = ballx;
      outY = bally;
    } else if (PLAYER_TWO == playerNametoIndex(player)){
      outX = boardWidth - ballx;
      outY = bally;
  } else{
    printf("wARNING: getting ball position for non existant player %s\n", player.c_str());
  }

}

void pong::setPaddleDirection(std::string player, int direction) {
  paddleDirection[playerNametoIndex(player)] = direction;
}

void  pong::setPaddlePos(std::string player, int x, int y){

    paddlex[playerNametoIndex(player)] = x;
    paddley[playerNametoIndex(player)] = y;

}

std::vector<int> pong::getPaddlePos(std::string player) {

    std::vector<int> pos (2);

    pos[0] = paddlex[playerNametoIndex(player)];
    pos[1] = paddley[playerNametoIndex(player)];
    return pos;
}

void pong::setPaddleDimensions(int h, int w){
  paddleWidth = w;
  paddleHeight = h;
}


void pong::setPlayerNames(std::string playerOne, std::string playerTwo) {
  playerName[0] = playerOne;
  playerName[1] = playerTwo;
}

// map an integer ID to a player
// whichPlayer: 0 or 1
// playerID:  the id to map to this player
void pong::setPlayerID(int whichPlayer, int playerID){
  if(whichPlayer == 0 || whichPlayer == 1){
    playerIDS[whichPlayer] = playerID;
  } else{
    printf("WARNING: setting an ID for invalid player %d\n", whichPlayer);
  }

}


void pong::setPlayerName(std::string player) {
    if (playerName[0].compare("!") == 0)
        playerName[0] = player;
    else
        playerName[1] = player;
}


// returns the name of the player mapped to given ID
std::string pong::getPlayerName(int playerID){
  return playerName[playerIDtoIndex(playerID)];
}

int pong::getPlayerNumber(std::string player){
  return playerNametoIndex(player) + 1;
}


int pong::getPlayerID(std::string player){
  return playerIDS[playerNametoIndex(player)];
}

// returns player's opponent's ID
int pong::getOtherPlayersID(std::string player){
  return playerNametoIndex(player) == PLAYER_ONE ? playerIDS[PLAYER_TWO] : playerIDS[PLAYER_ONE];
}

// returns player's opponent's ID -- the ID assigned to the player
int pong::getOtherPlayersID(int playerID){
  return playerIDtoIndex(playerID) == PLAYER_ONE ? playerIDS[PLAYER_TWO] : playerIDS[PLAYER_ONE];
}

std::string pong::getOtherPlayersName(std::string player){
  return playerNametoIndex(player) == PLAYER_ONE ? playerName[PLAYER_TWO] : playerName[PLAYER_ONE];

}

// returns playerID's opponent's name
std::string pong::getOtherPlayersName(int playerID){
  return playerIDtoIndex(playerID) == PLAYER_ONE ? playerName[PLAYER_TWO] : playerName[PLAYER_ONE];
}



int pong::getScore(std::string player) {
    return scores[playerNametoIndex(player)];
}
int pong::getScore(int playerID) {
    return scores[playerIDtoIndex(playerID)];
}

int pong::getTotalTries(std::string player) {
    return totalTries[playerNametoIndex(player)];
}

int pong::getTotalTries(int playerID) {
    return totalTries[playerIDtoIndex(playerID)];
}


// paddleDirection: 1/-1 for up/down
// paddleX assumed to be constant
void pong::update( double delTime) {

  ballx += (xspeed*delTime);
  bally += (yspeed*delTime);


  // Left Player hits the ball
  // at left wall
  bool closeX = ((ballx - ballradius)  <= (paddlex[PLAYER_ONE] + paddleWidth)) && ballx - ballradius >= 0; // left wall;
  bool closeY =(bally+ballradius >= paddley[PLAYER_ONE]) &&( bally-ballradius <= (paddley[PLAYER_ONE] + paddleHeight)) && (bally < boardHeight) && (bally > 0);
  if(closeX && closeY ){
    // close enough; counts as hit
    totalTries[PLAYER_ONE]++;
    scores[PLAYER_ONE]++;
    std::cout << "Player One hit ball" << std::endl;
    std::cout << "Player One Score: " << scores[PLAYER_ONE] << ", Player One Tries:" << totalTries[PLAYER_ONE] << ", Player Two Score: " << scores[PLAYER_TWO] << ", Player Two Tries:" << totalTries[PLAYER_TWO] << std::endl;
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


   //  Right Player hits the ball
  // at right wall
  closeX = (ballx + ballradius)  >= paddlex[PLAYER_TWO] && (ballx + ballradius <= boardWidth); // right wall
  if(closeX && (bally+ballradius >= paddley[PLAYER_TWO] &&( bally-ballradius <= (paddley[PLAYER_TWO] + paddleHeight)) ) && (bally < boardHeight) && (bally > 0)){
    // close enough; counts as hit
    totalTries[PLAYER_TWO]++;
    scores[PLAYER_TWO]++;
    std::cout << "Player Two hit ball" << std::endl;
    std::cout << "Player One Score: " << scores[PLAYER_ONE] << ", Player One Tries:" << totalTries[PLAYER_ONE] << ", Player Two Score: " << scores[PLAYER_TWO] << ", Player Two Tries:" << totalTries[PLAYER_TWO] << std::endl;
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
    ballx = ballx - WALL_OFFSET - ballradius;
    return;
  }


  if(ballx-ballradius < 0 ){
    // went through left wall
    xspeed  = -xspeed;
    reset();
    //score[PLAYER_TWO]++;
    totalTries[PLAYER_ONE]++;
    std::cout << "Player Two scores" << std::endl;
    std::cout << "Player One Score: " << scores[PLAYER_ONE] << ", Player One Tries:" << totalTries[PLAYER_ONE] << ", Player Two Score: " << scores[PLAYER_TWO] << ", Player Two Tries:" << totalTries[PLAYER_TWO] << std::endl;
    std::cout << "VECTOR | Player One Score: " << scores[PLAYER_ONE] << ", Player One Tries:" << totalTries[PLAYER_ONE] << ", Player Two Score: " << scores[PLAYER_TWO] << ", Player Two Tries:" << totalTries[PLAYER_TWO] << std::endl;
    return;
  } else if( ballx+ballradius >= boardWidth){
    // ball hit right wall
    xspeed  = -xspeed;
    reset();
    //score[PLAYER_ONE]++;
    totalTries[PLAYER_TWO]++;
    std::cout << "Player One scores" << std::endl;
    std::cout << "Player One Score: " << scores[PLAYER_ONE] << ", Player One Tries:" << totalTries[PLAYER_ONE] << ", Player Two Score: " << scores[PLAYER_TWO] << ", Player Two Tries:" << totalTries[PLAYER_TWO] << std::endl;
    std::cout << "VECTOR | Player One Score: " << scores[PLAYER_ONE] << ", Player One Tries:" << totalTries[PLAYER_ONE] << ", Player Two Score: " << scores[PLAYER_TWO] << ", Player Two Tries:" << totalTries[PLAYER_TWO] << std::endl;
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
//  paddlex[PLAYER_ONE] = paddlex[PLAYER_TWO] = 0;
//  paddley[PLAYER_ONE] = paddley[PLAYER_TWO]  = bally;
//  paddleDirection[PLAYER_ONE] = paddleDirection[PLAYER_TWO] = 1;


}

// Returns the index into all of the array variables
// corresponding to the player number of the string id
int pong::playerNametoIndex(std::string id){
  if(id.compare(playerName[0]) == 0)
    return PLAYER_ONE;

  if(id.compare(playerName[1]) == 0)
    return PLAYER_TWO;

  printf("WARNING: Unknown player %s\n", id.c_str());
  return PLAYER_ONE; // default to player one
}

// Returns the index into all of the array variables
// corresponding to the player number of the string id
int pong::playerIDtoIndex(int id){
  if(id == playerIDS[0])
    return PLAYER_ONE;
  if(id == playerIDS[1])
    return PLAYER_TWO;

  printf("WARNING: Unknown player ID %d\n", id);
  return PLAYER_ONE; // default to player one
}

