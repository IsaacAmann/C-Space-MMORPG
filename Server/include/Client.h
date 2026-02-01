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

typedef struct Client
{
	char ipAddress[23];
	int connectionID;
	int socket;
	pthread_t* clientThread;
} Client;

#endif
