#ifndef TCP_BINARY_PROTOCOL_CODES
#define TCP_BINARY_PROTOCOL_CODES

//Most codes are type-length-value or close to it


typedef struct TCPBinaryMessage
{
	uint16_t type;
	uint16_t length;
	char* data;
} TCPBinaryMessage;

#define TCP_LOGIN 0x0000

#endif
