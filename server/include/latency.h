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
 public:
    enum PacketType { BALL, PADDLE, SCORE };

private:
    friend class latencyTester;

    int ID; // to identify which client this buffer is associated with
    int totalNumPackets;
    std::queue<std::string> *sendBallBuffer;
    std::queue<std::string> *sendPaddleBuffer;
    std::queue<std::string> *sendScoreBuffer;
    std::queue<int> *sendBallIDs;
    std::queue<int> *sendPaddleIDs;
    std::queue<int> *sendScoreIDs;
    std::queue<int> *receiveIDs;
    std::queue<std::string> *receiveBuffer;
    int latencyTimeMax; // in millisecond
    int latencyTimeMin; // in millisecond
    pong *pongGame;
    webSocket *server;
    bool sendAndReceive;
    int messageThread;
    //    int messageLock, receiveLock, sendLock;
    pthread_mutex_t rcvLock, ballLock, paddleLock, scoreLock;

    void init(); // allocate all the memory things
    // deals with message server gets from client
    void handleIncomingMessage(int clientID, std::string message);

    // thread routine to grab messages off the buffer and send them
    // or process incoming messages
    void* messageReceivingLoop();
    void* sendingLoop();
    double clientLatency; // the latency from server to client
    double averageClientLatency;
    void lockRightThing(PacketType);
    void unlockRightThing(PacketType);

public:

    //    enum PacketType { BALL, PADDLE, SCORE };

    Latency(pong *game, webSocket *serverToUse, int id);

    Latency(webSocket *serverToUse, int id);

    ~Latency();

    int getID();
    double getClientLatency(); // returns latency from server to client
    void stopThread();

    static void *startSendLoop(void *classRef);
    static void *startRcvLoop(void *classRef);
    // Start the message sending/receiving thread
    void setPongGame(pong *game);

    // called when server gets message
    void receiveMessage(int clientID, std::string message);

    void sendMessage(int clientIDToSendTo, std::string message, PacketType type);
    void sendAdministrativeMessage(int clientID, string message);

    std::string addTimestamp(std::string message); // adds timestamp to message and returns stamped message
    double getReceiveLatency();

    void clearSendBuffer(PacketType type);

    void clearSendBallBuffer();
    void clearSendPaddleBuffer();
    void clearSendScoreBuffer();

    void sendBufferMessage();

    queue<std::string>* getSendBuffer(PacketType type);
    queue<int>* getSendIDs(PacketType type);

    void clearReceiveBuffer();

    void printBufferInfo();


};


#endif
 
