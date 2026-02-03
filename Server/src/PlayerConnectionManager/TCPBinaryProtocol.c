#include "TCPBinaryProtocol.h"

TCPBinaryMessage* createTCPBinaryMessage(uint16_t type, uint16_t length, char* data)
{
	TCPBinaryMessage* message = malloc(sizeof(TCPBinaryMessage));
	
	message->type = type;
	message->length = length;
	
	memcpy(message->data, data, length);
	
	return message;
}

void freeTCPBinaryMessage(TCPBinaryMessage* message)
{
	free(message->data);
	free(message);
}
