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

extern bool gameObjectsSet;

#define LATENCY_TIME 50


// Latency Constructors
Latency::Latency(pong *game, webSocket *serverToUse, int id) {
    pongGame = game;
    server = serverToUse;
    latencyTime = LATENCY_TIME;
    ID = id;
    init();


}

Latency::Latency(webSocket *serverToUse, int id) {
    pongGame = NULL;
    server = serverToUse;
    latencyTime = LATENCY_TIME;
    ID = id;
    init();
}

// Frees all the memory objects
Latency::~Latency() {
    if (sendBuffer != NULL)
        delete sendBuffer;
    if (receiveBuffer != NULL)
        delete receiveBuffer;
    if (sendIDs != NULL)
        delete sendIDs;
    if (receiveIDs)
        delete receiveIDs;

}

// Initializes the memory objects
void Latency::init() {
    sendBuffer = new std::queue<std::string>;
    receiveBuffer = new std::queue<std::string>;
    sendIDs = new std::queue<int>();
    receiveIDs = new std::queue<int>();
    sendAndReceive = false;
    messageLock = 0;
    receiveLock = 0;
}


int Latency::getID() {
    return ID;
}

void Latency::setPongGame(pong *game) {
    pongGame = game;
}

// Clears out the send buffer
void Latency::clearSendBuffer() {
    while (messageLock);
    messageLock = 1;
    delete sendBuffer;
    delete sendIDs;
    sendBuffer = new std::queue<std::string>;
    sendIDs = new std::queue<int>;
    messageLock = 0;
}


// Clears out the Receive buffer
void Latency::clearReceiveBuffer() {
    while (receiveLock);
    receiveLock = 1;
    delete receiveBuffer;
    delete receiveIDs;
    receiveBuffer = new std::queue<std::string>;
    receiveIDs = new std::queue<int>;
    receiveLock = 0;
}

// called when server gets message
void Latency::receiveMessage(int clientID, std::string message) {
    printf("%d:RCVMessage\n", ID);
    while (receiveLock);
    receiveLock = 1;
    // add time stamp
    //  std::string thingRcvd = message; // for debugging, should be deleted
    //thingRcvd = addTimestamp(message);
    receiveBuffer->push(message);
    receiveIDs->push(clientID);
    receiveLock = 0;
}

// add message to send queue
// will send after some Latency time
void Latency::sendMessage(int clientID, std::string message) {
    printf("%d:SendMessage\n", ID);

    while (messageLock);
    messageLock = 1;
    sendBuffer->push(message);
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
    printf("Time: %s\n", stamp.str().c_str());
    root["time_stamp"] = stamp.str();

    return writer.write(root);


}

double Latency::getReceiveLatency() {
    return clientLatency;
}

void *Latency::threadWrapperFunction(void *classRef) {
    ((Latency *) classRef)->messageHandlingLoop();
}


// thread routine to grab messages off the buffer and send them
void *Latency::messageHandlingLoop() {
    printf("%d:Thread Starting\n", ID);
    sendAndReceive = true;

    // allow the thread to be cancelled
    int s;
    s = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    if (s != 0)
        printf("WARNING: Unable to set cancellation on message Thread\n");
    // start sending and receiving
    printf("%d: Starting sending rcving loop\n", ID);
    while (sendAndReceive) {
        if (!sendBuffer->empty()) {
            usleep(1000 * latencyTime);
            while (messageLock); // lock on send buffer
            messageLock = 1;
            std::string thingToSend = sendBuffer->front();
            thingToSend = addTimestamp(sendBuffer->front());
            server->wsSend(sendIDs->front(), thingToSend);
            sendIDs->pop();
            sendBuffer->pop();
            messageLock = 0;


        }
        if (!receiveBuffer->empty()) {
            usleep(1000 * latencyTime);
            while (receiveLock); // lock on receive buffer
            receiveLock = 1;
            handleIncomingMessage(receiveIDs->front(), receiveBuffer->front());
            receiveIDs->pop();
            receiveBuffer->pop();
            receiveLock = 0;
        }

    }
}

// Processes the incoming message
void Latency::handleIncomingMessage(int clientID, std::string message) {
    printf("%d: Got message\n", ID);




    // Let's parse it
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(message,
            root,
            false);
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


    clientLatency = currMillis - timeStamp;
    printf("clientLatency: %.4f\n", clientLatency);
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

        //setMapInfo(mapDims, paddleDims);

        int ballRadius = pongGame->ballradius;

        ostringstream json;
        json << "{\"phase\":\"initial_ball_position\",\"ball_position\":[";
        json << pongGame->ballx << ", " << pongGame->bally << "],";
        json << "\"ball_size\":" << ballRadius << "}";

	sendMessage(clientID, json.str());
	//   server->wsSend(clientID, json.str());

    } else if (phaseString.compare("ready_to_start") == 0) {


        // send request for player's information
	sendMessage(clientID,  "{\"phase\":\"send_info\"}");
	//        server->wsSend(clientID, "{\"phase\":\"send_info\"}");
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
        cout << "Client " << clientID << " exchange_info" << endl;


        if (opponent->getAssignedClientID() != -1) {

            Json::FastWriter writer;
            Json::Value jsonToSend;

            // Send the opponent name to the first player to connect
            jsonToSend["phase"] = "set_opponent";
            jsonToSend["name"] = playerName;
	    sendMessage(pongGame->playerOne->getAssignedClientID(), writer.write(jsonToSend));
	    //     server->wsSend(pongGame->playerOne->getAssignedClientID(), writer.write(jsonToSend));

            // Send the opponent name to the second player to connect
            jsonToSend.clear();
            jsonToSend["phase"] = "set_opponent";
            jsonToSend["name"] = opponent->getName();
	    sendMessage(pongGame->playerTwo->getAssignedClientID(), writer.write(jsonToSend));
	    //   server->wsSend(pongGame->playerTwo->getAssignedClientID(), writer.write(jsonToSend));



            // Tell the first player to connect to start
	    sendMessage(pongGame->playerOne->getAssignedClientID(), "{\"phase\":\"start\"}");

	    //       server->wsSend(pongGame->playerOne->getAssignedClientID(), "{\"phase\":\"start\"}");

            // Tell the second player to connect to start
	    sendMessage(pongGame->playerTwo->getAssignedClientID(), "{\"phase\":\"start\"}");
	    // server->wsSend(pongGame->playerTwo->getAssignedClientID(), "{\"phase\":\"start\"}");


            gameObjectsSet = true; // TODO:: clients need to use same object

        } else {
            // send wait signal
	  sendMessage(clientID, "{\"phase\":\"wait\"}");
	  //       server->wsSend(clientID, "{\"phase\":\"wait\"}");
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

        curPlayer->setPaddleDirection(paddleDirection);

        if (curPlayer == pongGame->playerOne) {
            curPlayer->setPaddlePosition(paddlePos[0], paddlePos[1]);
        }
        else {
            curPlayer->setPaddlePosition(984, paddlePos[1]);
        }

        /*cout << "Paddle x: " << paddlePos[0] << ", Paddle y: " << paddlePos[1] << endl;
          int x = distribution(generator);
          int y = distribution(generator);*/

        /*ostringstream json;
          json << "{\"phase\":\"ball_update\",\"ball_position\":[";
          json << x << ", " << y << "]}";

          server->wsSend(clientID, json.str());*/
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
	    sendMessage(partnerID, writer.write(jsonToSend));

	    // server->wsSend(partnerID, writer.write(jsonToSend));

        } else {
            // has no partner
            //////// DELETE ME ///////////
            printf("Phase String: disconnect\n");
            ///////////////////////////
            gameObjectsSet = false;
            delete pongGame;

        }

    } else {

    }
}

void Latency::stopThread() {
    sendAndReceive = false;
}
