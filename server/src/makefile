#####################################
# /**				    #
#    Kevin Malby UCID: 36928917	    #
#    Kathryn Rodgers UCID: 39483825 #
# **/				    #
#####################################

all: pong.o sha1.o websocket.o base64.o 
	g++ -g  -I../include -D__linux__ pong.o sha1.o websocket.o base64.o main.cpp -lpthread -L../libraries -ljsoncpp -static -o server


pong: twoPlayerPong.cpp
	g++ -g -I../include -D__linux__  -mno-cygwin twoPlayerPong.cpp -o pong

tests: twoPlayerPong.cpp testTwoPlayer.cpp
	g++ -g -Wall  -I../include twoPlayerPong.cpp testTwoPlayer.cpp -o tests



base64.o: base64.cpp
	g++ -c -I../include base64.cpp
websocket.o: websocket.cpp
	g++ -c -I../include websocket.cpp
pong.o: pong.cpp
	g++ -c -I../include pong.cpp
sha1.o: sha1.cpp
	g++ -c -I../include sha1.cpp



clean:
	rm -f *.o tests server



