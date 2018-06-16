#include "websocket.h"
#include <stdio.h>






int main(){
  webSocket timesocket;
  timesocket.setMessageHandler(messageHandler);
  int port;
  cout << "Please set server port: ";
  cin >> port;
  
  server.startServer(port);
}
