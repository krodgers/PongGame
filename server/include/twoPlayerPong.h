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
#include "player.h"


class pong {

private:
    double xspeed, yspeed;

    void init();


public:
    double ballx, bally;
    int ballradius;

    int boardHeight, boardWidth;
    bool gameObjectSet;

    Player *playerOne;
    Player *playerTwo;

    Player *players[2];

    pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddle1StartX, int paddle1StartY, int paddle2StartX, int paddle2StartY);

    pong(int h, int w);

    pong();

    void setBallPos(int x, int y);

    void setBallRadius(int);

    void setBallSpeed(double speedX, double speedY);

    void getBallPosition(std::string player, int &outX, int &outY);

    Player *getPlayerFromClientID(int clientID);

    Player *getPlayerFromName(std::string name);

    Player *getOpponent(Player *player);


    void update(double delTime);

    double distance();

    void reset();


};

#endif
