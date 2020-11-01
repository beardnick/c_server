#include "socket/socket.h"
#include <algorithm>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

long long number;

void *count_request(void *) {
  while (1) {
    sleep(1);
    std::cout << "request:" << number << std::endl;
    number = 0;
  }
}

int communicate(int conn) {
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  int len = recv(conn, buffer, sizeof(buffer), 0);
  number++;
  //if (strcmp(buffer, "exit\n") == 0 || strcmp(buffer, "") == 0) {
  //  close(conn);
  //  return -1;
  //}
   //printf("recv client %d: %s", conn, buffer);
  send(conn, buffer, len, 0);
  return 0;
}

// 10 0000 requests per second
//! g++ -std=c++11 select_echo.cc ../socket/socket.cc -o select
int main(int argc, char *argv[]) {
  struct sockaddr_in client_addr;
  socklen_t length = sizeof(client_addr);
  int listen_fd = listen(":9032");
  if (listen_fd == -1) {
    perror("faile to listen");
    exit(EXIT_FAILURE);
  }
  pthread_t temp;
  pthread_create(&temp, NULL, count_request, NULL);
  auto clients = std::vector<int>();
  struct timeval tv = {0, 100};
  int max_fd = listen_fd;
  clients.push_back(listen_fd);
  while (1) {
    fd_set read_set;
    fd_set write_set;
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    for (auto c : clients) {
      FD_SET(c, &read_set);
    }
    int retval = select(max_fd + 1, &read_set, &write_set, NULL, &tv);
    if (retval < 0) {
      perror("select()");
      break;
    }
    if (retval == 0) {
       //std::cout << "read timeout" << std::endl;
      continue;
    }
    /* #IMP: 20-07-03 这里在遍历clients的同时修改clients，导致端错误 */
    for (int n = clients.size() -1 ; n >= 0; n --) {
        auto c = clients[n];
      if (FD_ISSET(c, &read_set)) {
        // 服务器的fd被set了，说明有新的连接进来
        if (c == listen_fd) {
          int conn =
              accept(listen_fd, (struct sockaddr *)&client_addr, &length);
          if (conn < 0) {
            perror("connect");
            std::cout << "clients:" << clients.size() << std::endl;
            std::cout << "max_fd:" << max_fd << std::endl;
            continue;
          }
          max_fd = conn > max_fd ? conn : max_fd;
          clients.push_back(conn);
          continue;
        }
        if (communicate(c) == -1) {
          auto iter = clients.begin() + n;
          if (iter != clients.end()) {
            clients.erase(iter);
          }
        }
      }
    }
  }
  close(listen_fd);
  pthread_exit(NULL);
}
