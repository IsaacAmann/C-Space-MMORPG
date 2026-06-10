#include "Client.h"
#include <stdlib.h>
#include <stdio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

void freeClient(Client* client)
{
	free(client->readThread);
	free(client->writeThread);
	free(client);
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

void initClient(Client* client)
{
	uuid_generate_random(client->clientID);
}

