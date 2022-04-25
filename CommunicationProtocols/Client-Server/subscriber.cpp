#include "structs.h"
#include <string>

using namespace std;

string ID;

void usage(string name) {
	printf("Usage: %s [ID_client] [server_ip] [server_port]\n", name.c_str());
	exit(EXIT_SUCCESS);
}

void error_exit(string error) {
	printf("Error: %s\n", error.c_str());
	exit(EXIT_FAILURE);
}

void send_client_request(int sock, char type, string topic, char flags) {
	// sends a message to sock using client_request protocol
	struct client_request id_request;
	memcpy(id_request.ID, ID.c_str(), 10);
	id_request.type = type;
	strcpy(id_request.topic, topic.c_str());
	id_request.topic[topic.length()] = '\0';
	id_request.flags = flags;

	int res = send(sock, &id_request, sizeof(client_request), 0);
	if (res < 0)
		error_exit("cannot send request!");
}

int main(int argc, char *argv[]) {
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);

	if (argc < 4)
		usage(argv[0]);

	ID = argv[1];

	// populate socket information
	struct sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[3]));
	serv_addr.sin_addr.s_addr = inet_addr(argv[2]);

	// create TCP socket
	int serv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (serv_sock < 0)
		error_exit("cannot create socket!");

	// connect to server
	int res = connect(serv_sock, (sockaddr*) &serv_addr, sizeof(serv_addr));
	if (res < 0)
		error_exit("cannot connect to server!");

	// send id to server
	send_client_request(serv_sock, IDENTIFY, "", 0);
	
	// setup fd sets
	fd_set read_fds, tmp_fds;

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	FD_SET(serv_sock, &read_fds);
	FD_SET(0, &read_fds);

	char buffer[BUFFER_LEN];
	while (1) {

		tmp_fds = read_fds; 
		res = select(serv_sock + 1, &tmp_fds, NULL, NULL, NULL);
		if (res < 0)
			error_exit("select failed!");
		
		if (FD_ISSET(serv_sock, &tmp_fds)) {
			// got msg from server
			res = recv(serv_sock, buffer, sizeof(buffer), 0);
			if (res < 0)
				error_exit("cannot read from socket!");

			if (strstr(buffer, "DISCONNECT_SIGNAL") != nullptr) {
				break;
			}
			printf("%s", buffer);
		}

		if (FD_ISSET(0, &tmp_fds)) {
			// got msg on stdin
			fgets(buffer, BUFFER_LEN - 1, stdin);
			if (strncmp(buffer, "exit", 4) == 0) {
				send_client_request(serv_sock, DISCONNECT, "", 0);
				break;
			}

			else if (strncmp(buffer, "subscribe", 9) == 0) {
				// send subscribe request
				char flags = 0;
				if (buffer[strlen(buffer) - 2] == '1')
					flags |= SF;

				string topic = string(buffer + 10);
				topic = topic.substr(0, topic.find(' '));
				send_client_request(serv_sock, SUBSCRIBE, topic, flags);

				printf("Subscribed to topic.\n");
			}

			else if (strncmp(buffer, "unsubscribe", 11) == 0) {
				// send unsubscribe request
				string topic = string(buffer + 12);
				topic = topic.substr(0, topic.length() - 1);
				send_client_request(serv_sock, UNSUBSCRIBE, topic, 0);

				printf("Unsubscribed to topic.\n");
			}
		}
	}

	close(serv_sock);
	return 0;
}
