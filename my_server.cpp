
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket/socket.h"

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
    printf("%s",buffer);
    return 0;
}
