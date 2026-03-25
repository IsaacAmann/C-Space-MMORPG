#include "main.h"

int serverRunning;

int main()
{
	serverRunning = 1;
	
	
	while(serverRunning)
	{
		
	}
	
	return 0;
}

//Allow outside files to check isServerRunning variable
int isServerRunning()
{
	return serverRunning;
}
