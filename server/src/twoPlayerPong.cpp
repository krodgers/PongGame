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


pong::pong() {
    init();
}

pong::pong(int screenHeight, int screenWidth) {
    init();
    boardHeight = screenHeight;
    boardWidth = screenWidth;
    ballx = std::floor(screenWidth / 2);
    bally = std::floor(screenHeight / 2);
}

pong::pong(int screenHeight, int screenWidth, int ballStartX, int ballStartY, int paddle1StartX, int paddle1StartY, int paddle2StartX, int paddle2StartY) {

    init();

    boardHeight = screenHeight;
    boardWidth = screenWidth;

    ballx = ballStartX;
    bally = ballStartY;

}

// Initialize all of the things
void pong::init() {
    xspeed = X_VELOCITY;
    yspeed = rand() % Y_VELOCITY;

    // paddlex = new int[2];
    // paddley = new int[2];
    // paddleDirection = new int[2];
    // score = new int[2];
    // totalTries = new int[2];
    // playerName = new std::string[2];

    ballx = 250;
    bally = 250;
    ballradius = 5;
    boardHeight = 500;
    boardWidth = 500;
    gameObjectSet = false;

    playerOne = new Player();
    playerTwo = new Player();

    players[0] = playerOne;
    players[1] = playerTwo;
}

void pong::setBallRadius(int r) {
    ballradius = r;
}

void  pong::setBallPos(int x, int y) {
    ballx = x;
    bally = y;

}

void pong::setBallSpeed(double speedX, double speedY) {
    xspeed = speedX;
    yspeed = speedY;
}

// Returns coordinates corrected for both players being on the left
// i.e. when the ball is really on the right and player 2 wants coordinates,
//                  will give coordinates as if that player 2 is on the left
void pong::getBallPosition(std::string player, int &outX, int &outY) {
    if (playerOne == getPlayerFromName(player)) {
        outX = ballx;
        outY = bally;
    } else if (playerTwo == getPlayerFromName(player)) {
        outX = boardWidth - ballx;
        outY = bally;
    } else {
        printf("wARNING: getting ball position for non existant player %s\n", player.c_str());
    }

}


// paddleDirection: 1/-1 for up/down
// paddleX assumed to be constant
void pong::update(double delTime) {

    ballx += (xspeed * delTime);
    bally += (yspeed * delTime);


    // Left Player hits the ball
    // at left wall
    bool closeX = ((ballx - ballradius) <= (playerOne->getPaddleXPosition() + playerOne->getPaddleWidth())) && ballx - ballradius >= 0; // left wall;
    bool closeY = (bally + ballradius >= playerOne->getPaddleYPosition()) && (bally - ballradius <= (playerOne->getPaddleYPosition() + playerOne->getPaddleHeight())) && (bally < boardHeight) && (bally > 0);
    if (closeX && closeY) {
        // close enough; counts as hit
        playerOne->incrementTries();
        playerOne->incrementHits();
        std::cout << "Player One hit ball" << std::endl;
        std::cout << "Player One Score: " << playerOne->getHits() << ", Player One Tries:" << playerOne->getTries() << ", Player Two Score: " << playerTwo->getHits() << ", Player Two Tries:" << playerTwo->getTries() << std::endl;
        std::cout << "Player One Score: " << playerOne->getHits() << ", Player One Tries:" << playerOne->getTries() << ", Player Two Score: " << playerTwo->getHits() << ", Player Two Tries:" << playerTwo->getTries() << std::endl;
        xspeed = -xspeed;
        //    std::cout << paddleDirection << std::endl;
        xspeed += xspeed * 0.07;
        switch (playerOne->getPaddleDirection()) {
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
    closeX = (ballx + ballradius) >= playerTwo->getPaddleXPosition() && (ballx + ballradius <= boardWidth); // right wall
    if (closeX && (bally + ballradius >= playerTwo->getPaddleYPosition() && (bally - ballradius <= (playerTwo->getPaddleYPosition() + playerTwo->getPaddleHeight()))) && (bally < boardHeight) && (bally > 0)) {
        // close enough; counts as hit
        playerTwo->incrementTries();
        playerTwo->incrementHits();
        std::cout << "Player Two hit ball" << std::endl;
        std::cout << "Player One Score: " << playerOne->getHits() << ", Player One Tries:" << playerOne->getTries() << ", Player Two Score: " << playerTwo->getHits() << ", Player Two Tries:" << playerTwo->getTries() << std::endl;
        xspeed = -xspeed;
        //    std::cout << paddleDirection << std::endl;
        xspeed += xspeed * 0.07;
        switch (playerTwo->getPaddleDirection()) {
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


    if (ballx - ballradius < 0) {
        // went through left wall
        xspeed = -xspeed;
        reset();
        //score[PLAYER_TWO]++;
        playerOne->incrementTries();
        std::cout << "Player Two scores" << std::endl;
        std::cout << "Player One Score: " << playerOne->getHits() << ", Player One Tries:" << playerOne->getTries() << ", Player Two Score: " << playerTwo->getHits() << ", Player Two Tries:" << playerTwo->getTries() << std::endl;
        return;
    } else if (ballx + ballradius >= boardWidth) {
        // ball hit right wall
        xspeed = -xspeed;
        reset();
        //score[PLAYER_ONE]++;
        playerTwo->incrementTries();
        std::cout << "Player One scores" << std::endl;
        std::cout << "Player One Score: " << playerOne->getHits() << ", Player One Tries:" << playerOne->getTries() << ", Player Two Score: " << playerTwo->getHits() << ", Player Two Tries:" << playerTwo->getTries() << std::endl;
        return;
        // ballx = boardWidth - WALL_OFFSET - ballradius;
        // xspeed = -xspeed;

    }

    if (bally - ballradius < 0) {
        // went through top
        yspeed = -yspeed;
        bally = WALL_OFFSET + ballradius;
    } else if (bally + ballradius > boardHeight) {
        // went through floor
        yspeed = -yspeed;
        bally = boardHeight - WALL_OFFSET - ballradius; // put just about the floor
    }

    if (xspeed >= boardWidth)
        xspeed = boardWidth;
    if (yspeed >= boardHeight)
        yspeed = boardHeight;

}


void  pong::reset() {

    xspeed = X_VELOCITY;
    yspeed = rand() % Y_VELOCITY;
    ballx = std::floor(boardWidth / 2);
    bally = std::floor(boardHeight / 2);
//  paddlex[PLAYER_ONE] = paddlex[PLAYER_TWO] = 0;
//  paddley[PLAYER_ONE] = paddley[PLAYER_TWO]  = bally;
//  paddleDirection[PLAYER_ONE] = paddleDirection[PLAYER_TWO] = 1;


}

Player *pong::getPlayerFromClientID(int clientID) {
    if (clientID == playerOne->getAssignedClientID())
        return playerOne;
    else if (clientID == playerTwo->getAssignedClientID())
        return playerTwo;
    else {
        // There is no player for that client ID, default to player one
        return playerOne;
    }

}

Player *pong::getPlayerFromName(std::string name) {
    if (name.compare(playerOne->getName()) == 0)
        return playerOne;
    else if (name.compare(playerTwo->getName()) == 0)
        return playerTwo;
    else {
        // There is no player with this name, default to player one
        return playerOne;
    }
}

Player *pong::getOpponent(Player *player) {
    if (player == playerOne)
        return playerTwo;
    else
        return playerOne;
}


