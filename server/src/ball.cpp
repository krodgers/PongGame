/*
  Pong Ball Object
  Kathryn Rodgers UCID: 39483825
  Kevin Malby UCID: 36928917

 */

 #include "ball.h"

/*
  Constructor
  Sets ball's initial position and direction
  startDirection:  -1 (left <--) or 1 (right -->)
  Angle defaults to 0
 */
PongBall::PongBall(int startX, int startY, int startDirection){
  ballx = startX;
  bally = startY;
  
  timeStep = 1;

  ballVelocity = startDirection == 1 || startDirection == -1 ? startDirection * DEFAULT_BALL_VELOCITY : DEFAULT_BALL_VELOCITY;

  ballAngle = 0;
  
}

////// Accessors /////////

// Returns ball's position at this moment in time
void PongBall::getCurrentPosition(int& outX, int& outY){
  outX = ballx;
  outY = bally;
}
/* Set's the ball's speed and angle
   Angle relative to X axis, which is in the middle of the board
   Negative angles denote downward motion

   direction:   -1 (left <--) or 1 (right -->)
 */
void PongBall::setVelocity(int magnitude, double angle, int direction){
  ballVelocity = direction == -1 || direction == 1 ? direction*magnitude : magnitude;
  ballAngle = angle;
}

// Sets the ball's position to x,y
// Assumes the origin in the middle of the screen
void PongBall::setPosition(int x, int y){
  ballx = x;
  bally = y;

}

// returns the velocity of the ball
// negative if ball going left <---, positive if ball going right --->
int PongBall::getVelocity(){
  return ballVelocity;
}


// returns the angle from the X-axis
// negative means below the X-axis; positive means above X axis
double PongBall::getAngle(){
  return ballAngle;
}



////// Utility Functions //////////////

/*
  Estimates ball's new position based on current velocity and angle
  from X-axis
 */
void PongBall::calculatePosition(int& outX, int& outY){
  double fx = ballVelocity * std::cos(ballAngle);
  double fy = ballVelocity * std::sin(ballAngle);

  outX = ballx + ballVelocity*timeStep;
  outY = bally + ballVelocity*timeStep;

  
}



