/*************************************************************************
	> File Name: test.cpp
	> Author: cjj
	> Created Time: 2013年10月27日 星期日 02时23分51秒
 ************************************************************************/
#include "TcpServer.h"
#include <stdio.h>
#include <unistd.h>
#include"WSService.h"
TcpServer tcpServer;
WS_Service ws;
void printMessage(const char * buf, size_t nlen, int sock)
{
	if(NULL == buf)
	{
		return;
	}
	ServerData * serverData = (ServerData*)buf;
	printf("Message:%s\n", serverData->buf);
	tcpServer.SendData(serverData->buf, sizeof(serverData->buf), serverData->socket);
}



int main()
{
	if(false == ws.Initialize(8768, NULL))
	{
		printf("Initialize failed\n");
		return -1;
	}
	printf("tcpserver:%ld\n", sizeof(ws));
	while(1)
	{
		sleep(2);
	}
	return 0;
}

