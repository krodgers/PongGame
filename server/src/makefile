all: pong.cpp sha1.cpp websocket.cpp base64.cpp Server.cpp
	g++ -g -std=gnu++11 -I../include pong.cpp sha1.cpp websocket.cpp base64.cpp Server.cpp filesforponggame/main.cpp -lpthread -L../include -ljsoncpp


ChipTest: ChipmunkExample.cpp
	g++ -g -Wall  -I../include -I../../../ChipmunkLibrary/Chipmunk-7.0.0/include/chipmunk  PongGameMain.cpp -L../include -lchipmunk -lpthread -static

tests: pong.cpp testNewPHysics.cpp
	g++ -g -Wall  -I../include pong.cpp testNewPHysics.cpp


