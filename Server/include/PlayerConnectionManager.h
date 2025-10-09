#ifndef PLAYER_CONNECTION_MANAGER_H
#define PLAYER_CONNECTION_MANAGER_H

#define PLAYER_CONNECTION_PORT 30200
#define SERVER_NODE_CONNECTION_PORT 30201

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

typedef struct PlayerConnection
{
	char username[60];
	char* ipAddress;
	//Will probably be pulled from database upon connection
	char* playerID;
	
	
} PlayerConnection;

void* playerConnectionManagerThreadRun(void* args);

void* playerConnectionThreadRun(void* args);

#endif
