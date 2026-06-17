#include "Client.h"
#include <stdlib.h>
#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void freeClient(Client* client)
{
	free(client->readThread);
	free(client->writeThread);
	//cleanup write buffer
	
	
	free(client->writeThread);
	free(client->readThread);
	free(client);

}

void initClient(Client* client)
{
	uuid_generate_random(client->clientID);
	pthread_mutex_init(&(client->writeBufferLock), NULL);
	client->writeBuffer = g_ptr_array_new();
}

int clientListEqualFunction(const void * a, const void * b)
{
	
	int output = 0;
	
	/*
	if(a && b)
	{
		Client* clientA = (Client*)a;
		Client* clientB = (Client*)b;
		
		if(clientA->connectionID == clientB->connectionID)
			output = 1;
	}
	*/
	if(a && b)
	{
		Client* clientA = (Client*)a;
		Client* clientB = (Client*)b;
		
		if(uuid_compare(clientA->clientID, clientB->clientID) == 0)
			output = 1;
	}	
	
	return output;
}



void addClientMessage(Client* client, TCPBinaryMessage* message)
{
	//grab lock
	pthread_mutex_lock(&client->writeBufferLock);
	g_ptr_array_add(client->writeBuffer, message);
	//release lock
	pthread_mutex_unlock(&client->writeBufferLock);

}


