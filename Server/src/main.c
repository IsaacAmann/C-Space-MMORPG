
#include "main.h"
#include "PlayerConnectionManager.h"
#include <mysql.h>

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
	
	//test database connection
	MYSQL *connection;
	connection = mysql_init(0);
	
	mysql_real_connect(connection, "localhost", "mmo_server", "password", "C_MMORPG", 3306, NULL, 0);
	
	printf("%s\n", mysql_error(connection));
	
	const char* serverVersion;
	mariadb_get_infov(connection, MARIADB_CONNECTION_SERVER_VERSION, (void*)&serverVersion);

	printf("%s\n", serverVersion);
	
	//Hold for threads to exit
	pthread_join(playerConnectionManagerThread, NULL);
	
	
	//Shutdown server
	
	return 0;
}
