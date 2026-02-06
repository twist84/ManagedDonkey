#include "main.hpp"

int main(int argc, char const* argv[])
{
	c_raw_connection raw_connection;

	char const* remote_ip = REMOTE_IP;
	short remote_port = REMOTE_PORT;

	if (argc == 3)
	{
		remote_ip = argv[1];
		remote_port = static_cast<short>(atol(argv[2]));
	}

	raw_connection.set_remote_ip(remote_ip);
	raw_connection.set_remote_port(remote_port);

	raw_connection.connect();
	raw_connection.update();

	return 0;
}

void prompt_print(char const* prefix = "")
{
	printf("%s> ", prefix);
	fflush(stdout);
}

long prompt_read(char* buffer, long max_buffer_length)
{
	fgets(buffer, max_buffer_length, stdin);
	buffer[strcspn(buffer, "\n")] = 0; // remove newline character
	return static_cast<long>(strlen(buffer));
}

c_raw_connection::c_raw_connection()
{
	initialize();
}

c_raw_connection::~c_raw_connection()
{
	dispose();
}

void c_raw_connection::initialize()
{
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
	{
		perror("WSAStartup failed");
		throw;
	}

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		perror("socket failed");
		WSACleanup();
		throw;
	}
}

void c_raw_connection::dispose()
{
	closesocket(m_socket);
	WSACleanup();
}

void c_raw_connection::set_remote_ip(char const* remote_ip)
{
	strcpy_s(m_remote_ip, remote_ip);
}

void c_raw_connection::set_remote_port(short remote_port)
{
	m_remote_port = remote_port;
}

void c_raw_connection::connect()
{
	SOCKADDR_IN server_addr;

	assert(m_remote_ip && m_remote_ip[0] != nullptr);
	assert(m_remote_port);

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(m_remote_ip);
	server_addr.sin_port = htons(m_remote_port);
	if (::connect(m_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		perror("connect failed");
		closesocket(m_socket);
		WSACleanup();
		throw;
	}
}

void c_raw_connection::update()
{
	char buffer[BUFFER_SIZE] = {};
	long len = 0;

	while (true)
	{
		prompt_print(m_remote_ip);
		len = prompt_read(buffer, BUFFER_SIZE);

		if (strcmp(buffer, "quit") == 0)
		{
			printf("Quitting...\n");
			break;
		}

		if (strcmp(buffer, "exit_game") == 0)
		{
			send(m_socket, buffer, len, 0);
			break;
		}

		if (send(m_socket, buffer, len, 0) == SOCKET_ERROR)
		{
			printf("Failed to send data to server. Error code: %d\n", WSAGetLastError());
			break;
		}

		len = recv(m_socket, buffer, sizeof(buffer), 0);
		if (len > 0 && len < sizeof(buffer))
		{
			buffer[len] = 0;
			printf("%s", buffer);
		}
	}
}

