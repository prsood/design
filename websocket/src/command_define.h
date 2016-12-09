/*************************************************************************
	> File Name: command_define.h
	> Author: cjj
	> Created Time: 2013年10月27日 星期日 00时57分04秒
 ************************************************************************/
#ifndef COMMAND_DEFINE_H
#define COMMAND_DEFINE_H
#include <stddef.h>
#include <string>
using namespace std;
const int MAX_LISTEN = 3000;
const int SERVER_LISTEN_PORT = 8768;
const int MAX_DATA_LEN = 4096;

typedef struct
{
	unsigned char buf[MAX_DATA_LEN + 3];
	size_t nLen;
	int socket;
}ServerData, *pServerData;



typedef void (*pCallBack)(const char * szBuf, size_t nLen, int socket);

const char response1[] = "HTTP/1.1 101 Switching Protocols\r\nConnection:Upgrade\r\nServer:ZYM Websocket Server\r\nUpgrade:WebSocket\r\nSec-WebSocket-Accept:";
const char response2[] = "\r\n\r\n";
const int HANDSHAKE_UNINITIALIZE = -10;
const int HANDSHAKE_PROTOCOL_REQUEST_FAIL = -11;
const int HANDSHAKE_PROTOCOL_SUCCESS = 0;

const int ERROR_FRAME=0xFF00;
const int INCOMPLETE_FRAME=0xFE00;
const int OPENING_FRAME=0x3300;
const int CLOSING_FRAME=0x3400;
const int INCOMPLETE_TEXT_FRAME=0x01;
const int INCOMPLETE_BINARY_FRAME=0x02;
const int TEXT_FRAME=0x81;
const int BINARY_FRAME=0x82;
const int PING_FRAME=0x19;
const int PONG_FRAME=0x1A;

#endif
