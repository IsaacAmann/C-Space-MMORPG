
#include "main.h"
#include "PlayerConnectionManager.h"
#include <glib.h>
#include "Client.h"
#include "string.h"
#include "TCPBinaryProtocol.h"

struct sockaddr_in playerConnectionAddress;

pthread_mutex_t clientListLock;
GPtrArray* clientList;

//Thread function to listen for new player connections
//TCP connection for menus, chat, slow things
//Clients will connect to sector on separate UDP connection
void* playerConnectionManagerThreadRun(void *args)
{
	pthread_mutex_init(&clientListLock, NULL);
	clientList = g_ptr_array_new();
	
	int currentConnectionID = 0;
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
		
		//Create new client struct
		Client* currentClient = malloc(sizeof(Client));
		
		//Create new thread to handle client
		currentClient->clientThread = malloc(sizeof(pthread_t));
		currentClient->connectionID = currentConnectionID++;
		
		strcpy(currentClient->ipAddress, inet_ntoa(clientAddress.sin_addr));
		currentClient->socket = playerSocket;
		pthread_create(currentClient->clientThread, NULL, &playerConnectionThreadRun, (void*)currentClient);

		//add to client array
		pthread_mutex_lock(&clientListLock);
		g_ptr_array_add(clientList, currentClient);

		pthread_mutex_unlock(&clientListLock);

	}
	
	printf("Player connection listener is exitting,,,\n");
}

void* playerConnectionThreadRun(void *args)
{
	Client* client = (Client*)args;
	char buffer;
	
	int recvOutput = recv(client->socket, &buffer, 1, 0);
	while(recvOutput != 0 && recvOutput != -1)
	{
		printf("%s sent %c\n", client->ipAddress, buffer);
		recvOutput = recv(client->socket, &buffer, 1, 0);
	}
	
	printf("%s connection closed with recv code %d\n", client->ipAddress, recvOutput);
	
	//Remove client from client list
	clientListRemoveByConnectionID(client->connectionID);
	
	//free client struct
	//doing this frees the pointer to this thread, hopefully that is not bad
	freeClient(client);
	
}

bool clientListRemoveByConnectionID(int connectionID)
{
	bool output = FALSE;
	
	pthread_mutex_lock(&clientListLock);
	int index =  clientListGetIndexByConnectionID(connectionID);
	if(index != -1)
	{
		output = TRUE;
		g_ptr_array_remove_index(clientList, index);
		printf("Removing %d\n", connectionID);
	}
	pthread_mutex_unlock(&clientListLock);
	
	return output;
}

Client* clientListGetByConnectionID(int connectionID)
{
	Client* output = NULL;
	
	pthread_mutex_lock(&clientListLock);
	int index = clientListGetIndexByConnectionID(connectionID);
	if(index != -1)
	{
		output = g_ptr_array_index(clientList, index);
	}
	pthread_mutex_unlock(&clientListLock);
	
	return output;
}

//Should only be used by other functions that have already grabbed lock
int clientListGetIndexByConnectionID(int connectionID)
{
	int output;
	
	//Have to create a blank struct to compare against
	Client compareClient;
	compareClient.connectionID = connectionID;
	
	bool found = g_ptr_array_find_with_equal_func(clientList, &compareClient, clientListEqualFunction, &output);
	
	if(found == FALSE)
		output = -1;
	
	return output;
}
