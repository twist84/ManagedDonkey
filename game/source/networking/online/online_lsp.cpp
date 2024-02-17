#include "networking/online/online_lsp.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "multithreading/synchronization.hpp"
#include "networking/transport/transport_address.hpp"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h> // inet_addr, htons
#undef _WINSOCK_DEPRECATED_NO_WARNINGS

REFERENCE_DECLARE_ARRAY(0x0199FD80, c_static_string<4>, g_server_descriptions, 9);
REFERENCE_DECLARE_ARRAY(0x0199FDA8, s_server_connect_info, g_additional_raw_servers, 1);
REFERENCE_DECLARE(0x0199FE88, c_online_lsp_manager, g_online_lsp_manager);

HOOK_DECLARE_CLASS_MEMBER(0x00431440, c_online_lsp_manager, acquire_server);

char const* const k_service_type_descriptions[k_online_lsp_service_type_count]
{
	// H3
	/* ttl */ "title_files",
	/* usr */ "user_files",
	/* shr */ "mass_storage",
	/* dbg */ "debug",
	/* web */ "webstats",

	// HO
	/* mmk */ "mmk", // matchmaking?
	/* amm */ "amm", // ?
	/* prs */ "prs", // presence from reach?
	/* ofr */ "ofr"  // offers?
};

transport_address lsp_server_address(inet_addr("127.0.0.1"), htons(8000), sizeof(dword));

void online_lsp_get_info(long* ip_address, word* port)
{
	if (ip_address)
		*ip_address = ntohl(lsp_server_address.ipv4_address);

	if (port)
		*port = ntohs(lsp_server_address.port);
}

void online_lsp_set_info(char const* host, char const* port)
{
	transport_address address{};
	transport_address_from_host(host, address);

	if (host) lsp_server_address.ipv4_address = htonl(address.ipv4_address);
	if (port) lsp_server_address.port = htons(static_cast<word>(atol(port)));
}

//.text:004313C0 ; c_online_lsp_manager::c_online_lsp_manager

//e_online_lsp_server_acquire_result c_online_lsp_manager::acquire_server(e_online_lsp_service_type service_type, long* out_connection_token, long* ip_address_out, word* port_out, char const* service_description)
long __thiscall c_online_lsp_manager::acquire_server(e_online_lsp_service_type service_type, long* out_connection_token, long* ip_address_out, word* port_out, char const* service_description)
{
	*out_connection_token = 1;
	*ip_address_out = lsp_server_address.ipv4_address;
	*port_out = lsp_server_address.port;
	return 1;
}

//.text:004316A0 ; c_online_lsp_manager::anyone_connected
//.text:004316E0 ; c_online_lsp_manager::any_client_connecting_to_service

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

void c_online_lsp_manager::disconnect_from_server(long connection_token, bool success)
{
	//DECLFUNC(0x00431830, void, __thiscall, c_online_lsp_manager*, long, bool)(this, connection_token, success);

	ASSERT(connection_token != NONE);

	c_critical_section_scope section_scope(_critical_section_lsp_manager);
	long slot_index = find_slot_index_from_token(connection_token);
	if (slot_index == NONE)
		return;

	ASSERT(m_current_clients[slot_index].client_state != _client_state_none);
	generate_event(_event_level_message, "networking:online:lsp: token=%d, server disconnected with %s",
		connection_token,
		success ? "success" : "failure");

	if (!success)
		m_best_service_indices[m_service[m_current_clients[slot_index].service_type].currently_activated_server_index]++;

	clear_client(slot_index);
	lsp_search_finish_time = system_milliseconds();
}

long c_online_lsp_manager::find_empty_slot_index()
{
	//return DECLFUNC(0x00431830, long, __thiscall, c_online_lsp_manager*)(this);

	for (long slot_index = 0; slot_index < k_maximum_simultaneous_clients; slot_index++)
	{
		if (m_current_clients[slot_index].client_state == _client_state_none)
			return slot_index;
	}
	return NONE;
}

long c_online_lsp_manager::find_slot_index_from_token(long connection_token)
{
	//return DECLFUNC(0x00431830, long, __thiscall, c_online_lsp_manager*, long)(this, connection_token);

	for (long slot_index = 0; slot_index < k_maximum_simultaneous_clients; slot_index++)
	{
		if (m_current_clients[slot_index].client_state != _client_state_none && m_current_clients[slot_index].connection_token == connection_token)
			return slot_index;
	}
	return NONE;
}

c_online_lsp_manager* c_online_lsp_manager::get()
{
	//return INVOKE(0x004319F0, c_online_lsp_manager::get);

	return &g_online_lsp_manager;
}

//.text:00431A00 ; c_online_lsp_manager::get_best_server_index
//.text:00431B00 ; c_online_lsp_manager::get_lsp_port
//.text:00431BF0 ; c_online_lsp_manager::get_recommended_retry_count

void c_online_lsp_manager::go_into_crash_mode()
{
	//DECLFUNC(0x00431C40, void, __thiscall, c_online_lsp_manager*)(this);

	c_critical_section_scope section_scope(_critical_section_lsp_manager);
	generate_event(_event_level_message, "networking:online:lsp: entering crash mode");
	reset();
	m_crash_mode = true;
}

//.text:00431CF0 ; c_online_lsp_manager::search_results_still_fresh
//.text:00431DF0 ; c_online_lsp_manager::register_best_server

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

void c_online_lsp_manager::server_connected(long connection_token)
{
	//DECLFUNC(0x00431F50, void, __thiscall, c_online_lsp_manager*, long)(this, connection_token);

	ASSERT(connection_token != NONE);

	c_critical_section_scope section_scope(_critical_section_lsp_manager);
	long slot_index = find_slot_index_from_token(connection_token);
	if (slot_index != NONE && m_current_clients[slot_index].client_state == _client_state_connecting)
	{
		generate_event(_event_level_message, "networking:online:lsp: token=%d, server connected", connection_token);
		m_current_clients[slot_index].client_state = _client_state_connected;
	}
}

//.text:00431FD0 ; c_online_lsp_manager::service_type_in_list
//.text:004322A0 ; c_online_lsp_manager::update
//.text:00432400 ; c_online_lsp_manager::update_online_status

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

