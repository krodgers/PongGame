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
#include "twoPlayerPong.h"
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

	  //////// DELETE ME ///////////
	  //	  printf("Game Loop: Updating ball position\n");
	  ///////////////////////////


	  vector<int> clientIDs = server.getClientIDs();
	  for (int i = 0; i < clientIDs.size(); i++){
	      ostringstream json;
	      int x;
	      int y;
	      pongGame->getBallPosition(pongGame->getPlayerName(i), x, y);
	      json << "{\"phase\":\"ball_update\",\"ball_position\":[";
          json << x << ", " << y << "]}";
          server.wsSend(clientIDs[i], json.str());
	  }

	  // Only send score updates sometimes
	  if(scoreUpdateCounter % 10 == 0){
	    scoreUpdateCounter = 0;

	    Json::FastWriter writer;
	    Json::Value jsonToSend;
	    for (int i = 0; i < clientIDs.size(); i++){
	      int oppID = clientIDs[pongGame->getOtherPlayersID(clientIDs[i])];

	      // Send scores
	      // my scores
	      jsonToSend.clear();
	      jsonToSend["phase"] = "score_update";
	      jsonToSend["new_score"] = pongGame->getScore(clientIDs[i]);
	      jsonToSend["num_tries"] = pongGame->getTotalTries(clientIDs[i]);
	      jsonToSend["opp_new_score"] = pongGame->getScore(oppID);
	      jsonToSend["opp_num_tries"] = pongGame->getTotalTries(oppID);

	      server.wsSend(clientIDs[i], writer.write(jsonToSend));

	      ////// DELETE ME /////
	      printf("client %d::Scores: score: %d, tries %d, hisScore: %d, hisTries: %d\n", i, pongGame->getScore(clientIDs[i]), pongGame->getTotalTries(clientIDs[i]),pongGame->getScore(oppID),  pongGame->getTotalTries(oppID));



	      server.wsSend(clientIDs[i], writer.write(jsonToSend));

	      jsonToSend.clear();
	      jsonToSend["phase"] = "opponent_paddle_update";
	      vector<int> opponentPaddle;
	      opponentPaddle = pongGame->getPaddlePos(pongGame->getPlayerName(oppID));
	      ostringstream oppPaddle;
        oppPaddle << "[" << 984 << "," << opponentPaddle[1] << "]";
	      jsonToSend["opponent_paddle"] = oppPaddle.str();
	      server.wsSend(clientIDs[i], writer.write(jsonToSend));
	      cout << "Opponnet Paddle: " << oppPaddle.str();

	      ostringstream out;
	      vector<int> paddleOne = pongGame->getPaddlePos(pongGame->getPlayerName(clientIDs[i]));
	      vector<int> paddleTwo = pongGame->getPaddlePos(pongGame->getPlayerName(oppID));
	      out << "Player " << pongGame->getPlayerName(clientIDs[i]) << "'s paddle position: " << "[" << paddleOne[0] << "," << paddleOne[1] << "]" << endl;
	      out << "Player " << pongGame->getPlayerName(oppID) << "'s paddle position: " << "[" << paddleTwo[0] << "," << paddleTwo[1] << "]" << endl;
	      cout << out.str();

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

  vector<int> clientIDs = server.getClientIDs();
  int partnerID = -1;
  for (int i = 0; i < clientIDs.size(); i++){
    if (clientIDs[i] != clientID) {
      partnerID = clientIDs[i];
    }
  }

  if (partnerID == -1)
    pongGame = new pong();


  Json::FastWriter writer;
  Json::Value jsonToSend;

  // Send the opponent name to the first player to connect
  jsonToSend["phase"] = "initialization";
  jsonToSend["player_number"] = clientID;
  server.wsSend(clientID, writer.write(jsonToSend));
  cout << "here" << endl;
}

/* called when a client disconnects */
void closeHandler(int clientID){

  printf("Client %d disconnected\n", clientID);
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){

  //cout << message << endl;


  // Let's parse it
  Json::Value root;
  Json::Reader reader;
  bool parsedSuccess = reader.parse(message,
				    root,
				    false);
  string phaseString = root["phase"].asString();
  string playerName = root["name"].asString();

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


    pongGame->setPlayerName(playerName);
    pongGame->setPlayerID(pongGame->getPlayerNumber(playerName)-1, clientID);

    pongGame->boardWidth = mapDims[2];
    pongGame->boardHeight = mapDims[3];

    if (pongGame->getPlayerNumber(playerName) == 1) {
      pongGame->setPaddlePos(playerName, paddleDims[0], paddleDims[1]);
    }
    else {
      pongGame->setPaddlePos(playerName, 984, paddleDims[1]);
    }
    pongGame->setPaddleDimensions(paddleDims[2], paddleDims[3]);
    pongGame->setBallPos(mapDims[2]/2, mapDims[3]/2);
    pongGame->setBallRadius(10);

    //setMapInfo(mapDims, paddleDims);

    int ballRadius = pongGame->ballradius;

    ostringstream json;
    json << "{\"phase\":\"initial_ball_position\",\"ball_position\":[";
    json << pongGame->ballx << ", " << pongGame->bally << "],";
    json << "\"ball_size\":" << ballRadius << "}";

    server.wsSend(clientID, json.str());

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

      Json::FastWriter writer;
      Json::Value jsonToSend;

      // Send the opponent name to the first player to connect
      jsonToSend["phase"] = "set_opponent";
      jsonToSend["name"] = playerName;
      server.wsSend(partnerID, writer.write(jsonToSend));

      // Send the opponent name to the second player to connect
      jsonToSend.clear();
      jsonToSend["phase"] = "set_opponent";
      jsonToSend["name"] = pongGame->getPlayerName(partnerID);
      server.wsSend(clientID, writer.write(jsonToSend));



      // Tell the first player to connect to start
      server.wsSend(partnerID,"{\"phase\":\"start\"}");
      printf("Client %d told to start\n", partnerID);

      // Tell the second player to connect to start
      server.wsSend(clientID,"{\"phase\":\"start\"}");
      printf("Client %d told to start\n", clientID);


      gameObjectsSet = true; // TODO:: clients need to use same object

    } else {
      // send wait signal
      server.wsSend(clientID, "{\"phase\":\"wait\"}");
      printf("Client %d told to wait\n", clientID);
    }


  } else if (phaseString.compare("paddle_update") == 0) {
    //////// DELETE ME ///////////
    //    printf("Phase String: paddle_update\n");
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

    if (playerName.compare("abe") == 0 && paddlePos[0] == 0) {
      cout << "suuuhhhhweeeeet" << endl;
    }

    if (playerName.compare("Kevin") == 0 && paddlePos[0] == 0) {
      cout << "suuuhhhhweeeeet" << endl;
    }

    pongGame->setPaddleDirection(playerName, paddleDirection);

    if (pongGame->getPlayerNumber(playerName) == 1) {
      pongGame->setPaddlePos(playerName, paddlePos[0], paddlePos[1]);
    }
    else {
      pongGame->setPaddlePos(playerName, 984, paddlePos[1]);
    }

    /*cout << "Paddle x: " << paddlePos[0] << ", Paddle y: " << paddlePos[1] << endl;
      int x = distribution(generator);
      int y = distribution(generator);*/

    /*ostringstream json;
      json << "{\"phase\":\"ball_update\",\"ball_position\":[";
      json << x << ", " << y << "]}";

      server.wsSend(clientID, json.str());*/
  } else if(phaseString.compare("disconnect") == 0){
    // assuming that only 2 clients are going to be allowed to connect
    // send message to partner telling him to disconnect
    vector<int> clientIDs = server.getClientIDs();
    int partnerID = -1;
    for (int i = 0; i < clientIDs.size(); i++){
      if (clientIDs[i] != clientID) {
        partnerID = clientIDs[i];
      }
    }

    Json::FastWriter writer;
    Json::Value jsonToSend;
    if(partnerID != -1 ){
      // has partner
      jsonToSend["phase"] = "disconnected";
      server.wsSend(partnerID, writer.write(jsonToSend));

    } else {
      // has no partner
      //////// DELETE ME ///////////
      printf("Phase String: disconnect\n");
      ///////////////////////////
      gameObjectsSet = false;
      delete pongGame;

    }

  }else{

  }
}
