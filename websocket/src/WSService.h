/*
 * WSService.h
 *
 *  Created on: 2016年12月8日
 *      Author: james
 */

#ifndef SRC_WSSERVICE_H_
#define SRC_WSSERVICE_H_

#include "TcpServer.h"
#include <map>
#include <string>
#include <vector>
#include "command_define.h"
using namespace std;
class WS_Service: public TcpServer {
public:
	WS_Service();
	virtual ~WS_Service();

	// websocket的握手协议的实现
	bool WS_HandShake(pServerData recvdata);
	// 握手结束后，接收到数据后的处理程序
	virtual bool OnReceive(pServerData recvdata);
	virtual unsigned char *ProcessService(unsigned char *buf,int inSize,int *outSize);
private:
	int parseHtmlHead(const char * buf);
	int buildHtmlHead();
	map<string,string> head;
	int mProtocolStatus;
	vector<string> HeadSplit(string buf);
	char *base64_encoder(const uint8_t * input, int length,bool with_new_line = 0);
	string trim(string& s);
	int makeFrame(int frame_type, unsigned char* msg, int msg_length, pServerData outData);
	int getFrame(const pServerData inData,unsigned char* out_buffer, int out_size, int* out_length);
	unsigned char Messagebuf[MAX_DATA_LEN];
};

#endif /* SRC_WSSERVICE_H_ */
