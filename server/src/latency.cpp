////////////////////////////////////
// Kathryn Rodgers UCID: 39483825 //
// Kevin Malby UCID: 36928917	  //
////////////////////////////////////

#ifdef __linux__

#include <sys/time.h>

#endif

#include <unistd.h>
#include <stdio.h>
#include "latency.h"
#include <assert.h>

extern bool gameObjectsSet;

#define LATENCY_TIME_MIN 50
#define LATENCY_TIME_MAX 100
#define MAX_ALLOWED_DELAY 1500


// Latency Constructors
Latency::Latency(pong *game, webSocket *serverToUse, int id) {
     pongGame = game;
     server = serverToUse;
     latencyTimeMin = LATENCY_TIME_MIN;
     latencyTimeMax = LATENCY_TIME_MAX;
     ID = id;
     srand(time(NULL));
     init();
}

Latency::Latency(webSocket *serverToUse, int id) {
     pongGame = NULL;
     server = serverToUse;
     latencyTimeMin = LATENCY_TIME_MIN;
     latencyTimeMax = LATENCY_TIME_MAX;
     ID = id;
     srand(time(NULL));
     init();
}

// Frees all the memory objects
Latency::~Latency() {
     if (sendBallBuffer != NULL)
	  delete sendBallBuffer;
     if (sendPaddleBuffer != NULL)
	  delete sendPaddleBuffer;
     if (sendScoreBuffer != NULL)
	  delete sendScoreBuffer;
     if (receiveBuffer != NULL)
	  delete receiveBuffer;
     if (sendBallIDs != NULL)
	  delete sendBallIDs;
     if (sendPaddleIDs != NULL)
	  delete sendPaddleIDs;
     if (sendScoreIDs != NULL)
	  delete sendScoreIDs;
     if (receiveIDs)
	  delete receiveIDs;

}

// Initializes the memory objects
void Latency::init() {
     sendBallBuffer = new std::queue<std::string>;
     sendPaddleBuffer = new std::queue<std::string>;
     sendScoreBuffer = new std::queue<std::string>;
     receiveBuffer = new std::queue<std::string>;
     sendBallIDs = new std::queue<int>;
     sendPaddleIDs = new std::queue<int>;
     sendScoreIDs = new std::queue<int>;
     receiveIDs = new std::queue<int>;
     sendAndReceive = false;
     messageLock = 0;
     clientLatency = 0;
     receiveLock = 0;
     totalNumPackets = 0;
}


int Latency::getID() {
     return ID;
}

void Latency::setPongGame(pong *game) {
     pongGame = game;
}

// Clears out the send buffer
void Latency::clearSendBuffer(PacketType type) {
     switch (type) {
     case BALL:
	  clearSendBallBuffer();
	  break;
     case PADDLE:
	  clearSendPaddleBuffer();
	  break;
     case SCORE:
	  clearSendScoreBuffer();
	  break;
     default:
	  return;
     }
}

// Clears out the send ball buffer
void Latency::clearSendBallBuffer() {
     while (messageLock);
     messageLock = 1;
     printf("%d:clearSendBallBuffer\n", ID);
     //  delete sendBallBuffer;
     // delete sendBallIDs;
     try {
	  // sendBallBuffer = new std::queue<std::string>;
	  //sendBallIDs = new std::queue<int>;
      
	  std::queue<std::string> *newQ = new std::queue<std::string>;
	  std::queue<int> *newInt = new std::queue<int>;
	  std::swap(*sendBallBuffer, *newQ);
	  std::swap(*sendBallIDs, *newInt);

	  assert(sendBallIDs->size() == 0);
	  assert(sendBallBuffer->size() == 0);

     } catch (const std::exception &except) {
	  printf("Caught bad_alloc in Latency::clearSendBallBuffer\n");
     }
     printf("successfully cleared ball buffer\n");
     messageLock = 0;
}

// Clears out the send paddle buffer
void Latency::clearSendPaddleBuffer() {
     while (messageLock);
     messageLock = 1;
     printf("%d:clearSendPaddleBuffer\n", ID);
     //    delete sendPaddleBuffer;
     //delete sendPaddleIDs;
     //sendPaddleBuffer = new std::queue<std::string>;
     //sendPaddleIDs = new std::queue<int>;

     std::queue<std::string> *newQ = new std::queue<std::string>;
     std::queue<int> *newInt = new std::queue<int>;
     std::swap(*sendPaddleBuffer, *newQ);
     std::swap(*sendPaddleIDs, *newInt);

     messageLock = 0;
}

// Clears out the send score buffer
void Latency::clearSendScoreBuffer() {
     while (messageLock);
     messageLock = 1;
     printf("%d:clearSendScoreBuffer\n", ID);
     try {

	  //      delete sendScoreBuffer;
	  //delete sendScoreIDs;
	  //sendScoreBuffer = new std::queue<std::string>;
	  //sendScoreIDs = new std::queue<int>;
	  std::queue<std::string> *newQ = new std::queue<std::string>;
	  std::queue<int> *newInt = new std::queue<int>;
	  std::swap(*sendScoreBuffer, *newQ);
	  std::swap(*sendScoreIDs, *newInt);      


     } catch (const std::exception &except) {
	  printf("Caught bad_alloc in Latency::clearSendScoreBuffer()\n");
     }

     messageLock = 0;
}


// Clears out the Receive buffer
void Latency::clearReceiveBuffer() {
     while (receiveLock);
     receiveLock = 1;
     printf("%d:clearRcvBuffer\n", ID);

     delete receiveBuffer;
     delete receiveIDs;
     printf("clearRcvBuffer\n");
     receiveBuffer = new std::queue<std::string>;
     receiveIDs = new std::queue<int>;

     receiveLock = 0;
}

// called when server gets message
void Latency::receiveMessage(int clientID, std::string message) {
     while (receiveLock);
     receiveLock = 1;

     receiveBuffer->push(message);
     receiveIDs->push(clientID);
     receiveLock = 0;
}

queue<std::string> *Latency::getSendBuffer(PacketType type) {
     switch (type) {
     case BALL:
	  return sendBallBuffer;
	  break;
     case PADDLE:
	  return sendPaddleBuffer;
	  break;
     case SCORE:
	  return sendScoreBuffer;
	  break;
     default:
	  return sendBallBuffer;
     }
}

queue<int> *Latency::getSendIDs(PacketType type) {
     switch (type) {
     case BALL:
	  return sendBallIDs;
	  break;
     case PADDLE:
	  return sendPaddleIDs;
	  break;
     case SCORE:
	  return sendScoreIDs;
	  break;
     default:
	  return sendBallIDs;
     }
}

// add message to send queue
// will send after some Latency time
void Latency::sendMessage(int clientID, std::string message, PacketType type) {
     while (messageLock);
     messageLock = 1;
     queue<std::string> *sendBuffer = getSendBuffer(type);
     queue<int> *sendIDs = getSendIDs(type);
     int buffsize = sendBuffer->size();
     /* Do stuff for checking max buffer size and whatnot */
//    cout << "Send buffer size in send message: " << sendBuffer->size() << endl;
//    cout << "game objects set: " << gameObjectsSet << endl;
//    cout << "condtion for buffer too large is: " << (sendBuffer->size() > (double) MAX_ALLOWED_DELAY / (double) (pongGame->getPlayerFromClientID(this->ID))->getAverageLatency()) << endl;
     if (gameObjectsSet && buffsize  > (double) MAX_ALLOWED_DELAY / (double) (pongGame->getPlayerFromClientID(this->ID))->getAverageLatency()) {
	  messageLock = 0; // unlock so clearing buffer can lock
	  cout << "buffer too large" << endl;
	  this->clearSendBuffer(type);
	  //	messageLock = 0; // move lock out to here
     } else {
	  messageLock = 0; // unlock to avoid deadlock

     }
     while (messageLock);
     messageLock = 1;
     sendBuffer = getSendBuffer(type);
     sendIDs = getSendIDs(type);
     //    cout << "Pushing a packet" << endl;
     sendBuffer->push(addTimestamp(message));
     sendIDs->push(clientID);
     messageLock = 0;
}

// adds timestamp to message and returns stamped message
// added bit looks like "time_stamp:hr,min,sec,millisec"
std::string Latency::addTimestamp(std::string message) {
     // Let's parse it
     Json::Value root;
     Json::Reader reader;
     Json::FastWriter writer;

     bool parsedSuccess = reader.parse(message, root, false);
     if (!parsedSuccess)
	  printf("WARNING: %d- Unsucessful parse when adding time stamp.\n", ID);

     // Get milliseconds
     struct timeval tv;

     gettimeofday(&tv, NULL);

     unsigned long long millis =
	  (unsigned long long) (tv.tv_sec) * 1000 +
	  (unsigned long long) (tv.tv_usec) / 1000;
     ostringstream stamp;
     stamp << millis;
     root["time_stamp"] = stamp.str();

     return writer.write(root);
}

double Latency::getReceiveLatency() {
     return clientLatency;
}

void *Latency::startSendLoop(void *classRef) {
     ((Latency *) classRef)->sendingLoop();
}

void *Latency::startRcvLoop(void *classRef) {
     ((Latency *) classRef)->messageReceivingLoop();

}

void Latency::sendBufferMessage() {
     while (messageLock); // lock on send buffer
     messageLock = 1;
  
     if (sendBallBuffer->size() > 0 || sendPaddleBuffer->size() > 0 || sendScoreBuffer->size() > 0) {
	  
	  int r = rand() % ((latencyTimeMax + 1) - latencyTimeMin) + latencyTimeMin;
	  Player *curPlayer = pongGame->getPlayerFromClientID(this->ID);
	  Player *opponent = pongGame->getOpponent(curPlayer);
	  if (totalNumPackets > 20 && opponent->getAverageLatency() > curPlayer->getAverageLatency()) {
	       //cout << "Difference between " << curPlayer->getName() << "'s latency and " << opponent->getName() <<
	       //        "'s latency: " << opponent->getAverageLatency() - curPlayer->getAverageLatency() << endl;
	       r += opponent->getAverageLatency() - curPlayer->getAverageLatency();
	  }
	  messageLock = 0;

	  // Get all the info to send
	  while(messageLock);
	  messageLock = 1;
	  std::string ballString = "", paddleString = "", scoreString = "";
	  int ballID = -1, scoreID = -1, paddleID = -1;

	  if(sendBallBuffer->size() > 0){
	       ballString = sendBallBuffer->front();
	       ballID = sendBallIDs->front();
	       sendBallIDs->pop();
	       sendBallBuffer->pop();
	    
	  }
	  if(sendPaddleBuffer->size() > 0){
	       paddleString = sendPaddleBuffer->front();
	       paddleID = sendPaddleIDs->front(); 
	       sendPaddleIDs->pop();
	       sendPaddleBuffer->pop();
	  }
	  if(sendScoreBuffer->size() > 0){
	       scoreString = sendScoreBuffer->front();
	       scoreID = sendScoreIDs->front();
	       sendScoreIDs->pop();
	       sendScoreBuffer->pop();
	  }
     

	  messageLock = 0;

	  // Send all of the info
	  usleep(1000*r);
	  if(ballID != -1)
	       server->wsSend(ballID, ballString);
	  usleep(1000*r);
	  if(paddleID != -1)
	       server->wsSend(paddleID, paddleString);
	  usleep(1000*r);
	  if(scoreID != -1)
	       server->wsSend(scoreID, scoreString);


     }

     // 	  //cout << "Send buffer size: " << sendBuffer->size() << endl;
     // 	  while (messageLock);
     // 	  messageLock = 1;
     // 	  if (sendBallBuffer->size() > 0) {
     // 	       std::string thingToSend = sendBallBuffer->front();
     // 	       int clientID = sendBallIDs->front();
     // 	       sendBallIDs->pop();
     // 	       sendBallBuffer->pop();
     // 	       messageLock = 0;
     // 	       usleep(1000 * r);
     // 	       server->wsSend(sendBallIDs->front(), sendBallBuffer->front());
     // 	  } else {
     // 	       messageLock = 0;
     // 	  }

     // 	  while (messageLock);
     // 	  messageLock = 1;
     // 	  if (sendPaddleBuffer->size() > 0) {
     // 	       std::string thingToSend = sendPaddleBuffer->front();
     // 	       int clientID = sendPaddleIDs->front();
     // 	       sendPaddleIDs->pop();
     // 	       sendPaddleBuffer->pop();
     // 	       messageLock = 0;
     // 	       usleep(1000 * r);
     // 	       server->wsSend(sendPaddleIDs->front(), sendPaddleBuffer->front());
     // 	  }



     // 	  if (sendScoreBuffer->size() > 0) {
     // 	       while (messageLock);
     // 	       messageLock = 1;
     // 	       std::string thingToSend = sendScoreBuffer->front();
     // 	       int clientID = sendScoreIDs->front();
     // 	       sendScoreIDs->pop();
     // 	       sendScoreBuffer->pop();
     // 	       messageLock = 0;
     // 	       usleep(1000 * r);
     // 	       server->wsSend(sendScoreIDs->front(), sendScoreBuffer->front());
     // 	  }

     // }
     else
	  messageLock = 0;

}

void Latency::sendAdministrativeMessage(int clientID, string message) {
     int r = rand() % ((latencyTimeMax + 1) - latencyTimeMin) + latencyTimeMin;
     string lateMessage = addTimestamp(message);
     usleep(1000 * r);
     server->wsSend(clientID, lateMessage);

}

// thread routine to grab messages off the buffer and send them
void *Latency::sendingLoop() {
     sendAndReceive = true;
     int s;
     s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
     if (s != 0)
	  printf("WARNING: Unable to set cancellation on message Thread\n");
     // start sending and receiving
     while (sendAndReceive) {
	  sendBufferMessage();
     }
}


// thread routine to grab messages off the buffer and send them
void *Latency::messageReceivingLoop() {
     sendAndReceive = true;
     int countS, countR = 0; // DELETE ME
     // allow the thread to be cancelled
     int s;
     s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
     if (s != 0)
	  printf("WARNING: Unable to set cancellation on message Thread\n");
     // start sending and receiving
     while (sendAndReceive) {
	  if (!receiveBuffer->empty()) {
	       int r = rand() % ((latencyTimeMax + 1) - latencyTimeMin) + latencyTimeMin;
	       usleep(1000 * r);
	       while (receiveLock); // lock on receive buffer

	       receiveLock = 1;
	       handleIncomingMessage(receiveIDs->front(), receiveBuffer->front());
	       //  cout << "Recv buffer size: " << receiveBuffer->size() << endl;
	       receiveIDs->pop();
	       receiveBuffer->pop();
	       if (countR > 100) {
		    countR = 0;
		    printf("%d: Receive buffer size: %d\n", ID, (int) receiveBuffer->size());
	       }
	       receiveLock = 0;

	       countR++;
	  }

     }
}

// Processes the incoming message
void Latency::handleIncomingMessage(int clientID, std::string message) {
     // Let's parse it
     Json::Value root;
     Json::Reader reader;
     bool parsedSuccess = reader.parse(message, root, false);
     string phaseString = root["phase"].asString();
     string playerName = root["name"].asString();

     Player *curPlayer = pongGame->getPlayerFromClientID(clientID);
     Player *opponent = pongGame->getOpponent(curPlayer);

     long long timeStamp = root["time_stamp"].asInt64();
     struct timeval tv;
     gettimeofday(&tv, NULL);
     unsigned long long currMillis =
	  (unsigned long long) (tv.tv_sec) * 1000 +
	  (unsigned long long) (tv.tv_usec) / 1000;


     clientLatency += (currMillis - timeStamp);
     totalNumPackets += 1;
     averageClientLatency = clientLatency / totalNumPackets;
     (pongGame->getPlayerFromClientID(this->ID))->setAverageLatency(averageClientLatency);
     printf("Client %d Packet Latency: %d\n", ID, currMillis - timeStamp);
     printf("Total average Latency: %.4g\n", averageClientLatency);
     if (phaseString.compare("initial_dimensions") == 0) {

	  const Json::Value mapDimJson = root["map_dimensions"];
	  const Json::Value paddleDimJson = root["paddle_dimensions"];

	  vector<int> mapDims(mapDimJson.size());
	  vector<int> paddleDims(paddleDimJson.size());
	  for (int i = 0; i < mapDimJson.size(); i++) {
	       mapDims[i] = mapDimJson[i].asInt();
	  }

	  for (int i = 0; i < paddleDimJson.size(); i++) {
	       paddleDims[i] = paddleDimJson[i].asInt();
	  }


	  curPlayer->setName(playerName);

	  pongGame->boardWidth = mapDims[2];
	  pongGame->boardHeight = mapDims[3];

	  if (curPlayer == pongGame->playerOne) {
	       curPlayer->setPaddlePosition(paddleDims[0], paddleDims[1]);
	  }
	  else {
	       curPlayer->setPaddlePosition(984, paddleDims[1]);
	  }
	  curPlayer->setPaddleDimensions(paddleDims[3], paddleDims[2]);
	  pongGame->setBallPos(mapDims[2] / 2, mapDims[3] / 2);
	  pongGame->setBallRadius(10);

	  int ballRadius = pongGame->ballradius;

	  ostringstream json;
	  json << "{\"phase\":\"initial_ball_position\",\"ball_position\":[";
	  json << pongGame->ballx << ", " << pongGame->bally << "],";
	  json << "\"ball_size\":" << ballRadius << "}";

	  sendAdministrativeMessage(clientID, json.str());

     } else if (phaseString.compare("ready_to_start") == 0) {
	  cout << "Client " << clientID << " ready_to_start" << endl;

	  // send request for player's information
	  sendAdministrativeMessage(clientID, "{\"phase\":\"send_info\"}");
	  const Json::Value ballPosJson = root["ball_position"];
	  vector<int> ballPos(ballPosJson.size());

	  for (int i = 0; i < ballPosJson.size(); i++) {
	       ballPos[i] = ballPosJson[i].asInt();
	  }

	  if (ballPos[0] != pongGame->ballx || ballPos[1] != pongGame->bally) {
	       // todo, do something to remedy situation where ball
	       // position is not correct
	  } else {
	  }


     } else if (phaseString.compare("exchange_info") == 0) {
	  if (opponent->getAssignedClientID() != -1) {

	       Json::FastWriter writer;
	       Json::Value jsonToSend;

	       // Send the opponent name to the first player to connect
	       jsonToSend["phase"] = "set_opponent";
	       jsonToSend["name"] = playerName;
	       sendAdministrativeMessage(pongGame->playerOne->getAssignedClientID(), writer.write(jsonToSend));
	       // Send the opponent name to the second player to connect
	       jsonToSend.clear();
	       jsonToSend["phase"] = "set_opponent";
	       jsonToSend["name"] = opponent->getName();
	       sendAdministrativeMessage(pongGame->playerTwo->getAssignedClientID(), writer.write(jsonToSend));

	       // Tell the first player to connect to start
	       sendAdministrativeMessage(pongGame->playerOne->getAssignedClientID(), "{\"phase\":\"start\"}");

	       // Tell the second player to connect to start
	       sendAdministrativeMessage(pongGame->playerTwo->getAssignedClientID(), "{\"phase\":\"start\"}");
	       gameObjectsSet = true; // TODO:: clients need to use same object

	  } else {
	       // send wait signal
	       sendAdministrativeMessage(clientID, "{\"phase\":\"wait\"}");
	  }


     } else if (phaseString.compare("paddle_update") == 0) {
	  int paddleDirection = root["paddle_direction"].asInt();
	  const Json::Value paddlePosJson = root["paddle_position"];
	  vector<int> paddlePos(paddlePosJson.size());
	  for (int i = 0; i < paddlePosJson.size(); i++) {
	       paddlePos[i] = paddlePosJson[i].asInt();
	  }
	  /* todo, Now need to do stuff to actually use this information
	  ** in the physics engine to update paddle location
	  */

	  curPlayer->setPaddleDirection(paddleDirection);

	  if (curPlayer == pongGame->playerOne) {
	       curPlayer->setPaddlePosition(paddlePos[0], paddlePos[1]);
	  }
	  else {
	       curPlayer->setPaddlePosition(984, paddlePos[1]);
	  }

     } else if (phaseString.compare("disconnect") == 0) {
	  // assuming that only 2 clients are going to be allowed to connect
	  // send message to partner telling him to disconnect
	  vector<int> clientIDs = server->getClientIDs();
	  int partnerID = -1;
	  for (int i = 0; i < clientIDs.size(); i++) {
	       if (clientIDs[i] != clientID) {
		    partnerID = clientIDs[i];
	       }
	  }

	  Json::FastWriter writer;
	  Json::Value jsonToSend;
	  if (partnerID != -1) {
	       // has partner
	       jsonToSend["phase"] = "disconnected";
	       sendAdministrativeMessage(partnerID, writer.write(jsonToSend));

	  }
	  gameObjectsSet = false;
     } else {

     }

}

void Latency::stopThread() {
     printf("%d: Latency::stopThread\n", ID);
     sendAndReceive = false;
}

void Latency::printBufferInfo() {
     cout << "Buffers for client: " << this->ID << endl;
     cout << "---------------------" << endl;

     cout << "Ball buffer:" << endl;
     cout << "\taddress: " << &sendBallBuffer << endl;
     cout << "\tsize: " << sendBallBuffer->size() << endl;

     cout << "Paddle buffer:" << endl;
     cout << "\taddress: " << &sendPaddleBuffer << endl;
     cout << "\tsize: " << sendPaddleBuffer->size() << endl;

     cout << "Score buffer:" << endl;
     cout << "\taddress: " << &sendScoreBuffer << endl;
     cout << "\tsize: " << sendScoreBuffer->size() << endl;
}
