#pragma once

#include <assert.h>
#include <stdio.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>

#define BUFFER_SIZE 4096

#define REMOTE_IP "127.0.0.1"
#define REMOTE_PORT 1030

class c_raw_connection
{
public:
	c_raw_connection();
	virtual ~c_raw_connection();

	void initialize();
	void dispose();

	void set_remote_ip(char const* remote_ip);
	void set_remote_port(short remote_port);

	void connect();
	void update();

private:
	SOCKET m_socket;
	char m_remote_ip[INET_ADDRSTRLEN]{};
	short m_remote_port;
};

