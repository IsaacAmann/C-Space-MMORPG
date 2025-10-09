
#include "main.h"
#include "PlayerConnectionManager.h"


struct sockaddr_in playerConnectionAddress;

//Thread function to listen for new player connections
void* playerConnectionManagerThreadRun(void *args)
{
	//Setup socket for incoming player connections
	int playerConnectionSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	playerConnectionAddress.sin_family = AF_INET;
	playerConnectionAddress.sin_port = htons(PLAYER_CONNECTION_PORT);
	playerConnectionAddress.sin_addr.s_addr = INADDR_ANY;
	
	bind(playerConnectionSocket, (struct sockaddr *) &playerConnectionAddress, sizeof(playerConnectionAddress));
	
	listen(playerConnectionSocket, 50);
	
	
	printf("Player connection listener accepting connections...\n");
	while(isServerRunning)
	{
		socklen_t sockLen = sizeof(playerConnectionAddress);

		struct sockaddr_in clientAddress;
		int playerSocket = accept(playerConnectionSocket, (struct sockaddr *) &clientAddress, &sockLen);
		
		char* currentClientIP = inet_ntoa(clientAddress.sin_addr);
		printf("New client connection @ %s\n", currentClientIP); 
	}
	
	printf("Player connection listener is exitting,,,\n");
}

void* playerConnectionThreadRun(void *args)
{
	
}
