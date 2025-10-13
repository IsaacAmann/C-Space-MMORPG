#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>

#include "DatabaseManager.h"


//test database connection
MYSQL *connection;

void initializeDatabase()
{
	connection = mysql_init(0);
		
	if(mysql_real_connect(connection, DB_HOST, DB_USER, DB_PASSWORD, DB_DEFAULT_DB, DB_PORT, NULL, 0))
	{
		printf("Database connection successful\n");
	}
	else
	{
		printf("Database connection error: %s\n", mysql_error(connection));
		mysql_close(connection);
		exit(1);
	}
		
	printf("%s\n", mysql_error(connection));
		
	const char* serverVersion;
	mariadb_get_infov(connection, MARIADB_CONNECTION_SERVER_VERSION, (void*)&serverVersion);

	printf("%s\n", serverVersion);
}
