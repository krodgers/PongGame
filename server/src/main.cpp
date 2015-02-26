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
#include "latency.h"
#include <cstdlib>

#ifdef __linux__

#include <pthread.h>
#include <unistd.h>

#elif _WIN32
#include <process.h>
#endif

using namespace std;

int serverThread;
pthread_t messageThread[2];
int gameLoopThread;
bool gameObjectsSet;
webSocket server;
pong *pongGame;
int loopCount;
Latency *bufferC1; // buffer for Client 1
Latency *bufferC2; // buffer for Client 2


void Server(int);

void openHandler(int);

void closeHandler(int);

void checkPartnerPresent(int);

void messageHandler(int, string);

void *GameLoop(void *);

void incomingBuffer(int clientID, string message); // sends incoming message to recie
bool stopThread(int clientID);// Call when messages should no longer be sent/receieved



int main(int argc, char *argv[]) {
    int port;
    gameObjectsSet = false;

    //setPongGame(*pongGame);

    cout << "Please set server port: ";
    cin >> port;

    loopCount = 1;
    // messageThread[0] = pthread_t ;
    // messageThread[1] = pthread();


#ifdef __linux__
    pthread_t id;
    void *voidStar = &port;
    serverThread = pthread_create(&id, NULL, GameLoop, voidStar);

#elif _WIN32
  _beginthread( Server, 0, (void*)port );
#endif

    Server(port);


}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void *GameLoop(void *arg) {
    int scoreUpdateCounter = 0;
    while (true) {
        if (gameObjectsSet == true) {
            //  cout << "here" << endl;
            pongGame->update(1 / 60.0);

            vector<int> clientIDs = server.getClientIDs();
            for (int i = 0; i < clientIDs.size(); i++) {
                ostringstream json;
                int x;
                int y;
                pongGame->getBallPosition((pongGame->getPlayerFromClientID(clientIDs[i]))->getName(), x, y);
                json << "{\"phase\":\"ball_update\",\"ball_position\":[";
                json << x << ", " << y << "]}";
                if (pongGame->getPlayerFromClientID(clientIDs[i]) == pongGame->playerOne) {
                    bufferC1->sendMessage(clientIDs[i], json.str());
                    //    server.wsSend(clientIDs[i], json.str());
                } else {
                    bufferC2->sendMessage(clientIDs[i], json.str());

                }
		// Send Paddle Updates
		Json::FastWriter writer;
                Json::Value jsonToSend;
		Player *curPlayer = pongGame->players[clientIDs[i]];
		Player *opponent = pongGame->getOpponent(curPlayer);
		jsonToSend["phase"] = "opponent_paddle_update";
		vector<int> opponentPaddle;
		opponentPaddle = opponent->getPaddlePosition();
		ostringstream oppPaddle;
		oppPaddle << "[" << 984 << "," << opponentPaddle[1] << "]";
		jsonToSend["opponent_paddle"] = oppPaddle.str();
		
		//server.wsSend(clientIDs[i], writer.write(jsonToSend));
		if (pongGame->getPlayerFromClientID(clientIDs[i]) == pongGame->playerOne) {
		  bufferC1->sendMessage(clientIDs[i], writer.write(jsonToSend));
		  
		} else {
		  bufferC2->sendMessage(clientIDs[i], writer.write(jsonToSend));
		}
		
		
            }

            // Only send score updates sometimes
            if (scoreUpdateCounter % 10 == 0) {
                scoreUpdateCounter = 0;
                Json::FastWriter writer;
                Json::Value jsonToSend;
                for (int i = 0; i < clientIDs.size(); i++) {
                    Player *curPlayer = pongGame->players[clientIDs[i]];
                    Player *opponent = pongGame->getOpponent(curPlayer);

                    // Send scores
                    // my scores
                    jsonToSend.clear();
                    jsonToSend["phase"] = "score_update";
                    jsonToSend["new_score"] = curPlayer->getHits();
                    jsonToSend["num_tries"] = curPlayer->getTries();
                    jsonToSend["opp_new_score"] = opponent->getHits();
                    jsonToSend["opp_num_tries"] = opponent->getTries();

                    if (pongGame->getPlayerFromClientID(clientIDs[i]) == pongGame->playerOne) {
		      bufferC1->sendMessage(clientIDs[i], writer.write(jsonToSend));
                    } else {
                        bufferC2->sendMessage(clientIDs[i], writer.write(jsonToSend));
                    }
                }

            }
            scoreUpdateCounter++;
            usleep(1000000 / 60);
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

void openHandler(int clientID) {
    //////// DELETE ME ///////////
    printf("Open Handler: Client %d connecting\n", clientID);
    ///////////////////////////

    vector<int> clientIDs = server.getClientIDs();
    int partnerID = -1;
    for (int i = 0; i < clientIDs.size(); i++) {
        if (clientIDs[i] != clientID) {
            partnerID = clientIDs[i];
        }
    }


    if (partnerID == -1) {
        pongGame = new pong();
        // Start up latency thread for the first client
        int res;
        bufferC1 = new Latency(pongGame, &server, clientID);
        res = pthread_create(&messageThread[0], NULL, &bufferC1->threadWrapperFunction, bufferC1);
        if (res != 0) {
            printf("WARNING:Message thread failed to create for client %d\n", clientID);
        }
    } else {
        // Start up latency thread for second client
        int res;
        bufferC2 = new Latency(pongGame, &server, clientID);
        res = pthread_create(&messageThread[1], NULL, &bufferC2->threadWrapperFunction, bufferC2);
        if (res != 0) {
            printf("WARNING:Message thread failed to create for client %d\n", clientID);
        }

    }

    for (int i = 0; i < clientIDs.size(); i++) {
        if (pongGame->playerOne->getAssignedClientID() == -1)
            pongGame->playerOne->setAssignedClientID(clientIDs[i]);
        else
            pongGame->playerTwo->setAssignedClientID(clientIDs[i]);
    }

    Json::FastWriter writer;
    Json::Value jsonToSend;

    // Send the opponent name to the first player to connect
    jsonToSend["phase"] = "initialization";
    jsonToSend["player_number"] = clientID;


    //  server.wsSend(clientID, writer.write(jsonToSend));
    if (pongGame->getPlayerFromClientID(clientID) == pongGame->playerOne) {
        bufferC1->sendMessage(clientID, writer.write(jsonToSend));

    } else {
        bufferC2->sendMessage(clientID, writer.write(jsonToSend));
    }

}

/* called when a client disconnects */
void closeHandler(int clientID) {

    stopThread(clientID);
    printf("Client %d disconnected\n", clientID);

}

void messageHandler(int clientID, std::string message) {

    if (pongGame->getPlayerFromClientID(clientID) == pongGame->playerOne)
        bufferC1->receiveMessage(clientID, message);
    else
        bufferC2->receiveMessage(clientID, message);

}

// Call when messages should no longer be sent/receieved
// returns if threads were succesfully stopped
// clientID:  the clientID of the buffer to stop
bool stopThread(int clientID) {
    int s;
    void *res;
    bool returnVal = true;

    pthread_t threadToCancel = clientID = bufferC1->getID() ? messageThread[0] : messageThread[1];

    s = pthread_cancel(threadToCancel);
    if (s != 0) {
        printf("WARNING: Unable to cancel messageThread\n");
        returnVal = false;
    }
    s = pthread_join(threadToCancel, &res);
    if (res != 0) {
        printf("WARNING: Joining message thread %d went wrong.\n", clientID);
        returnVal = false;
    }

    return returnVal;

}



