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
//#include "ntpTool.h"
#include <cstdlib>
#include <assert.h>

#ifdef __linux__

#include <pthread.h>
#include <unistd.h>

#elif _WIN32
#include <process.h>
#endif

using namespace std;

int serverThread;
pthread_t clientOneSendThread;
pthread_t clientTwoSendThread;
pthread_t clientOneRcvThread;
pthread_t clientTwoRcvThread;
int gameLoopThread;
bool gameObjectsSet;
webSocket server;
pong *pongGame;
int loopCount;
Latency *bufferC1; // buffer for Client 1
Latency *bufferC2; // buffer for Client 2
extern int paddleDistanceThreshold; // how far away ball is from paddle to send message


void Server(int);

void openHandler(int);

void closeHandler(int);

void checkPartnerPresent(int);

void messageHandler(int, string);

void *GameLoop(void *);

void sendBallPosition(int, int &ballx, int &bally);

void sendPaddleUpdate(int, int, int);

void sendScoreUpdate(int);

void incomingBuffer(int clientID, string message); // sends incoming message to recie
bool stopThread(int clientID);// Call when messages should no longer be sent/receieved
//long getNtpTime();


int main(int argc, char *argv[]) {

    //  long ntpTime = getNtpTime();

    int port;
    gameObjectsSet = false;

    //setPongGame(*pongGame);

    cout << "Please set server port: ";
    cin >> port;

    loopCount = 1;
    //messageThread[0] = pthread_t ;
    //messageThread[1] = pthread();


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
    int currBallX, currBallY;
    while (true) {
        if (gameObjectsSet == true) {
            pongGame->update(1 / 60.0);
            vector<int> clientIDs = server.getClientIDs();
            for (int i = 0; i < clientIDs.size(); i++) {
                sendBallPosition(clientIDs[i], currBallX, currBallY);
                sendPaddleUpdate(clientIDs[i], currBallX, currBallY);
            }
                       printf("Total average Latency: %f\n", (bufferC2->getClientLatency() + bufferC1->getClientLatency()) / 2.0);

            if (scoreUpdateCounter % 10 == 0) {
                scoreUpdateCounter = 0;

                 // if(i == bufferC1->getID())
                 //   printf("%d: Total average Latency: %.4g\n",i, bufferC1->getClientLatency());
                 // else
                 //   printf("Total average Latency: %f\n", (bufferC2->getClientLatency() + bufferC			  1->getClientLatency()) / 2.0);
            }

            // Only send score updates sometimes
            if (scoreUpdateCounter % 10 == 0) {

                for (int i = 0; i < clientIDs.size(); i++) {
                    sendScoreUpdate(clientIDs[i]);
                }

            }

            scoreUpdateCounter++;
            usleep(1000000 / 60);
        }
    }
}

// Sends opponent's paddle update to client with clientID
void sendPaddleUpdate(int clientID, int ballx, int bally) {
    // Send Paddle Updates
    Json::FastWriter writer;
    Json::Value jsonToSend;
    Player *curPlayer = pongGame->players[clientID];
    Player *opponent = pongGame->getOpponent(curPlayer);
    int paddle = curPlayer->getPaddleXPosition();
    int threshold = pongGame->boardWidth / 8;
    if (ballx < threshold || ballx > 3 * threshold) {
        //    printf("Sending more paddle updates\n");
        jsonToSend["phase"] = "opponent_paddle_update";
        vector<int> opponentPaddle;
        opponentPaddle = opponent->getPaddlePosition();
        ostringstream oppPaddle;
        oppPaddle << "[" << 984 << "," << opponentPaddle[1] << "]";
        jsonToSend["opponent_paddle"] = oppPaddle.str();

        if (bufferC1->getID() == clientID) {
            bufferC1->sendMessage(clientID, writer.write(jsonToSend), Latency::PADDLE);

        } else {
            bufferC2->sendMessage(clientID, writer.write(jsonToSend), Latency::PADDLE);
        }



    }

}


// Sends score update to client
void sendScoreUpdate(int clientID) {
    Json::FastWriter writer;
    Json::Value jsonToSend;
    Player *curPlayer = pongGame->players[clientID];
    Player *opponent = pongGame->getOpponent(curPlayer);

    // Send scores
    // my scores
    jsonToSend.clear();
    jsonToSend["phase"] = "score_update";
    jsonToSend["new_score"] = curPlayer->getHits();
    jsonToSend["num_tries"] = curPlayer->getTries();
    jsonToSend["opp_new_score"] = opponent->getHits();
    jsonToSend["opp_num_tries"] = opponent->getTries();

    if (pongGame->getPlayerFromClientID(clientID) == pongGame->playerOne) {
        bufferC1->sendMessage(clientID, writer.write(jsonToSend), Latency::SCORE);
    } else {
        bufferC2->sendMessage(clientID, writer.write(jsonToSend), Latency::SCORE);
    }

    // send an update to the client for the latency
    Json::Value request;
    request["phase"] = "request_sync";

    if (clientID == bufferC1->getID()) {
        request["clock_offset"] = bufferC1->getClockOffset();
        bufferC1->sendAdministrativeMessage(clientID, writer.write(request));
    }
    else {
        request["clock_offset"] = bufferC2->getClockOffset();
        bufferC2->sendAdministrativeMessage(clientID, writer.write(request));
    }
    

}

// Sends the ball's position
void sendBallPosition(int clientID, int &outx, int &outy) {
    ostringstream json;
    int x;
    int y;
    pongGame->getBallPosition(clientID, x, y);

    json << "{\"phase\":\"ball_update\",\"ball_position\":[";
    json << x << ", " << y << "]}";
    if (pongGame->getPlayerFromClientID(clientID) == pongGame->playerOne) {
        bufferC1->sendMessage(clientID, json.str(), Latency::BALL);
    } else {
        bufferC2->sendMessage(clientID, json.str(), Latency::BALL);

    }
    //  cout << json.str() << endl;


    outx = x;
    outy = y;
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

        res = pthread_create(&clientOneRcvThread, NULL, &bufferC1->startRcvLoop, bufferC1);
        if (res != 0) {
            printf("WARNING:Message thread failed to create for client %d\n", clientID);
        }
    } else {
        // Start up latency thread for second client
        int res;
        bufferC2 = new Latency(pongGame, &server, clientID);
        res = pthread_create(&clientOneSendThread, NULL, &bufferC1->startSendLoop, bufferC1);
        res += pthread_create(&clientTwoSendThread, NULL, &bufferC2->startSendLoop, bufferC2);
        res += pthread_create(&clientTwoRcvThread, NULL, &bufferC2->startRcvLoop, bufferC2);
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
        bufferC1->sendAdministrativeMessage(clientID, writer.write(jsonToSend));

    } else {
        bufferC2->sendAdministrativeMessage(clientID, writer.write(jsonToSend));
    }

}

/* called when a client disconnects */
void closeHandler(int clientID) {

    stopThread(clientID);
    printf("Client %d disconnected\n", clientID);

}

void messageHandler(int clientID, std::string message) {

    if (clientID == bufferC1->getID())
        bufferC1->receiveMessage(clientID, message);
    else
        bufferC2->receiveMessage(clientID, message);

}

// long getNtpTime() {
// // can be any timing server
// // you might have to change the IP if the server is no longer available
//     char *hostname = (char *) "200.20.186.76";
// // ntp uses port 123
//     int portno = 123;
//     int maxlen = 1024;
//     int i;
// // buffer for the socket request
//     unsigned char msg[48] = {010, 0, 0, 0, 0, 0, 0, 0, 0};
// // buffer for the reply
//     unsigned long buf[maxlen];
// //struct in_addr ipaddr;
//     struct protoent *proto; //
//     struct sockaddr_in server_addr;
//     int s; // socket
//     long tmit; // the time -- This is a time_t sort of

// // open a UDP socket
//     proto = getprotobyname("udp");
//     s = socket(PF_INET, SOCK_DGRAM, proto->p_proto);

// //here you can convert hostname to ipaddress if needed
// //$ipaddr = inet_aton($HOSTNAME);

//     memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = inet_addr(hostname);
//     server_addr.sin_port = htons(portno);

// /*
//  * build a message. Our message is all zeros except for a one in the
//  * protocol version field
//  * msg[] in binary is 00 001 000 00000000
//  * it should be a total of 48 bytes long
// */

// // send the data to the timing server
//     i = sendto(s, msg, sizeof(msg), 0, (struct sockaddr *) &server_addr, sizeof(server_addr));
// // get the data back
//     struct sockaddr saddr;
//     socklen_t saddr_l = sizeof(saddr);
// // here we wait for the reply and fill it into our buffer
//     i = recvfrom(s, buf, 48, 0, &saddr, &saddr_l);

// //We get 12 long words back in Network order

// /*
//  * The high word of transmit time is the 4th word we get back
//  * tmit is the time in seconds not accounting for network delays which
//  * should be way less than a second if this is a local NTP server
//  */

//     tmit = ntohl((time_t) buf[4]); //# get transmit time

//     tmit -= 2208988800U;
//     std::cout << "NTP time is " << ctime(&tmit) << std::endl;
//     i = time(0);
//     std::cout << "System time is " << (i - tmit) << " seconds off" << std::endl;

//     return tmit;
// }

// Call when messages should no longer be sent/receieved
// returns if threads were succesfully stopped
// clientID:  the clientID of the buffer to stop
bool stopThread(int clientID) {
    printf("%d: Main::stopThread\n", clientID);
    int s;
    void *res;
    bool returnVal = true;

    assert(bufferC1 != NULL);
    assert(bufferC2 != NULL);
    // cancel clients
    bufferC1->stopThread();
    bufferC2->stopThread();

    // reap client 1 threads
    s = pthread_join(clientOneRcvThread, &res);
    if (res != 0) {
        printf("WARNING: Joining message thread %d went wrong.\n", clientID);
        returnVal = false;
    }
    s = pthread_join(clientOneSendThread, &res);
    if (res != 0) {
        printf("WARNING: Joining message thread %d went wrong.\n", clientID);
        returnVal = false;
    }
    bufferC1->clearSendBuffer(Latency::BALL);
    bufferC1->clearSendBuffer(Latency::PADDLE);
    bufferC1->clearSendBuffer(Latency::SCORE);
    bufferC1->clearReceiveBuffer();


    // reap client 2 threads
    s = pthread_join(clientTwoRcvThread, &res);
    if (res != 0) {
        printf("WARNING: Joining message thread %d went wrong.\n", clientID);
        returnVal = false;
    }
    s = pthread_join(clientTwoSendThread, &res);
    if (res != 0) {
        printf("WARNING: Joining message thread %d went wrong.\n", clientID);
        returnVal = false;
    }
    bufferC2->clearSendBuffer(Latency::BALL);
    bufferC2->clearSendBuffer(Latency::PADDLE);
    bufferC2->clearSendBuffer(Latency::SCORE);
    bufferC2->clearReceiveBuffer();


    return returnVal;

}



