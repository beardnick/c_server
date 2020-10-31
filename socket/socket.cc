/*
 * http.cc
 * Copyright (C) 2020 mac <mac@HUGOCHENG-MB0>
 *
 * Distributed under terms of the MIT license.
 */

#include "socket.h"
#include <arpa/inet.h>
#include <iostream>
#include <regex>
#include <string>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>

int i4_address(std::string address, struct sockaddr_in *addr) {
  std::regex remote_rgx =
      std::regex("([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+):([0-9]+)");
  std::regex local_rgx = std::regex(":([0-9]+)");
  bool remote_port = std::regex_match(address, remote_rgx);
  bool local_port = std::regex_match(address, local_rgx);
  if (!local_port && !remote_port) {
    fprintf(stderr, "invalid address: %s\n", address.c_str());
    return -1;
  }
  if (local_port) {
    address = "0.0.0.0" + address;
  }
  addr->sin_family = AF_INET;
  std::smatch matches;
  std::regex_search(address, matches, remote_rgx);
  addr->sin_port = htons(std::stoi(matches[2].str()));
  addr->sin_addr.s_addr = inet_addr(matches[1].str().c_str());
  return 0;
}

int listen(std::string address) {
  struct sockaddr_in server_sockaddr;
  if (i4_address(address, &server_sockaddr) == -1) {
    fprintf(stderr, "ipv4 address error\n");
    return -1;
  }
  int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0) {
    perror("create socket");
    return -1;
  }
  if (bind(listen_fd, (struct sockaddr *)&server_sockaddr,
           sizeof(server_sockaddr)) == -1) {
    perror("bind");
    return -1;
  }
  if (listen(listen_fd, 128) == -1) {
    perror("listen");
    return -1;
  }
  std::cout << "listen on " << address << std::endl;
  return listen_fd;
}

int connect(std::string address) {
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0) {
    perror("create socket");
    return -1;
  }
  struct sockaddr_in server_addr;
  if (i4_address(address, &server_addr) == -1) {
    fprintf(stderr, "ipv4 address error\n");
    return -1;
  }
  if (connect(client_fd, (struct sockaddr *)&server_addr,
              sizeof(server_addr)) == -1) {
    perror("connect");
    return -1;
  }
  return client_fd;
}

int select_listen(std::string address) { return 1; }

void test() {
  std::string address = std::string("127.0.0.1:8888");
  std::regex rgx = std::regex("([0-9]+\\.[0-9]+\\.[0-9]+\\.[0-9]+):([0-9]+)");
  std::smatch matches;
  std::regex_search(address, matches, rgx);
  std::cout << matches[1].str() << std::endl;
  std::cout << matches[2].str() << std::endl;
}
