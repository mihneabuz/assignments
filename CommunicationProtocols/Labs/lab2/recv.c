#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10001

int main(int argc, char** argv) {
  msg r;
  init(HOST,PORT);

  // receive filename
  if (recv_message(&r) < 0){
    perror("Receive message");
    return -1;
  }
  char* filename = malloc(strlen(r.payload) + 3);
  strcpy(filename + 3, r.payload);
  printf("[recv] Got filename: <%s>, sending ACK...\n", filename + 3);

  // Send ACK:
  sprintf(r.payload,"%s", "ACK");
  r.len = strlen(r.payload) + 1;
  send_message(&r);
  printf("[recv] ACK sent\n");

  // receive filesize
  if (recv_message(&r) < 0){
    perror("Receive message");
    return -1;
  }
  int size = atoi(r.payload);
  printf("[recv] Got filesize: <%d>, sending ACK...\n", size);

  // Send ACK:
  sprintf(r.payload,"%s", "ACK");
  r.len = strlen(r.payload) + 1;
  send_message(&r);
  printf("[recv] ACK sent\n");

  // Creating file
  filename[0] = 'n';
  filename[1] = 'e';
  filename[2] = 'w';
  int to_receive = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);

  // receive file information
  while (size > 0) {
    if (recv_message(&r) < 0){
      perror("Receive message");
      close(to_receive);
      return -1;
    }
    write(to_receive, r.payload, r.len - 1);
    printf("[recv] Got information of size <%d>, sending ACK...\n", r.len - 1);
    size = size - r.len + 1;
  
    // Send ACK:
    sprintf(r.payload,"%s", "ACK");
    r.len = strlen(r.payload) + 1;
    send_message(&r);
    printf("[recv] ACK sent\n");
  }

  close(to_receive);
  return 0;
}
