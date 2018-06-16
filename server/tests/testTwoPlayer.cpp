#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include "twoPlayerPong.h"

int main(){

  std::string alice = "Alice";
  std::string tyler = "Tyler";
  int score = 0, tries = 0;
  
  pong game(10, 10, 5, 5, 0, 6, 9, 2);
  game.setBallSpeed(1,1);
  game.setBallRadius(1);

  // test setting and getting player ids
  game.setPlayerNames(alice,tyler);
  game.setPlayerID(0, 0);
  game.setPlayerID(1,1);
  assert(alice.compare(game.getPlayerName(0)) == 0);
  assert(tyler.compare(game.getPlayerName(1)) == 0);
  assert(game.getPlayerNumber(alice) == 1);
  assert(game.getPlayerNumber(tyler) == 2);
	 

  // test getting player scores
  int ballx = 0;
  int bally = 4;
  int xspeed = -1;
  int yspeed = 1;

  game.setPaddleDirection(alice, 0);
  game.setPaddleDirection(tyler, 0);
  game.setPaddleDimensions(2, 1);

  // score on left : player two scores
  game.setBallPos(ballx,bally);
  game.setBallSpeed(xspeed,yspeed);
  game.update(1);

  assert(game.getScore(alice) == 0);
  assert(game.getScore(tyler) == 1);
  assert(game.getTotalTries(alice) == 1);
  assert(game.getTotalTries(tyler) == 0);

  // score on right : player one scores
  ballx = 9;
  bally = 8;
  xspeed = 1;
  yspeed = 1;
   
  game.setBallPos(ballx,bally);
  game.setBallSpeed(xspeed,yspeed);
  game.update(1);

  assert(game.getScore(alice) == 1);
  assert(game.getScore(tyler) == 1);
  assert(game.getTotalTries(alice) == 1);
  assert(game.getTotalTries(tyler) == 1);

  // test scoring
  // Player One, alice, hits ball
  ballx = 2;
  bally = 7;
  xspeed = -1;
  yspeed = 1;
   
  game.setBallPos(ballx,bally);
  game.setBallSpeed(xspeed,yspeed);
  game.update(1);

  assert(game.getScore(alice) == 1);
  assert(game.getScore(tyler) == 1);
  assert(game.getTotalTries(alice) == 2);
  assert(game.getTotalTries(tyler) == 1);


  // Player two, tyler, hits ball
  ballx = 8;
  bally = 3;
  xspeed = 1;
  yspeed = 1;
   
  game.setBallPos(ballx,bally);
  game.setBallSpeed(xspeed,yspeed);
  game.update(1);

  assert(game.getScore(alice) == 1);
  assert(game.getScore(tyler) == 1);
  assert(game.getTotalTries(alice) == 2);
  assert(game.getTotalTries(tyler) == 2);


  printf("Tests Completed\n");
}
