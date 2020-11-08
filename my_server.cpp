#include <algorithm>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket/socket.h"
#include <unistd.h>
#include <iostream>

void parseHttp(char* buffer){
    std::string content = std::string(buffer);
    std::cout<<content<<std::endl;
}

// 打开浏览器看效果
// http://127.0.0.1:8080
int main(int argc, char *argv[]) {
    int listen_fd = listen(":8080");
    if (listen_fd == -1){
        perror("listen");
    }
    sockaddr_in client_addr ;
    socklen_t length = sizeof(client_addr);
    int conn = accept(listen_fd,(sockaddr*)&client_addr,&length);
    if (conn == -1){
        perror("connect");
    }
    char buffer[1024];
    int len = recv(conn,&buffer,sizeof(buffer),0);
    if (len <= 0 ) {
        perror("read tcp data error");
    }
    parseHttp(buffer);
    close(conn);
    return 0;
}
