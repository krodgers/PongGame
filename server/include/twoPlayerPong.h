/**
   Kevin Malby UCID: 36928917
   Kathryn Rodgers UCID: 39483825

**/

#ifndef __PONG_H__
#define __PONG_H__
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>


class pong{

 private:
  double xspeed, yspeed;
  int paddleHeight, paddleWidth;
  int  paddlex[2], paddley[2]; // player zero is on Left, player one is on Right
  int paddleDirection[];
  int score[2]; // player zero is on Left, player one is on Right
  std::vector<int> scores;
  std::vector<int> totalTries;
  //int totalTries[2]; // player zero is on Left, player one is on Right
  std::string playerName[2]; // maps players' names to playerOne/playerTwo
  int playerIDS[2]; // maps player to an ID (like a clientID)
  void init();
  int playerNametoIndex(std::string);
  int playerIDtoIndex(int);

 public:
  double ballx, bally;
  int ballradius;

  int boardHeight, boardWidth;
  bool gameObjectSet;

  pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddle1StartX, int paddle1StartY, int paddle2StartX, int paddle2StartY);

  pong(int h, int w);
  pong();

  void setBallPos(int x, int y);
  void setBallRadius(int);
  void setBallSpeed(double speedX, double speedY);
  void getBallPosition(std::string player, int& outX, int& outY);

  void setPaddleDirection(std::string player, int direction);
  void setPaddlePos(std::string player, int x, int y);
  void setPaddleDimensions(int h , int w);
  std::vector<int> getPaddlePos(std::string player);

  void setPlayerNames(std::string playerOne, std::string playerTwo);
  void setPlayerID(int whichPlayer, int playerID); // whichPlayer: 0 or 1, maps to playerID
  void setPlayerName(std::string player);
  //  std::string getPlayerName(int whichPlayer); // player 0  or player 1
  std::string getPlayerName(int playerID);
  int getPlayerNumber(std::string player); // returns player 1 or player 2
  int getPlayerID(std::string player);
  int getOtherPlayersID(std::string player);
  int getOtherPlayersID(int playerID);
  std::string getOtherPlayersName(std::string player);
  std::string getOtherPlayersName(int);

  int getScore(std::string playerName);
  int getScore(int);
  int getTotalTries(std::string playerName);
  int getTotalTries(int);
  void update(double delTime);

  double distance();

  void reset();


};

#endif
