
#include "main.h"
#include "ClientConnectionManager.h"
#include "DatabaseManager.h"

int serverRunning;

int main()
{
	serverRunning = 1;

	//Initialize database
	initializeDatabase();
	
	//Initialize sectors
	
	//Initialize player connection manager
	pthread_t clientConnectionManagerThread;
	pthread_create(&clientConnectionManagerThread, NULL, &clientConnectionManagerThreadRun, NULL);
	
	//Initialize server connection manager
	
	
	//Hold for threads to exit
	pthread_join(clientConnectionManagerThread, NULL);
	
	
	//Shutdown server
	
	return 0;
}

int isServerRunning()
{
	return serverRunning;
}

