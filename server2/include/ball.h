#ifndef __BALL_H__
#define __BALL_H__


#define DEFAULT_BALL_VELOCITY  10
class PongBall{

 public:
  void calculatePosition(int& outX, int& outY);
  void getCurrentPosition(int& outX, int& outY);
  void setVelocity(int magnitude, double angle,int direction);
  void setPosition(int x, int y);

  int getVelocity();
  double getAngle();

  PongBall(int startX, int startY, int startDirection);
  


 private:
  int ballx;
  int bally;
  int ballVelocity; // negative if left, positive if right
  double ballAngle; // center of screen is (0,0)
  

};




#endif
