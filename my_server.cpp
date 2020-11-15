#include <algorithm>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include "socket/socket.h"
#include <unistd.h>
#include <iostream>
#include <regex>
#include <sstream>
#include <fstream>

class Request{
public:
    std::string Protocol;
    std::string Method;
    std::string Path;
    std::vector<std::pair<std::string, std::string>> Headers;
    friend std::ostream &operator<<( std::ostream &output,
                                const  Request &req)
    {
        output<<req.Method<<" "<<req.Path<<std::endl;
        output<<"headers:"<<std::endl;
        for(auto h: req.Headers){
            output<<h.first<<":"<<h.second<<std::endl;
        }
        return output;
    }

private:
};


class Response{
public:
    std::string Protocol;
    int StatusCode;
    std::string Status;
    std::string Body;
};

// 静态网站的根路径
// 因为编译输出的目录是 bin,所以要使用这样的相对路径
// 为了简单，也可以使用绝对路径
std::string web_root= "../dist";

Request parseHttp(char* buffer){
    std::string content = std::string(buffer);

    // 使用换行符号划分成多行
    // https://www.zhihu.com/question/36642771
    std::regex sep("\n");
    std::vector<std::string> lines(std::sregex_token_iterator(content.begin(),content.end(),sep, -1),std::sregex_token_iterator());

    Request req;
    // auto是自动检测类型，这里可以自动检测到l为std::string类型,用于遍历vector等集合
    int empty_line = 0;
    for (int i = 0; i <lines.size(); ++i) {
        std::string l = lines[i];
        if (l == "\r"){
            // 空行,接下来是请求体
            empty_line = i;
            break;
        }
        // 以空格为分隔符，依次读取参数
        std::istringstream is(l);
        if (i == 0){
            // 请求头
            is>>req.Method;
            is>>req.Path;
            is>>req.Protocol;
            continue;
        }
        // 解析header
        std::string key, value;
        is>>key;
        is>>value;
        // 去掉多余的冒号
        key =key.replace(key.find(":"),1,"");
        // 将解析出来的头加入
        req.Headers.emplace_back(key,value);
    }
    // todo: 解析请求体，暂时不解析请求体
    return req;
}

Response handle(Request req){
    if (req.Path == "/"){
        // 如果是路径是/的话默认返回index.html页面
        req.Path = "/index.html";
    }
    Response resp;
    resp.Protocol = "HTTP/1.0";
    resp.StatusCode = 200;
    resp.Status = "OK";

    // 构建资源路径
    std::string resource_path = web_root +req.Path;
    // 读取文件内容到响应体中
    std::ifstream ifile;
    ifile.open(resource_path);
    // 一次性读取所有内容
    std::ostringstream tmp;
    tmp << ifile.rdbuf();
    resp.Body = tmp.str();

    ifile.close();
    return resp;
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
    for (;;) {
        // 循环接受http请求
        // 新建一个连接
        int conn = accept(listen_fd,(sockaddr*)&client_addr,&length);
        if (conn == -1){
            perror("connect");
        }
        char buffer[1024];
        int len = recv(conn,&buffer,sizeof(buffer),0);
        if (len <= 0 ) {
            perror("read tcp data error");
        }
        Request req = parseHttp(buffer);
        std::cout<<"request:"<<std::endl;
        std::cout<<req.Method<<" "<<req.Path<<std::endl;
        Response resp = handle(req);
        char buf[1024];
        sprintf(buf,"%s %d %s\n\r\n\r%s",resp.Protocol.c_str(),resp.StatusCode,resp.Status.c_str(),resp.Body.c_str());
        std::cout<<"response:"<<std::endl<<buf<<std::endl;
        send(conn,buf,strlen(buf),0);
        // http请求结束，直接断开连接
        // 如果不断开连接，你将看到浏览器不停地转圈圈
        close(conn);
    }
    return 0;
}
