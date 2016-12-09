/*
 * WSService.cpp
 *
 *  Created on: 2016年12月8日
 *      Author: james
 */

#include "WSService.h"
#include "command_define.h"
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <stdio.h>
#include <memory.h>
#include <openssl/buffer.h>
#include <time.h>
WS_Service::WS_Service() {
	// TODO Auto-generated constructor stub
	mProtocolStatus = HANDSHAKE_UNINITIALIZE;
}

WS_Service::~WS_Service() {
	// TODO Auto-generated destructor stub
}

vector<string> WS_Service::HeadSplit(string buf)
{
  std::string::size_type pos;
  std::vector<std::string> result;
  string pattern="\r\n";

  int size=buf.size();

  for(int i=0; i<size; i++)
  {
    pos=buf.find(pattern,i);
    if(pos<size)
    {
      string s = buf.substr(i,pos-i);
      result.push_back(s);
      i=pos+pattern.size()-1;
    }
  }
  return result;
}

bool WS_Service::WS_HandShake(pServerData recvdata)
{
	char response[512] = {0};
	char date[64];
	char magic[]="258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
	char request[64];
	unsigned char p[64];
	char * hashvalue;
	parseHtmlHead((const char *)recvdata->buf);
	if(head["Sec-WebSocket-Key"] == "")
	{
		mProtocolStatus = HANDSHAKE_PROTOCOL_REQUEST_FAIL;
		printf("Request is not websocket protocol\n");
		return false;
	}
	if(head["Upgrade"].compare("websocket") != 0)
	{
		mProtocolStatus = HANDSHAKE_PROTOCOL_REQUEST_FAIL;
		printf("Request is not websocket protocol\n");
		return false;
	}

	if(head["Sec-WebSocket-Version"].compare("13") != 0)
	{
		mProtocolStatus = HANDSHAKE_PROTOCOL_REQUEST_FAIL;
		printf("Request is not websocket protocol\n");
		return false;
	}

	strcpy(request,head["Sec-WebSocket-Key"].c_str());
	SHA_CTX ctx;
	SHA1_Init(&ctx);
	SHA1_Update(&ctx, request,strlen(request));
	SHA1_Update(&ctx, magic,strlen(magic));
	SHA1_Final(p, &ctx);

	hashvalue = base64_encoder(p,20,0);
	strcat(response,response1);
	strcat(response,hashvalue);
	strcat(response,response2);
	printf("%s\n\n",response);
	SendData((unsigned char *)response,strlen(response),recvdata->socket);
	mProtocolStatus = HANDSHAKE_PROTOCOL_SUCCESS;
	free(hashvalue);
	return true;
}


bool WS_Service::OnReceive(pServerData recvdata)
{
	int retSize,frameType;
	unsigned char * outBuf;
	if (HANDSHAKE_UNINITIALIZE == mProtocolStatus)
		return WS_HandShake(recvdata);
	else
		memset(Messagebuf,0,MAX_DATA_LEN);
		frameType = getFrame(recvdata,Messagebuf,MAX_DATA_LEN,&retSize);
		outBuf = ProcessService(Messagebuf,retSize,&retSize);
		ServerData sendData;
		sendData.socket = recvdata->socket;
		retSize = makeFrame(frameType,outBuf,retSize,&sendData);
		sendData.nLen = retSize;
		SendData(sendData.buf,sendData.nLen,sendData.socket);
		return true;
}

unsigned char *WS_Service::ProcessService(unsigned char *buf,int inSize,int *outSize)
{
	return buf;
}
int WS_Service::parseHtmlHead(const char * buf)
{
	vector<string> lines;
	string keyword,value;
	int pos;
	//printf("buffer content \n%s\n",buf);
	head.clear();

	lines = HeadSplit(buf);
	auto it=lines.begin();
	for(;it!=lines.end();it++)
	{
		keyword = *it;
		pos = keyword.find(":");
		if(pos <0)
			continue;
		value = keyword.substr(pos + 1,keyword.length() - pos );
		value = trim(value);
		keyword = keyword.substr(0,pos);
		head[keyword] = value;
		//printf("%s<->%s\n",keyword.c_str(),value.c_str());
	}
	return 0;
}

char *WS_Service::base64_encoder(const uint8_t * input, int length,
		bool with_new_line) {
	BIO * bmem = NULL;
	BIO * b64 = NULL;
	BUF_MEM * bptr = NULL;

	b64 = BIO_new(BIO_f_base64());
	if (!with_new_line) {
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	}
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, input, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);

	char * buff = new char[bptr->length + 1];
	memcpy(buff, bptr->data, bptr->length);
	buff[bptr->length] = 0;
	BIO_free_all(b64);
	return buff;
}

string WS_Service::trim(string& s)
{
	char whitespace[] = " \t\r\n";
	string::size_type pos = s.find_last_not_of(whitespace);
	if(pos != string::npos) {
		s.erase(pos + 1);
		pos = s.find_first_not_of(whitespace);
		if(pos != string::npos) s.erase(0, pos);
	}
	else {
		return NULL;
	}
	return s;
}

int WS_Service::makeFrame(int frame_type, unsigned char* msg, int msg_length, pServerData outData)
{
	unsigned char* buffer = outData->buf;
	int pos = 0;
	int size = msg_length;
	buffer[pos++] = (unsigned char)frame_type; // text frame

	if(size <= 125) {
		buffer[pos++] = size;
	}
	else if(size <= 65535) {
		buffer[pos++] = 126; //16 bit length follows
		buffer[pos++] = (size >> 8) & 0xFF; // leftmost first
		buffer[pos++] = size & 0xFF;
	}
	else { // >2^16-1 (65535)
		buffer[pos++] = 127; //64 bit length follows
		// write 8 bytes length (significant first)
		// since msg_length is int it can be no longer than 4 bytes = 2^32-1
		// padd zeroes for the first 4 bytes
		for(int i=3; i>=0; i--) {
			buffer[pos++] = 0;
		}
		// write the actual 32bit msg_length in the next 4 bytes
		for(int i=3; i>=0; i--) {
			buffer[pos++] = ((size >> 8*i) & 0xFF);
		}
	}
	memcpy((void*)(buffer+pos), msg, size);
	return (size+pos);
}

int  WS_Service::getFrame(const pServerData inData,unsigned char* out_buffer, int out_size, int* out_length)
{
	//printf("getTextFrame()\n");
	int in_length = inData->nLen;
	unsigned char * in_buffer = inData->buf;
	if(in_length < 3) return INCOMPLETE_FRAME;

	unsigned char msg_opcode = in_buffer[0] & 0x0F;
	unsigned char msg_fin = (in_buffer[0] >> 7) & 0x01;
	unsigned char msg_masked = (in_buffer[1] >> 7) & 0x01;

	// *** message decoding

	int payload_length = 0;
	int pos = 2;
	int length_field = in_buffer[1] & (~0x80);
	unsigned int mask = 0;

	//printf("IN:"); for(int i=0; i<20; i++) printf("%02x ",buffer[i]); printf("\n");

	if(length_field <= 125) {
		payload_length = length_field;
	}
	else if(length_field == 126) { //msglen is 16bit!
		payload_length = in_buffer[2] + (in_buffer[3]<<8);
		pos += 2;
	}
	else if(length_field == 127) { //msglen is 64bit!
		payload_length = in_buffer[2] + (in_buffer[3]<<8);
		pos += 8;
	}
	//printf("PAYLOAD_LEN: %08x\n", payload_length);
	if(in_length < payload_length+pos) {
		return INCOMPLETE_FRAME;
	}

	if(msg_masked) {
		mask = *((unsigned int*)(in_buffer+pos));
		//printf("MASK: %08x\n", mask);
		pos += 4;

		// unmask data:
		unsigned char* c = in_buffer+pos;
		for(int i=0; i<payload_length; i++) {
			c[i] = c[i] ^ ((unsigned char*)(&mask))[i%4];
		}
	}

	if(payload_length > out_size) {
		//TODO: if output buffer is too small -- ERROR or resize(free and allocate bigger one) the buffer ?
	}

	memcpy((void*)out_buffer, (void*)(in_buffer+pos), payload_length);
	out_buffer[payload_length] = 0;
	*out_length = payload_length+1;

	//printf("TEXT: %s\n", out_buffer);

	if(msg_opcode == 0x0) return (msg_fin)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME; // continuation frame ?
	if(msg_opcode == 0x1) return (msg_fin)?TEXT_FRAME:INCOMPLETE_TEXT_FRAME;
	if(msg_opcode == 0x2) return (msg_fin)?BINARY_FRAME:INCOMPLETE_BINARY_FRAME;
	if(msg_opcode == 0x9) return PING_FRAME;
	if(msg_opcode == 0xA) return PONG_FRAME;

	return ERROR_FRAME;
}
