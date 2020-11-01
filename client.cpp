#include <stdio.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include "socket/socket.h"
#include <iostream>

int main(int argc, char *argv[]) {
  int client_fd = connect(std::string("127.0.0.1:9032"));
  if (client_fd == -1) {
    perror("connect");
    return 1;
  }
  char send_buf[1024], recv_buf[1024];
  while (1) {
    strcpy(send_buf, "hello\n");
    send(client_fd, send_buf, sizeof(send_buf), 0);
    recv(client_fd, recv_buf, sizeof(recv_buf), 0);
  }
  close(client_fd);
  std::cout<< "hello"<<std::endl;
  return 0;
}
