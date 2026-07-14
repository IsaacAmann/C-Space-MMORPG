
#include "main.h"
#include "ClientConnectionManager.h"
#include <glib.h>
#include "Client.h"
#include "string.h"
#include "TCPBinaryProtocol.h"
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <uuid/uuid.h>

struct sockaddr_in clientConnectionAddress;

pthread_mutex_t clientListLock;
GPtrArray* clientList;


void* clientConnectionManagerThreadRun(void *args)
{
	void* test = malloc(2);
	pthread_mutex_init(&clientListLock, NULL);
	clientList = g_ptr_array_new();
	
	SSL_CTX* sslCtx = SSL_CTX_new(TLS_server_method());
	//Creating self signed certificate for testing, this should be replaced later with a proper certificate
	//Create private key
	EVP_PKEY* privateKey;
	EVP_PKEY_CTX* privateKeyCtx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);
	EVP_PKEY_keygen_init(privateKeyCtx);
	EVP_PKEY_CTX_set_rsa_keygen_bits(privateKeyCtx, 2048);
	EVP_PKEY_keygen(privateKeyCtx, &privateKey); 
	
	X509* certificate = X509_new();
	
    X509_gmtime_adj(X509_getm_notBefore(certificate), 0);
    X509_gmtime_adj(X509_getm_notAfter(certificate), 31536000L);
	
	
	X509_NAME* name = X509_get_subject_name(certificate);
	X509_NAME_add_entry_by_txt(name, "C", MBSTRING_ASC, (unsigned char*)"US", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "O", MBSTRING_ASC, (unsigned char*)"None", -1, -1, 0);
    X509_NAME_add_entry_by_txt(name, "CN", MBSTRING_ASC, (unsigned char*)"localhost", -1, -1, 0);
    	
	X509_set_issuer_name(certificate, name);
	
	X509_set_pubkey(certificate, privateKey);
	X509_sign(certificate, privateKey, EVP_sha256());
	
	SSL_CTX_set_security_level(sslCtx, 0);
	SSL_CTX_set_session_cache_mode(sslCtx, SSL_SESS_CACHE_OFF);
	SSL_CTX_use_certificate(sslCtx, certificate);
	SSL_CTX_use_PrivateKey(sslCtx, privateKey);
	const unsigned char* idCTX = (unsigned char*) "server";
	printf("context: %d\n", SSL_CTX_set_session_id_context(sslCtx, idCTX, strlen(idCTX)));
	
	SSL_CTX_set_verify(sslCtx, SSL_VERIFY_NONE, NULL);

	BIO* accepterBio = BIO_new_accept("8080");
	BIO_set_nbio(accepterBio, 1);
	BIO_do_accept(accepterBio);
	//may need to switch to non-blocking, client renegotiating causes segfault when altering ssl struct 
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
		/*
		if(acceptRet <= 0)
		{
			printf("handshake\n");
			printf("sslgeterror: %d\n", SSL_get_error(ssl, acceptRet));
			ERR_print_errors_fp(stderr);
			SSL_free(ssl);
			continue;
		}
		*/
		int acceptRet = SSL_accept(ssl);
		while(acceptRet < 0 && acceptRet != 1)
		{
			printf("handshake\n");
			printf("sslgeterror: %d\n", SSL_get_error(ssl, acceptRet));
			
			acceptRet = SSL_accept(ssl);
		}
		if(acceptRet <= 0)
		{
			ERR_print_errors_fp(stderr);
			SSL_free(ssl);
			continue;
		}
		
		//Create new client struct
		Client* currentClient = malloc(sizeof(Client));
		initClient(currentClient);
		currentClient->ssl = ssl;
		currentClient->readThread = malloc(sizeof(pthread_t));
		currentClient->writeThread = malloc(sizeof(pthread_t));
		//Pass off to another thread
		pthread_create(currentClient->readThread, NULL, &clientReadThreadRun, (void*)currentClient);
		pthread_create(currentClient->writeThread, NULL, &clientWriteThreadRun, (void*)currentClient);
		//add to client array
		pthread_mutex_lock(&clientListLock);
		g_ptr_array_add(clientList, currentClient);
		pthread_mutex_unlock(&clientListLock);
		
	}
	
	printf("Client connection listener is exitting,,,\n");
}

void* clientReadThreadRun(void *args)
{
	Client* client = (Client*)args;
	unsigned char buffer[8000];
	size_t bytesRead;
	
	TCPBinaryMessage currentMessage;
	char* messagePointer = (char*)&currentMessage;
	int messageStarted = FALSE;
	int bytesCopiedToMessage = 0;
	
	//Need another way to know when connection closes, non-blocking exits
	while(SSL_read_ex(client->ssl, buffer, sizeof(buffer), &bytesRead) > 0)
	{
		printf("read\n");
		//Start at the beginning of the buffer
		int currentByte = 0;
		while(currentByte < bytesRead)
		{
			//Case where message type has not been copied yet
			if(bytesCopiedToMessage < 2)
			{
				messagePointer[bytesCopiedToMessage] = buffer[currentByte];
				printf("type: %d\n", currentMessage.type);
				currentByte++;
				bytesCopiedToMessage++;
			}
			//Data length not yet copied
			else if(bytesCopiedToMessage < 4)
			{
				messagePointer[bytesCopiedToMessage] = buffer[currentByte];
				printf("length: %d\n", currentMessage.length);

				currentByte++;
				bytesCopiedToMessage++;
				//If length fully read, allocate data memory
				if(bytesCopiedToMessage == 4)
				{
					currentMessage.data = malloc(currentMessage.length);
					printf("Allocated %d for message data\n", currentMessage.length);
				}
			}
			//Copy message
			else
			{
				//Get desired bytes to complete message (add 4 to account for the four bytes used for message type and length)
				int desiredBytes = currentMessage.length - bytesCopiedToMessage + 4;
				int actualBytesToCopy = desiredBytes;
				int bytesLeft = bytesRead - currentByte;
				//Check if desired bytes are available in the buffer
				if(desiredBytes > bytesLeft)
				{
					actualBytesToCopy = bytesLeft;
				}
				printf("d: %d a: %d r: %d\n", desiredBytes, actualBytesToCopy, bytesRead);

				//Copy bytes up to buffer contents
				memcpy(currentMessage.data + (bytesCopiedToMessage - 4), buffer, actualBytesToCopy);
				bytesCopiedToMessage += actualBytesToCopy;
				currentByte += actualBytesToCopy;
				printf("bytes copied: %d\n", bytesCopiedToMessage);
				//If message is complete, handle the message
				if(bytesCopiedToMessage == 4 + currentMessage.length)
				{
					printf("Full message recieved\n");
					printf("type: %d\n", currentMessage.type);
					printf("length: %d\n", currentMessage.length);
					//clear message
					bytesCopiedToMessage = 0;
				}
			}
		}
		
		//printf("read: %s, total bytes: %d\n", buffer, bytesRead);
	}
	
	printf("connection closed\n");
	
	//Remove client from client list
	clientListRemoveByConnectionID(client->clientID);
	pthread_join(*(client->writeThread), NULL);
	freeClient(client);
}

void* clientWriteThreadRun(void *args)
{
	Client* client = (Client*)args;
	size_t bytesWritten;
	int writeOutput = 1;
	unsigned char* buffer = NULL;
	
	while(writeOutput > 0)
	{
		int bytesToWrite = 0;
		//Fill buffer with messages from client write buffer
		pthread_mutex_lock(&(client->writeBufferLock));
		if(client->writeBuffer->len > 0)
		{
			for(int i = 0; i < client->writeBuffer->len; i++)
			{
				TCPBinaryMessage* currentMessage = g_ptr_array_index(client->writeBuffer, i);
				bytesToWrite += sizeof(TCPBinaryMessage) - sizeof(currentMessage->data) + currentMessage->length;
			}
			
			buffer = malloc(bytesToWrite);
			//copy each message into the buffer
			int currentByte = 0;
			for(int i = 0; i < client->writeBuffer->len; i++)
			{
				TCPBinaryMessage* currentMessage = g_ptr_array_index(client->writeBuffer, i);
				//copy type and length
				memcpy(buffer + currentByte, currentMessage, sizeof(uint16_t)*2);
				currentByte += sizeof(uint16_t)*2;
				
				//copy data
				memcpy(buffer + currentByte, currentMessage->data, currentMessage->length);
				currentByte += currentMessage->length;
				free(currentMessage->data);
				free(currentMessage);
			}
		}
		
		//clear client write buffer
		g_ptr_array_set_size(client->writeBuffer, 0);
		//release lock
		pthread_mutex_unlock(&(client->writeBufferLock));
		
		writeOutput = SSL_write_ex(client->ssl, buffer, bytesToWrite, &bytesWritten);
		free(buffer);
	}

	printf("write thread\n");
	//Remove client from client list
	//clientListRemoveByConnectionID(client->connectionID);
	//free client struct
	//freeClient(client);
}

bool clientListRemoveByConnectionID(uuid_t clientID)
{
	bool output = FALSE;
	
	pthread_mutex_lock(&clientListLock);
	int index =  clientListGetIndexByConnectionID(clientID);
	if(index != -1)
	{
		output = TRUE;
		g_ptr_array_remove_index(clientList, index);
		char stringID[37];
		uuid_unparse(clientID, stringID);
		printf("Removing %s\n", stringID);
	}
	pthread_mutex_unlock(&clientListLock);
	
	return output;
}

Client* clientListGetByConnectionID(uuid_t clientID)
{
	Client* output = NULL;
	
	pthread_mutex_lock(&clientListLock);
	int index = clientListGetIndexByConnectionID(clientID);
	if(index != -1)
	{
		output = g_ptr_array_index(clientList, index);
	}
	pthread_mutex_unlock(&clientListLock);
	
	return output;
}

//Should only be used by other functions that have already grabbed lock
int clientListGetIndexByConnectionID(uuid_t clientID)
{
	int output;
	
	//Have to create a blank struct to compare against
	Client compareClient;
	uuid_copy(compareClient.clientID, clientID);
	
	bool found = g_ptr_array_find_with_equal_func(clientList, &compareClient, clientListEqualFunction, &output);
	
	if(found == FALSE)
		output = -1;
	
	return output;
}
