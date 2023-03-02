#include "donkey_networking/donkey_command_server.hpp"

#include "cseries/console.hpp"
#include "game/game_results.hpp"
#include "interface/user_interface_networking.hpp"
#include "memory/module.hpp"
#include "networking/tools/remote_command.hpp"
#include "networking/transport/transport.hpp"

#include <assert.h>
#include <thread>
#include <WS2tcpip.h>

void patch_remote_command()
{
	patch_pointer({ .address = 0x01655B90 }, remote_command_initialize);
	patch_pointer({ .address = 0x01655B94 }, remote_command_dispose);
}

c_static_string<32> sock2string(SOCKET sock)
{
	c_static_string<32> address_result;

	sockaddr_in addr;
	int len = sizeof(addr);

	int result = getsockname(sock, (sockaddr*)&addr, &len);
	if (result != 0)
	{
		address_result.set("<unknown client>");
	}
	else
	{
		// access the IP address and port number
		char ip_buffer[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &addr.sin_addr, ip_buffer, INET_ADDRSTRLEN);
		word port = ntohs(addr.sin_port);

		address_result.print("%s:%d", ip_buffer, port);
	}

	return address_result;
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

#define SOCKET2ADDRESS(sock) sock2string((sock)).get_string()

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
		else
		{
			c_console::write_line("New client connected: '%s'", SOCKET2ADDRESS(client_socket));
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
		assert(result == 0xFFFFFFFF || result < sizeof(buffer));

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
			else if (message.starts_with("quit:"))
			{
				c_console::write_line("quit command sent");

				message.set("game quit");
				send_string(client_socket, message);

				//exit('quit');
			}
			if (message.starts_with("game_results:"))
			{
				c_console::write_line("game_results command sent");
				message.set("\n");
				s_game_matchmaking_options& matchmaking_options = g_current_game_results.matchmaking_options;
				{
					message.append("matchmaking_options:\n");
					message.append_print("  hopper_identifier:            %04X\n", matchmaking_options.hopper_identifier);
					message.append_print("  xlast_index:                  %d\n", matchmaking_options.xlast_index);
					message.append_print("  is_ranked:                    %s\n", matchmaking_options.is_ranked ? "true" : "false");
					message.append_print("  team_game:                    %s\n", matchmaking_options.team_game ? "true" : "false");
					message.append_print("  hopper_name:                  %ls\n", matchmaking_options.hopper_name);
					message.append_print("  draw_probability:             %d\n", matchmaking_options.draw_probability);
					message.append_print("  beta:                         %.3f\n", matchmaking_options.beta);
					message.append_print("  tau:                          %.3f\n", matchmaking_options.tau);
					message.append_print("  experience_base_increment:    %d\n", matchmaking_options.experience_base_increment);
					message.append_print("  experience_penalty_decrement: %d\n", matchmaking_options.experience_penalty_decrement);
					message.append("\n");
				}

				s_game_results_game_description& game_description = g_current_game_results.game_description;
				{
					message.append("game_description:\n");
					message.append_print("  team_game:          %s\n", game_description.team_game ? "true" : "false");
					message.append_print("  game_instance:      %llX\n", game_description.game_instance);
					message.append_print("  game_variant:       %s\n", game_description.game_variant.m_base_variant.m_name);
					message.append_print("  map_variant_name:   %ls\n", game_description.map_variant_name);
					message.append_print("  map_id:             %d\n", game_description.map_id);
					message.append_print("  scenario_path:      %s\n", game_description.scenario_path.get_string());
					message.append_print("  started:            %s\n", game_description.started ? "true" : "false");
					message.append_print("  start_time:         %d\n", game_description.start_time);
					message.append_print("  finished:           %s\n", game_description.finished ? "true" : "false");
					message.append_print("  finish_time:        %d\n", game_description.finish_time);
					message.append_print("  simulation_aborted: %s\n", game_description.simulation_aborted ? "true" : "false");
					message.append("\n");
				}

				c_static_array<s_game_results_player_data, 16>& players = g_current_game_results.players;
				{
					message.append("players:\n");

					for (long i = 0; i < 16; i++)
					{
						s_game_results_player_data& player = players[i];
						if (player.player_exists)
						{
							message.append_print("  player: %d\n", i);
							message.append_print("    machine_index:     %d\n", player.machine_index);
							message.append_print("    player_identifier: %02X:%02X:%02X:%02X:%02X:%02X:%02X:%02X\n", player.data[0], player.data[1], player.data[2], player.data[3], player.data[4], player.data[5], player.data[6], player.data[7]);
							message.append_print("    service_tag:       %ls\n", player.configuration.host.appearance.service_tag.get_string());
							message.append_print("    name:              %ls\n", player.configuration.host.name.get_string());
							message.append_print("    team_index:        %d\n", player.configuration.host.team_index);
							message.append_print("    player_standing:   %d\n", player.player_standing);
							message.append_print("    player_score:      %d\n", player.player_score);
							message.append("\n");
						}
					}
				}

				c_static_array<s_game_results_team_data, 16>& teams = g_current_game_results.teams;
				{
					message.append("teams:\n");

					for (long i = 0; i < 16; i++)
					{
						s_game_results_team_data& team = teams[i];
						if (team.team_exists)
						{
							message.append_print("  team: %d\n", i);
							message.append_print("    team_standing: %d\n", team.team_standing);
							message.append_print("    team_score:    %d\n", team.team_score);
							message.append("\n");
						}
					}
				}

				// #TODO: statistics
				// #TODO: events

				c_static_array<s_game_results_machine_data, 17>& machines = g_current_game_results.machines;
				{
					message.append("machines:\n");

					for (long i = 0; i < 16; i++)
					{
						s_game_results_machine_data& machine = machines[i];
						if (machine.machine_exists)
						{
							message.append_print("  machine: %d\n", i);
							message.append_print("  machine_identifier:     %04X:%04X:%04X:%04X\n", machine.machine_identifier.parts[0], machine.machine_identifier.parts[1], machine.machine_identifier.parts[2], machine.machine_identifier.parts[3]);
							message.append_print("  machine_connected:      %s\n", machine.machine_connected ? "true" : "false");
							message.append_print("  machine_host:           %s\n", machine.machine_host ? "true" : "false");
							message.append_print("  machine_initial_host:   %s\n", machine.machine_initial_host ? "true" : "false");
							message.append_print("  machine_voluntary_quit: %s\n", machine.machine_voluntary_quit ? "true" : "false");
							message.append("\n");
						}
					}
				}

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
			c_console::write_line("Failed to receive message from client: `%s` %d", SOCKET2ADDRESS(client_socket), WSAGetLastError());
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
		c_console::write_line("Failed to send message to client: `%s` %d", SOCKET2ADDRESS(client_socket), WSAGetLastError());
	}
}

