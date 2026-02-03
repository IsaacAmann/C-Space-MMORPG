#ifndef TCP_BINARY_PROTOCOL
#define TCP_BINARY_PROTOCOL

#include "TCPBinaryProtocolCodes.h"



TCPBinaryMessage* createTCPBinaryMessage(uint16_t type, uint16_t length, char* data);

void freeTCPBinaryMessage(TCPBinaryMessage* message);


#endif
