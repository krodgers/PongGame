/**
   Kevin Malby UCID: 36928917
   Kathryn Rodgers UCID: 39483825
**/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include "websocket.h"
#include <json/config.h>
#include <json/value.h>
#include <json/writer.h>
#include <json/json.h>
#include "pong.h"
#include <cstdlib>
#ifdef __linux__
#include <pthread.h>
#include <unistd.h>
#elif _WIN32
#include <process.h>
#endif

using namespace std;

int serverThread;
int gameLoopThread;
bool gameObjectsSet;
webSocket server;
pong* pongGame;
int loopCount;


void Server(int);
void openHandler(int);
void closeHandler(int);
void checkPartnerPresent(int);
void messageHandler(int, string);
void* GameLoop(void*);



int main(int argc, char *argv[]){
  int port;
  gameObjectsSet = false;

  //setPongGame(*pongGame);

  cout << "Please set server port: ";
  cin >> port;

  loopCount = 1;

#ifdef __linux__
  pthread_t id;
  void* voidStar = &port;
  serverThread = pthread_create(&id, NULL, GameLoop, voidStar);

#elif _WIN32
  _beginthread( Server, 0, (void*)port );
#endif

  Server(port);


}

void* GameLoop(void* arg) {
  int scoreUpdateCounter = 0;
  while (true)
    {
      if (gameObjectsSet == true)
	{
	  //  cout << "here" << endl;
	  pongGame->update(1/60.0);

	  ostringstream json;

	  //////// DELETE ME ///////////
	  //	  printf("Game Loop: Updating ball position\n");
	  ///////////////////////////

	  json << "{\"phase\":\"ball_update\",\"ball_position\":[";
	  json << (int)pongGame->ballx << ", " << (int)pongGame->bally << "]}";
	  vector<int> clientIDs = server.getClientIDs();
	  for (int i = 0; i < clientIDs.size(); i++){
            server.wsSend(clientIDs[i], json.str());
	  }

	  // Only send score updates sometimes
	  if(scoreUpdateCounter % 10 == 0){

	    scoreUpdateCounter = 0;
	    ostringstream jsonScoring;
	    jsonScoring << "{\"phase\":\"score_update\",";
	    jsonScoring << "\"new_score\":\"" << pongGame->score << "\", \"num_tries\":\"" << pongGame->totalTries << "\"}";
	    for (int i = 0; i < clientIDs.size(); i++){
	      server.wsSend(clientIDs[i], jsonScoring.str());
	      //////// DELETE ME ///////////
	      //   printf("Game Loop: Updating score for client %d\n", i);
	      ///////////////////////////

	    }

	  }
	  scoreUpdateCounter++;
	  usleep(1000000/60);
	}
    }
}

void Server(int port) {
  server.setOpenHandler(openHandler);
  server.setCloseHandler(closeHandler);
  server.setMessageHandler(messageHandler);
  //server.setPeriodicHandler(periodicHandler);

  /* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
  server.startServer(port);
}

void openHandler(int clientID){
  //////// DELETE ME ///////////
  printf("Open Handler: Client %d connecting\n", clientID);
  ///////////////////////////

  pongGame = new pong();

  string json = "{\"phase\":\"initialization\"}";
  server.wsSend(clientID, json);
}

/* called when a client disconnects */
void closeHandler(int clientID){

  printf("Client %d disconnected\n", clientID);
}

void checkPartnerPresent(int clientID) {
  vector<int> clientIDs = server.getClientIDs();
  int partner = -1;
  for (int i = 0; i < clientIDs.size(); i++){
    if (clientIDs[i] != clientID)
      partner = clientIDs[i];
  }


    // todo, here we would handle the case where
    // one person is connected and not the other
    // which should wait may or may not wait until
    // the other player is connected
    // Since we don't have another player right now
    // it will just send data back to client
    //////// DELETE ME ///////////
    printf("checkPartnerPresent: Has partner %d\n", partner);
    ///////////////////////////

    int ballRadius = pongGame->ballradius;

    ostringstream json;
    json << "{\"phase\":\"initial_ball_position\",\"ball_position\":[";
    json << pongGame->ballx << ", " << pongGame->bally << "],";
    json << "\"ball_size\":" << ballRadius << "}";

    server.wsSend(clientID, json.str());

}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){

  // Let's parse it
  Json::Value root;
  Json::Reader reader;
  bool parsedSuccess = reader.parse(message,
				    root,
				    false);
  string phaseString = root["phase"].asString();

  if (phaseString.compare("initial_dimensions") == 0) {
    //////// DELETE ME ///////////
    printf("Phase String: initial_dimensions\n");
    ///////////////////////////

    const Json::Value mapDimJson = root["map_dimensions"];
    const Json::Value paddleDimJson = root["paddle_dimensions"];

    vector<int> mapDims(mapDimJson.size());
    vector<int> paddleDims(paddleDimJson.size());

    cout << "mapJSOn size: " << mapDimJson.size() << "paddleJson Size: " << paddleDimJson.size() << endl;

    cout << "Map Dimensions:" << endl;
    for (int i = 0; i < mapDimJson.size(); i++) {
      mapDims[i] = mapDimJson[i].asInt();
      cout << mapDims[i] << endl;
    }

    cout << "Paddle Dimensions:" << endl;
    for (int i = 0; i < paddleDimJson.size(); i++) {
      paddleDims[i] = paddleDimJson[i].asInt();
      cout << paddleDims[i] << endl;
    }

    pongGame->boardWidth = mapDims[2];
    pongGame->boardHeight = mapDims[3];
    pongGame->setPaddlePos(paddleDims[0], paddleDims[1]);
    pongGame->setPaddleDimensions(paddleDims[2], paddleDims[3]);
    pongGame->setBallPos(mapDims[2]/2, mapDims[3]/2);
    pongGame->setBallRadius(10);

    //setMapInfo(mapDims, paddleDims);
    checkPartnerPresent(clientID);

  } else if (phaseString.compare("ready_to_start") == 0) {
    cout << "Client " << clientID << " ready_to_start" << endl;
    // todo, figure out what to do when we have two clients perhaps
    // create a while loop that waits until another client has
    // connected or something

    // send request for player's information
    server.wsSend(clientID, "{\"phase\":\"send_info\"}");
    const Json::Value ballPosJson = root["ball_position"];
    vector<int> ballPos(ballPosJson.size());

    for (int i = 0; i < ballPosJson.size(); i++) {
      ballPos[i] = ballPosJson[i].asInt();
    }

    if (ballPos[0] != pongGame->ballx || ballPos[1] != pongGame->bally){
      // todo, do something to remedy situation where ball
      // position is not correct
    } else {
    }




  } else if (phaseString.compare("exchange_info") == 0){
    cout << "Client " << clientID << " exchange_info" << endl;

    // check for presence of another player
    // assuming that only 2 clients are going to be allowed to connect
    vector<int> clientIDs = server.getClientIDs();
    int partnerID = -1;
    for (int i = 0; i < clientIDs.size(); i++){
      if (clientIDs[i] != clientID) {
        partnerID = clientIDs[i];
      }
    }
    cout << "Client " << clientID << " has partner " << partnerID <<  endl;


    if(partnerID != -1 ){
      const Json::Value playerName = root["name"].asString();

      Json::Value jsonToSend;
      Json::FastWriter writer;
      jsonToSend["phase"] = "set_opponent";
      jsonToSend["name"] = playerName;
      string tmp = writer.write(jsonToSend);
      tmp = tmp.substr(0, tmp.length() - 1);

//      ostringstream os;
//      os << "{\"phase\":\"set_opponent\",\"name\":" << playerName <<  "\" }";
//      string tmp = os.str();
      server.wsSend(partnerID, writer.write(jsonToSend));



      server.wsSend(clientID,"{\"phase\":\"start\"}");
      printf("Client %d told to start\n", clientID);
      gameObjectsSet = true; // TODO:: clients need to use same object

    } else {
      // send wait signal
      // could handle this in a couple different ways:
      // 1) have client ping back in a few seconds
      // 2) have server ping back ready client when another client connects
      server.wsSend(clientID, "{\"phase\":\"wait\"}");
      printf("Client %d told to wait\n", clientID);

    }


  } else if (phaseString.compare("paddle_update") == 0) {
    //////// DELETE ME ///////////
    printf("Phase String: paddle_update\n");
    ///////////////////////////
    int paddleDirection = root["paddle_direction"].asInt();
    const Json::Value paddlePosJson = root["paddle_position"];
    vector<int> paddlePos(paddlePosJson.size());
    for (int i = 0; i < paddlePosJson.size(); i++) {
      paddlePos[i] = paddlePosJson[i].asInt();
    }
    /* todo, Now need to do stuff to actually use this information
    ** in the physics engine to update paddle location
    */

    pongGame->setPaddleDirection(paddleDirection);
    pongGame->setPaddlePos(paddlePos[0], paddlePos[1]);
    /*cout << "Paddle x: " << paddlePos[0] << ", Paddle y: " << paddlePos[1] << endl;
      int x = distribution(generator);
      int y = distribution(generator);*/

    /*ostringstream json;
      json << "{\"phase\":\"ball_update\",\"ball_position\":[";
      json << x << ", " << y << "]}";

      server.wsSend(clientID, json.str());*/
  } else if(phaseString.compare("disconnect") == 0){
    //////// DELETE ME ///////////
    printf("Phase String: disconnect\n");
    ///////////////////////////
    gameObjectsSet = false;
    delete pongGame;
  }else{

  }
}
