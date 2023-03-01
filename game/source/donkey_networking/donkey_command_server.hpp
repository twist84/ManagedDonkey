#pragma once

#include "cseries/cseries.hpp"

#include <queue>
#include <mutex>
#include <winsock2.h>

class c_remote_command_server
{
public:
	c_remote_command_server();
	~c_remote_command_server();

	void start(short port);
	void stop();

private:
	void accept_loop();
	void client_loop(SOCKET client_socket);

	void send_string(SOCKET client_socket, c_static_string<4096> message);

	bool m_running;
	SOCKET m_listen_socket;
	std::queue<SOCKET> m_clients;
	std::mutex m_clients_mutex;
};

static c_remote_command_server command_server = {};

extern void patch_remote_command();

template<typename T>
std::queue<T> queue_pop(std::queue<T> list, T entry)
{
	std::queue<T> new_list;
	while (!list.empty())
	{
		T current_client = list.front();
		list.pop();
		if (current_client != entry)
		{
			new_list.push(current_client);
		}
	}
	return new_list;
}

