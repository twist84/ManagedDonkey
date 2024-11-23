#pragma once

#include "cseries/cseries.hpp"

enum e_online_lsp_service_type
{
	// H3
	/* ttl */ _online_lsp_service_type_title_files = 0,
	/* usr */ _online_lsp_service_type_user_files,
	/* shr */ _online_lsp_service_type_mass_storage,
	/* dbg */ _online_lsp_service_type_debug,
	/* web */ _online_lsp_service_type_webstats,

	// HO
	/* mmk */ _online_lsp_service_type_mmk, // matchmaking?
	/* amm */ _online_lsp_service_type_amm, // ?
	/* prs */ _online_lsp_service_type_prs, // presence from reach?
	/* ofr */ _online_lsp_service_type_ofr, // offers?

	k_online_lsp_service_type_count
};

enum e_online_lsp_state
{
	_online_lsp_state_search_start = 0,
	_online_lsp_state_searching,
	_online_lsp_state_search_succeeded,
	_online_lsp_state_search_failed,

	k_online_lsp_state_count
};

struct s_server_connect_info
{
	c_static_string<16> ip;
	word port;
	c_static_string<200> services_supported;
};
static_assert(sizeof(s_server_connect_info) == 0xDA);

struct c_online_lsp_manager
{
	enum e_client_state
	{
		_client_state_none = 0,
		_client_state_connecting,
		_client_state_connected,

		k_client_state_count
	};

	long __thiscall acquire_server(e_online_lsp_service_type service_type, long* out_connection_token, long* ip_address_out, word* port_out, char const* service_description);
	
	void clear_activated_servers();
	void clear_client(long client_index);
	void disconnect_from_server(long connection_token, bool success);
	long find_empty_slot_index();
	long find_slot_index_from_token(long connection_token);
	static c_online_lsp_manager* get();
	void go_into_crash_mode();
	void reset();
	void server_connected(long connection_token);

	void update();

	static long const k_client_description_length = 48;

	bool m_last_online_state;
	bool m_crash_mode;

	struct
	{
		long currently_activated_server_index;
		long currently_activated_server_ip;
		word currently_activated_server_port;
	} m_service[9];

	long m_last_search_start_time;
	long m_last_use_time;
	long m_lsp_server_count;
	long m_raw_server_count;
	c_static_array<long, 20> m_best_service_indices;

	struct
	{
		c_enum<e_client_state, long, _client_state_none, k_client_state_count> client_state;
		long service_type;
		long connection_token;
		c_static_string<k_client_description_length> client_description;
	}  m_current_clients[16];

	long connection_token;
};
static_assert(sizeof(c_online_lsp_manager) == 0x494);

extern c_static_string<4>(&g_server_descriptions)[9];
extern s_server_connect_info(&g_additional_raw_servers)[1];
extern c_online_lsp_manager& g_online_lsp_manager;

extern char const* const k_service_type_descriptions[k_online_lsp_service_type_count];

extern bool __cdecl online_lsp_activate_and_retrieve_server(int server_index, long* ip_address_out);
extern bool __cdecl online_lsp_begin_search();
extern void __cdecl online_lsp_deactivate();
extern void __cdecl online_lsp_dispose();
extern dword __cdecl online_lsp_get_server_count();
extern bool __cdecl online_lsp_get_server_description(long server_index, c_static_string<200>* server_description);
extern e_online_lsp_state __cdecl online_lsp_get_state();
extern void __cdecl online_lsp_initialize();
extern bool __cdecl online_lsp_service_available();
extern void __cdecl online_lsp_update();

extern void online_lsp_get_info(long* ip_address, word* port);
extern void online_lsp_set_info(char const* host, char const* port);

