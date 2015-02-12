#include "Server.h"


using namespace std;
using namespace rapidjson;

webSocket server;
pong* pongGame;
default_random_engine generator;
uniform_int_distribution<int> distribution(70,600);




// constructor
void* Server(void* port) {
  //printf("SERVER\n");
	server.setOpenHandler(openHandler);
	server.setCloseHandler(closeHandler);
	server.setMessageHandler(messageHandler);
	//server.setPeriodicHandler(periodicHandler);

	/* start the chatroom server, listen to ip '127.0.0.1' and port '8000' */
	server.startServer((int)port);
}

void setPongGame(pong &game) {
	*pongGame = game;
}

/* called when a client connects */
void openHandler(int clientID){
  printf("OPENHAND\n");
	string json = "{\"phase\":\"initialization\"}";
	server.wsSend(clientID, json);
}

/* called when a client disconnects */
void closeHandler(int clientID){

}

void setMapInfo(vector<int> map, vector<int> paddle) {
	/* todo, Now need to do stuff to actually use this information
	** in the physics engine to set map size and initial paddle location
	** and size
	*/
}

void checkPartnerPresent(int clientID) {
	vector<int> clientIDs = server.getClientIDs();
	int partner = -1;
	for (int i = 0; i < clientIDs.size(); i++){
		if (clientIDs[i] != clientID)
			partner = clientIDs[i];
	}

	if (partner != -1) {
		// There are two clients, so we should send
		// both of them ball information and wait for
		// both responses
	} else {
		// todo, here we would handle the case where
		// one person is connected and not the other
		// which should wait may or may not wait until
		// the other player is connected
		// Since we don't have another player right now
		// it will just send data back to client

		int ballRadius = pongGame->ballradius;

		ostringstream json;
		json << "{\"phase\":\"initial_ball_position\",\"ball_position\":[";
		json << pongGame->ballx << ", " << pongGame->bally << "],";
		json << "\"ball_size\":" << ballRadius << "}";

		server.wsSend(clientID, json.str());

	}
}

/* called when a client sends a message to the server */
void messageHandler(int clientID, string message){

	Document d;
	const char * c = message.c_str();
	d.Parse(c);
	string phaseString = d["phase"].GetString();

	if (phaseString.compare("initial_dimensions") == 0) {
		Value &mapDimJson = d["map_dimensions"];
		Value &paddleDimJson = d["paddle_dimensions"];

		vector<int> mapDims(mapDimJson.Size());
		vector<int> paddleDims(paddleDimJson.Size());

		for (SizeType i = 0; i < mapDimJson.Size(); i++) {
			mapDims[i] = mapDimJson[i].GetInt();
		}
		for (SizeType i = 0; i < paddleDimJson.Size(); i++) {
			paddleDims[i] = paddleDimJson[i].GetInt();
		}

		pongGame->boardWidth = mapDims[0];
		pongGame->boardHeight = mapDims[1];
		pongGame->paddleWidth = paddleDims[0];
		pongGame->paddleHeight = paddleDims[1];
		pongGame->setBallPos(mapDims[0]/2, mapDims[1]/2);
		pongGame->setBallRadius(10);

		//setMapInfo(mapDims, paddleDims);
		checkPartnerPresent(clientID);

	} else if (phaseString.compare("ready_to_start") == 0) { 
		// todo, figure out what to do when we have two clients
		// perhaps create a while loop that waits until another
		// client has connected or something

		// for now, just send start signal to single client
		Value &ballPosJson = d["ball_position"];
		vector<int> ballPos(ballPosJson.Size());

		for (SizeType i = 0; i < ballPosJson.Size(); i++) {
			ballPos[i] = ballPosJson[i].GetInt();
		}

		if (ballPos[0] != pongGame->ballx || ballPos[1] != pongGame->bally)
		{
			// todo, do something to remedy situation where ball
			// position is not correct
		} else {
			server.wsSend(clientID, "{\"phase\":\"start\"}");
			pongGame->gameObjectSet = true;
		}
	} else if (phaseString.compare("paddle_update") == 0) {
		Value &paddlePosJson = d["paddle_position"];
		vector<int> paddlePos(paddlePosJson.Size());
		for (SizeType i = 0; i < paddlePosJson.Size(); i++) {
			paddlePos[i] = paddlePosJson[i].GetInt();
		}
		/* todo, Now need to do stuff to actually use this information
		** in the physics engine to update paddle location
		*/

		pongGame->setPaddlePos(paddlePos[0], paddlePos[1]);
		/*cout << "Paddle x: " << paddlePos[0] << ", Paddle y: " << paddlePos[1] << endl;
		int x = distribution(generator);
		int y = distribution(generator);*/

		/*ostringstream json;
		json << "{\"phase\":\"ball_update\",\"ball_position\":[";
		json << x << ", " << y << "]}";

		server.wsSend(clientID, json.str());*/
	} else {

	}
	cout << "woop wop!" << endl;
}

/* called once per select() loop */
//void periodicHandler(){
//	static time_t next = time(NULL) + 10;
//	time_t current = time(NULL);
//	if (current >= next){
//		ostringstream os;
//		string timestring = ctime(&current);
//		timestring = timestring.substr(0, timestring.size() - 1);
//		os << timestring;
//
//		vector<int> clientIDs = server.getClientIDs();
//		for (int i = 0; i < clientIDs.size(); i++)
//			server.wsSend(clientIDs[i], os.str());
//
//		next = time(NULL) + 10;
//	}
//}
