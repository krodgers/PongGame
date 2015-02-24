////////////////////////////////////
// Kathryn Rodgers UCID: 39483825 //
// Kevin Malby UCID: 36928917	  //
////////////////////////////////////

#include <time.h>
#include <unistd.h>
#include "latency.h"

extern bool gameObjectsSet;

#define LATENCY_TIME 3



// Latency Constructors
Latency::Latency(pong* game, webSocket* serverToUse, int id){
pongGame = game;
server = serverToUse;
latencyTime = LATENCY_TIME;
ID = id;
init();


}

Latency::Latency( webSocket* serverToUse, int id){
pongGame = NULL;
server = serverToUse;
latencyTime = LATENCY_TIME;
ID = id;
init();
}

// Frees all the memory objects
Latency::~Latency(){
if(sendBuffer != NULL)
  delete sendBuffer;
if(receiveBuffer != NULL)
  delete receiveBuffer;
if(sendIDs != NULL)
  delete sendIDs;
if(receiveIDs)
  delete receiveIDs;
  
}

// Initializes the memory objects
void Latency::init(){
sendBuffer = new std::stack<std::string>;
receiveBuffer = new std::stack<std::string>;
sendIDs = new std::stack<int>();
receiveIDs = new std::stack<int>();
sendAndReceive = false;
messageLock = 0;
receiveLock = 0; 
}


int Latency::getID(){
  return ID;
}

void Latency::setPongGame(pong* game){
pongGame = game;
}

// Clears out the send buffer
void Latency::clearSendBuffer(){
  while(messageLock);
  messageLock = 1;
  delete sendBuffer;
  delete sendIDs;
  sendBuffer = new std::stack<std::string>;
  sendIDs = new std::stack<int>;
  messageLock = 0;
}


// Clears out the Receive buffer
void Latency::clearReceiveBuffer(){
  while(receiveLock);
  receiveLock = 1;
  delete receiveBuffer;
  delete receiveIDs;
  receiveBuffer = new std::stack<std::string>;
  receiveIDs = new std::stack<int>;
  receiveLock = 0;
}

// called when server gets message
void Latency::receiveMessage(int clientID, std::string message){
while(receiveLock);
receiveLock = 1;
receiveBuffer->push(message);
receiveIDs->push(clientID);
receiveLock = 0;
}
// add message to send queue
// will send after some Latency time
void Latency::sendMessage(int clientID, std::string message ) {
while(messageLock);
messageLock = 1;
sendBuffer->push(message);
sendIDs->push(clientID);
messageLock = 0;
}

void* Latency::threadWrapperFunction(void* classRef){
((Latency*)classRef)->messageHandlingLoop();
}


bool Latency::startMessageLoop(){
//   pthread_t id;
// messageThread = pthread_create(&id, NULL, &Latency::threadWrapperFunction, &this);
//   sendAndReceive = true;
  
//   if(messageThread != 0) {
//     printf("Error: pthread_create() failed\n");
//     pthread_exit(NULL);
//     return false;
//   }
return true;
}

// thread routine to grab messages off the buffer and send them
void* Latency::messageHandlingLoop(){
// allow the thread to be cancelled
int s; 
s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
if (s != 0)
  printf("WARNING: Unable to set cancellation on message Thread\n");
// start sending and receiving
while(sendAndReceive){
if(!sendBuffer->empty()){
usleep(1000*latencyTime);
while(messageLock); // lock on send buffer
messageLock = 1;
server->wsSend(sendIDs->top(), sendBuffer->top());
sendIDs->pop();
sendBuffer->pop();
messageLock = 0;

}
if(!receiveBuffer->empty()){
usleep(1000*latencyTime);
while(receiveLock); // lock on receive buffer
receiveLock = 1;
handleIncomingMessage(receiveIDs->top(), receiveBuffer->top());
receiveIDs->pop();
receiveBuffer->pop();
receiveLock = 0;
}

}
}

// Processes the incoming message
void Latency::handleIncomingMessage(int clientID, std::string message){

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

std::ostringstream json;
json << "{\"phase\":\"initial_ball_position\",\"ball_position\":[";
json << pongGame->ballx << ", " << pongGame->bally << "],";
json << "\"ball_size\":" << ballRadius << "}";

server->wsSend(clientID, json.str());

} else if (phaseString.compare("ready_to_start") == 0) {
cout << "Client " << clientID << " ready_to_start" << endl;
  
// send request for player's information
server->wsSend(clientID, "{\"phase\":\"send_info\"}");
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
  vector<int> clientIDs = server->getClientIDs();
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
    server->wsSend(partnerID, writer.write(jsonToSend));

    // Send the opponent name to the second player to connect
    jsonToSend.clear();
    jsonToSend["phase"] = "set_opponent";
    jsonToSend["name"] = pongGame->getPlayerName(partnerID);
    server->wsSend(clientID, writer.write(jsonToSend));



    // Tell the first player to connect to start
    server->wsSend(partnerID,"{\"phase\":\"start\"}");
    printf("Client %d told to start\n", partnerID);

    // Tell the second player to connect to start
    server->wsSend(clientID,"{\"phase\":\"start\"}");
    printf("Client %d told to start\n", clientID);


    gameObjectsSet = true; // TODO:: clients need to use same object

  } else {
    // send wait signal
    server->wsSend(clientID, "{\"phase\":\"wait\"}");
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

    server->wsSend(clientID, json.str());*/
 } else if(phaseString.compare("disconnect") == 0){
  // assuming that only 2 clients are going to be allowed to connect
  // send message to partner telling him to disconnect
  vector<int> clientIDs = server->getClientIDs();
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
    server->wsSend(partnerID, writer.write(jsonToSend));

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
