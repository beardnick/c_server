//#include "socket.h"
#include "socket/socket.h"
#include <arpa/inet.h>
#include <pthread.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <iostream>

long long number;

 void communicate(int conn) {
  int pid = fork();
  if (pid < 0) {
    perror("fork error");
    return;
  }
  if (pid > 0) {
    return;
  }
  char buffer[1024];
  while (1) {
    memset(buffer, 0, sizeof(buffer));
    int len = recv(conn, buffer, sizeof(buffer), 0);
    number++;
    if (strcmp(buffer, "exit\n") == 0 || strcmp(buffer, "") == 0) {
      //printf("client %d closed  connection\n", conn);
      break;
    }
    //printf("recv client %d: %s", conn, buffer);
    send(conn, buffer, len, 0);
  }
  close(conn);
}


void* count_request(void*){
    while(1){
        sleep(1);
        std::cout<<"request:"<<number<<std::endl;
        number = 0;
    }
}

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
  while (1) {
    int conn = accept(listen_fd, (struct sockaddr *)&client_addr, &length);
    if (conn < 0) {
      perror("connect");
      exit(1);
    }
    //printf("connect client %d\n", conn);
     communicate(conn);
  }
  close(listen_fd);
  pthread_exit(NULL);
}
