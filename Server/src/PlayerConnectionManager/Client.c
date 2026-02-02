#include "Client.h"
#include <stdlib.h>
#include <stdio.h>


void freeClient(Client* client)
{
	free(client->clientThread);
	free(client);
}

int clientListEqualFunction(const void * a, const void * b)
{
	int output = 0;
	
	if(a && b)
	{
		Client* clientA = (Client*)a;
		Client* clientB = (Client*)b;
		
		if(clientA->connectionID == clientB->connectionID)
			output = 1;
	}
	
	return output;
}

