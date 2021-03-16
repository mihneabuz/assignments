#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "lib.h"
#include "checksum.c"

#define HOST "127.0.0.1"
#define PORT 10000

#define FILENAME "test.txt"

int w = 5;
int waitting_acks = 0;
int to_send = 10;

int send_msg(const msg *m) {
	if (waitting_acks < w && to_send > 0) {
		int res = send_message(m);
		if (res < 0) {
			perror("[SENDER] Send error. Exiting.\n");
			exit(-1);
		}
		waitting_acks ++;
		to_send --;
		printf("[send]: one sent! %d to go!\n", to_send);
		return 0;
	}
	return 1;
}

int recv_ack(msg *m) {
	if (waitting_acks >= w || to_send == 0) {
		int res = recv_message(m);
		if (res < 0) {
			perror("[SENDER] Receive error. Exiting.\n");
			exit(-1);
		}
		waitting_acks --;
		return 0;
	}
	return 1;
}


int main(int argc, char *argv[])
{
	msg t;
	int res;
	printf("[SENDER] Starting.\n");	
	init(HOST, PORT);

	char filename[30];
	strcpy(filename, FILENAME);

	int file = open(filename, O_RDONLY);
	int filename_size = strlen(filename);
	int size = lseek(file, 0, SEEK_END);
	lseek(file, 0, SEEK_SET);

	// declaring how many packets we are sending
	int packets = (filename_size + size) / MSG_SIZE + 4;
	
	sprintf(t.payload, "%d", packets);	
	t.len = strlen(t.payload);

	// sending number of packets 
	res = send_message(&t);
	if (res < 0) {
		printf("[Sender] Cannot connect. Exiting.\n");
		return -1;
	}
		
	res = recv_message(&t);
	if (res < 0) {
		perror("[SENDER] Receive error. Exiting.\n");
		return -1;
	}

	printf("[SENDER] Sending %d packets!\n", packets);
	to_send = packets;

	// send filename
	if (filename_size <= MSG_SIZE) {
		strcpy(t.payload, filename);
		t.len = filename_size;
		send_msg(&t);
		recv_ack(&t);
	}

	// send null packet to signal end of filename
	t.payload[0] = '\0';
	t.len = 1;
	send_msg(&t);
	recv_ack(&t);

	// send file contents
	char checksum[MSG_SIZE];
	memset(checksum, 0, MSG_SIZE);

	int read_bytes;
	while (to_send > 1) {
		// read MSG_SIZE bytes into payload
		read_bytes = read(file, t.payload, MSG_SIZE);
		if (read_bytes < 0) {
			perror("[send] Cannot read file");
			close(to_send);
			return -1;
		}
		t.len = read_bytes + 1;
		t.payload[t.len - 1] = 0;
		xor_checksum(checksum, t.payload);
		send_msg(&t);
	
		recv_ack(&t);
	}

	// sending checksum
	int sent_checksum = 0;
	while (!sent_checksum) {
		strncpy(t.payload, checksum, MSG_SIZE);	
		t.len = MSG_SIZE;
		if (!send_msg(&t))
			sent_checksum = 1;
		recv_ack(&t);
	}

	while (waitting_acks > 0)
		recv_ack(&t);

	printf("[SENDER] Job done, all sent.\n");
	return 0;
}
