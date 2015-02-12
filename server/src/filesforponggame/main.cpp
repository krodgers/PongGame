#include "Server.h"
#include "pong.h"
#include <cstdlib>
#ifdef __linux__
#include <pthread.h>
#elif _WIN32
#include <process.h>
#endif

using namespace std;

int serverThread;
int gameLoopThread;
//pong* pongGame;

  extern pong* pongGame;

int main(int argc, char *argv[]){
	int port;
	pongGame = new pong();

	//setPongGame(*pongGame);

	cout << "Please set server port: ";
	cin >> port;

#ifdef __linux__
	pthread_t id;
	void* voidStar = &port;
	serverThread = pthread_create(&id, NULL, Server, voidStar);

#elif _WIN32
	_beginthread( Server, 0, (void*)port );
#endif

	


	while (true)
	{
	  //	  cout << "a" << endl;
		if (pongGame->gameObjectSet == true)
		{

		}
	}

	return 1;
}
