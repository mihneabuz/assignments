#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "link_emulator/lib.h"

#define HOST "127.0.0.1"
#define PORT 10000
#define BUFFER_SIZE 16

int main(int argc, char** argv) {
  init(HOST,PORT);
  msg t;

  // check if given cli argument
  if (argc < 2) {
    printf("[send] Eror no file specified\n");
    return -1;
  }

  // open file
  int to_send = open(argv[1], O_RDONLY);

  // first msg -> file name
  sprintf(t.payload, "%s", argv[1]);
  t.len = strlen(t.payload);
  send_message(&t);
  printf("[send] Sent file name\n");

  if (recv_message(&t) < 0) {
    perror("[send] Error sending file name");
    close(to_send);
    return -1;
  }
  else {
    printf("[send] Got response: %s\n", t.payload);
  }

  // get size
  unsigned int size = lseek(to_send, 0, SEEK_END);
  lseek(to_send, 0, SEEK_SET);

  // second msg -> file size
  sprintf(t.payload, "%d", size); 
  t.len = strlen(t.payload);
  send_message(&t);
  printf("[send] Sent file size\n");
  
  if (recv_message(&t) < 0) {
    perror("[send] Error sending file size");
    close(to_send);
    return -1;
  }
  else {
    printf("[send] Got response: %s\n", t.payload);
  }

  // next msgs -> file information
  int read_bytes;
  while ((read_bytes = read(to_send, t.payload, BUFFER_SIZE))) {
    if (read_bytes < 0) {
      perror("[send] Cannot read file");
      close(to_send);
      return -1;
    }
    t.len = read_bytes + 1;
    t.payload[t.len - 1] = 0;
    send_message(&t);
    printf("[send] Sent information of size <%d>\n", t.len - 1);

    if (recv_message(&t) < 0) {
      perror("[send] Error sending file information");
      close(to_send);
      return -1;
    }
    else {
      printf("[send] Got response: %s\n", t.payload);
    }
  }

  close(to_send);
  return 0;
}
