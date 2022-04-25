#include "structs.h"

const char* msg_identifier[] = {
	"INT",
	"SHORT_REAL",
	"FLOAT",
	"STRING"
};

char* ip_tostring(struct sockaddr_in addr) {
	struct in_addr aux;
	aux.s_addr = addr.sin_addr.s_addr; 
	return inet_ntoa(aux);
}

char* msg_tostring(struct sockaddr_in sender, char *msg) {
	char* str = new char[BUFFER_LEN];

	struct publish_msg *post = (struct publish_msg*)msg;

	if (post->type == STRING)
		sprintf(str, "%s:%d - %s - %s - %s\n",
				ip_tostring(sender), ntohs(sender.sin_port),
				post->topic, msg_identifier[post->type], post->data);
	else {
		char data[50];

		if (post->type == INT) {
			unsigned int x = htonl(*(int*)(post->data + 1));
			if (post->data[0])
				sprintf(data, "-%u", x);
			else
				sprintf(data, "%u", x);
		}
		else if (post->type == SHORT_REAL) {
			int x = htons(*(int*)(post->data));
			sprintf(data, "%.2f", (float)x / 100);
		}
		else if (post->type == FLOAT) {
			unsigned int x = htonl(*(int*)(post->data + 1));
			unsigned int e = post->data[5];
			unsigned int y = 1;
			while (e) {
				y *= 10;
				e--;
			}
			if (y == 1) {
				if (post->data[0])
					sprintf(data, "-%d", x);
				else
					sprintf(data, "%d", x);
			}
			else if (x < y) {
				int k = 0;
				if (post->data[0])
					data[k++] = '-';
					
				data[k++] = '0';
				data[k++] = '.';
				
				while (x < y) {
					data[k++] = '0';
					y /= 10;
				}
				sprintf(data + k - 1, "%d", x);
			}
			else {
				if (post->data[0])
					sprintf(data, "-%d.%d", x/y, x%y);
				else
					sprintf(data, "%d.%d", x/y, x%y);
			}
		}
		sprintf(str, "%s:%d - %s - %s - %s\n",
				ip_tostring(sender), ntohs(sender.sin_port),
				post->topic, msg_identifier[post->type], data);
	}
	return str;
}
