
/**
   Kevin Malby UCID: 36928917
   Kathryn Rodgers UCID: 39483825
**/

#include <iostream>
#include <stdio.h>
#include "chipmunk.h"
#include "pongGame.h"


#define BALL_MASS 10
#define PADDLE_MASS 10

int main(int argc, char** argv){
  
  // Make the space
  cpSpace *world = cpSpaceNew();

  // Make some objects

  cpFloat ballMoment = cpMomentForCircle(BALL_MASS, 0 , 5, cpvzero);

  cpBody* theBallBody = cpBodyNew(BALL_MASS,ballMoment);
  cpBodySetPosition(theBallBody, cpv(0,0));
  cpShape* theBall = cpCircleShapeNew(theBallBody, BALL_MASS, cpvzero);
 
  cpVect endPoints = cpv(0,10);
  cpBody* thePaddleBody = cpBodyNew(10,0);
  cpShape* thePaddle = cpSegmentShapeNew(thePaddleBody, cpvzero, endPoints, 3.0f );


  

  cpSpaceAddBody(world,theBallBody);
  cpSpaceAddBody(world, thePaddleBody);
  cpSpaceAddShape(world, theBall);
  cpSpaceAddShape(world, thePaddle);
  
  // Verify Information
  


  // Set collision things

  // Run something...
  cpFloat timeStep = (1.0/60.0);
  for(cpFloat time = 0; time < 2; time+= timeStep){
    cpVect pos = cpBodyGetPosition(theBallBody);
    cpVect vel = cpBodyGetVelocity(theBallBody);
    printf( "Time %.2f: Ball position (%.2f, %.2f), Ball velocity(%.2f, %.2f)\n", time, pos.x, pos.y, vel.x, vel.y);

    
    cpSpaceStep(world, timeStep);
 }

  // Clean up
  cpSpaceFree(world);
  cpBodyFree(theBallBody);
  cpBodyFree(thePaddleBody);
  cpShapeFree(theBall);
  char a;
  std::cin>>a;
  return 0;

}

