#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include "structs.h"

using namespace std;

struct Client {
	bool online;
	int socket;
	vector<shared_ptr<char*>> post_queue;
};

struct Subscriber {
	string ID;
	bool persistent;
};

struct Topic {
	vector<Subscriber> subscribers;
};

void usage(string name) {
	printf("Usage: %s [server port]\n", name.c_str());
	exit(EXIT_SUCCESS);
}

void error_exit(string error) {
	printf("Error: %s\n", error.c_str());
	exit(EXIT_FAILURE);
}

void send_post(int sock, shared_ptr<char*> post) {
	send(sock, *(post.get()), BUFFER_LEN, 0);
}

int main(int argc, char* argv[]) {
	setvbuf(stdout, NULL, _IONBF, BUFSIZ);

	if (argc != 2)
		usage(argv[0]);

	// populate socket information
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[1]));
	server_addr.sin_addr.s_addr = INADDR_ANY;

	// open TCP and UDP sockets
	int S_UDP = socket(AF_INET, SOCK_DGRAM, 0);
	int S_TCP = socket(AF_INET, SOCK_STREAM, 0);

	if (S_UDP < 0 || S_TCP < 0)
		error_exit("cannot create sockets!");

	// bind socket
	int res;
	res = bind(S_UDP, (sockaddr*) &server_addr, sizeof(server_addr));
	if (res < 0)
		error_exit("cannot bind UDP socket!");

	res = bind(S_TCP, (sockaddr*) &server_addr, sizeof(server_addr));
	if (res < 0)
		error_exit("cannot bind TCP socket!");

	// listen on TCP socket
	res = listen(S_TCP, 10);
	if (res < 0)
		error_exit("cannot listen on TCP socket!");

	// setup fd sets
	fd_set read_fds, tmp_fds;
	int fdmax;

	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	FD_SET(S_UDP, &read_fds);
	FD_SET(S_TCP, &read_fds);
	FD_SET(0, &read_fds);
	fdmax = (S_UDP > S_TCP ? S_UDP : S_TCP);

	// setup data structures
	unordered_map<string, Client> clients;
	unordered_map<string, Topic> topics;

	// create buffers
	struct sockaddr_in tmp_addr;
	socklen_t tmp_addr_len;
	char buffer[BUFFER_LEN];

	// main loop
	while (true) {

		// select
		tmp_fds = read_fds; 
		res = select(fdmax + 1, &tmp_fds, NULL, NULL, NULL);
		if (res < 0)
			error_exit("select failed!");
		
		// check stdin fd for "exit" command
		if (FD_ISSET(0, &tmp_fds)) {
			fgets(buffer, BUFFER_LEN - 1, stdin);
			if (strncmp(buffer, "exit", 4) == 0) {
				for (auto c : clients) {
						if (c.second.online)
							send(c.second.socket, "DISCONNECT_SIGNAL\n", 18, 0);
				}
				break;
			}
		}

		// check other fds
		for (int i = 1; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {

				if (i == S_TCP) {
					// got a new connection request
					int new_sock = accept(S_TCP, (sockaddr*) &tmp_addr, &tmp_addr_len);
					if (new_sock < 0)
						error_exit("cannot accept connection!");

					FD_SET(new_sock, &read_fds);
					if (new_sock > fdmax)
						fdmax = new_sock;
				}

				else if (i == S_UDP) {
					// got a new post (UDP msg)
					res = recvfrom(S_UDP, buffer, BUFFER_LEN, 0,
							           (sockaddr*) &tmp_addr, &tmp_addr_len);
					if (res < 0)
						error_exit("cannot read UDP socket!");
					buffer[res] = '\0';

					// extract msg information
					struct publish_msg *msg = (struct publish_msg*) buffer;
					string topic(msg->topic);

					// convert post to string
					shared_ptr<char*> post = make_shared<char*>(msg_tostring(tmp_addr, buffer));

					// find all subs and send msg
					if (topics.find(topic) != topics.end()) {
						auto &subs = topics[topic].subscribers;
						for (auto s : subs) {
							string ID = s.ID;

							// if client is online, forward post
							if (clients[ID].online)
								send_post(clients[ID].socket, post);

							// if client is oflline but SF is set, add msg to queue
							else if (s.persistent)
								clients[ID].post_queue.push_back(post);
						}
					}
				}

				else {
					// got a request from client
					res = recv(i, buffer, BUFFER_LEN, 0);
					if (res < 0)
						error_exit("cannot read message from client!");

					struct client_request *request = (client_request*)buffer;
					
					string ID = request->ID;
					if (request->type == IDENTIFY) {
						if (clients.find(ID) == clients.end()) {
							// first login with ID
							clients[ID].online = true;
							clients[ID].socket = i;
								
							getpeername(i, (sockaddr*) &tmp_addr, &tmp_addr_len);
							printf("New client %s connected from %s:%d.\n",
								   ID.c_str(), ip_tostring(tmp_addr), ntohs(tmp_addr.sin_port));

						} else {
							if (clients[ID].online) {
							  // if client is online, refuse connection and sent disconnect signal
								printf("Client %s already connected.\n", ID.c_str());
								send(i, "DISCONNECT_SIGNAL\n", 18, 0);
								
								// remove socket from fd set and close
								FD_CLR(i, &read_fds);	
								FD_CLR(i, &tmp_fds);
								close(i);
							}
							else {
								// set client online
								clients[ID].online = true;
								clients[ID].socket = i;

								// log client connection
								getpeername(i, (sockaddr*) &tmp_addr, &tmp_addr_len);
								printf("New client %s connected from %s:%d.\n",
									   ID.c_str(), ip_tostring(tmp_addr), ntohs(tmp_addr.sin_port));

								// send all missed posts from queue
								for (auto m : clients[ID].post_queue)
									send_post(i, m);

								// delete the post queue
								clients[ID].post_queue.erase(clients[ID].post_queue.begin(),
																		 clients[ID].post_queue.end());

							}
						}
					} else if (request->type == SUBSCRIBE) {
						// add client to subscribers of topic
						topics[request->topic].subscribers.push_back(
										{ID, request->flags == SF});

					} else if (request->type == UNSUBSCRIBE) {
						// remove client from subscribers of topic
						if (topics.find(request->topic) != topics.end()) {
							auto &subs = topics[request->topic].subscribers;

							// find ID in vector and delete it
							for (auto s = subs.begin(); s != subs.end(); s++) {
								if ((*s).ID == ID) {
									subs.erase(s);
									break;
								}
							}
						}
					} else if (request->type == DISCONNECT) {
						// set client to offline
						clients[ID].online = false;
						clients[ID].socket = 0;

						// remove sock from fd set and close socket
						FD_CLR(i, &read_fds);	
						FD_CLR(i, &tmp_fds);
						close(i);

						printf("Client %s disconnected.\n", ID.c_str());
					}
				}
			}
		}
	}

	// close sockets and exit
	close(S_UDP);
	close(S_TCP);
	return 0;
}
