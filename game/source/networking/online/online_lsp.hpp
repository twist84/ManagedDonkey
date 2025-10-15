#pragma once

#include "cseries/cseries.hpp"

enum e_online_lsp_server_acquire_result
{
	_online_lsp_server_acquire_result_pending = 0,
	_online_lsp_server_acquire_result_success = 1,
	_online_lsp_server_acquire_result_failure = 2,
};

struct s_server_connect_info
{
	c_static_string<16> ip;
	uns16 port;
	c_static_string<200> services_supported;
};
static_assert(sizeof(s_server_connect_info) == 0xDA);

class c_online_lsp_manager
{
public:
	enum e_client_state
	{
		_client_state_none = 0,
		_client_state_connecting,
		_client_state_connected,

		k_client_state_count
	};

	e_online_lsp_server_acquire_result __thiscall acquire_server(e_online_lsp_service_type service_type, int32* out_connection_token, int32* ip_address_out, uns16* port_out, const char* service_description);
	
	void clear_activated_servers();
	void clear_client(int32 client_index);
	void disconnect_from_server(int32 connection_token, bool success);
	int32 find_empty_slot_index();
	int32 find_slot_index_from_token(int32 connection_token);
	static c_online_lsp_manager* get();
	void go_into_crash_mode();
	void reset();
	void server_connected(int32 connection_token);

	void update();

	static int32 const k_client_description_length = 48;

	bool m_last_online_state;
	bool m_crash_mode;

	struct
	{
		int32 currently_activated_server_index;
		int32 currently_activated_server_ip;
		uns16 currently_activated_server_port;
	} m_service[9];

	int32 m_last_search_start_time;
	int32 m_last_use_time;
	int32 m_lsp_server_count;
	int32 m_raw_server_count;
	c_static_array<int32, 20> m_best_service_indices;

	struct
	{
		c_enum<e_client_state, int32, _client_state_none, k_client_state_count> client_state;
		int32 service_type;
		int32 connection_token;
		c_static_string<k_client_description_length> client_description;
	}  m_current_clients[16];

	int32 connection_token;
};
static_assert(sizeof(c_online_lsp_manager) == 0x494);

extern c_static_string<4>(&g_server_descriptions)[9];
extern s_server_connect_info(&g_additional_raw_servers)[1];
extern c_online_lsp_manager& g_online_lsp_manager;

extern const char* const k_service_type_descriptions[k_online_lsp_service_type_count];

extern bool __cdecl online_lsp_activate_and_retrieve_server(int server_index, int32* ip_address_out);
extern bool __cdecl online_lsp_begin_search();
extern void __cdecl online_lsp_deactivate();
extern void __cdecl online_lsp_dispose();
extern uns32 __cdecl online_lsp_get_server_count();
extern bool __cdecl online_lsp_get_server_description(int32 server_index, c_static_string<200>* server_description);
extern e_online_lsp_state __cdecl online_lsp_get_state();
extern void __cdecl online_lsp_initialize();
extern bool __cdecl online_lsp_service_available();
extern void __cdecl online_lsp_update();

extern void online_lsp_get_info();
extern void online_lsp_set_info(const char* host, uns16 port);

