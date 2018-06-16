#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <cmath>
#include "pong.h"

#define BOARD_SIZE 40
#define PADDLE_OBJ 1
#define BALL_OBJ -1
class boardThings{
private:
  int board[BOARD_SIZE][BOARD_SIZE];
  int currChar;
  int currPaddChar;

public:
  boardThings(){
    currChar = 'A';
    currPaddChar = 'z';
    for(int i = 0; i < BOARD_SIZE; i ++){
      for(int j = 0; j < BOARD_SIZE; j ++){
	board[i][j] = 0;
	
      } 

    }
  }
  
  void setBoard(int x, int y, int object){
    if( x < BOARD_SIZE && y < BOARD_SIZE){
      if(object == PADDLE_OBJ)
	board[-y][x] = PADDLE_OBJ;
      if(object == BALL_OBJ)
	board[-y][x] = BALL_OBJ;
    }
  }
  void unsetBoard(int i, int j){
    if( i < BOARD_SIZE && j < BOARD_SIZE)
      board[j][i] = 0;
  }    
  void drawBoard(){
    printf("\n------------------------------------\n");
    for(int i = 0; i < BOARD_SIZE; i ++){
      for(int j = 0; j < BOARD_SIZE; j ++){
	if(board[i][j] == PADDLE_OBJ)
	  printf("%c", currChar++);
	else if(board[i][j] == BALL_OBJ)
	  printf("%c", currPaddChar--);
	else
	  printf(".");
      }
      
      printf("\n");
    } 

  }

};

int getXspeed(pong game){
  int currx = game.ballx;
  
  game.update(0,BOARD_SIZE/2);
  game.update(0,BOARD_SIZE/2);
  
  if(game.ballx - currx > 0)
    return 1;
  else
    return -1;
  
  
}
int getYspeed(pong game){
  int curry = game.bally;
  
  game.update(0,BOARD_SIZE/2);
  // game.update(0,BOARD_SIZE/2);
  
  if(std::abs(game.bally) -std::abs( curry) > 0)
    return -1;
  else
    return 1;
  
}


int main(){
  boardThings board;

  int ballx = BOARD_SIZE/2;
  int bally = -ballx;
  
  int score = 0, tries = 0;
  pong p(BOARD_SIZE, BOARD_SIZE,ballx,bally,0, -10);

  /////////////////////////////////////////////////
  /// test scoring
  ///
  ///////////////////////////////////////////////
  //close 
  p.setPaddleDimensions(10,1);
  p.setPaddlePos(0,-10);
  p.setBallPos(40, -10);
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == 0);
  assert(p.totalTries == tries);
  p.setBallPos(20,-20);
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == 0);
  assert(p.totalTries == tries);

  //hit
  // furthest x could be
  ballx = 1 + p.ballradius + 10 ;
  p.setBallPos(ballx, -10); // top of paddle
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == ++score);
  assert(p.totalTries == ++ tries);
 
  p.setBallPos(ballx, -15); // mid of paddle
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == ++score);
  assert(p.totalTries == ++ tries);

  p.setBallPos(ballx, -20); // bottom of paddle
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == ++score);
  assert(p.totalTries == ++ tries);

  //closer to paddle
  ballx = 1 + p.ballradius + 10 ;

  p.setBallPos(ballx, -10); // top of paddle
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == ++score);
  assert(p.totalTries == ++ tries);

  p.setBallPos(ballx, -15); // mid of paddle
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == ++score);
  assert(p.totalTries == ++ tries);

  p.setBallPos(ballx, -20); // bottom of paddle
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == ++score);
  assert(p.totalTries == ++ tries);

 
   // miss
 // at the wall
  ballx = 5;
  p.setBallPos(ballx, 1);  // above paddle
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == score);
  assert(p.totalTries == ++ tries);

  p.setBallPos(ballx, -30); // below paddle
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == score);
  assert(p.totalTries == ++ tries);
 

  // behind the wall
  ballx = 3;
  p.setBallPos(ballx, 1);  // above paddle
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == score);
  assert(p.totalTries == ++ tries);


  p.setBallPos(ballx, -30); // below paddle
  p.setBallSpeed(-10,0);
  p.update(0, -10);
  assert(p.score == score);
  assert(p.totalTries == ++ tries);




  /////////////////////////////////////////////////
  /// test speeds
  ///
  ///////////////////////////////////////////////
  pong pSpeed(BOARD_SIZE, BOARD_SIZE);
  pSpeed.setBallSpeed(10,10);
  // starts right
  int res =  getXspeed(pSpeed);
  printf("res: %d\n", res);
  assert( res == 1);
  // hit right wall, going left
  ballx = BOARD_SIZE - pSpeed.ballradius -1;
  pSpeed.setBallPos(ballx, BOARD_SIZE/2);
  pSpeed.setBallSpeed(3, 2);
  res = getXspeed(pSpeed);
  assert(res == -1);
  // hit top wall, yspeed negative, x unchanged
  ballx = BOARD_SIZE/2;
  pSpeed.setBallPos(ballx, -(BOARD_SIZE-pSpeed.ballradius-1));
  pSpeed.setBallSpeed(1,10);
  res = getXspeed(pSpeed);
  assert(res == 1);
  res = getYspeed(pSpeed);
  assert(res == -1);
  // hit bottom wall, yspeed positive, x unchanged
  ballx = BOARD_SIZE/2;
  pSpeed.setBallPos(ballx, -(BOARD_SIZE-pSpeed.ballradius-1));
  pSpeed.setBallSpeed(-1,-10);
  res = getXspeed(pSpeed);
  assert(res == -1);
  res = getYspeed(pSpeed);
  assert(res == 1);
  // hit left wall, x negated
  ballx = pSpeed.ballradius - 3;
  pSpeed.setBallPos(ballx, -3);
  pSpeed.setBallSpeed(-10,1);
  res = getXspeed(pSpeed);
  assert(res == -1);
  
  // hit paddle not moving, yspeed same, x negated
  pSpeed.setBallPos(ballx, -BOARD_SIZE/2);
  pSpeed.setBallSpeed(-10,-3);
  pSpeed.update(0,BOARD_SIZE/2 + 5);
  res = getXspeed(pSpeed);
  assert(res == 1);
  res = getYspeed(pSpeed);
  assert(res == -1);
  // hit paddle going up, yspeed positive, x negated
  pSpeed.setBallPos(ballx, -BOARD_SIZE/2);
  pSpeed.setBallSpeed(-10,-3);
  pSpeed.update(1,BOARD_SIZE/2 + 5);
  res = getXspeed(pSpeed);
  assert(res == 1);
  res = getYspeed(pSpeed);
  assert(res == 1);
  // hit paddle going down, yspeed negative, x negated
  pSpeed.setBallPos(ballx, -BOARD_SIZE/2);
  pSpeed.setBallSpeed(-10,-3);
  pSpeed.update(-1,BOARD_SIZE/2 + 5);
  res = getXspeed(pSpeed);
  assert(res == 1);
  res = getYspeed(pSpeed);
  assert(res == -1);
  
  

  return 0;
  
}


