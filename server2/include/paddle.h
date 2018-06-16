#ifndef __PADDLE_H__
#define __PADDLE_H__


#define DEFAULT_PADDLE_VELOCITY 0
class PongPaddle{
 public:
  void calculatePosition(int& outX, int& outY);
  void setVelocity(int magnitude, int direction);
  void setPosition(int y);
  void setPaddleDimensions(int width, int height);

  int getCurrentPosition();
  int getSpeed();
  int getDirection();

  PongPaddle(int startY, int startDir);


 private:
  int paddleY;
  int paddleVelocity;
  int paddleDirection; // pos (1) or neg(-1) === up or down
  int paddleHeight;
  int paddleWidth;

  
  

};




#endif
