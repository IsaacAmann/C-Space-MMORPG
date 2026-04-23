
#include "main.h"
#include "ClientConnectionManager.h"
#include <glib.h>
#include "Client.h"
#include "string.h"
#include "TCPBinaryProtocol.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

struct sockaddr_in clientConnectionAddress;

pthread_mutex_t clientListLock;
GPtrArray* clientList;


void* clientConnectionManagerThreadRun(void *args)
{
	pthread_mutex_init(&clientListLock, NULL);
	clientList = g_ptr_array_new();
	
	SSL_CTX* sslCtx = SSL_CTX_new(TLS_server_method());
	
	//Creating self signed certificate for testing, this should be replaced later with a proper certificate
	
	//Create private key
	EVP_PKEY* privateKey = EVP_PKEY_new();
	privateKey = EVP_RSA_gen(128);
	
	
	X509* certificate = X509_new();
	
	ASN1_TIME* before = malloc(sizeof(ASN1_TIME));
	ASN1_TIME* after = malloc(sizeof(ASN1_TIME));
	ASN1_TIME_set_string_X509(before, "2601010101011");
	ASN1_TIME_set_string_X509(after, "9901010101011");
	
	X509_set1_notBefore(certificate, before);
	X509_set1_notAfter(certificate, after);
	
	X509_set_pubkey(certificate, privateKey);
	
	SSL_CTX_use_certificate(sslCtx, certificate);
	SSL_CTX_use_PrivateKey(sslCtx, privateKey);
	
	
	SSL_CTX_set_verify(sslCtx, SSL_VERIFY_NONE, NULL);
	
	//Maybe come back and set this up later (sounds like something I should have)
	SSL_CTX_set_session_cache_mode(sslCtx, SSL_SESS_CACHE_OFF);
	
	//Not sure if I need that
	//SSL_CTX_set_alpn_select_cb(ctx, select_alpn, NULL);
	
	/*
	//Create socket
	int socketFileDesc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	printf("fd: %d\n", socketFileDesc);
	
	struct sockaddr_in socketAddress;
	socketAddress.sin_family = AF_INET;
	socketAddress.sin_port = htons(8080);
	
	*/
	//socketAddress.sin_addr.s_addr = INADDR_ANY;

	//Bind socket
	//printf("Bind output:%d\n",bind(socketFileDesc, (const struct sockaddr*)&socketAddress, sizeof(socketAddress)));
	
	//Create listener 
	//SSL* listener = SSL_new_listener(sslCtx, 0);
	//SSL_set_fd(listener, socketFileDesc);
	//printf("listen out %d\n",SSL_listen(listener));

	BIO* accepterBio = BIO_new_accept("8080");
	BIO_set_bind_mode(accepterBio, BIO_BIND_REUSEADDR);
	
	long opts;
	
	opts = SSL_OP_IGNORE_UNEXPECTED_EOF;
	opts |= SSL_OP_NO_RENEGOTIATION;
	opts |= SSL_OP_SERVER_PREFERENCE;
	
	SSL_CTX_set_options(sslCtx, opts);
	
	printf("Player connection listener accepting connections...\n");
	while(1)
	{
		BIO* clientBio;
		SSL* ssl;
		ERR_clear_error();
		
		if(BIO_do_accept(accepterBio) <= 0)
		{
			continue;
		}
		
		clientBio = BIO_pop(accepterBio);
		
		ssl = SSL_new(sslCtx);
		
		SSL_set_bio(ssl, clientBio, clientBio);
		
		//ssl handshake
		if(SSL_accept(ssl) <= 0)
		{
			ERR_print_errors_fp(stderr);
			SSL_free(ssl);
			continue;
		}
		
		
	}
	
	printf("Client connection listener is exitting,,,\n");
}

void* clientConnectionThreadRun(void *args)
{
	
}

/*
//Thread function to listen for new player connections
//TCP connection for menus, chat, slow things
//Clients will connect to sector on separate UDP connection
void* playerConnectionManagerThreadRun(void *args)
{
	pthread_mutex_init(&clientListLock, NULL);
	clientList = g_ptr_array_new();
	
	int currentConnectionID = 0;
	//Setup socket for incoming player connections
	int playerConnectionSocket = socket(AF_INET, SOCK_STREAM, 0);
	
	playerConnectionAddress.sin_family = AF_INET;
	playerConnectionAddress.sin_port = htons(PLAYER_CONNECTION_PORT);
	playerConnectionAddress.sin_addr.s_addr = INADDR_ANY;
	
	bind(playerConnectionSocket, (struct sockaddr *) &playerConnectionAddress, sizeof(playerConnectionAddress));
	
	listen(playerConnectionSocket, 50);
	
	
	printf("Player connection listener accepting connections...\n");
	while(isServerRunning)
	{
		socklen_t sockLen = sizeof(playerConnectionAddress);

		struct sockaddr_in clientAddress;
		int playerSocket = accept(playerConnectionSocket, (struct sockaddr *) &clientAddress, &sockLen);
		
		char* currentClientIP = inet_ntoa(clientAddress.sin_addr);
		printf("New client connection @ %s\n", currentClientIP); 
		
		//Create new client struct
		Client* currentClient = malloc(sizeof(Client));
		
		//Create new thread to handle client
		currentClient->clientThread = malloc(sizeof(pthread_t));
		currentClient->connectionID = currentConnectionID++;
		
		strcpy(currentClient->ipAddress, inet_ntoa(clientAddress.sin_addr));
		currentClient->socket = playerSocket;
		pthread_create(currentClient->clientThread, NULL, &playerConnectionThreadRun, (void*)currentClient);

		//add to client array
		pthread_mutex_lock(&clientListLock);
		g_ptr_array_add(clientList, currentClient);

		pthread_mutex_unlock(&clientListLock);

	}
	
	printf("Player connection listener is exitting,,,\n");
}

void* playerConnectionThreadRun(void *args)
{
	Client* client = (Client*)args;
	char buffer;
	
	int recvOutput = recv(client->socket, &buffer, 1, 0);
	while(recvOutput != 0 && recvOutput != -1)
	{
		printf("%s sent %c\n", client->ipAddress, buffer);
		recvOutput = recv(client->socket, &buffer, 1, 0);
	}
	
	printf("%s connection closed with recv code %d\n", client->ipAddress, recvOutput);
	
	//Remove client from client list
	clientListRemoveByConnectionID(client->connectionID);
	
	//free client struct
	//doing this frees the pointer to this thread, hopefully that is not bad
	freeClient(client);
	
}

*/

bool clientListRemoveByConnectionID(int connectionID)
{
	bool output = FALSE;
	
	pthread_mutex_lock(&clientListLock);
	int index =  clientListGetIndexByConnectionID(connectionID);
	if(index != -1)
	{
		output = TRUE;
		g_ptr_array_remove_index(clientList, index);
		printf("Removing %d\n", connectionID);
	}
	pthread_mutex_unlock(&clientListLock);
	
	return output;
}

Client* clientListGetByConnectionID(int connectionID)
{
	Client* output = NULL;
	
	pthread_mutex_lock(&clientListLock);
	int index = clientListGetIndexByConnectionID(connectionID);
	if(index != -1)
	{
		output = g_ptr_array_index(clientList, index);
	}
	pthread_mutex_unlock(&clientListLock);
	
	return output;
}

//Should only be used by other functions that have already grabbed lock
int clientListGetIndexByConnectionID(int connectionID)
{
	int output;
	
	//Have to create a blank struct to compare against
	Client compareClient;
	compareClient.connectionID = connectionID;
	
	bool found = g_ptr_array_find_with_equal_func(clientList, &compareClient, clientListEqualFunction, &output);
	
	if(found == FALSE)
		output = -1;
	
	return output;
}
