#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Player {

private:
    class Paddle {
    public:
        int x;
        int y;
        int width;
        int height;
        int direction;

        Paddle() {
            x = y = width = height = direction = 0;
        }
    };

private:
    Paddle *paddle;
    string name;
    int assignedClientID;
    int score;
    int hits;
    int tries;
    string opponentName;

public:
    Player();

    void setName(string name);

    string getName();

    void setAssignedClientID(int clientID);

    int getAssignedClientID();

    void setScore(int score);

    int getScore();

    void setHits(int hits);

    int getHits();

    void setTries(int tries);

    int getTries();

    void incrementScore();

    void incrementHits();

    void incrementTries();

    void resetStats();

    void reset();

    // Paddle Stuff
    vector<int> getPaddlePosition();

    vector<int> getPaddleDimensions();

    void setPaddlePosition(int x, int y);

    void setPaddleDimensions(int width, int height);

    int getPaddleXPosition();

    int getPaddleYPosition();

    int getPaddleWidth();

    int getPaddleHeight();

    void setPaddleDirection(int direction);

    int getPaddleDirection();
  
};

#endif
