#####################################
# 				    #
#    Kevin Malby UCID: 36928917	    #
#    Kathryn Rodgers UCID: 39483825 #
#    Usage: make all		    #	
# 				    #
#####################################



all: twoPlayerPong.o sha1.o websocket.o base64.o  latency.o player.o
	g++ -g -I../include -D__linux__ player.o twoPlayerPong.o latency.o sha1.o websocket.o base64.o ../src/main.cpp -lpthread -L../libraries -ljsoncpp -static -o server

onePlayer: pong.o sha1.o websocket.o base64.o 
	g++ -g  -I../include -D__linux__ pong.o sha1.o websocket.o base64.o ../src/main.cpp -lpthread -L../libraries -ljsoncpp -static -o server


pong: ../src/twoPlayerPong.cpp
	g++ -g -I../include -D__linux__  -mno-cygwin twoPlayerPong.cpp -o pong

tests: ../src/twoPlayerPong.cpp ../src/testTwoPlayer.cpp
	g++ -g -Wall -I../include twoPlayerPong.cpp testTwoPlayer.cpp -o tests

lat_tests: latency.o websocket.o sha1.o base64.o twoPlayerPong.o latencyTester.cpp
	g++ -g -Wall -I../include sha1.o base64.o websocket.o twoPlayerPong.o latency.o latencyTester.cpp -lpthread -L../libraries -lgtest_main -ljsoncpp -o latTests


player.o: ../src/player.cpp
	g++ -c -I../include ../src/player.cpp
base64.o: ../src/base64.cpp
	g++ -c -I../include ../src/base64.cpp
websocket.o: ../src/websocket.cpp
	g++ -g -c -I../include ../src/websocket.cpp
twoPlayerPong.o: ../src/twoPlayerPong.cpp
	g++ -g -c -I../include ../src/twoPlayerPong.cpp
pong.o: ../src/pong.cpp
	g++ -g -c -I../include ../src/pong.cpp
latency.o: ../src/latency.cpp
	g++ -g -c -I../include -D__linux__ ../src/latency.cpp
sha1.o: ../src/sha1.cpp
	g++ -c -I../include ../src/sha1.cpp


clean:
	rm -f *.o *~ tests server pong 



