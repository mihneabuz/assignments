#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/tcp.h>

#define BUFFER_LEN 1000

// UDP protocol
struct publish_msg {
	char topic[50];
	unsigned char type;
	char data[BUFFER_LEN];
} __attribute__((packed));

enum {
	INT,
	SHORT_REAL,
	FLOAT,
	STRING
};

// Protocol used by clients to send requests to server
struct client_request {
	char ID[10];              // ID of client
	char type;                // Type of request
	char topic[50];           // Topic requested
	char flags;               // Additional flags
} __attribute__((packed));

// request types
enum {
	IDENTIFY,
	SUBSCRIBE,
	UNSUBSCRIBE,
	DISCONNECT
};

// flags
enum {
	SF = 1
};

// converts sockaddr_in to string rep of ip address
char* ip_tostring(struct sockaddr_in addr);
// converts msg to string representation
char* msg_tostring(struct sockaddr_in sender, char *msg);
