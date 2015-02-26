////////////////////////////////////
// Kathryn Rodgers UCID: 39483825 //
// Kevin Malby UCID: 36928917	  //
////////////////////////////////////
#include <json/config.h>
#include <json/value.h>
#include <json/writer.h>
#include <json/json.h>
#include <sstream>
#include <pthread.h>
#include <queue>

#include "twoPlayerPong.h"
#include "websocket.h"

#ifndef __latency_h_
#define __latency_h_


class Latency {
private:
    friend class latencyTester;

    int ID; // to identify which client this buffer is associated with
    int totalNumPackets;
    std::queue<std::string> *sendBuffer;
    std::queue<int> *sendIDs;
    std::queue<int> *receiveIDs;
    std::queue<std::string> *receiveBuffer;
    int latencyTimeMax; // in millisecond
    int latencyTimeMin; // in millisecond
    pong *pongGame;
    webSocket *server;
    bool sendAndReceive;
    int messageThread;
    int messageLock, receiveLock;

    void init(); // allocate all the memory things
    // deals with message server gets from client
    void handleIncomingMessage(int clientID, std::string message);

    // thread routine to grab messages off the buffer and send them
    // or process incoming messages
    void *messageHandlingLoop();

    double clientLatency; // the latency from server to client

public:

    Latency(pong *game, webSocket *serverToUse, int id);

    Latency(webSocket *serverToUse, int id);

    ~Latency();

    int getID();

    void stopThread();

    static void *threadWrapperFunction(void *classRef);

    // Start the message sending/receiving thread
    void setPongGame(pong *game);

    // called when server gets message
    void receiveMessage(int clientID, std::string message);

    void sendMessage(int clientIDToSendTo, std::string message);

    std::string addTimestamp(std::string message); // adds timestamp to message and returns stamped message
    double getReceiveLatency();

    void clearSendBuffer();

    void clearReceiveBuffer();


};


#endif
 
