#ifndef CLIENT_H
#define CLIENT_H

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define CLIENT_TYPE_PLAYER 0
#define CLIENT_TYPE_SECTOR_SERVER 1

typedef struct Client
{
	char ipAddress[23];
	int connectionID;
	int socket;
	int clientType;
	pthread_t* clientThread;
} Client;


void freeClient(Client* client);

int clientListEqualFunction(const void * a, const void * b);
#endif
