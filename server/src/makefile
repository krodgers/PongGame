#####################################
# /**				    #
#    Kevin Malby UCID: 36928917	    #
#    Kathryn Rodgers UCID: 39483825 #
# **/				    #
#####################################

all: twoPlayerPong.o sha1.o websocket.o base64.o  latency.o player.o
	g++ -g -I../include -D__linux__ player.o twoPlayerPong.o latency.o sha1.o websocket.o base64.o main.cpp -lpthread -L../libraries -ljsoncpp -static -o server

onePlayer: pong.o sha1.o websocket.o base64.o 
	g++ -g  -I../include -D__linux__ pong.o sha1.o websocket.o base64.o main.cpp -lpthread -L../libraries -ljsoncpp -static -o server


pong: twoPlayerPong.cpp
	g++ -g -I../include -D__linux__  -mno-cygwin twoPlayerPong.cpp -o pong

tests: twoPlayerPong.cpp testTwoPlayer.cpp
	g++ -g -Wall -I../include twoPlayerPong.cpp testTwoPlayer.cpp -o tests

lat_tests: latency.o websocket.o sha1.o base64.o twoPlayerPong.o latencyTester.cpp
	g++ -g -Wall -I../include sha1.o base64.o websocket.o twoPlayerPong.o latency.o latencyTester.cpp -lpthread -L../libraries -lgtest_main -ljsoncpp -o latTests


player.o:
	g++ -c -I../include player.cpp
base64.o: base64.cpp
	g++ -c -I../include base64.cpp
websocket.o: websocket.cpp
	g++ -g -c -I../include websocket.cpp
twoPlayerPong.o: twoPlayerPong.cpp
	g++ -g -c -I../include twoPlayerPong.cpp
pong.o: pong.cpp
	g++ -g -c -I../include pong.cpp
latency.o: latency.cpp
	g++ -g -c -I../include -D__LINUX__ latency.cpp
sha1.o: sha1.cpp
	g++ -c -I../include sha1.cpp



clean:
	rm -f *.o *~ tests server pong 



