#include "donkey_networking/donkey_command_server.hpp"

#include "cseries/console.hpp"
#include "interface/user_interface_networking.hpp"
#include "memory/module.hpp"
#include "networking/transport/transport.hpp"

#include <assert.h>
#include <thread>

static void __cdecl remote_command_initialize()
{
	command_server.start(11770);
}

static void __cdecl remote_command_dispose()
{
	command_server.stop();
}

bool patch_pointer(module_address address, const void* pointer)
{
	dword protect;
	if (!VirtualProtect(address.pointer, sizeof(void*), PAGE_READWRITE, &protect))
		return false;

	memcpy(address.pointer, &pointer, sizeof(void*));

	if (!VirtualProtect(address.pointer, sizeof(void*), protect, &protect))
		return false;

	return true;
}


void patch_remote_command()
{
	patch_pointer({ .address = 0x01655B90 }, remote_command_initialize);
	patch_pointer({ .address = 0x01655B94 }, remote_command_dispose);
}

c_remote_command_server::c_remote_command_server() :
	m_running(false),
	m_listen_socket(INVALID_SOCKET)
{
	//WSADATA wsa_data;
	//int result = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	//if (result != 0)
	//{
	//	c_console::write_line("Failed to initialize Winsock.");
	//	return;
	//}
}

c_remote_command_server::~c_remote_command_server()
{
	stop();
	//WSACleanup();
}

void c_remote_command_server::start(short port)
{
	if (m_running)
	{
		return;
	}

	// Create the socket
	m_listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_listen_socket == INVALID_SOCKET)
	{
		return;
	}

	// Bind the socket to a local address and port
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = INADDR_ANY;
	service.sin_port = htons(port);
	int result = bind(m_listen_socket, (sockaddr*)&service, sizeof(service));
	if (result == SOCKET_ERROR)
	{
		stop();
		return;
	}

	// Listen for incoming connections
	result = listen(m_listen_socket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		stop();
		return;
	}

	m_running = true;

	// Start a new thread to handle incoming connections
	std::thread thread(&c_remote_command_server::accept_loop, this);
	thread.detach();

	//accept_loop();
}

void c_remote_command_server::stop()
{
	if (!m_running)
	{
		return;
	}

	m_running = false;

	// Stop accepting new connections
	closesocket(m_listen_socket);

	// Disconnect all clients
	while (!m_clients.empty())
	{
		SOCKET clientSocket = m_clients.front();
		m_clients.pop();
		shutdown(clientSocket, SD_BOTH);
		closesocket(clientSocket);
	}
}

void c_remote_command_server::accept_loop()
{
	while (m_running)
	{
		// Wait for a new client to connect
		SOCKET client_socket = accept(m_listen_socket, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
		{
			// Ignore errors caused by shutting down the socket
			if (m_running)
			{
				c_console::write_line("Failed to accept client connection: %d", WSAGetLastError());
			}
			continue;
		}

		// Add the new client to the queue
		{
			std::lock_guard<std::mutex> lock(m_clients_mutex);
			m_clients.push(client_socket);
		}

		// Start a new thread to handle messages from the client
		std::thread thread(&c_remote_command_server::client_loop, this, client_socket);
		thread.detach();

		//client_loop(client_socket);
	}
}

void c_remote_command_server::client_loop(SOCKET client_socket)
{
	// Receive messages from the client
	while (m_running)
	{
		c_static_string<4096> message;
		char buffer[4096];

		long result = recv(client_socket, buffer, sizeof(buffer), 0);
		assert(result < sizeof(buffer));

		if (result > 0)
		{
			buffer[result] = '\0';
			message.append(buffer);

			// Handle messages
			if (message.starts_with("test:"))
			{
				c_console::write_line("test command sent");

				message.set("test command response");
				send_string(client_socket, message);
			}
			else
			{
				c_console::write_line("unknown command sent '%s'", message.get_string());

				message.set("unknown command");
				send_string(client_socket, message);
			}
		}
		else if (result == 0)
		{
			// The client has disconnected
			break;
		}
		else
		{
			c_console::write_line("Failed to receive message from client: %d", WSAGetLastError());
			break;
		}
	}

	// Remove the client from the queue
	{
		std::lock_guard<std::mutex> lock(m_clients_mutex);
		m_clients = queue_pop(m_clients, client_socket);
	}

	// Disconnect the client
	shutdown(client_socket, SD_BOTH);
	closesocket(client_socket);
}

void c_remote_command_server::send_string(SOCKET client_socket, c_static_string<4096> message)
{
	// Send the message to the client
	int result = send(client_socket, message.get_string(), message.length(), 0);
	if (result == SOCKET_ERROR)
	{
		c_console::write_line("Failed to send message to client: %d", WSAGetLastError());
	}
}

