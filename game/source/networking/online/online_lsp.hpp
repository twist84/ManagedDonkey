#pragma once

#include "cseries/cseries.hpp"

struct s_server_connect_info
{
	c_static_string<16> ip_address;
	word port;
	c_static_string<200> description;
};
static_assert(sizeof(s_server_connect_info) == 0xDA);

enum e_client_state
{
	_client_state_none = 0

	// #TODO: map the rest
	//...
};

struct c_online_lsp_manager
{
	static long __fastcall acquire_server(c_online_lsp_manager* _this, void* unused, long service_type, long* connection_token_out, long* ip_address, unsigned short* port, char const* service_description);

	struct s_server_data
	{
		long currently_activated_server_index;
		dword ip_address;
		word port;
	};
	static_assert(sizeof(s_server_data) == 0xC);

	struct s_client_data
	{
		// e_client_state
		long client_state;

		long service_type;
		long connection_token;

		c_static_string<48> description;
	};
	static_assert(sizeof(s_client_data) == 0x3C);

	bool m_service_available;
	bool m_crash_mode;
	c_static_array<s_server_data, 9> m_service;
	dword lsp_search_start_time;
	dword lsp_search_finish_time;
	long m_lsp_server_count;
	long m_total_server_count;
	c_static_array<long, 20> __unknown80;
	c_static_array<s_client_data, 16> m_current_clients;
	long connection_token;
};
static_assert(sizeof(c_online_lsp_manager) == 0x494);

extern c_static_string<4>(&g_server_descriptions)[9];
extern s_server_connect_info(&g_additional_raw_servers)[1];
extern c_online_lsp_manager& g_online_lsp_manager;


