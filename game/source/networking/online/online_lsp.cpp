#include "networking/online/online_lsp.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // inet_addr, htons
#undef _WINSOCK_DEPRECATED_NO_WARNINGS

REFERENCE_DECLARE_ARRAY(0x0199FD80, c_static_string<4>, g_server_descriptions, 9);
REFERENCE_DECLARE_ARRAY(0x0199FDA8, s_server_connect_info, g_additional_raw_servers, 1);
REFERENCE_DECLARE(0x0199FE88, c_online_lsp_manager, g_online_lsp_manager);

HOOK_DECLARE_CLASS_MEMBER(0x00431440, c_online_lsp_manager, acquire_server);

long const lsp_server_ip = inet_addr("127.0.0.1");
unsigned short const lsp_server_port = htons(8000);

//enum e_online_lsp_server_acquire_result __cdecl c_online_lsp_manager::acquire_server(enum e_online_lsp_service_type service_type, long, long* connection_token_out, long* ip_address, unsigned short* port, char const* service_description)
long __thiscall c_online_lsp_manager::acquire_server(long service_type, long* connection_token_out, long* ip_address, unsigned short* port, char const* service_description)
{
	*connection_token_out = 1;
	*ip_address = lsp_server_ip;
	*port = lsp_server_port;

	return 1;
}

void c_online_lsp_manager::clear_activated_servers()
{
	//DECLFUNC(0x00431740, void, __thiscall, c_online_lsp_manager*)(this);

	for (s_server_data& server : m_service)
	{
		server.currently_activated_server_index = NONE;
		server.ip_address = 0;
		server.port = 0;
	}
	
	for (long i = 0; i < k_maximum_simultaneous_clients; i++)
		clear_client(i);
}

void c_online_lsp_manager::clear_client(long client_index)
{
	//DECLFUNC(0x004317F0, void, __thiscall, c_online_lsp_manager*, long)(this, client_index);

	m_current_clients[client_index].client_state = 0;
	m_current_clients[client_index].service_type = NONE;
	m_current_clients[client_index].connection_token = NONE;
	m_current_clients[client_index].description.clear();
}

c_online_lsp_manager* c_online_lsp_manager::get()
{
	//return INVOKE(0x004319F0, c_online_lsp_manager::get);

	return &g_online_lsp_manager;
}

void c_online_lsp_manager::go_into_crash_mode()
{
	//DECLFUNC(0x00431C40, void, __thiscall, c_online_lsp_manager*)(this);

	internal_critical_section_enter(_critical_section_lsp_manager);
	generate_event(_event_level_message, "networking:online:lsp: entering crash mode");
	reset();
	m_crash_mode = true;
	internal_critical_section_leave(_critical_section_lsp_manager);
}

void c_online_lsp_manager::reset()
{
	//DECLFUNC(0x00431EF0, void, __thiscall, c_online_lsp_manager*)(this);

	generate_event(_event_level_message, "networking:online:lsp: resetting");
	m_lsp_server_count = false;
	m_total_server_count = 0;
	m_best_service_indices.clear();
	online_lsp_deactivate();
	clear_activated_servers();
	lsp_search_start_time = 0;
}

bool __cdecl online_lsp_activate_and_retrieve_server(int server_index, long* ip_address_out)
{
	return INVOKE(0x0043C440, online_lsp_activate_and_retrieve_server, server_index, ip_address_out);
}

bool __cdecl online_lsp_begin_search()
{
	return INVOKE(0x0043C470, online_lsp_begin_search);
}

void __cdecl online_lsp_deactivate()
{
	INVOKE(0x0043C480, online_lsp_deactivate);
}

void __cdecl online_lsp_dispose()
{
	INVOKE(0x0043C490, online_lsp_dispose);
}

dword __cdecl online_lsp_get_server_count()
{
	return INVOKE(0x0043C4A0, online_lsp_get_server_count);
}

bool __cdecl online_lsp_get_server_description(long server_index, c_static_string<200>* server_description)
{
	return INVOKE(0x0043C4B0, online_lsp_get_server_description, server_index, server_description);
}

e_online_lsp_state __cdecl online_lsp_get_state()
{
	return INVOKE(0x0043C570, online_lsp_get_state);
}

void __cdecl online_lsp_initialize()
{
	INVOKE(0x0043C580, online_lsp_initialize);
}

bool __cdecl online_lsp_service_available()
{
	return INVOKE(0x0043C5A0, online_lsp_service_available);
}

void __cdecl online_lsp_update()
{
	INVOKE(0x0043C5B0, online_lsp_update);
}

