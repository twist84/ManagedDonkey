#include "networking/online/online_lsp.hpp"

#include "cseries/cseries_events.hpp"
#include "main/console.hpp"
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

transport_address lsp_server_address(inet_addr("127.0.0.1"), htons(8000), sizeof(uns32));

const char* const k_service_type_descriptions[k_online_lsp_service_type_count]
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

//.text:004313C0 ; c_online_lsp_manager::c_online_lsp_manager

e_online_lsp_server_acquire_result __thiscall c_online_lsp_manager::acquire_server(e_online_lsp_service_type service_type, int32* out_connection_token, int32* ip_address_out, uns16* port_out, const char* service_description)
{
	*out_connection_token = 1;
	*ip_address_out = lsp_server_address.ipv4_address;
	*port_out = lsp_server_address.port;

	return _online_lsp_server_acquire_result_success;
}

//.text:004316A0 ; c_online_lsp_manager::anyone_connected
//.text:004316E0 ; c_online_lsp_manager::any_client_connecting_to_service

void c_online_lsp_manager::clear_activated_servers()
{
	//INVOKE_CLASS_MEMBER(0x00431740, c_online_lsp_manager, clear_activated_servers);

	for (int32 i = 0; i < NUMBEROF(m_service); i++)
	{
		m_service[i].currently_activated_server_index = NONE;
		m_service[i].currently_activated_server_ip = 0;
		m_service[i].currently_activated_server_port = 0;
	}
	
	for (int32 i = 0; i < NUMBEROF(m_current_clients); i++)
	{
		clear_client(i);
	}
}

void c_online_lsp_manager::clear_client(int32 client_index)
{
	//INVOKE_CLASS_MEMBER(0x004317F0, c_online_lsp_manager, clear_client, client_index);

	m_current_clients[client_index].client_state = 0;
	m_current_clients[client_index].service_type = NONE;
	m_current_clients[client_index].connection_token = NONE;
	m_current_clients[client_index].client_description.clear();
}

void c_online_lsp_manager::disconnect_from_server(int32 connection_token, bool success)
{
	//INVOKE_CLASS_MEMBER(0x00431830, c_online_lsp_manager, disconnect_from_server, connection_token, success);

	ASSERT(connection_token != NONE);

	c_critical_section_scope section_scope(k_crit_section_lsp_manager);
	int32 slot_index = find_slot_index_from_token(connection_token);
	if (slot_index == NONE)
	{
		return;
	}

	ASSERT(m_current_clients[slot_index].client_state != _client_state_none);
	event(_event_message, "networking:online:lsp: token=%d, server disconnected with %s",
		connection_token,
		success ? "success" : "failure");

	if (!success)
	{
		int32 service_type = m_current_clients[slot_index].service_type;
		int32 currently_activated_server_index = m_service[service_type].currently_activated_server_index;
		m_best_service_indices[currently_activated_server_index]++;
	}

	clear_client(slot_index);
	m_last_use_time = system_milliseconds();
}

int32 c_online_lsp_manager::find_empty_slot_index()
{
	//return INVOKE_CLASS_MEMBER(0x00431910, c_online_lsp_manager, find_empty_slot_index);

	for (int32 slot_index = 0; slot_index < NUMBEROF(m_current_clients); slot_index++)
	{
		if (m_current_clients[slot_index].client_state == _client_state_none)
		{
			return slot_index;
		}
	}
	return NONE;
}

int32 c_online_lsp_manager::find_slot_index_from_token(int32 connection_token)
{
	//return INVOKE_CLASS_MEMBER(0x00431930, c_online_lsp_manager, find_slot_index_from_token, connection_token);

	for (int32 slot_index = 0; slot_index < NUMBEROF(m_current_clients); slot_index++)
	{
		if (m_current_clients[slot_index].client_state != _client_state_none && m_current_clients[slot_index].connection_token == connection_token)
		{
			return slot_index;
		}
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
	//INVOKE_CLASS_MEMBER(0x00431C40, c_online_lsp_manager, go_into_crash_mode);

	c_critical_section_scope section_scope(k_crit_section_lsp_manager);
	event(_event_message, "networking:online:lsp: entering crash mode");
	reset();
	m_crash_mode = true;
}

//.text:00431CF0 ; c_online_lsp_manager::search_results_still_fresh
//.text:00431DF0 ; c_online_lsp_manager::register_best_server

void c_online_lsp_manager::reset()
{
	//INVOKE_CLASS_MEMBER(0x00431EF0, c_online_lsp_manager, reset);

	event(_event_message, "networking:online:lsp: resetting");
	m_lsp_server_count = false;
	m_raw_server_count = 0;
	m_best_service_indices.clear();
	online_lsp_deactivate();
	clear_activated_servers();
	m_last_search_start_time = 0;
}

void c_online_lsp_manager::server_connected(int32 connection_token)
{
	//INVOKE_CLASS_MEMBER(0x00431F50, c_online_lsp_manager, server_connected, connection_token);

	ASSERT(connection_token != NONE);

	c_critical_section_scope section_scope(k_crit_section_lsp_manager);
	int32 slot_index = find_slot_index_from_token(connection_token);
	if (slot_index != NONE && m_current_clients[slot_index].client_state == _client_state_connecting)
	{
		event(_event_message, "networking:online:lsp: token=%d, server connected", connection_token);
		m_current_clients[slot_index].client_state = _client_state_connected;
	}
}

//.text:00431FD0 ; c_online_lsp_manager::service_type_in_list

void c_online_lsp_manager::update()
{
	INVOKE_CLASS_MEMBER(0x004322A0, c_online_lsp_manager, update);
}

//.text:00432400 ; c_online_lsp_manager::update_online_status

bool __cdecl online_lsp_activate_and_retrieve_server(int server_index, int32* ip_address_out)
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

uns32 __cdecl online_lsp_get_server_count()
{
	return INVOKE(0x0043C4A0, online_lsp_get_server_count);
}

bool __cdecl online_lsp_get_server_description(int32 server_index, c_static_string<200>* server_description)
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

void online_lsp_get_info()
{
	union
	{
		int32 ip_address = 0;
		uns8 ina[4];
	};

	ip_address = ntohl(lsp_server_address.ipv4_address);
	uns16 port = ntohs(lsp_server_address.port);
	console_printf_color(global_real_argb_cyan, "%hd.%hd.%hd.%hd:%hd", ina[3], ina[2], ina[1], ina[0], port);
}

void online_lsp_set_info(const char* host, uns16 port)
{
	transport_address address{};
	transport_address_from_host(host, address);

	lsp_server_address.ipv4_address = htonl(address.ipv4_address);
	lsp_server_address.port = htons(port);
}

