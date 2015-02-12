#ifndef SERVER_H
#define SERVER_H

#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <random>
#include "websocket.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "pong.h"


void* Server(void* port);
void setPongGame(pong &game);
void openHandler(int clientID);
void closeHandler(int clientID);
void setMapInfo(vector<int> map, vector<int> paddle);
void checkPartnerPresent(int clientID);
void messageHandler(int clientID, string message);



#endif /* SERVER_H */
