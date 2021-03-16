#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"
#include "checksum.c"

#define HOST "127.0.0.1"
#define PORT 10001

int main(void)
{
	msg r;
	int i, res;

	printf("[RECEIVER] Starting.\n");
	init(HOST, PORT);

	// get number of packets
	res = recv_message(&r);
	if (res < 0) {
		perror("[RECEIVER] Receive error. Exiting.\n");
		return -1;
	}

	res = send_message(&r);
	if (res < 0) {
		perror("[RECEIVER] Send ACK error. Exiting.\n");
		return -1;
	}

	int packets = atoi(r.payload);
	i = 0;
	printf("[RECEIVER] Receiving %d packets.\n", packets);

	char filename[30];
	memset(filename, 0, 30);

	// get filename
	res = recv_message(&r);
	if (res < 0) {
		perror("[RECEIVER] Receive error. Exiting.\n");
		return -1;
	}
	i++;
	printf("[recv]: %d received!\n", i);

	while (r.len != 1 && r.payload[0] != '\0') {
		strcat(filename + 3, r.payload);
		res = send_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Send ACK error. Exiting.\n");
			return -1;
		}
		
		res = recv_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Receive error. Exiting.\n");
			return -1;
		}
		i++;
		printf("[recv]: %d received!\n", i);
	}

	res = send_message(&r);
	if (res < 0) {
		perror("[RECEIVER] Send ACK error. Exiting.\n");
		return -1;
	}

	// prepend filename with "new"
	filename[0] = 'n';
	filename[1] = 'e'; 
	filename[2] = 'w';
	printf("[RECEIVER] Receiving file %s...\n", filename + 3);
  	int file = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);

	// receive file contents
	char checksum[MSG_SIZE];
	memset(checksum, 0, MSG_SIZE);

	for (; i < packets - 1; i++) {
		res = recv_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Receive error. Exiting.\n");
			return -1;
		}
		printf("[recv]: %d received!\n", i + 1);
		xor_checksum(checksum, r.payload);
		write(file, r.payload, r.len - 1);

		res = send_message(&r);
		if (res < 0) {
			perror("[RECEIVER] Send ACK error. Exiting.\n");
			return -1;
		}
	}

	// receive checksum
	res = recv_message(&r);
	if (res < 0) {
		perror("[RECEIVER] Receive error. Exiting.\n");
		return -1;
	}
	printf("[recv]: %d received!\n", i + 1);
	
	res = send_message(&r);
	if (res < 0) {
		perror("[RECEIVER] Send ACK error. Exiting.\n");
		return -1;
	}

	// verify_checksum
	if (verify_checksum(checksum, r.payload))
		printf("[RECEIVER] Finished receiving, checksum correct!\n");
	else {
		printf("[RECEIVER] Receive error. checksum wrong\n");
		return -1;
	}

	return 0;
}
