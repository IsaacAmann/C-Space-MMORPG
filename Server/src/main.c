
#include "main.h"
#include "PlayerConnectionManager.h"

int isServerRunning;

int main()
{
	
	//Initialize sectors
	
	//Initialize player connection manager
	pthread_t playerConnectionManagerThread;
	pthread_create(&playerConnectionManagerThread, NULL, &playerConnectionManagerThreadRun, NULL);
	
	//Initialize server connection manager
	
	//Global variable. Other threads should monitor this for signal to shutdown
	isServerRunning = 1;
	

	
	//Hold for threads to exit
	pthread_join(playerConnectionManagerThread, NULL);
	
	
	//Shutdown server
	
	return 0;
}
