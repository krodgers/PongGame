#include <cmath>
#include "player.h"

Player::Player() {
    paddle = new Paddle();
    name = "";
    assignedClientID = -1;
    score = 0;
    hits = 0;
    tries = 0;
}

void Player::setName(string name) {
    this->name = name;
}

string Player::getName() {
    return this->name;
}

void Player::setAssignedClientID(int clientID) {
    this->assignedClientID = clientID;
}

int Player::getAssignedClientID() {
    return this->assignedClientID;
}

void Player::setScore(int score) {
    this->score = score;
}

int Player::getScore() {
    return this->score;
}

void Player::setHits(int hits) {
    this->hits = hits;
}

int Player::getHits() {
    return this->hits;
}

void Player::setTries(int tries) {
    this->tries = tries;
}

int Player::getTries() {
    return this->tries;
}

void Player::incrementScore() {
    this->score++;
}

void Player::incrementHits() {
    this->hits++;
}

void Player::incrementTries() {
    this->tries++;
}

void Player::resetStats() {
    score = 0;
    hits = 0;
    tries = 0;
}

void Player::reset() {
    name = "";
    assignedClientID = -1;
    score = 0;
    hits = 0;
    tries = 0;
    setPaddlePosition(0, 0);
    setPaddleDimensions(0, 0);
}


// Paddle Function Implementations
vector<int> Player::getPaddlePosition() {
    vector<int> v;
    v.push_back(paddle->x);
    v.push_back(paddle->y);
    return v;
}

vector<int> Player::getPaddleDimensions() {
    vector<int> v;
    v.push_back(paddle->width);
    v.push_back(paddle->height);
    return v;
}

void Player::setPaddlePosition(int x, int y) {
    paddle->x = x;
    paddle->y = y;
}

void Player::setPaddleDimensions(int width, int height) {
    paddle->width = width;
    paddle->height = height;
}

int Player::getPaddleXPosition() {
    return paddle->x;
}

int Player::getPaddleYPosition() {
    return paddle->y;
}

void Player::setPaddleDirection(int direction) {
    paddle->direction = direction;
}

int Player::getPaddleDirection() {
    return paddle->direction;
}

int Player::getPaddleWidth() {
    return paddle->width;
}

int Player::getPaddleHeight() {
    return paddle->height;
}

