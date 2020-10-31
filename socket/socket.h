/*
 * http.h
 * Copyright (C) 2020 mac <mac@HUGOCHENG-MB0>
 *
 * Distributed under terms of the MIT license.
 */

/*
 * socket.h
 * Copyright (C) 2020 mac <mac@HUGOCHENG-MB0>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SOCKET_H
#define SOCKET_H

#include <string>

int listen(std::string address);

int connect(std::string address);

int select_listen(std::string address);

int select_connect(std::string address);

int i4_address(std::string address, struct sockaddr_in *addr);

#endif /* !SOCKET_H */
