// Test the functionality of the latency inducer


#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "latency.h"





bool gameObjectsSet;
class latencyTester{
public:
  Latency* lat;

  latencyTester(){
    lat = new Latency(NULL, 8);
  }
  int getSendBufferSize(){
    return lat->sendBuffer->size();
  }
  std::string getSendBufferTop(){
    return lat->sendBuffer->top();
  }
  int getRcvBufferSize(){
    return lat->receiveBuffer->size();
  }
  std::string getRcvBufferTop(){
    return lat->receiveBuffer->top();
  }
  int getSendIDTop(){
    return lat->sendIDs->top();
  }
  int getSendIDSize(){
    return lat->sendIDs->size();
  }
  int getRcvIDTop(){
    return  lat->receiveIDs->top();
  }
  int getRcvIDSize(){
    return  lat->receiveIDs->size();
  }

};

int main(){
  int clientOne = 3;
  int clientTwo = 5;
  std::string messageOne = ":D";
  std::string messageTwo = ":(";

  latencyTester lt;

  
  


  // Test adding to send queue
  lt.lat->sendMessage(clientTwo, messageTwo);
  assert(1 ==  lt.getSendBufferSize());
  assert(1 == lt.getSendIDSize());
  assert(clientTwo == lt.getSendIDTop());
  assert(messageTwo.compare(lt.getSendBufferTop()) == 0);
  assert(0 == lt.getRcvBufferSize());


  // Test adding to receive queue
  lt.lat->receiveMessage(clientOne, messageOne);
  assert(1 ==  lt.getRcvBufferSize());
  assert(1 == lt.getRcvIDSize());
  assert(clientOne == lt.getRcvIDTop());
  assert(messageOne.compare(lt.getRcvBufferTop()) == 0);
  assert(1 == lt.getSendBufferSize());
  

  printf("Testing Completed\n");
  return 0;


}
