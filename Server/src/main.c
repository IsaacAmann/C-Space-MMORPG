
#include "main.h"
#include "ClientConnectionManager.h"
#include "DatabaseManager.h"

int isServerRunning;

int main()
{
	//Initialize database
	initializeDatabase();
	
	//Initialize sectors
	
	//Initialize player connection manager
	pthread_t clientConnectionManagerThread;
	pthread_create(&clientConnectionManagerThread, NULL, &clientConnectionManagerThreadRun, NULL);
	
	//Initialize server connection manager
	
	//Global variable. Other threads should monitor this for signal to shutdown
	isServerRunning = 1;
	
	
	
	//Hold for threads to exit
	pthread_join(clientConnectionManagerThread, NULL);
	
	
	//Shutdown server
	
	return 0;
}
